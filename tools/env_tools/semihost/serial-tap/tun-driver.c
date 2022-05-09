#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "tun-driver.h"

int tun_alloc(char dev[IFNAMSIZ], short flags)
{
	// Interface request structure
	struct ifreq ifr;

	// File descriptor
	int fileDescriptor;

	// Open the tun device, if it doesn't exists return the error
	char *cloneDevice = "/dev/net/tun";
	if ((fileDescriptor = open(cloneDevice, O_RDWR)) < 0) {
		perror("open /dev/net/tun");
		return fileDescriptor;
	}

	// Initialize the ifreq structure with 0s and the set flags
	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_flags = flags;

	// If a device name is passed we should add it to the ifreq struct
	// Otherwise the kernel will try to allocate the next available
	// device of the given type
	if (*dev) {
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);
	}

	// Ask the kernel to create the new device
	int err = ioctl(fileDescriptor, TUNSETIFF, (void *) &ifr);
	if (err < 0) {
		// If something went wrong close the file and return
		perror("ioctl TUNSETIFF");
		close(fileDescriptor);
		return err;
	}

	// Write the device name back to the dev variable so the caller
	// can access it
	strcpy(dev, ifr.ifr_name);

	// Return the file descriptor
	return fileDescriptor;
}
