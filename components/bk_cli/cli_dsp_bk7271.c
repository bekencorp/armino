#include <common/bk_include.h>
#include "stdarg.h"
#include <os/mem.h>
#include "sys_rtos.h"
#include <os/os.h>
#include <common/bk_kernel_err.h>

#include "bk_sys_ctrl.h"
#include "bk_cli.h"
#include "bk_mailbox.h"
#include <driver/mailbox.h>
#include "ff.h"
#include "diskio.h"
#include "audio_pub.h"
#include <os/str.h>
#include <components/log.h>
#include "cli.h"
#include <driver/mailbox.h>

#if CONFIG_USB_HOST
static dma_buffer_node record_buffer_nodes[DAC_PLAY_NODE_MAX];
struct adc_dac_context g_record_context;
volatile int record_flag = 0;
volatile int thread_flag = 0;
beken_thread_t usb_record_thread_handle;
beken_semaphore_t usb_record_sem = NULL;
beken_semaphore_t usb_play_sem = NULL;
FIL record_file;

FATFS *pfs = NULL;
uint8_t mount_flag = 0;

static const unsigned long PCM_8000[] = {
	0x00010000, 0x5A825A81, 0x7FFF7FFF, 0x5A825A83, 0x00000000, 0xA57FA57E, 0x80018002, 0xA57EA57E,
};

static const unsigned long PCM_16000[] = {
	0x00000001, 0x30FC30FA, 0x5A815A82, 0x76417641, 0x7FFF7FFF, 0x76417641, 0x5A815A82, 0x30FB30FB,
	0x00000000, 0xCF05CF04, 0xA57EA57E, 0x89BF89BF, 0x80018000, 0x89C089BF, 0xA57EA57F, 0xCF05CF04,
};

