#include <rtpsession.h>
#include <rtpudpv4transmitter.h>
#include <rtpipv4address.h>
#include <rtpsessionparams.h>
#include <rtperrors.h>
#include <rtplibraryversion.h>
#include <rtppacket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <common/bk_include.h>
#include <os/os.h>
#include <components/system.h>
 
using namespace jrtplib;

extern "C" {
#if (CONFIG_TASK_WDT)
extern void bk_task_wdt_feed(void);
#endif
int test_rtp_send(u8 *destip, u16 portbase, u16 destport)
{    
#if 1
	unsigned int i = 1;
	int status;
	RTPSession *sess = new RTPSession;    
	RTPUDPv4TransmissionParams transparams;
	RTPSessionParams sessparams;

	sessparams.SetOwnTimestampUnit(1.0/10.0);        
	sessparams.SetAcceptOwnPackets(false);
	transparams.SetPortbase(portbase);
	//transparams.SetRTCPMultiplexing(true);
	os_printf("rtp send demo\r\n");
	sess->Create(sessparams,&transparams);    
	RTPIPv4Address addr(destip,destport,false);
	sess->AddDestination(addr);

	while(1)
	{	    
		status = sess->SendPacket((void *)"123456789011111",15,0,false,10);
		if (!status)
			os_printf("\nSending packet %d\n",i++);
		sess->Poll();
		RTPTime::Wait(RTPTime(1,0));
	}

	sess->BYEDestroy(RTPTime(10,0),0,0);
	delete []sess;
#else
	unsigned int i = 1;
	int status;
	char *buffer = NULL;
	RTPSession *sess = new RTPSession;
	RTPUDPv4TransmissionParams transparams;
	RTPSessionParams sessparams;

	buffer = (char *)malloc(1400*sizeof(char));
	memset(buffer, 'a', 1400);
	sessparams.SetOwnTimestampUnit(1.0/10.0);        
	sessparams.SetAcceptOwnPackets(false);
	transparams.SetPortbase(portbase);
	//transparams.SetRTCPMultiplexing(true);

	os_printf("rtp send demo\r\n");

	sess->Create(sessparams,&transparams);    
	RTPIPv4Address addr(destip,destport,false);
	sess->AddDestination(addr);

	while(1)
	{
		status = sess->SendPacket((void *)buffer,1400,0,false,10);
		if (!status)
			os_printf("\nSending packet %d\n",i++);
		//sess->Poll();
		//RTPTime::Wait(RTPTime(1,0));
#if (CONFIG_TASK_WDT)
		bk_task_wdt_feed();
#endif
	}

	sess->BYEDestroy(RTPTime(10,0),0,0);
	delete []sess;
#endif

	return 0;
}

int test_rtp_recv(u8 *destip, u16 destport)
{
	RTPSession *sess = new RTPSession;
	uint16_t portbase = 7180;
	bool done = false;
	unsigned int i = 1;

	RTPUDPv4TransmissionParams transparams;
	RTPSessionParams sessparams;
	sessparams.SetOwnTimestampUnit(1.0/10.0);        
	sessparams.SetAcceptOwnPackets(false);
	transparams.SetPortbase(portbase);
	//transparams.SetRTCPMultiplexing(true);
	sess->Create(sessparams,&transparams);

#if 1 //rtcp test by linwei
	RTPIPv4Address addr(destip,destport,false);
	sess->AddDestination(addr);
#endif
	sess->BeginDataAccess();
	RTPTime delay(0.020);

	while (!done)
	{
		sess->Poll();
		if (sess->GotoFirstSourceWithData())
		{
			do {
				RTPPacket *pack;
				while ((pack = sess->GetNextPacket()) != NULL)
				{
					os_printf("receiving packet %d\r\n",i++);
					/*int len = pack->GetPayloadLength();
					for (int i = 0; i <len; i ++)
					    os_printf("%c ", *(pack->GetPayloadData() + i));
					os_printf("\r\n");*/
					sess->DeletePacket(pack);
				}
			} while (sess->GotoNextSourceWithData());
		}
#if (CONFIG_TASK_WDT)
		bk_task_wdt_feed();
#endif
	}

	sess->EndDataAccess();
	delay = RTPTime(10.0);
	sess->BYEDestroy(delay,0,0);
	delete []sess;

	return 0;
}
}
