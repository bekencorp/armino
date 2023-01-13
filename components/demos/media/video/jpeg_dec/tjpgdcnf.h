/*----------------------------------------------*/
/* TJpgDec System Configurations R0.03          */
/*----------------------------------------------*/

#define	JD_SZBUF		1024
/* Specifies size of stream input buffer */

extern int g_jd_format;
#define JD_FORMAT		g_jd_format
typedef enum{
	JD_FORMAT_RGB888 = 0,
	JD_FORMAT_RGB565 = 1,
	JD_FORMAT_Grayscale = 2,
	JD_FORMAT_VYUY,
	JD_FORMAT_YUYV
}JD_FORMAT_OUTPUT;
	
/* Specifies output pixel format.
/  0: RGB888 (24-bit/pix)
/  1: RGB565 (16-bit/pix)
/  2: Grayscale (8-bit/pix)
/  3: VYUY (16-bit/pix)
/  4: YUYV (16-bit/pix)
*/

#define	JD_USE_SCALE	0
/* Switches output descaling feature.
/  0: Disable
/  1: Enable
*/

#define JD_TBLCLIP		1
/* Use table conversion for saturation arithmetic. A bit faster, but increases 1 KB of code size.
/  0: Disable
/  1: Enable
*/

#define JD_FASTDECODE	2
/* Optimization level
/  0: Basic optimization. Suitable for 8/16-bit MCUs.
/  1: + 32-bit barrel shifter. Suitable for 32-bit MCUs.
/  2: + Table conversion for huffman decoding (wants 6 << HUFF_BIT bytes of RAM)
*/

