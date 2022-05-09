#include "cli.h"
#include "bk_saradc.h"
#include <driver/adc.h>
#include "adc_statis.h"
#include <os/os.h>
#include <os/os.h>

static void cli_adc_help(void)
{
	CLI_LOGI("adc_driver init/deinit\n");
	CLI_LOGI("adc_test [channel] [init/deinit/start/stop/dump_statis/get_value/single_step_mode_read\n");
	CLI_LOGI("adc_test [channel] [config] [mode] [src_clk] [adc_clk] [sature_mode] [sampl_rate]\n");
	CLI_LOGI("adc_test [channel] [read/rag_cb] [size] [time_out]\n");
	CLI_LOGI("adc_api_test [set_mode/get_mode/set_clk/set_sample_rate/set_channel/set_filter/\
		set_steady_time/set_sample_cnt/set_saturate\n");
}

static void cli_adc_register_cb(uint32_t param)
{
	CLI_LOGI("param:%d , adc isr cb \r\n", param);
}

static void cli_adc_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_adc_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_adc_driver_init());
		CLI_LOGI("adc driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_adc_driver_deinit());
		CLI_LOGI("adc driver deinit\n");
	} else {
		cli_adc_help();
		return;
	}
}

static void cli_adc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t adc_chan;

	if (argc < 2) {
		cli_adc_help();
		return;
	}

	adc_chan = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "config") == 0) {
		adc_config_t config = {0};
		os_memset(&config, 0, sizeof(adc_config_t));

		config.chan = adc_chan;

		config.adc_mode = os_strtoul(argv[3], NULL, 10);
		config.src_clk = os_strtoul(argv[4], NULL, 10);
		config.clk = os_strtoul(argv[5], NULL, 10);
		config.saturate_mode = os_strtoul(argv[6], NULL, 10);
		config.steady_ctrl= 7;
		config.adc_filter = 0;
		if(config.adc_mode == ADC_CONTINUOUS_MODE) {
			config.sample_rate = os_strtoul(argv[7], NULL, 10);
		}

		BK_LOG_ON_ERR(bk_adc_init(adc_chan));
		BK_LOG_ON_ERR(bk_adc_set_config(&config));
		CLI_LOGI("adc init test:adc_channel:%x, adc_mode:%x,src_clk:%x,adc_clk:%x, saturate_mode:%x, sample_rate:%x",
			adc_chan, config.adc_mode, config.src_clk, config.clk, config.saturate_mode, config.sample_rate);
	} else if (os_strcmp(argv[2], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_adc_deinit(adc_chan));
		CLI_LOGI("adc deinit test");
	} else if (os_strcmp(argv[2], "start") == 0) {
		BK_LOG_ON_ERR(bk_adc_set_channel(adc_chan));
		BK_LOG_ON_ERR(bk_adc_start());
		CLI_LOGI("start adc test\n");
	} else if (os_strcmp(argv[2], "stop") == 0) {
		BK_LOG_ON_ERR(bk_adc_stop());
		CLI_LOGI("adc_stop test: %d\n", adc_chan);
	} else if (os_strcmp(argv[2], "get_value") == 0) {
		uint16_t value = 0;
		float cali_value = 0;
		bk_adc_acquire();

		if(bk_adc_set_channel(adc_chan)) {
			bk_adc_release();
			return;
		}

		BK_LOG_ON_ERR(bk_adc_read(&value, ADC_READ_SEMAPHORE_WAIT_TIME));
		cali_value = saradc_calculate(value);
		bk_adc_release();
		CLI_LOGI("adc value:%d mv\n", (uint32_t)(cali_value * 1000));
	} else if (os_strcmp(argv[2], "read") == 0) {
		uint32_t sum = 0;
		uint32_t size = os_strtoul(argv[3], NULL, 10);
		uint16_t *recv_data = (uint16_t *)os_malloc(size);
		if (recv_data == NULL) {
			CLI_LOGE("recv buffer malloc failed\r\n");
			return;
		}

		int time_out = os_strtoul(argv[4], NULL, 10);
		if (time_out < 0) {
			time_out = BEKEN_WAIT_FOREVER;
		}

		bk_adc_acquire();

		if(bk_adc_set_channel(adc_chan)) {
			bk_adc_release();
			return;
		}

		BK_LOG_ON_ERR(bk_adc_read_raw(recv_data, size, time_out));
#if 0
		if((length > ADC_TEST_CNT_MAX) || (length == 0))
			return;
#endif

		bk_adc_release();

		if (!recv_data) {
			CLI_LOGE("adc read failed, recv_data is null \r\n");
		}
		CLI_LOGI("adc: read length :time_out:%d read_size:%d\n",time_out, size);

		for (int i = 0; i < size; i++) {
			sum = sum + recv_data[i];
			CLI_LOGI("recv_buffer[%d]=%02x, sum =%d\n", i, recv_data[i], sum);
		}
		sum = sum / size;
		CLI_LOGI("adc read size:%d, adc_result_from _data_reg:%d\n", size, sum);
		os_free(recv_data);
	} else if (os_strcmp(argv[2], "dump_statis") == 0) {
		adc_statis_dump();
		CLI_LOGI("adc dump statis ok\r\n");
	} else if (os_strcmp(argv[2], "single_step_mode_read") == 0) {
		bk_adc_acquire();
		bk_adc_set_channel(adc_chan);
		uint32_t ret = bk_adc_set_mode(ADC_SINGLE_STEP_MODE);
		if(ret) {
			bk_adc_release();
			return ;
		}

		uint16_t sum = 0;
		sum = bk_adc_read_raw(NULL, 1, 0);
		bk_adc_release();
		CLI_LOGI("adc single step mode: adc value is %d\n", sum);
	} else if (os_strcmp(argv[2], "reg_cb") == 0) {
		uint32_t size = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_adc_register_isr(cli_adc_register_cb, size));
		CLI_LOGI("adc isr cb register\r\n");
    }
    else if (0 == os_strcmp(argv[2], "saradc_val_read"))
    {
        uint32_t status;
        status=0;
        //status = manual_cal_load_adc_cali_flash();
        if (status != 0)
        {
            os_printf("Can't read cali value, use default!\r\n");
            os_printf("calibrate low value:[%x]\r\n", saradc_val.low);
            os_printf("calibrate high value:[%x]\r\n", saradc_val.high);
        }
    }
    else if(0 == os_strcmp(argv[2], "use_sample_set_saradc_val"))
    {
        uint16_t sample_value= 0;
        uint8_t ret;
        SARADC_MODE saradc_cal_mode;

        adc_config_t config = {0};
        os_memset(&config, 0, sizeof(adc_config_t));

        config.chan = adc_chan;

        config.adc_mode = os_strtoul(argv[3], NULL, 10);
        config.src_clk = os_strtoul(argv[4], NULL, 10);
        config.clk = os_strtoul(argv[5], NULL, 10);
        config.saturate_mode = os_strtoul(argv[6], NULL, 10);
        config.steady_ctrl= 7;
        config.adc_filter = 0;
        if(config.adc_mode == ADC_CONTINUOUS_MODE)
        {
            config.sample_rate = os_strtoul(argv[7], NULL, 10);
        }
        BK_LOG_ON_ERR(bk_adc_init(adc_chan));
        BK_LOG_ON_ERR(bk_adc_set_config(&config));
        CLI_LOGI("adc init test:adc_channel:%x, adc_mode:%x,src_clk:%x,adc_clk:%x, saturate_mode:%x, sample_rate:%x",
            adc_chan, config.adc_mode, config.src_clk, config.clk, config.saturate_mode, config.sample_rate);

        BK_LOG_ON_ERR(bk_adc_set_channel(adc_chan));

        if (0 == os_strcmp(argv[8], "low"))
        {
            saradc_cal_mode = SARADC_CALIBRATE_LOW;
        }
        else if (0 == os_strcmp(argv[3], "high"))
        {
            saradc_cal_mode = SARADC_CALIBRATE_HIGH;
        }
        else
        {
            os_printf("invalid parameter\r\n");
            return;
        }

        BK_LOG_ON_ERR(bk_adc_start());
        CLI_LOGI("start adc test\n");

        bk_adc_acquire();
        BK_LOG_ON_ERR(bk_adc_read(&sample_value, ADC_READ_SEMAPHORE_WAIT_TIME));

        ret = saradc_set_calibrate_val(&sample_value, saradc_cal_mode);
        if(ret == SARADC_FAILURE)
        {
            os_printf("saradc_set_calibrate_val fail\r\n");
        }
        os_printf("saradc_set_calibrate_val success\r\n");
        os_printf("mode:[%s] value:[%d]\r\n", (saradc_cal_mode ? "high" : "low"), sample_value);
        //BK_LOG_ON_ERR(bk_adc_stop());
    }
    else
    {
        cli_adc_help();
        return;
    }
}

