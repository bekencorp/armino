//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <common/bk_include.h>
#include <os/os.h>
#include <stdio.h>
#include <driver/media_types.h>
#include <driver/uvc_camera.h>
#include <os/mem.h>
#include "bk_usb.h"
#include <modules/usb.h>
#include <common/bk_err.h>
#include <driver/dma.h>

#include <driver/gpio.h>
#include "gpio_driver.h"
#include "bk_misc.h"
#include <modules/pm.h>

#define TAG "uvc_device"

#define LOGI(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

static uint8_t uvc_camera_state = UVC_STATE_UNKNOW;
static uint8_t *uvc_rx_stream_buffer = NULL;
static uvc_config_t *uvc_camera_config = NULL;

static bk_err_t uvc_camera_deinit(uvc_state_t state)
{
	int ret = BK_OK;
	void *parameter;

	LOGI("%s, %d\r\n", __func__, uvc_camera_state);

	// step 1: set uvc state
	uvc_camera_state = state;

	// step 2: check need power off uvc
	if (uvc_camera_state == UVC_DISCONNECT_NORMAL)
	{
		bk_uvc_stop();

		usb_device_set_using_status(0, USB_UVC_DEVICE);

		bk_usb_close();

		bk_uvc_camera_power_enable(0);

		// unregister connect & disconnect
		parameter = NULL;
		bk_uvc_register_disconnect_callback(parameter);
		parameter = NULL;
		bk_uvc_register_config_callback(parameter);
	}

	if (uvc_rx_stream_buffer)
	{
		os_free(uvc_rx_stream_buffer);
		uvc_rx_stream_buffer = NULL;
	}

	if (uvc_camera_state == UVC_DISCONNECT_ABNORMAL && uvc_camera_config
		&& uvc_camera_config->connect_state_change_cb)
	{
		uvc_camera_config->connect_state_change_cb(uvc_camera_state);
	}

	LOGI("%s complete, %d\r\n", __func__, uvc_camera_state);

	return ret;

}

static void uvc_camera_connect(void)
{
	uvc_camera_state = UVC_CONNECTED;

	if (uvc_camera_config && uvc_camera_config->connect_state_change_cb)
	{
		uvc_camera_config->connect_state_change_cb(uvc_camera_state);
	}
}

static void uvc_camera_disconnect(void)
{
	if (uvc_camera_state != UVC_DISCONNECT_NORMAL)
	{
		uvc_camera_state = UVC_DISCONNECT_ABNORMAL;
		uvc_camera_deinit(UVC_DISCONNECT_ABNORMAL);
	}
}

static void uvc_fiddle_rx_vs_callback(void)
{
	if (uvc_camera_state == UVC_WORKING_START)
		bk_uvc_receive_video_stream();
}

static void uvc_get_packet_rx_vs_callback(uint8_t *addr, uint32_t len)
{
	if (uvc_camera_state != UVC_WORKING_START)
		return;

	uvc_camera_config->uvc_packet_rx_cb(addr, len);
}

static bk_err_t uvc_camera_reset(void)
{
	int ret = BK_OK;
	uint8_t wait_count = 0;

	LOGI("%s, state:%d\r\n", __func__, uvc_camera_state);

	// uvc power up
	if (uvc_camera_state != UVC_DISCONNECT_ABNORMAL)
	{
		void *parameter = (void *)uvc_camera_connect;
		bk_uvc_register_config_callback(parameter);
		bk_uvc_camera_power_enable(1);
		bk_usb_open(USB_HOST_MODE);
		usb_device_set_using_status(1, USB_UVC_DEVICE);
	}

	while(MGC_DevGetConnectStatus() != 1) {
		if(wait_count > 100) {
			LOGE("wait usb connect timeout 1s!\r\n");
			ret = BK_UVC_NO_RESPON;
			break;
		}
		rtos_delay_milliseconds(10);
		wait_count++;
	}

	if (ret == BK_OK)
	{
		uvc_camera_state = UVC_CONNECTED;
	}

	return ret;
}

uvc_state_t uvc_camera_get_state(void)
{
	return uvc_camera_state;
}

void uvc_camera_set_state(uvc_state_t state)
{
	uvc_camera_state = state;
}