static const unsigned long PCM_44100[] = {
	0x00000000, 0x122D122E, 0x23FC23FB, 0x350F350F, 0x450F4510, 0x53AA53AA, 0x60926092, 0x6B866B85,
	0x744A744A, 0x7AB57AB5, 0x7EA37EA2, 0x7FFF7FFF, 0x7EC37EC4, 0x7AF77AF6, 0x74AB74AC, 0x6C036C04,
	0x612B612A, 0x5459545A, 0x45D445D3, 0x35E335E3, 0x24DB24DA, 0x13141313, 0x00EA00EA, 0xEEBAEEBB,
	0xDCE5DCE4, 0xCBC5CBC6, 0xBBB6BBB5, 0xAD08AD08, 0xA008A008, 0x94FA94FA, 0x8C188C17, 0x858E858E,
	0x81818181, 0x80038003, 0x811D811D, 0x84C984CA, 0x8AF58AF5, 0x93809381, 0x9E3E9E3E, 0xAAF7AAF7,
	0xB969B96A, 0xC94AC94A, 0xDA46DA46, 0xEC05EC06, 0xFE2DFE2E, 0x105E105E, 0x223B223B, 0x33653365,
	0x43854384, 0x52465246, 0x5F5D5F5D, 0x6A856A85, 0x73847384, 0x7A2C7A2C, 0x7E5B7E5B, 0x7FFA7FFA,
	0x7F007F01, 0x7B747B75, 0x75697569, 0x6CFB6CFB, 0x62586258, 0x55B755B7, 0x47594759, 0x37893789,
	0x26992699, 0x14E114E1, 0x02BC02BC, 0xF08AF08A, 0xDEA6DEA6, 0xCD72CD72, 0xBD42BD43, 0xAE6DAE6D,
	0xA13FA13F, 0x95FD95FD, 0x8CE18CE2, 0x86198619, 0x81CA81CB, 0x800B800C, 0x80E380E3, 0x844E844E,
	0x8A3C8A3C, 0x928B928B, 0x9D149D13, 0xA99CA99C, 0xB7E6B7E6, 0xC7A4C7A5, 0xD889D888, 0xEA39EA39,
	0xFC5AFC5A, 0x0E8F0E8F, 0x20782077, 0x31B831B8, 0x41F641F6, 0x50DF50DF, 0x5E235E23, 0x697F697F,
	0x72B972B8, 0x799F799E, 0x7E0D7E0D, 0x7FEE7FEE, 0x7F387F38, 0x7BED7BED, 0x761F761F, 0x6DEC6DED,
	0x63806380, 0x570F5710, 0x48DB48DA, 0x392C392C, 0x28552855, 0x16AC16AD, 0x048F048F, 0xF25AF259,
	0xE06BE06B, 0xCF1FCF1F, 0xBED2BED3, 0xAFD8AFD7, 0xA27CA27B, 0x97059706, 0x8DB08DB1, 0x86AB86AB,
	0x821C821C, 0x801A801A, 0x80B080AF, 0x83DA83DA, 0x89888988, 0x919C919C, 0x9BED9BEE, 0xA847A846,
	0xB666B666, 0xC603C603, 0xD6CED6CD, 0xE86DE86D, 0xFA89FA87, 0x0CBE0CBF, 0x1EB31EB3, 0x30083007,
	0x40644064, 0x4F734F73, 0x5CE55CE4, 0x68756874, 0x71E771E7, 0x790A790A, 0x7DB87DB9, 0x7FDC7FDD,
	0x7F677F68, 0x7C5F7C5E, 0x76CF76D0, 0x6ED96EDA, 0x64A364A3, 0x58635863, 0x4A594A58, 0x3ACD3ACC,
	0x2A102A0F, 0x18781878, 0x06610661, 0xF429F42A, 0xE230E22F, 0xD0D0D0D0, 0xC066C066, 0xB145B145,
	0xA3BCA3BD, 0x98149813, 0x8E848E85, 0x87438743, 0x82738273, 0x802F8030, 0x80848083, 0x836B836B,
	0x88DA88DB, 0x90B390B3, 0x9ACE9ACD, 0xA6F5A6F5, 0xB4EAB4EA, 0xC464C465, 0xD515D515, 0xE6A3E6A3,
	0xF8B6F8B6, 0x0AED0AEE, 0x1CEC1CED, 0x2E562E57, 0x3ED03ED0, 0x4E024E03, 0x5BA15BA0, 0x67646764,
	0x710E710F, 0x786F786E, 0x7D5E7D5E, 0x7FC37FC3, 0x7F917F91, 0x7CCA7CC9, 0x777A777A, 0x6FBF6FC0,
	0x65C165C2, 0x59B259B3, 0x4BD34BD3, 0x3C693C69, 0x2BC62BC7, 0x1A411A40, 0x08330834, 0xF5FBF5FB,
	0xE3F6E3F7, 0xD283D284, 0xC1FCC1FD, 0xB2B7B2B8, 0xA503A503, 0x99279926, 0x8F608F60, 0x87E187E2,
	0x82D282D2, 0x804D804C, 0x805D805C, 0x83038304, 0x88338833, 0x8FCF8FCF, 0x99B299B1, 0xA5A8A5A8,
	0xB371B372, 0xC2C8C2CA, 0xD35ED35E, 0xE4DAE4DB, 0xF6E4F6E4, 0x091C091C, 0x1B261B25, 0x2CA22CA2,
	0x3D373D37, 0x4C8E4C8E, 0x5A595A59, 0x664E664E, 0x70307030, 0x77CD77CE, 0x7CFD7CFD, 0x7FA37FA3,
	0x7FB37FB4, 0x7D2E7D2E, 0x781F781F, 0x70A170A0, 0x66DA66DA, 0x5AFE5AFD, 0x4D494D49, 0x3E033E04,
	0x2D7D2D7D, 0x1C091C0A, 0x0A050A05, 0xF7CCF7CD, 0xE5BEE5BE, 0xD439D439, 0xC396C396, 0xB42DB42D,
	0xA64DA64E, 0x9A3E9A3E, 0x90409040, 0x88868886, 0x83378337, 0x806E806E, 0x803D803E, 0x82A382A1,
	0x87928791, 0x8EF18EF2, 0x989C989C, 0xA45FA45F, 0xB1FEB1FD, 0xC131C131, 0xD1AAD1AA, 0xE313E313,
	0xF512F512, 0x074A074B, 0x195D195C, 0x2AEC2AEC, 0x3B9C3B9C, 0x4B164B16, 0x590C590B, 0x65336532,
	0x6F4D6F4C, 0x77267726, 0x7C957C95, 0x7F7D7F7D, 0x7FD17FD1, 0x7D8C7D8C, 0x78BD78BC, 0x717B717A,
	0x67ED67ED, 0x5C445C43, 0x4EBB4EBB, 0x3F9B3F9A, 0x2F302F30, 0x1DD01DD0, 0x0BD60BD7, 0xF99FF99E,
	0xE788E789, 0xD5F1D5F1, 0xC533C533, 0xB5A8B5A7, 0xA79DA79D, 0x9B5D9B5D, 0x91289127, 0x89308930,
	0x83A283A2, 0x80988098, 0x80248024, 0x82468247, 0x86F786F6, 0x8E1A8E19, 0x978C978C, 0xA31CA31C,
	0xB08EB08D, 0xBF9CBF9B, 0xCFF7CFF8, 0xE14DE14C, 0xF342F342, 0x05780578, 0x17931792, 0x29332933,
	0x39FE39FD, 0x499B499A, 0x57B957BA, 0x64126413, 0x6E636E64, 0x76787678, 0x7C277C27, 0x7F517F50,
	0x7FE67FE5, 0x7DE47DE3, 0x79557955, 0x72517250, 0x68FA68FB, 0x5D855D84, 0x50295029, 0x412E412E,
	0x30E030E1, 0x1F961F95, 0x0DA70DA6, 0xFB71FB70, 0xE954E953, 0xD7ABD7AB, 0xC6D4C6D3, 0xB726B725,
	0xA8F1A8F1, 0x9C809C80, 0x92139213, 0x89E189E0, 0x84138413, 0x80C880C9, 0x80128011, 0x81F381F3,
	0x86618663, 0x8D478D48, 0x96819680, 0xA1DDA1DD, 0xAF22AF22, 0xBE0ABE0A, 0xCE48CE48, 0xDF88DF89,
	0xF172F171, 0x03A603A5, 0x15C715C8, 0x27782777, 0x385B385B, 0x481B481A, 0x56635664, 0x62ED62EC,
	0x6D746D75, 0x75C475C5, 0x7BB17BB2, 0x7F1C7F1C, 0x7FF57FF4, 0x7E347E35, 0x79E679E6, 0x731F731F,
	0x6A046A03, 0x5EC15EC1, 0x51935194, 0x42BE42BE, 0x328E328F, 0x21592159, 0x0F770F77, 0xFD43FD44,
	0xEB20EB1F, 0xD967D966, 0xC877C877, 0xB8A8B8A7, 0xAA49AA49, 0x9DA89DA9, 0x93059306, 0x8A988A98,
	0x848B848B, 0x80FF80FF, 0x80068006, 0x81A581A4, 0x85D485D3, 0x8C7B8C7C, 0x957B957A, 0xA0A4A0A2,
	0xADBAADBA, 0xBC7BBC7B, 0xCC9CCC9C, 0xDDC6DDC6, 0xEFA2EFA2, 0x01D201D3, 0x13FB13FB, 0x25BA25BA,
	0x36B636B7, 0x46964697, 0x550A5509, 0x61C261C2, 0x6C806C7F, 0x750B750A, 0x7B377B36, 0x7EE37EE2,
	0x7FFD7FFE, 0x7E7F7E80, 0x7A717A71, 0x73E973E8, 0x6B066B06, 0x5FF85FF8, 0x52F952F8, 0x444A444B,
	0x343B343A, 0x231B231A, 0x11461146, 0xFF17FF17, 0xECEDECEC, 0xDB26DB25, 0xCA1DCA1E, 0xBA2CBA2D,
	0xABA6ABA6, 0x9ED59ED5, 0x93FD93FD, 0x8B558B55, 0x85098509, 0x813C813D, 0x80018001, 0x815E815E,
	0x854B854C, 0x8BB58BB5, 0x947B947B, 0x9F6D9F6F, 0xAC56AC57, 0xBAF1BAF0, 0xCAF1CAF1, 0xDC05DC04,
	0xEDD2EDD4,
};

