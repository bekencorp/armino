#include <common/bk_include.h>
#include "video_demo_config.h"
#include <components/video_transfer.h>
#include "cli.h"

#if (CONFIG_NET_WORK_VIDEO_TRANSFER == 1)
#include <os/str.h>
#include "bk_uart.h"
#include <os/mem.h>

#include <os/os.h>

#define VIDE_MODE_STATION        (0x1)
#define VIDE_MODE_SOFTAP         (0x2)
#define VIDE_MODE_P2P            (0x4)
extern void app_demo_sta_start(char *oob_ssid, char *connect_key);
extern void app_demo_sta_exit(void);

extern void app_demo_softap_start(char *oob_ssid, char *connect_key);
extern void app_demo_softap_exit(void);

extern void app_demo_p2p_start(char *oob_ssid, char *connect_key);
extern void app_demo_p2p_exit(void);

media_camera_device_t device =
{
	.camera_type = MEDIA_DVP_MJPEG,
	.ppi = PPI_640X480,
	.fps = FPS20,
}; /**< config of camera */

media_camera_device_t *get_camera_device(void)
{
	return &device;
}


media_camera_type_t cmd_camera_type(int argc, char **argv)
{
	int i;
	media_camera_type_t type = MEDIA_CAMERA_UNKNOW;

	for (i = 0; i < argc; i++)
	{
		if (os_strcmp(argv[i], "uvc_jpg") == 0)
		{
			type = MEDIA_UVC_MJPEG;
		}

		if (os_strcmp(argv[i], "uvc_h264") == 0)
		{
			type = MEDIA_UVC_H264;
		}

		if (os_strcmp(argv[i], "dvp_jpg") == 0)
		{
			type = MEDIA_DVP_MJPEG;
		}

		if (type != MEDIA_CAMERA_UNKNOW)
			break;
	}

	if (type == MEDIA_CAMERA_UNKNOW)
		type = MEDIA_DVP_MJPEG;

	return type;
}

media_ppi_t cmd_camera_ppi(int argc, char **argv)
{
	int i;
	media_ppi_t ppi = PPI_DEFAULT;

	for (i = 0; i < argc; i++)
	{
		ppi = get_string_to_ppi(argv[i]);
		if (ppi != PPI_DEFAULT)
			break;
	}

	if (ppi == PPI_DEFAULT)
	{
		ppi = PPI_640X480;
	}

	return ppi;
}

sensor_fps_t cmd_camera_fps(int argc, char **argv)
{
	int i;
	sensor_fps_t fps = FPS0;

	for (i = 0; i < argc; i++)
	{
		fps = get_string_to_fps(argv[i]);
		if (fps != FPS0)
			break;
	}

	if (fps == FPS0)
	{
		fps = FPS20;
	}

	return fps;
}

void video_transfer_usage(void)
{
	os_printf("Usage: video_transfer [-s|-a ssid key]\n");
	os_printf("       video_transfer [-h|-stop]\n");
	os_printf("\n");
	os_printf("Sation mode:\n");
	os_printf("  -s           connect to specific AP with ssid & key\n");
	os_printf("\n");
	os_printf("Softap mode:\n");
	os_printf("  -a           startup softap with specific ssid & key\n");
	os_printf("\n");
	os_printf("Miscellaneous:\n");
	os_printf("  -h           print this message and quit\n");
	os_printf("  --stop       stop \n");

	return;
}

void cmd_video_transfer(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	char *oob_ssid = NULL, *tmp_oob_ssid = "beken p2p";
	char *connect_key = NULL;
	static UINT32 video_transfer_mode = 0;

	if (argc > 3)
	{
		device.camera_type = cmd_camera_type(argc - 3, argv + 3);

		device.fps = cmd_camera_fps(argc - 3, argv + 3);

		device.ppi = cmd_camera_ppi(argc - 3, argv + 3);
	}

	if (argc == 1)
	{
		goto __usage;
	}
	else if (os_strcmp(argv[1], "-h") == 0)
	{
		goto __usage;
	}
	else if (os_strcmp(argv[1], "-stop") == 0)
	{
		os_printf("not implement!\n");
	}
	else if (os_strcmp(argv[1], "-a") == 0)
	{

#if (APP_VIDEO_TRANSFER_MODE & VIDEO_TRANSFER_AP_MODE)

		if (argc < 3)
		{
			goto __usage;
		}

		if ((video_transfer_mode & VIDE_MODE_SOFTAP) == 0)
		{
			oob_ssid = argv[2];
			if (argc >= 4 && os_strlen(argv[3]) >= 8)
			{
				/* video_transfer -s ssid key [options] */
				connect_key = argv[3];
			}

			app_demo_softap_start(oob_ssid, connect_key);
			video_transfer_mode |= VIDE_MODE_SOFTAP;
		}
		else
		{
			os_printf("softap mode aready started\n");
		}
#else
		os_printf("APP_VIDEO_TRANSFER_MODE not support softap mode \n");
#endif

	}
	else if (os_strcmp(argv[1], "-s") == 0)
	{

#if (APP_VIDEO_TRANSFER_MODE & VIDEO_TRANSFER_STA_MODE)
		if (argc < 3)
		{
			goto __usage;
		}

		if ((video_transfer_mode & VIDE_MODE_STATION) == 0)
		{
			oob_ssid = argv[2];
			if (argc >= 4 && os_strlen(argv[3]) >= 8)
			{
				/* video_transfer -s ssid key [options] */
				connect_key = argv[3];
			}
			app_demo_sta_start(oob_ssid, connect_key);
			video_transfer_mode |= VIDE_MODE_STATION;
		}
		else
		{
			os_printf("station mode aready started\n");
		}
#else
		os_printf("APP_VIDEO_TRANSFER_MODE not support station mode \n");
#endif

	}
	 else if (os_strcmp(argv[1], "-p") == 0)
	{

#if (APP_VIDEO_TRANSFER_MODE & VIDEO_TRANSFER_P2P_MODE)
		if (argc < 3)
		{
			goto __usage;
		}

		if ((video_transfer_mode & VIDE_MODE_P2P) == 0)
		{
			if (argv[2][0] == '0')
				oob_ssid = tmp_oob_ssid;
			else
				oob_ssid = argv[2];

			if (argc >= 4 && os_strlen(argv[3]) >= 8)
			{
				/* video_transfer -s ssid key [options] */
				connect_key = argv[3];
			}

#if CONFIG_P2P
			app_demo_p2p_start(oob_ssid, connect_key);
			video_transfer_mode |= VIDE_MODE_P2P;
#endif
		}
		else
		{
			os_printf("p2p mode aready started\n");
		}
#else
		os_printf("APP_VIDEO_TRANSFER_MODE not support station mode \n");
#endif

	}
	else if (os_strcmp(argv[1], "start") == 0)
	{
		bk_video_transfer_start();
	}
	else if (os_strcmp(argv[1], "stop") == 0)
	{
		bk_video_transfer_stop();
	}
	else
	{
		goto __usage;
	}

	msg = CLI_CMD_RSP_SUCCEED;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;

__usage:
	video_transfer_usage();
	msg = CLI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

#endif

