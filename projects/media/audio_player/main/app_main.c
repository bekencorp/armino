#include "bk_private/legacy_init.h"
#include <components/system.h>
#include <os/os.h>
#include <components/shell_task.h>
#include "cli.h"
#include "audio_player.h"
#include "netstream_player.h"
#include <components/aud_intf.h>
#include <components/aud_intf_types.h>


extern uint8_t netstream_download_stop_flag;
extern void user_app_main(void);
extern void rtos_set_user_app_entry(beken_thread_function_t entry);

uint32_t current_song, song_number;

const char *song_list[] = {
	"http://dl.bekencorp.com/d/test/2023/music/qingtian.mp3?sign=YzsQI3YhI1QU6KQ9trGAzMo6OHJxAo0aMVNQtr2DLsg=:0",
	"http://dl.bekencorp.com/d/test/2023/music/chengdu.mp3?sign=WSKOehaDFGv8csOmTEGjHXHLaygTurgT5NEgjgw4jCY=:0",
	"http://dl.bekencorp.com/d/test/2023/music/myheart.mp3?sign=R9SLp-MvEEkiauuuKDmPsyV_74wePmKsRdJbFWw4mBo=:0",
};

#define CMDS_COUNT  (sizeof(s_audio_player_commands) / sizeof(struct cli_command))

void cli_audio_player_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;

	if (os_strcmp(argv[1], "start") == 0) {
		audio_player_start(song_list[current_song - 1]);
	} else if (os_strcmp(argv[1], "pause") == 0) {
		audio_player_pause();
	} else if (os_strcmp(argv[1], "play") == 0) {
		audio_player_play();
	} else if (os_strcmp(argv[1], "set_volume") == 0) {
		uint8_t value = os_strtoul(argv[2], NULL, 0); // The range of value is 0x0 ~ 0x3F.
		audio_player_set_volume(value);
	} else if (os_strcmp(argv[1], "last") == 0) {
		audio_player_last();
	} else if (os_strcmp(argv[1], "next") == 0) {
		audio_player_next();
	} else if (os_strcmp(argv[1], "close") == 0) {
		ret = audio_player_deinit();
		if (ret != BK_OK)	{
			os_printf("audio player deinit fail\r\n");
			return;
		}
	}
}

static const struct cli_command s_audio_player_commands[] =
{
	{"audio_player", "audio_player", cli_audio_player_cmd},
};

int cli_audio_player_init(void)
{
	return cli_register_commands(s_audio_player_commands, CMDS_COUNT);
}

void user_app_main(void)
{
	bk_err_t ret = BK_OK;

	current_song = 1;

	song_number = sizeof(song_list) / sizeof(song_list[0]);

	os_printf("current_song: %d, song_number: %d\r\n", current_song, song_number);

	ret = audio_player_init();
	if (ret != BK_OK)	{
		os_printf("audio player init fail\r\n");
		return;
	}

	ret = netstream_player_init();
	if (ret != BK_OK)	{
		os_printf("netstream player init fail\r\n");
		return;
	}
}

int main(void)
{
#if (!CONFIG_SLAVE_CORE)
	rtos_set_user_app_entry((beken_thread_function_t)user_app_main);
	// bk_set_printf_sync(true);
	// shell_set_log_level(BK_LOG_WARN);
#endif
	legacy_init();

#if (!CONFIG_SLAVE_CORE)
	cli_audio_player_init();
#endif

	return 0;
}