static const unsigned long PCM_48000[] = {
	0x0000FFFF, 0x10B510B5, 0x21202121, 0x30FB30FC, 0x40003FFF, 0x4DEC4DEC, 0x5A825A82, 0x658C658C,
	0x6EDA6ED9, 0x76417641, 0x7BA37BA2, 0x7EE77EE7, 0x7FFF7FFF, 0x7EE67EE6, 0x7BA27BA3, 0x76417641,
	0x6ED86EDA, 0x658C658B, 0x5A825A82, 0x4DEB4DEB, 0x3FFF3FFF, 0x30FC30FB, 0x21202120, 0x10B510B5,
	0x00010000, 0xEF4AEF4B, 0xDEDFDEDF, 0xCF05CF05, 0xC000C000, 0xB214B215, 0xA57EA57F, 0x9A759A74,
	0x91269127, 0x89C089BF, 0x845D845D, 0x811A8119, 0x80018001, 0x811A811A, 0x845E845D, 0x89BF89C0,
	0x91279127, 0x9A749A74, 0xA57EA57E, 0xB214B216, 0xC001C001, 0xCF05CF04, 0xDEDFDEDF, 0xEF4BEF4B,
};

static uint32_t dsp_atoi(char *src)
{
	uint32_t num = 0;

	if (NULL == src)
		return 0;

	while ((*src != 0) && ((*src < '0') || (*src > '9')))
		src++;

	if ((*src == '0') && ((*(src + 1) == 'x') || ((*(src + 1) == 'X')))) {
		src += 2;
		while (*src != 0) {
			if ((*src >= '0') && (*src <= '9'))
				num = num * 16 + *src - '0';
			else if ((*src >= 'A') && (*src <= 'F'))
				num = num * 16 + *src - 'A' + 10;
			else if ((*src >= 'a') && (*src <= 'f'))
				num = num * 16 + *src - 'a' + 10;
			else
				return num;
			src++;
		}
	} else {
		while (*src != 0) {
			if ((*src >= '0') && (*src <= '9'))
				num = num * 10 + *src - '0';
			else
				return num;
			src++;
		}
	}

	return num;
}

void dsp_wake_up_cb(mailbox_data_t *param)
{
	if (param->param0 == MAILBOX_CMD_AUDIO_WIFI_WAKEUP)
		CLI_LOGI("recv wake up from dsp!!!!\r\n");
}

void mic_test_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t enable;
	mailbox_data_t mailbox;

	if (argc > 1)
		enable = dsp_atoi(argv[1]);
	else
		enable = 0;
	bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_TEST, enable, 0, 0);
	bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
}