static void cli_adc_api_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_adc_help();
		return;
	}

	if (os_strcmp(argv[1], "set_clk") == 0) {
		uint32_t clk = os_strtoul(argv[2], NULL, 10);
		uint32_t src_clk = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_adc_set_clk(src_clk, clk));
		CLI_LOGI("adc set clk: clk_src:%d, clk:%d", src_clk, clk);
	} else if (os_strcmp(argv[1], "set_mode") == 0) {
		uint32_t mode = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(bk_adc_set_mode(mode));
		CLI_LOGI("adc set_mode: %x", mode);
	} else if (os_strcmp(argv[1], "set_channel") == 0) {
		uint32_t adc_chan = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(bk_adc_set_channel(adc_chan));
		CLI_LOGI("adc %d test", adc_chan);
	}else if (os_strcmp(argv[1], "set_sample_rate") == 0) {
		uint32_t sample_rate = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(bk_adc_set_sample_rate(sample_rate));
		CLI_LOGI("adc sample rate: %x", sample_rate);
	} else if (os_strcmp(argv[1], "set_filter") == 0) {
		uint32_t filter = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(bk_adc_set_filter(filter));
		CLI_LOGI("adc set_filter : %x", filter);
	} else if (os_strcmp(argv[1], "set_steady_time") == 0) {
		uint32_t time = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(bk_adc_set_steady_time(time));
		CLI_LOGI("adc set_steady time : %x", time);
	}else if (os_strcmp(argv[1], "set_sample_cnt") == 0) {
		uint32_t sample_cnt = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(bk_adc_set_sample_cnt(sample_cnt));
		CLI_LOGI("adc set_sample_cnt: %x", sample_cnt);
	} else if (os_strcmp(argv[1], "set_saturate") == 0) {
		uint32_t saturate = os_strtoul(argv[2], NULL, 10);
		BK_LOG_ON_ERR(bk_adc_set_saturate_mode(saturate));
		CLI_LOGI("adc set_set_saturate: %x", saturate);
	} else if (os_strcmp(argv[1], "get_mode") == 0) {
		uint32_t mode = 0;
		mode = bk_adc_get_mode();
		CLI_LOGI("adc get_mode: %x", mode);
	} else {
		cli_adc_help();
		return;
	}
}

#define ADC_CMD_CNT (sizeof(s_adc_commands) / sizeof(struct cli_command))
static const struct cli_command s_adc_commands[] = {
	{"adc_driver", "adc_driver [init/deinit]", cli_adc_driver_cmd},
	{"adc_test", "adc_test  [channel] [start/stop/dump_statis]", cli_adc_cmd},
	{"adc_api_test", "adc_api_test []", cli_adc_api_cmd},

};

int cli_adc_init(void)
{
	return cli_register_commands(s_adc_commands, ADC_CMD_CNT);
}
