/*
 * Copyright (c) 2020 Linaro Limited
 *
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <metal/device.h>
#include <metal/spinlock.h>
#include <openamp/open_amp.h>

#include "tfm_spe_openamp_interface.h"
#include "tfm_spm_log.h"
#include "tfm_spe_shm_openamp.h"

static metal_phys_addr_t shm_physmap[] = { SHM_START_PHY_ADDR };
static struct metal_device shm_device = {
    .name = SHM_DEVICE_NAME,
    .bus = NULL,
    .num_regions = 1,
    {
        {
            .virt       = (void *) SHM_START_VIRT_ADDR,
            .physmap    = shm_physmap,
            .size       = SHM_SIZE,
            .page_shift = 0xffffffff,
            .page_mask  = 0xffffffff,
            .mem_flags  = 0,
            .ops        = { NULL },
        },
    },
    .node = { NULL },
    .irq_num = 0,
    .irq_info = NULL
};

static struct virtio_vring_info rvrings[2];

static struct virtio_device vdev;
static struct rpmsg_virtio_device rvdev;
static struct metal_io_region *io;
static struct virtqueue *vq[2];
static struct rpmsg_virtio_shm_pool shpool;
static struct rpmsg_endpoint tfm_ept;
static struct rpmsg_endpoint *ep = &tfm_ept;
static struct metal_spinlock spe_map_slock;
static openamp_to_tfm_callback tfm_callback = NULL;
static openamp_to_tfm_notify tfm_notify = NULL;

static unsigned char virtio_get_status(struct virtio_device *vdev)
{
    (void)vdev;
    uint32_t status = *(uint32_t *)VDEV_STATUS_ADDR;
    return status;
}

static void virtio_set_status(struct virtio_device *vdev, unsigned char status)
{
    (void)vdev;
    *(uint32_t *)VDEV_STATUS_ADDR = status;
}

static uint32_t virtio_get_features(struct virtio_device *vdev)
{
    (void)vdev;
    return 1 << VIRTIO_RPMSG_F_NS;
}

static void virtio_notify(struct virtqueue *vq)
{
    (void)vq;
    tfm_notify();
}

static struct virtio_dispatch dispatch = {
    .get_status = virtio_get_status,
    .set_status = virtio_set_status,
    .get_features = virtio_get_features,
    .notify = virtio_notify,
};

int endpoint_cb(struct rpmsg_endpoint *ept, void *data,
        size_t len, uint32_t src, void *priv)
{
    (void)ept;
    (void)src;
    (void)priv;
    tfm_callback(data, len);
    return 0;
}

static void rpmsg_service_unbind(struct rpmsg_endpoint *ept)
{
    (void)ept;
    rpmsg_destroy_ept(ep);
}

void ns_bind_cb(struct rpmsg_device *rdev, const char *name, uint32_t dest)
{
    (void)rpmsg_create_ept(ep, rdev, name,
        RPMSG_ADDR_ANY, dest,
        endpoint_cb,
        rpmsg_service_unbind);
}

void tfm_to_openamp_notify(void)
{
    virtqueue_notification(vq[0]);
}

void tfm_to_openamp_spe_map_spinlock_acquire(void)
{
    metal_spinlock_acquire(&spe_map_slock);
}

void tfm_to_openamp_spe_map_spinlock_release(void)
{
    metal_spinlock_release(&spe_map_slock);
}

void tfm_to_openamp_reply_back(const void* data, size_t len)
{
    rpmsg_send(ep, data, len);
}

void tfm_to_openamp_reply_back_no_copy(const void* data, size_t len)
{
    rpmsg_send_nocopy(ep, data, len);
}

void tfm_to_openamp_hold_buffer(const void *buffer)
{
    rpmsg_hold_rx_buffer(ep, (void*)buffer);
}

void tfm_to_openamp_release_buffer(const void *buffer)
{
    rpmsg_release_rx_buffer(ep, (void*)buffer);
}

void *tfm_to_openamp_get_buffer(uint32_t *len)
{
    return rpmsg_get_tx_payload_buffer(ep, len, 1);
}

int tfm_to_openamp_get_buffer_size(void)
{
    return rpmsg_virtio_get_buffer_size(&rvdev.rdev);
}

void *tfm_to_openamp_translate_non_secure_to_secure_ptr(const void *ptr)
{
    metal_phys_addr_t phys = 0;
    phys = (metal_phys_addr_t)ptr;
    return metal_io_phys_to_virt(io, phys);
}

void *tfm_to_openamp_translate_secure_to_non_secure_ptr(const void *ptr)
{
    metal_phys_addr_t phys = metal_io_virt_to_phys(io, (void*)ptr);
    return (void*)phys;
}

int32_t tfm_to_openamp_init(openamp_to_tfm_callback cb,
                   openamp_to_tfm_notify notify)
{
    int status = 0;
    struct metal_device *device;
    struct metal_init_params metal_params = METAL_INIT_DEFAULTS;

    SPMLOG_INFMSG("TF-M OpenAMP[master] starting initialization...\r\n");

    if (cb == NULL || notify == NULL) {
        SPMLOG_ERRMSG("invalid parameters\r\n");
        return ERROR;
    }
    tfm_callback = cb;
    tfm_notify = notify;

    metal_spinlock_init(&spe_map_slock);

    status = metal_init(&metal_params);
    if (status != 0) {
        SPMLOG_ERRMSG("metal_init: failed - error code\r\n");
        return ERROR;
    }

    status = metal_register_generic_device(&shm_device);
    if (status != 0) {
        SPMLOG_ERRMSG("Couldn't register shared memory device\r\n");
        return ERROR;
    }

    status = metal_device_open("generic", SHM_DEVICE_NAME, &device);
    if (status != 0) {
        SPMLOG_ERRMSG("metal_device_open failed\r\n");
        return ERROR;
    }

    io = metal_device_io_region(device, 0);
    if (io == NULL) {
        SPMLOG_ERRMSG("metal_device_io_region failed to get region\r\n");
        return ERROR;
    }

    /* setup vdev */
    vq[0] = virtqueue_allocate(VRING_SIZE);
    if (vq[0] == NULL) {
        SPMLOG_ERRMSG("virtqueue_allocate failed to alloc vq[0]\r\n");
        return ERROR;
    }
    vq[1] = virtqueue_allocate(VRING_SIZE);
    if (vq[1] == NULL) {
        SPMLOG_ERRMSG("virtqueue_allocate failed to alloc vq[1]\r\n");
        return ERROR;
    }

    vdev.role = RPMSG_MASTER;
    vdev.vrings_num = VRING_COUNT;
    vdev.func = &dispatch;
    rvrings[0].io = io;
    rvrings[0].info.vaddr = (void *)VRING_TX_ADDRESS;
    rvrings[0].info.num_descs = VRING_SIZE;
    rvrings[0].info.align = VRING_ALIGNMENT;
    rvrings[0].vq = vq[0];

    rvrings[1].io = io;
    rvrings[1].info.vaddr = (void *)VRING_RX_ADDRESS;
    rvrings[1].info.num_descs = VRING_SIZE;
    rvrings[1].info.align = VRING_ALIGNMENT;
    rvrings[1].vq = vq[1];

    vdev.vrings_info = &rvrings[0];

    /* setup rvdev */
    rpmsg_virtio_init_shm_pool(&shpool, (void *)SHM_START_VIRT_ADDR, SHM_SIZE);
    status = rpmsg_init_vdev(&rvdev, &vdev, ns_bind_cb, io, &shpool);
    if (status != 0) {
        SPMLOG_ERRMSGVAL("rpmsg_init_vdev failed : ", status);
        return ERROR;
    }
    SPMLOG_INFMSG("rpmsg_init_vdev Done!\r\n");

    return SUCCESS;
}