void usb_mount_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	FRESULT fr;
	char sys_path[10];
	int number = DISK_NUMBER_UDISK;

	if (pfs)
		os_free(pfs);

	pfs = os_malloc(sizeof(FATFS));
	if (NULL == pfs) {
		CLI_LOGI("f_mount malloc failed!\r\n");
		return;
	}

	os_memset(sys_path, 0, sizeof(sys_path));
	sprintf(sys_path, "%d:", number);
	fr = f_mount(pfs, sys_path, 1);
	if (fr != FR_OK) {
		mount_flag = 0;
		CLI_LOGI("usb mount failed:%d\r\n", fr);
	} else {
		mount_flag = 1;
		CLI_LOGI("usb mount OK!\r\n");
	}
}



void usb_unmount_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	FRESULT fr;
	char sys_path[10];
	int number = DISK_NUMBER_UDISK;

	if (mount_flag != 1) {
		CLI_LOGI("usb hasn't initialization!\r\n");
		return;
	}

	os_memset(sys_path, 0, sizeof(sys_path));
	sprintf(sys_path, "%d:", number);
	fr = f_mount(NULL, sys_path, 1);
	if (fr != FR_OK) {
		CLI_LOGI("unmount %s fail.\r\n", sys_path);
		return;
	}

	if (pfs) {
		os_free(pfs);
		pfs = NULL;
	}

	mount_flag = 0;
	CLI_LOGI("usb unmount OK!\r\n");
}

static FRESULT usb_scan_files(char *path)
{
	FRESULT fr;
	DIR dir;
	FILINFO fno;
	char path_temp[255];

	fr = f_opendir(&dir, path);
	if (fr == FR_OK) {
		while (1) {
			fr = f_readdir(&dir, &fno);
			if (fr != FR_OK)
				break;
			if (fno.fname[0] == 0)
				break;
			if (fno.fattrib & AM_DIR) {
				/* It is a directory */
				os_memset(path_temp, 0, sizeof(path_temp));
				sprintf(path_temp, "%s/%s", path, fno.fname);
				fr = usb_scan_files(path_temp);
				if (fr != FR_OK) break;
			} else {
				/* It is a file. */
				CLI_LOGI("%s/%s\r\n", path, fno.fname);
			}
		}
		f_closedir(&dir);
	} else
		CLI_LOGI("f_opendir failed\r\n");

	return fr;
}

void usb_ls_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	FRESULT fr;
	char sys_path[10];
	int number = DISK_NUMBER_UDISK;

	if (mount_flag != 1) {
		CLI_LOGI("usb hasn't initialization!\r\n");
		return;
	}

	os_memset(sys_path, 0, sizeof(sys_path));
	sprintf(sys_path, "%d:", number);
	fr = usb_scan_files(sys_path);
	if (fr != FR_OK)
		CLI_LOGI("scan_files failed!\r\n");
}

void pcm_test_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int i;
	uint32_t rate;
	uint32_t *aud_ptr = 0;
	uint32_t aud_len = 0;
	uint32_t *aud_addr = (uint32_t *)DAC_PLAY_NODE_ADDR;
	mailbox_data_t mailbox;

	if (argc < 2) {
		CLI_LOGI("pcm_test <8000|16000|44100|48000>\r\n");
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		if (argc > 2)
			rate = dsp_atoi(argv[2]);
		else
			rate = 8000;

		if (rate == 8000) {
			aud_ptr = (uint32_t *)PCM_8000;
			aud_len = sizeof(PCM_8000) / sizeof(PCM_8000[0]);
		} else if (rate == 16000) {
			aud_ptr = (uint32_t *)PCM_16000;
			aud_len = sizeof(PCM_16000) / sizeof(PCM_16000[0]);
		} else if (rate == 44100) {
			aud_ptr = (uint32_t *)PCM_44100;
			aud_len = sizeof(PCM_44100) / sizeof(PCM_44100[0]);
		} else if (rate == 48000) {
			aud_ptr = (uint32_t *)PCM_48000;
			aud_len = sizeof(PCM_48000) / sizeof(PCM_48000[0]);
		} else
			CLI_LOGI("rate error: %d.\r\n", rate);

		sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_AUDIO_PLL, &rate);

		bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_DAC_SAMPLE_RATE_SET, rate, 0, 0);
		bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);

		for (i = 0; i < aud_len; i++) {
			aud_addr[2 * i] = ((uint32_t)aud_ptr[i] & 0xFFFF) << 8;
			aud_addr[2 * i + 1] = ((uint32_t)aud_ptr[i] & 0xFFFF) << 8;
		}
		bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_DAC_PCM_CHECK, ((uint32_t)aud_addr) - W_DSP_DMEM_64KB_BASE_ADDR, aud_len * 8, 1);
		bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
	} else if (os_strcmp(argv[1], "stop") == 0) {
		bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_DAC_PCM_CHECK, 0, 0, 0);
		bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
	}
}