bk_err_t bk_uvc_camera_init(uvc_config_t *config)
{
	int ret = BK_FAIL;
	void *parameter;

	if (uvc_camera_state == UVC_WORKING_START || uvc_camera_state == UVC_WORKING_STOP)
	{
		LOGW("uvc is working\r\n");
		ret = BK_OK;
	}

	if (config == NULL)
	{
		ret = BK_UVC_CONFIG_ERROR;
		return ret;
	}

	BK_ASSERT(config->uvc_packet_rx_cb != NULL);
	BK_ASSERT(config->connect_state_change_cb != NULL);

	uvc_camera_config = config;

	// step 1: check uvc connect
	ret = uvc_camera_reset();
	if (ret != BK_OK)
	{
		LOGE("uvc state: %d\r\n", ret);
		goto init_error;
	}

	// step 2: uvc init
	parameter = (void *)uvc_camera_disconnect;
	bk_uvc_register_disconnect_callback(parameter);

	parameter = (void *)uvc_fiddle_rx_vs_callback;
	bk_uvc_register_VSrxed_callback(parameter);

	parameter = (void *)uvc_get_packet_rx_vs_callback;
	bk_uvc_register_VSrxed_packet_callback(parameter);

	if (uvc_rx_stream_buffer == NULL)
	{
		uvc_rx_stream_buffer = (uint8_t *)os_malloc(128);
		if (uvc_rx_stream_buffer == NULL)
		{
			LOGE("malloc rx video stream buf error!\r\n");
			ret = BK_UVC_NO_MEMORY;
			goto init_error;
		}
	}

	parameter = (void *)uvc_rx_stream_buffer;
	bk_uvc_register_rx_vstream_buffptr(parameter);

	bk_uvc_register_rx_vstream_bufflen(128);

	bk_uvc_register_link(0);

	ret = bk_uvc_camera_set_config(config->type, &config->device);
	if (ret != BK_OK)
	{
		goto init_error;
	}

	return ret;

init_error:

	uvc_camera_state = UVC_DISCONNECT_ABNORMAL;

	LOGE("%s failed, state:%d\r\n", __func__, uvc_camera_state);

	if (uvc_rx_stream_buffer)
	{
		os_free(uvc_rx_stream_buffer);
		uvc_rx_stream_buffer = NULL;
	}

	return ret;
}


bk_err_t bk_uvc_camera_deinit(void)
{
	int ret = BK_OK;

	LOGI("%s\r\n", __func__);

	/*if (uvc_camera_state == UVC_DISCONNECT_NORMAL || uvc_camera_state == UVC_DISCONNECT_ABNORMAL)
	{
		LOGE("uvc have deinit\r\n");
		return ret;
	}*/

	ret = uvc_camera_deinit(UVC_DISCONNECT_NORMAL);

	return ret;
}

bk_err_t bk_uvc_camera_start(void)
{
	int ret = BK_FAIL;

	LOGI("%s, %d\r\n", __func__, uvc_camera_state);

	// only do start at connected or stopped
	if (uvc_camera_state < UVC_CONNECTED || uvc_camera_state == UVC_WORKING_START || uvc_camera_state == UVC_STATE_UNKNOW)
	{
		LOGW("Not permit start uvc when not connected or stop\r\n");
		return BK_UVC_NOT_PERMIT;
	}

	ret = bk_uvc_start();
	if (ret != BK_OK)
	{
		LOGE("start fail, %d\r\n", ret);
	}
	else
	{
		uvc_camera_state = UVC_WORKING_START;
	}

	return ret;
}

bk_err_t bk_uvc_camera_stop(void)
{
	int ret = BK_FAIL;

	if (uvc_camera_state != UVC_WORKING_START)
	{
		LOGW("Not permit stop uvc when uvc not working normal\r\n");
		return BK_UVC_NOT_PERMIT;
	}

	ret = bk_uvc_stop();
	if (ret != BK_OK)
	{
		LOGE("stop fail, %d\r\n", ret);
	}
	else
	{
		uvc_camera_state = UVC_WORKING_STOP;
	}

	return ret;
}


bk_err_t bk_uvc_camera_set_type(media_camera_type_t type)
{
	int ret = BK_FAIL;

	if (uvc_camera_state == UVC_WORKING_START || uvc_camera_state == UVC_DISCONNECT_ABNORMAL
		|| uvc_camera_state == UVC_DISCONNECT_NORMAL)
	{
		LOGW("Not permit set uvc config when is working or disconnted!\r\n");
		return BK_UVC_NOT_PERMIT;
	}

	if (type == MEDIA_UVC_H264)
	{
		ret = bk_uvc_enable_H264();
	}
	else if (type == MEDIA_UVC_MJPEG)
	{
		ret = bk_uvc_enable_mjpeg();
	}
	else
	{
		ret = BK_UVC_CONFIG_ERROR;
	}

	if (ret == BK_OK)
	{
		uvc_camera_config->type = type;
	}

	LOGI("%s, type:%d, ret:%d\r\n", __func__, type, ret);

	return ret;

}

