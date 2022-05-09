#pragma once
/**
 * Create a new TUN adapter
 * @param dev       The new adapter's path
 * @param flags     The new adapter's flag
 * @return The file descriptor to communicate with the device
 */
int tun_alloc(char dev[IFNAMSIZ], short flags);