void record2dac_cb(mailbox_data_t *param)
{
	mailbox_data_t mailbox;
	dma_buffer_node *node;

	switch (param->param0) {
	case MAILBOX_CMD_AUDIO_ADC_PCM_READ:
		if (NULL == (uint8_t *)param->param1) {
			CLI_LOGI("%s:%d param1 is invalid\r\n", __FUNCTION__, __LINE__);
			break;
		}
		node = (dma_buffer_node *)co_list_pop_front(&g_record_context.free_list);
		if (NULL == node) {
			CLI_LOGI("free_list is empty\r\n");
			break;
		}

		node->buffer = (uint8_t *)param->param1;
		node->size = param->param2;
		co_list_push_back(&g_record_context.using_list, (struct co_list_hdr *)node);
		bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_DAC_PCM_WRITE, param->param1, param->param2, 0);
		bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
		break;

	case MAILBOX_CMD_AUDIO_DAC_PCM_WRITE_DONE:
		if (NULL == (uint8_t *)param->param1) {
			CLI_LOGI("%s:%d param1 is invalid\r\n", __FUNCTION__, __LINE__);
			break;
		}
		node = (dma_buffer_node *)co_list_pick(&g_record_context.using_list);
		if (NULL == node) {
			CLI_LOGI("using_list is empty\r\n");
			break;
		}

		for (; NULL != node; node = (dma_buffer_node *)node->header.next) {
			if (node->buffer == (uint8_t *)param->param1) {
				co_list_extract(&g_record_context.using_list, (struct co_list_hdr *)node);
				break;
			}
		}

		if (node == NULL)
			CLI_LOGI("can't find 0x%x in dac_list\r\n", param->param1);
		else {
			co_list_push_back(&g_record_context.free_list, (struct co_list_hdr *)node);
			bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_PCM_READ_DONE, param->param1, param->param2, 0);
			bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
		}
		break;

	default:
		CLI_LOGI("%s:%d cmd=0x%x\r\n", __FUNCTION__, __LINE__, param->param0);
		break;
	}
}

void record2dac_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int i;
	mailbox_data_t mailbox;

	if (argc < 2) {
		CLI_LOGI("record2dac <start> <adcx|adc1|adc2>\r\n");
		CLI_LOGI("record2dac <stop>\r\n");
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		if (argc < 3) {
			CLI_LOGI("record2dac <start> <adcx|adc1|adc2>\r\n");
			return;
		}
		co_list_init(&g_record_context.using_list);
		co_list_init(&g_record_context.free_list);
		for (i = 0; i < sizeof(record_buffer_nodes) / sizeof(record_buffer_nodes[0]); i++) {
			record_buffer_nodes[i].buffer = (uint8_t *)NULL;
			record_buffer_nodes[i].size = 0;
			co_list_push_back(&g_record_context.free_list, &record_buffer_nodes[i].header);
		}
		bk_mailbox_recv_callback_register(MAILBOX_DSP, MAILBOX_CPU0, (mailbox_callback_t)record2dac_cb);

		if (os_strcmp(argv[2], "adcx") == 0) {
			bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_DAC_ENABLE, 1, 1, 1);
		} else {
			bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_DAC_ENABLE, 1, 2, 1);
		}
		bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);

		if (os_strcmp(argv[2], "adcx") == 0)
			bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_RECORD, 0, 1, 0);
		else if (os_strcmp(argv[2], "adc1") == 0)
			bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_RECORD, 1, 0, 0);
		else if (os_strcmp(argv[2], "adc2") == 0)
			bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_RECORD, 2, 0, 0);
		else
			bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_RECORD, 0, 0, 0);
		bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
	} else if (os_strcmp(argv[1], "stop") == 0) {
		bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_RECORD, 0, 0, 0);
		bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);

		bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_DAC_ENABLE, 0, 0, 0);
		bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
		bk_mailbox_recv_callback_unregister(MAILBOX_CPU0, MAILBOX_DSP);
	} else
		CLI_LOGI("NOT support command %s.\r\n", argv[1]);
}

static void usb_record_thread_main(void *arg)
{
	int ret;
	mailbox_data_t mailbox;
	dma_buffer_node *node;
	UINT bw;
	FRESULT fr;

	while (record_flag) {
		ret = rtos_get_semaphore(&usb_record_sem, BEKEN_WAIT_FOREVER);
		if (ret) {
			CLI_LOGI("get usb record semaphore fail.\r\n");
			break;
		}

		/*write adc data to usb*/
		node = (dma_buffer_node *)co_list_pick(&g_record_context.using_list);
		if (node) {
			for (; node != NULL; node = (dma_buffer_node *)node->header.next) {
				co_list_extract(&g_record_context.using_list, (struct co_list_hdr *)node);
				fr = f_write(&record_file, node->buffer, node->size, &bw);
				if (fr != FR_OK)
					CLI_LOGI("write %x to usb fail.\r\n", (uint32_t)node->buffer);
				if (node->size != bw)
					CLI_LOGI("write %x to usb bytes %d/%d.\r\n", (uint32_t)node->buffer, bw, node->size);
				co_list_push_back(&g_record_context.free_list, (struct co_list_hdr *)node);
				bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_PCM_READ_DONE, ((uint32_t)node->buffer) - W_DSP_DMEM_64KB_BASE_ADDR, node->size, 0);
				bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
			}
		}
	}

	/*set flag for thread end*/
	thread_flag = 0;
	rtos_delete_thread(&usb_record_thread_handle);
}

