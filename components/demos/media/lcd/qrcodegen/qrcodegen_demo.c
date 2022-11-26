// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
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
#include <os/str.h>
#include "modules/qrcodegen.h"
#if (CONFIG_FATFS)
#include "ff.h"
#include "diskio.h"
#endif

static char *g_qr_name;
static void printQr(const uint8_t qrcode[], uint8_t scale, uint8_t border)
{
#if (CONFIG_FATFS)

	char file_name[50];
	FIL qr_file;
	FRESULT fr;
	uint32 uiTemp = 0;

	sprintf(file_name, "%d:/%s", DISK_NUMBER_SDIO_SD, g_qr_name);
	fr = f_open(&qr_file, file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if (fr != FR_OK) {
		os_printf("open %s failed!\r\n", file_name);
		return;
	}
	os_printf("file open successfully!\r\n");

	int size = qrcodegen_getSize(qrcode);
	os_printf("size = %d\r\n", size);
	for (int y = -border; y < size + border; y++) {
		for (int j = 0; j < scale; j++) {
			for (int x = -border; x < size + border; x++) {
				for (int i = 0; i < scale; i++) {
					uint32_t color = qrcodegen_getModule(qrcode, x, y) ? 0x0000 : 0xFFFF;
					fr = f_write(&qr_file, &color, 2, &uiTemp);
					if (fr != FR_OK) {
						os_printf("write file failed.\r\n");
						return;
					}
				}
			}
		}
	}

	fr = f_close(&qr_file);
	if (fr != FR_OK) {
		os_printf("close %s fail!\r\n", file_name);
		return;
	}
#else
	os_printf("Not Support SDCard\r\n");
#endif
}

void cli_qrcodegen_text_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc != 3) {
		os_printf("qrcodegen_text_test {xxx.rgb} {scale} \r\n");
		return;
	}

	g_qr_name = argv[1];
	uint8_t scale = os_strtoul(argv[2], NULL, 10) & 0xFF;

	const char *text = "ACLSEMI";                // User-supplied text
	enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;  // Error correction level

	// Make and print the QR Code symbol
	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode, errCorLvl,
		qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
	if (ok) {
		os_printf("qr_code generate OK!\r\n");
		printQr(qrcode, scale, 4);
	}

}

