#include <common/bk_include.h>
#include "param_manage.h"
#include "bk_ef.h"

#if EASY_FLASH_DEMO
T_STRUCT_T tst = {
	.magic = TEST_MAGIC,
};

uint32_t pm_flag = 0;

uint32_t pm_init(void)
{
	#if CFG_EASY_FLASH
	easyflash_init();
	#endif
	
	tst.a = -1;
	tst.b = 2;
	tst.c = 0x9876;
	tst.resv = 0x5a5abbbb;
	
	return 0;
}

uint32_t pm_save(void)
{	
	EfErrCode ret;

	ret = bk_save_env();
	os_printf("bk_save_env\r\n");
	if(EF_NO_ERR == ret)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

uint32_t pm_read(void)

{
	char *flag;
	EfErrCode ret;
	char *ef_value;
	T_STRUCT_T tst_tmp = {0};

	flag = bk_get_env(FLAG_KEY_1);
	if(NULL == flag)
	{
		return 1;
	}

	os_printf("[rd]key:%s ", FLAG_KEY_1);
	ef_value = bk_get_env(FLAG_KEY_1);
	if(ef_value)
	{
		os_printf("value:%s", ef_value);
	}
	os_printf("\r\n");

	os_printf("[rd]key:%s ", TEST_KEY_2);
	ef_value = bk_get_env(TEST_KEY_2);
	if(ef_value)
	{
		os_printf("value:%s", ef_value);
	}
	os_printf("\r\n");
	
	os_printf("[rd]key:%s ", TEST_KEY_3);
	ef_value = bk_get_env(TEST_KEY_3);
	if(ef_value)
	{
		os_printf("value:%s", ef_value);
	}
	os_printf("\r\n");

	os_printf("[rd]key:%s ", TEST_KEY_4);
	ef_value = bk_get_env(TEST_KEY_4);
	if(ef_value)
	{
		os_printf("value:%s", ef_value);
	}
	os_printf("\r\n");

	os_printf("[rd]key:%s ", TEST_KEY_5);
	ef_value = bk_get_env(TEST_KEY_5);
	if(ef_value)
	{
		os_printf("value:%s", ef_value);
	}
	os_printf("\r\n");

	os_printf("[rd]key:%s ", TEST_KEY_6);
	ef_value = bk_get_env(TEST_KEY_6);
	if(ef_value)
	{
		os_printf("value:%s", ef_value);
	}
	os_printf("\r\n");

	os_printf("[rd]key:%s ", TEST_KEY_7);
	ef_value = bk_get_env(TEST_KEY_7);
	if(ef_value)
	{
		os_printf("value:%s", ef_value);
	}
	os_printf("\r\n");

	os_printf("[rd]key:%s ", TEST_KEY_8);
	ef_value = bk_get_env(TEST_KEY_8);
	if(ef_value)
	{
		os_printf("value:%s", ef_value);
	}
	os_printf("\r\n");

	os_printf("[rd]key:%s ", TEST_KEY_9);
	ef_value = bk_get_env(TEST_KEY_9);
	if(ef_value)
	{
		os_printf("value:%s", ef_value);
	}
	os_printf("\r\n");
		
	os_printf("[rd]key:%s \r\n", TEST_KEY_10);
	ret = bk_get_buf_env(TEST_KEY_10, (void *)&tst_tmp, sizeof(tst_tmp));
	if(EF_NO_ERR == ret)
	{
		os_printf("              tst_tmp.a:0x%1x \r\n", tst_tmp.a);
		os_printf("              tst_tmp.b:0x%1x \r\n", tst_tmp.b);
		os_printf("              tst_tmp.c:%hd \r\n", tst_tmp.c);
		os_printf("              tst_tmp.magic:0x%x \r\n", tst_tmp.magic);
		os_printf("              tst_tmp.resv:0x%x \r\n", tst_tmp.resv);
	}
	os_printf("\r\n");
	
	return 0;
}

uint32_t pm_write(void)
{
	char *flag;
	EfErrCode ret;
	T_STRUCT_T tst_tmp = {0};

	os_printf("[wr]key:%s ", FLAG_KEY_1);
	ret = bk_set_env(FLAG_KEY_1, FLAG_KEY_1);
	if(EF_NO_ERR == ret)
	{
		os_printf("value:%s", FLAG_KEY_1);
	}
	os_printf("\r\n");

	os_printf("[wr]key:%s ", TEST_KEY_2);
	ret = bk_set_env(TEST_KEY_2, TEST_KEY_2);
	if(EF_NO_ERR == ret)
	{
		os_printf("value:%s", TEST_KEY_2);
	}
	os_printf("\r\n");

	os_printf("[wr]key:%s ", TEST_KEY_3);
	ret = bk_set_env(TEST_KEY_3, TEST_KEY_3);
	if(EF_NO_ERR == ret)
	{
		os_printf("value:%s", TEST_KEY_3);
	}
	os_printf("\r\n");

	os_printf("[wr]key:%s ", TEST_KEY_4);
	ret = bk_set_env(TEST_KEY_4, TEST_KEY_4);
	if(EF_NO_ERR == ret)
	{
		os_printf("value:%s", TEST_KEY_4);
	}
	os_printf("\r\n");

	os_printf("[wr]key:%s ", TEST_KEY_5);
	ret = bk_set_env(TEST_KEY_5, TEST_KEY_5);
	if(EF_NO_ERR == ret)
	{
		os_printf("value:%s", TEST_KEY_5);
	}
	os_printf("\r\n");

	os_printf("[wr]key:%s ", TEST_KEY_6);
	ret = bk_set_env(TEST_KEY_6, TEST_KEY_6);
	if(EF_NO_ERR == ret)
	{
		os_printf("value:%s", TEST_KEY_6);
	}
	os_printf("\r\n");

	os_printf("[wr]key:%s ", TEST_KEY_7);
	ret = bk_set_env(TEST_KEY_7, TEST_KEY_7);
	if(EF_NO_ERR == ret)
	{
		os_printf("value:%s", TEST_KEY_7);
	}
	os_printf("\r\n");

	os_printf("[wr]key:%s ", TEST_KEY_8);
	ret = bk_set_env(TEST_KEY_8, TEST_KEY_8);
	if(EF_NO_ERR == ret)
	{
		os_printf("value:%s", TEST_KEY_8);
	}
	os_printf("\r\n");

	os_printf("[wr]key:%s ", TEST_KEY_9);
	ret = bk_set_env(TEST_KEY_9, TEST_KEY_9);
	if(EF_NO_ERR == ret)
	{
		os_printf("value:%s", TEST_KEY_9);
	}
	os_printf("\r\n");
		
	os_printf("[wr]key:%s \r\n", TEST_KEY_10);
	ret = bk_set_buf_env(TEST_KEY_10, (void *)&tst, sizeof(tst));
	if(EF_NO_ERR == ret)
	{
		os_printf("              tst.a:0x%1x \r\n", tst.a);
		os_printf("              tst.b:0x%1x \r\n", tst.b);
		os_printf("              tst.c:%hd \r\n", tst.c);
		os_printf("              tst.magic:0x%x \r\n", tst.magic);
		os_printf("              tst.resv:0x%x \r\n", tst.resv);
	}
	os_printf("\r\n");
	
	return 0;
}

uint32_t pm_main(void)
{
	os_printf("param_manage_demo\r\n");
	
	pm_init();

	if(pm_read())
	{
		pm_write();
		pm_save();
	}
	
	return 0;
}

int demo_start(void)
{
	return pm_main();
}

#endif

// eof