static void usb_record_cb_hdl(mailbox_data_t *param)
{
	int ret;
	dma_buffer_node *node;

	switch (param->param0) {
	case MAILBOX_CMD_AUDIO_ADC_PCM_READ:
		if ((uint8_t *)param->param1 == NULL) {
			CLI_LOGI("%s:%d param1 is invalid!\r\n", __FUNCTION__, __LINE__);
			break;
		}

		node = (dma_buffer_node *)co_list_pop_front(&g_record_context.free_list);
		if (node == NULL) {
			CLI_LOGI("free_list is empty!\r\n");
			break;
		}

		node->buffer = (uint8_t *)(param->param1 + W_DSP_DMEM_64KB_BASE_ADDR);
		node->size = param->param2;
		co_list_push_back(&g_record_context.using_list, (struct co_list_hdr *)node);
		ret = rtos_set_semaphore(&usb_record_sem);
		if (ret)
			CLI_LOGI("set usb record semaphore fail.\r\n");
		break;
	default:
		CLI_LOGI("%s:%d cmd=0x%x!\r\n", __FUNCTION__, __LINE__, param->param0);
		break;
	}
}

void record2usb_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret;
	uint32_t index = 0;
	mailbox_data_t mailbox;
	char file_name[50];
	FRESULT fr;
	int number = DISK_NUMBER_UDISK;

	if (argc < 2) {
		CLI_LOGI("record2usb <start> <adcx|adc1|adc2> [file name]\r\n");
		CLI_LOGI("record2usb <stop>\r\n");
		return;
	}

	if (mount_flag != 1) {
		CLI_LOGI("usb hasn't initialization!\r\n");
		return;
	}

	if (os_strcmp(argv[1], "start") == 0) {
		if (argc < 3) {
			CLI_LOGI("record2usb <start> <adcx|adc1|adc2> [file name]\r\n");
			return;
		}

		if (record_flag) {
			CLI_LOGW("record thread is running cmd is invalid!\r\n");
			return;
		}

		/*create&open record file*/
		os_memset(file_name, 0, sizeof(file_name));
		if (argc > 3)
			sprintf(file_name, "%d:/%s", number, argv[3]);
		else
			sprintf(file_name, "%d:/record.pcm", number);
		fr = f_open(&record_file, file_name, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
		if (fr != FR_OK) {
			CLI_LOGI("open %s fail.\r\n", file_name);
			return;
		}

		/*init list*/
		co_list_init(&g_record_context.using_list);
		co_list_init(&g_record_context.free_list);
		for (index = 0; index < sizeof(record_buffer_nodes) / sizeof(record_buffer_nodes[0]); index++) {
			record_buffer_nodes[index].buffer = (uint8_t *)NULL;
			record_buffer_nodes[index].size = 0;
			co_list_push_back(&g_record_context.free_list, &record_buffer_nodes[index].header);
		}

		/*create thread to write usb*/
		ret = rtos_init_semaphore(&usb_record_sem, 10);
		if (ret) {
			CLI_LOGI("create usb record semaphore fail.\r\n");
			return;
		}
		record_flag = 1;
		thread_flag = 1;
		ret = rtos_create_thread(&usb_record_thread_handle,
								 5,
								 "usb_record_thread",
								 (beken_thread_function_t)usb_record_thread_main,
								 2048,
								 (beken_thread_arg_t)0);
		if (ret) {
			CLI_LOGI("create usb record thread fail.\r\n");
			return;
		}

		/*set mailbox callback*/
		bk_mailbox_recv_callback_register(MAILBOX_CPU0, MAILBOX_DSP, (mailbox_callback_t)usb_record_cb_hdl);

		/*send record start to dsp*/
		if (os_strcmp(argv[2], "adcx") == 0)
			bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_RECORD, 0, 1, 0);
		else if (os_strcmp(argv[2], "adc1") == 0)
			bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_RECORD, 1, 0, 0);
		else if (os_strcmp(argv[2], "adc2") == 0)
			bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_RECORD, 2, 0, 0);
		else
			bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_RECORD, 0, 0, 0);
		bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
	} else if (os_strcmp(argv[1], "stop") == 0) {
		if (!usb_record_sem) {
			CLI_LOGW("please start record thread.\r\n");
			return;
		}

		/*send record stop to dsp*/
		bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_RECORD, 0, 0, 0);
		bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);

		/*delete usb record thread*/
		record_flag = 0;
		ret = rtos_set_semaphore(&usb_record_sem);
		if (ret) {
			CLI_LOGI("set usb record semaphore fail.\r\n");
			return;
		}
		while (thread_flag)
			rtos_delay_milliseconds(2);
		ret = rtos_deinit_semaphore(&usb_record_sem);
		if (ret)
			CLI_LOGI("delete usb record semaphore fail.\r\n");
		bk_mailbox_recv_callback_unregister(MAILBOX_CPU0, MAILBOX_DSP);

		/*close record file*/
		fr = f_close(&record_file);
		if (fr != FR_OK)
			CLI_LOGI("close record file fail.\r\n");
	}
}

