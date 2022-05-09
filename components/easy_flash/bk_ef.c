#include <common/bk_include.h>
#include "bk_ef.h"
#include "base_64.h"
#include <os/mem.h>
#include <os/str.h>

#if CONFIG_EASY_FLASH
char *bk_get_env(const char *key)
{
	return ef_get_env(key);
}

EfErrCode bk_set_env(const char *key, const char *value)
{
	return ef_set_env(key, value);
}

EfErrCode bk_save_env(void)
{
	return ef_save_env();
}

EfErrCode bk_set_buf_env(const char *key, const char *buf, int len)
{
	EfErrCode result;
	unsigned char ret;
	unsigned char *ef_value;
	uint32_t buf_len;
	int out_len;

	if ((0 == len) || (NULL == key)
		|| (NULL == buf))
		return EF_ENV_INIT_FAILED;

	buf_len = base64_calc_encode_length(len) + 4;
	ef_value = os_zalloc(buf_len);
	if (NULL == ef_value)
		return EF_ENV_INIT_FAILED;

	ret = base64_encode((unsigned char *)buf, len, &out_len, ef_value);
	if (0 == ret) {
		os_free(ef_value);
		ef_value = NULL;
		return EF_ENV_INIT_FAILED;
	}

	result = ef_set_env(key, (char *)ef_value);

	os_free(ef_value);

	return result;
}

EfErrCode bk_get_buf_env(const char *key, const char *buf, int len)
{
	unsigned char *out_ptr;
	unsigned char ret;
	char *ef_value;
	int out_buf_len, count;
	int out_len, value_len;

	ef_value = ef_get_env(key);
	if (NULL == ef_value)
		return EF_ENV_NAME_ERR;

	value_len = os_strlen(ef_value);
	if (0 == value_len)
		return EF_ENV_INIT_FAILED;

	out_buf_len = base64_calc_decode_length((unsigned char *)ef_value, value_len);
	out_ptr = (unsigned char *)os_zalloc(out_buf_len + 4);
	if (NULL == out_ptr)
		return EF_ENV_INIT_FAILED;

	ret = base64_decode((unsigned char *)ef_value, value_len, &out_len, out_ptr);
	if (0 == ret) {
		os_free(out_ptr);
		out_ptr = NULL;
		return EF_ENV_INIT_FAILED;
	}

	if (out_len) {
		count = min(len, out_len);
		BK_ASSERT(len == out_len);

		os_memcpy((void *)buf, out_ptr, count);
	}

	os_free(out_ptr);

	return EF_NO_ERR;
}
#endif // CONFIG_EASY_FLASH

// eof