bk_err_t bk_uvc_camera_set_config(media_camera_type_t type, uvc_camera_device_t *config)
{
	int ret = BK_FAIL;

	if (uvc_camera_state == UVC_WORKING_START || uvc_camera_state == UVC_DISCONNECT_ABNORMAL
		|| uvc_camera_state == UVC_DISCONNECT_NORMAL)
	{
		LOGW("Not permit set uvc config when is working or disconnted!\r\n");
		return BK_UVC_NOT_PERMIT;
	}

	uint16_t calibrate_fps = 0;

	uint8_t i = 0, temp = 0;


	if (type < MEDIA_CAMERA_UNKNOW)
	{
		ret = bk_uvc_camera_set_type(type);
		if (ret !=BK_OK)
		{
			return ret;
		}
	}
	else
	{
		ret = BK_UVC_CONFIG_ERROR;
		return ret;
	}

	uvc_camera_device_t support_config[UVC_PPI_TYPE_CNT] = {0};

	bk_uvc_get_resolution_framerate((void *)support_config, UVC_PPI_TYPE_CNT);

	ret = BK_FAIL;

	for (i = 0; i < UVC_PPI_TYPE_CNT; i++)
	{
		if (support_config[i].width == 0)
		{
			break;
		}

		if (config->width == support_config[i].width && config->height == support_config[i].height)
		{
			if (config->fps == support_config[i].fps)
			{
				ret = BK_OK;
				temp = i;
				break;
			}
			else
			{
				calibrate_fps = support_config[i].fps;
				temp = i;
				ret = BK_OK;
			}
		}
	}

	if (ret != BK_OK)
	{
		LOGE("uvc not support this resolution!\r\n");
		ret = BK_UVC_PPI_ERROR;
		return ret;
	}

	if (config->fps != support_config[temp].fps)
	{
		config->fps = calibrate_fps;
	}

	uvc_camera_config->device.fps = config->fps;
	uvc_camera_config->device.width = config->width;
	uvc_camera_config->device.height = config->height;

	bk_uvc_set_resolution_framerate(config);

	return ret;
}

bk_err_t bk_uvc_camera_get_config(uvc_camera_device_t *param, uint16_t count)
{
	if (param == NULL)
	{
		return BK_UVC_NO_MEMORY;
	}

	if (uvc_camera_state == UVC_DISCONNECT_ABNORMAL || uvc_camera_state == UVC_DISCONNECT_NORMAL
		|| uvc_camera_state == UVC_STATE_UNKNOW)
	{
		LOGE("uvc not connected!\r\n");
		return BK_UVC_DISCONNCT;
	}

	bk_uvc_get_resolution_framerate((void *)param, count);

	return BK_OK;
}

uvc_camera_device_t *bk_uvc_camera_get_device(void)
{
	if (uvc_camera_config == NULL)
	{
		return NULL;
	}

	return &uvc_camera_config->device;
}


bk_err_t bk_uvc_camera_power_enable(uint8_t enable)
{
#if (CONFIG_CAMERA_POWER_GPIO_CTRL)
	if (enable)
	{
		gpio_dev_unmap(CAMERA_LDO_CTRL_GPIO);
		bk_gpio_set_capacity(CAMERA_LDO_CTRL_GPIO, 0);
		BK_LOG_ON_ERR(bk_gpio_disable_input(CAMERA_LDO_CTRL_GPIO));
		BK_LOG_ON_ERR(bk_gpio_enable_output(CAMERA_LDO_CTRL_GPIO));

#if (CAMERA_LDO_CTRL_ACTIVE_LEVEL)
		bk_gpio_set_output_high(CAMERA_LDO_CTRL_GPIO);
#else
		bk_gpio_set_output_low(CAMERA_LDO_CTRL_GPIO);
#endif

		delay_ms(5);
	}
	else
	{

#if (CAMERA_LDO_CTRL_ACTIVE_LEVEL)
		bk_gpio_set_output_low(CAMERA_LDO_CTRL_GPIO);
#else
		bk_gpio_set_output_high(CAMERA_LDO_CTRL_GPIO);
#endif
	}

#endif

	return BK_OK;
}