static void usb_play_cb_hdl(mailbox_data_t *param)
{
	int ret;
	dma_buffer_node *node;

	switch (param->param0) {
	case MAILBOX_CMD_AUDIO_DAC_PCM_WRITE_DONE:
		node = (dma_buffer_node *)co_list_pick(&g_record_context.using_list);
		if (node == NULL) {
			CLI_LOGI("%s:%d using list is empty!\r\n", __FUNCTION__, __LINE__);
			break;
		}

		for (; node != NULL; node = (dma_buffer_node *)node->header.next) {
			if (node->buffer == (uint8_t *)(param->param1 + W_DSP_DMEM_64KB_BASE_ADDR)) {
				co_list_extract(&g_record_context.using_list, (struct co_list_hdr *)node);
				break;
			}
		}

		if (node == NULL)
			CLI_LOGI("can not find 0x%x in dac_list!\r\n", param->param1);
		else
			co_list_push_back(&g_record_context.free_list, (struct co_list_hdr *)node);

		if (param->param2 == 0) {
			ret = rtos_set_semaphore(&usb_play_sem);
			if (ret)
				CLI_LOGI("set usb play semaphore fail.\r\n");
		}
		break;

	default:
		CLI_LOGI("%s:%d cmd=0x%x!\r\n", __FUNCTION__, __LINE__, param->param0);
		break;
	}
}

void usb_play_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret;
	uint32_t index = 0;
	uint32_t address = DAC_PLAY_NODE_ADDR;
	mailbox_data_t mailbox;
	dma_buffer_node *node;
	char file_name[50];
	FRESULT fr;
	int number = DISK_NUMBER_UDISK;
	UINT br;
	uint32_t retry_cnt = 0;
	char *postfix;
	uint32_t chann;

	if (mount_flag != 1) {
		CLI_LOGI("usb hasn't initialization!\r\n");
		return;
	}

	if (argc < 2) {
		CLI_LOGI("usb_play <mono:1|stereo:2> [filename]\r\n");
		return;
	}

	/*init usb play semaphore*/
	ret = rtos_init_semaphore(&usb_play_sem, 1);
	if (ret) {
		CLI_LOGI("create usb play semaphore fail.\r\n");
		return;
	}

	/*get play mono or stereo*/
	chann = dsp_atoi(argv[1]);
	if (chann != 1 && chann != 2) {
		CLI_LOGI("usb_play <mono:1|stereo:2> [filename]\r\n");
		return;
	}

	/*open record file*/
	os_memset(file_name, 0, sizeof(file_name));
	if (argc > 2)
		sprintf(file_name, "%d:/%s", number, argv[2]);
	else
		sprintf(file_name, "%d:/record.pcm", number);

	fr = f_open(&record_file, file_name, FA_OPEN_EXISTING | FA_READ);
	if (fr != FR_OK) {
		CLI_LOGI("open %s fail.\r\n", file_name);
		return;
	}

	/*init list*/
	co_list_init(&g_record_context.using_list);
	co_list_init(&g_record_context.free_list);
	for (index = 0; index < sizeof(record_buffer_nodes) / sizeof(record_buffer_nodes[0]); index++) {
		record_buffer_nodes[index].buffer = (uint8_t *)address;
		record_buffer_nodes[index].size = DAC_PLAY_NODE_SIZE;
		co_list_push_back(&g_record_context.free_list, &record_buffer_nodes[index].header);
		address += DAC_PLAY_NODE_SIZE;
	}

	/*set mailbox callback*/
	bk_mailbox_recv_callback_register(MAILBOX_CPU0, MAILBOX_DSP, (mailbox_callback_t)usb_play_cb_hdl);

	/*send play start to dsp*/
	postfix = os_strrchr(file_name, '.');
	if (os_strcmp(postfix, ".opus") == 0) {
		bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_DAC_ENABLE, 1, chann, 0);
		bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
	} else if (os_strcmp(postfix, ".pcm") == 0) {
		bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_DAC_ENABLE, 1, chann, 1);
		bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
	} else {
		CLI_LOGI("unknown file type: %s.\r\n", file_name);
	}

	/*play record file*/
	while (1) {
		node = (dma_buffer_node *)co_list_pop_front(&g_record_context.free_list);
		if (node == NULL) {
			retry_cnt++;
			if (retry_cnt > 10000) {
				retry_cnt = 0;
				CLI_LOGI("get free list fail.\r\n");
			}
			rtos_delay_milliseconds(2);
			continue;
		}

		if (retry_cnt)
			retry_cnt = 0;

		fr = f_read(&record_file, node->buffer, DAC_PLAY_NODE_SIZE, &br);
		if (fr != FR_OK) {
			CLI_LOGI("read record file fail.\r\n");
			break;
		}

		node->size = br;
		co_list_push_back(&g_record_context.using_list, (struct co_list_hdr *)node);
		bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_DAC_PCM_WRITE, ((uint32_t)node->buffer) - W_DSP_DMEM_64KB_BASE_ADDR, node->size, 0);
		bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);

		if (br == 0) break;
	}

	/*wait for play stop*/
	ret = rtos_get_semaphore(&usb_play_sem, BEKEN_WAIT_FOREVER);
	if (ret)
		CLI_LOGI("get usb play semaphore fail.\r\n");

	/*send play stop to dsp*/
	bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_DAC_ENABLE, 0, 0, 0);
	bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
	bk_mailbox_recv_callback_unregister(MAILBOX_CPU0, MAILBOX_DSP);

	/*close record file*/
	fr = f_close(&record_file);
	if (fr != FR_OK)
		CLI_LOGI("close %s fail.\r\n", file_name);
}

