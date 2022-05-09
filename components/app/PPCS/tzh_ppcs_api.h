#ifndef __TZH_PPCS_API_H__
#define  __TZH_PPCS_API_H__

#define CGI_MAX_SOCKET  256
#define CGI_MAX_BUF  	1024
#define CGI_MAX_LEN 	1024

int UTILS_WriteInt(char *pbuf, char *pvar, int value);
int UTILS_WriteIntValue(char *pbuf, char *pvar, int value);
int UTILS_WriteStr(char *pbuf, char *pvar, char *pvalue);
int UTILS_WriteStrValue(char *pbuf, char *pvar, char *pvalue);

int UTILS_GetKeyValue(char *pparam, char *pkey, char *pvalue);
int UTILS_GetKeyInt(char *pparam, char *pkey, int *pvalue);
int UTILS_GetKeyStr(char *pparam, char *pkey, char *pvalue, int maxlen);

#define STREAM_START_CODE	0xa815aa55
#define htonet(A)     A
#define htolong(A)    A

typedef struct {
	unsigned int   		startcode;	//  0xa815aa55
	char				type;
	char  				streamid;
	unsigned short 		militime;
	unsigned int 		sectime;
	unsigned int    	frameno;
	unsigned int 		len;
	unsigned char		version;
	unsigned char		resolution;
	unsigned char		sessid;
	unsigned char		currsit;
	unsigned char		endflag;
	char				byzone;
	char				channel;		//for user in sit
	char				type1;
	char				h264refmode;
	char                h264reftype;
	char                reserved[2];
#if 0	//remove sample and index field
	short               sample;
	short				index;
#endif
} STREAMHEAD, *PSTREAMHEAD;

int TZH_StartPPCS(void);
int TZH_StopPPCS(void);

void TZH_PPCS_SetDIDCRCKey(char *pszCRCKey);
void TZH_PPCS_SetDIDString(char *pszDIDString);
char *TZH_PPCS_GetDIDString(void);
void TZH_PPCS_SetDIDLicense(char *pszDIDLicense);
void TZH_PPCS_SetServerInitString(char *pszInitString);
void TZH_PPCS_SetVideoFrameMaxSize(UINT32 len);
int PPCS_SendVideoData(UINT8 *data, UINT32 len);
int PPCS_SendAudioData(UINT8 *data, UINT32 len);
int TZH_PPCS_GetLiveFlag(void);

typedef void (*play_talk_buffer_cb)(unsigned char *pszAudioBuf, int nBufSize);

void TZH_PPCS_RegisterPlayTalkCB(play_talk_buffer_cb fn, int maxtalklen);

#endif