void line_in_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t enable;

	if (argc > 1)
		enable = dsp_atoi(argv[1]);
	else
		enable = 0;

	if (enable)
		sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_ENALBLE_ADC_LINE_IN, NULL);
	else
		sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_DISALBLE_ADC_LINE_IN, NULL);
}

void adc_analog_gain_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t param;
	mailbox_data_t mailbox;

	if (argc < 2) {
		CLI_LOGI("please input gain.\r\n");
		return;
	}
	param = dsp_atoi(argv[1]);
	bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_ANALOG_VOLUME_SET, param & 0x7F, 0, 0);
	bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
}

void adc_digital_gain_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t param;
	mailbox_data_t mailbox;

	if (argc < 2) {
		CLI_LOGI("please input gain.\r\n");
		return;
	}
	param = dsp_atoi(argv[1]);
	bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_DIGITAL_VOLUME_SET, param & 0x3F, 0, 0);
	bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
}

void adc_sample_rate_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t param;
	mailbox_data_t mailbox;

	if (argc < 2) {
		CLI_LOGI("please input sample rate.\r\n");
		return;
	}
	param = dsp_atoi(argv[1]);
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_AUDIO_PLL, &param);
	bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_ADC_SAMPLE_RATE_SET, param, 0, 0);
	bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
}

void dac_analog_gain_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t param;

	if (argc < 2) {
		CLI_LOGI("please input gain.\r\n");
		return;
	}
	param = dsp_atoi(argv[1]);
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_SET_DAC_VOLUME_ANALOG, &param);
}

void dac_digital_gain_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t param;
	mailbox_data_t mailbox;

	if (argc < 2) {
		CLI_LOGI("please input gain.\r\n");
		return;
	}
	param = dsp_atoi(argv[1]);
	bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_DAC_DIGITAL_VOLUME_SET, param & 0xFFF, 0, 0);
	bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
}

void dac_sample_rate_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t param;
	mailbox_data_t mailbox;

	if (argc < 2) {
		CLI_LOGI("please input sample rate.\r\n");
		return;
	}
	param = dsp_atoi(argv[1]);
	sddev_control(DD_DEV_TYPE_SCTRL, CMD_SCTRL_AUDIO_PLL, &param);
	bk_mailbox_set_param(&mailbox, MAILBOX_CMD_AUDIO_DAC_SAMPLE_RATE_SET, param, 0, 0);
	bk_mailbox_send(&mailbox, MAILBOX_CPU0, MAILBOX_DSP, NULL);
}

const struct cli_command dsp_clis[] = {
	{"mic_test", "mic_test <0|1|2|3|4>", mic_test_command},
	{"pcm_test", "pcm_test <8000|16000|44100|48000>", pcm_test_command},
	{"record2dac", "record2dac <start|stop>", record2dac_command},
	{"usb_mount", "usb mount", usb_mount_command},
	{"usb_unmount", "usb unmount", usb_unmount_command},
	{"usb_ls", "usb list system", usb_ls_command},
	{"record2usb", "record2usb <start|stop>", record2usb_command},
	{"usb_play", "usb play", usb_play_command},
	{"line_in", "line_in <0|1>", line_in_command},
	{"adc_analog_gain", "adc_analog_gain <gain>", adc_analog_gain_command},
	{"adc_digital_gain", "adc_digital_gain <gain>", adc_digital_gain_command},
	{"adc_sample_rate", "adc_sample_rate <rate>", adc_sample_rate_command},
	{"dac_analog_gain", "dac_analog_gain <gain>", dac_analog_gain_command},
	{"dac_digital_gain", "dac_digital_gain <gain>", dac_digital_gain_command},
	{"dac_sample_rate", "dac_sample_rate <rate>", dac_sample_rate_command}
};

void bk7271_dsp_cli_init(void)
{
	int ret;

	bk_mailbox_recv_callback_register(MAILBOX_DSP, MAILBOX_CPU0, (mailbox_callback_t)dsp_wake_up_cb);
	ret = cli_register_commands(dsp_clis, sizeof(dsp_clis) / sizeof(struct cli_command));
	if (ret)
		CLI_LOGI("register dsp commands fail.\r\n");
}
#endif
