/*
 * rs422_if.c
 */

#include "config.h"

#include "rs422_if.h"
#include "net_AS.h"
#include "LcasAction_task.h"

volatile uint32_t * pTxMem = (volatile uint32_t *)(0x50008000);	// 0x2000 (32bit) -- 0010 0000 0000 0000 << 2  --> 1000 0000 0000 0000 (0x8000) --> 0x50008000
volatile uint32_t * pRxMem = (volatile uint32_t *)(0x50009000);	// 0x2400 (32bit) -- 0010 0100 0000 0000 << 2  --> 1001 0000 0000 0000 (0x9000) --> 0x50009000

volatile uint8_t * pTxMem_byte = (volatile uint8_t *)(0x50008000);	// 0x2000 (32bit) -- 0010 0000 0000 0000 << 2  --> 1000 0000 0000 0000 (0x8000) --> 0x50008000
volatile uint8_t * pRxMem_byte = (volatile uint8_t *)(0x50009000);	// 0x2400 (32bit) -- 0010 0100 0000 0000 << 2  --> 1001 0000 0000 0000 (0x9000) --> 0x50009000

//#define APC_RS422_PACKET_FUNCTION_COUNT		(rs422_MAX_PACKET_NO + 1)
//void (*pfAPC_Rs422_Packet[APC_RS422_PACKET_FUNCTION_COUNT])(uint32_t slvid_addr);

RS422_ST rs422if;

RS422_BUF tx;

#if RS422_MY_ID	== RS422_ID_AS

char selfMsgBufHeader[24];
LCAS_SELFMSG_t rs422_selfMsgBuf[8];
char stateBufHeader[24];
//LCAS_STATE_t rs422_stateBuf[8]; // - 보낼 수 있을 때 그때 데이터를 보낸다.
char diagDataBufHeader[24];
LCAS_DIAG_DATA_t rs422_diagDataBuf[8];

char responseDataBufHeader[4][24];
char rs422_responseDataBuf[4][256];

#elif RS422_MY_ID	== RS422_ID_OVEN
	YL9030_OVEN_SELFMSG rs422_selfMsgBuf[8];
	YL9030_OVEN_STATE rs422_stateBuf[8];
	YL9030_OVEN_DIAGDATA rs422_diagDataBuf[8];

//YL9030_OVEN_SVCDATA
#elif RS422_MY_ID	== RS422_ID_UVD
	YL9020_UVD_SELFMSG rs422_selfMsgBuf[8];
	YL9020_UVD_STATE rs422_stateBuf[8];
	YL9020_UVD_DIAGDATA rs422_diagDataBuf[8];
	YL9020_UVD_SCANDATA rs422_scanDataBuf[8];
	char scanDataBufHeader[24];
	YL9020_UVD_ACQDATA rs422_acqDataBuf[4];
	char acqDataBufHeader[24];

#endif


extern LCAS_CONTROL_t LcasCon;//,tmpLcasCon;
extern LCAS_DEV_STATE_t devState;
extern LCAS_DEV_SET_t devSet;


extern SYSTEM_INFORM_t sysInform;
extern SYSTEM_CONFIG_t sysConfig;
extern LCAS_RUN_SETUP_t runSetup;
extern LCAS_TIME_EVENT_t timeEvent;
extern LCAS_MIX_t mixSet;

extern LCAS_SEQUENCE_t sequence;
extern LCAS_COMMAND_t command;
extern LCAS_SELFMSG_t selfMsg;

extern LCAS_SERVICE_t service;
extern LCAS_STATE_t LcasState;

extern LCAS_DIAG_DATA_t diagData;
extern LCAS_USED_TIME_t usedTime;
extern LCAS_TEMP_CAL_t tempCalData;
extern LCAS_SPECIAL_t special;

extern ETHERNET_CONFIG_t ethernetConfig;

extern char pPacketBuff[TCP_SEND_PACKET_BUFF_SIZE];




// ===========================================================================
// ===========================================================================
void rs422_SetMyBoardId(uint8_t Id)
{
	FPGA_WRITE_WORD(RS422_MY_ID_ADDR,Id); // Use 4bit
}

void InterruptEnable_rs422(void)
{
// RS422 Rx intr
	NVIC_EnableIRQ( FabricIrq0_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq0_IRQn );

// rs422 Auto Completion interrupt enable ----------
	NVIC_EnableIRQ( FabricIrq3_IRQn );
	NVIC_ClearPendingIRQ( FabricIrq3_IRQn );
}

void InterruptDisable_rs422(void)
{
// RS422 Rx intr
	NVIC_DisableIRQ(FabricIrq0_IRQn);
//	NVIC_ClearPendingIRQ( FabricIrq0_IRQn );

// rs422 Auto Completion interrupt 
	NVIC_DisableIRQ(FabricIrq3_IRQn);
//	NVIC_ClearPendingIRQ( FabricIrq3_IRQn );
}

void rs422_init(void)
{
	uint8_t i=0,j=0;
	
	rs422if.RunMode = RS422_RUN_CPUMODE;
	rs422if.State = RS422_STATE_RUN;
	rs422if.RxAllAllow = 0;
	
	rs422if.TSTAT = 1;	// 처음엔 완료로 셋팅

	tx.Head = 0;
	tx.Tail = 0;
}

#if 0 // RS422_MASTER
int rs422_PacketParse_Master()		// Chrozen LC : 주로 상태응답에 대한 처리
{
	uint8_t RSTAT;
	uint32_t rxinfo;
	uint8_t rxid;
	uint8_t slvid;
	uint8_t rxlen;
	uint8_t rxcmd;

	uint32_t packetID = 0;
	uint32_t slvid_addr;

	static int net_disable = 0;

	uint32_t packetCode;

	uint32_t rs422_CMD;
	LCAS_PACKET Packet;

	static char rs422Data_in[256];

	RSTAT = FPGA_READ_WORD(RS422_RX_STATE_ADDR);
	if(RSTAT&0x80)
	{
		FPGA_WR_RS422_RX_FLAG_CLEAR;
		rxinfo = FPGA_READ_WORD(RS422_RX_INFO_ADDR);
		rxid  = rxinfo; rxinfo = rxinfo>>8;
		rxlen = rxinfo; rxinfo = rxinfo>>8;
		rxcmd = rxinfo;
		slvid = rxid&0xF;
		rs422if.rx_slvid = rxid&0xF;

		switch(rxcmd) {
			case AUTO_MODE_CMD:
				dp("return  AUTO_MODE_CMD\n");
				return RE_OK;
				break;
			case RS422_CMD_PACKET_SEND:
	/*
					memcpy(pPacket, (uint8_t *)(pRxMem + 1), sizeof(LCAS_PACKET));			// Header
					memcpy(rs422Data_in, (uint8_t *)(pRxMem + 1), ((rxlen - 1) << 2));			// Data
					rs422_PacketParse(pPacket->nPacketCode, rs422Data_in);
	*/
				break;
			case STATE_REQUEST_CMD:		// Slave에서 응답
				memcpy(&Packet, (uint8_t *)(pRxMem + 1), sizeof(LCAS_PACKET));			// Header
				memcpy(rs422Data_in, (uint8_t *)(pRxMem + 1), ((rxlen - 1) << 2));			// Data
				rs422_PacketParse(Packet.nPacketCode, rs422Data_in);
				break;
		}

		if(net_disable == NO) {
			rs422if.connected = YES;
			NVIC_DisableIRQ(EthernetMAC_IRQn); /*MSS_MAC_disable_intr();*/ /* (+) 210621 CJKIM, disable mac interrupt before sending. */
			net_disable = YES;
		}

	}

	return RE_OK;
}

int rs422_REQUEST_Send(uint32_t slvid)
{
// Master에서 주기적으로 요청한다.
// 상태값을 요청하고 , FPGA에서 자동으로 읽어 들인다.	
// GLOBAL ID로 설정하지 말라
// 
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;
				
		pTxMem[0] = STATE_REQUEST_CMD;
		pTxMem[1] = 0x0000000B;

		FPGA_WR_RS422_TX_LENGTH(2); 	// tx_len

		// APC_RUN_AUTOMODE 는 너무 많이 기다려서 하나씩 요청한다.
	//apc_APCCPUModeSetting();	// 없어도 Default CPU Mode
		FPGA_WR_RS422_MODE_CPU;

		FPGA_WR_RS422_SEND_TRIG(slvid); // tx_start - RS422_ID_GLOBAL 은 응답없음

		return RE_OK;
	}
	else {
		// 현재 보낼 수 없는 상태일 경우는 무시하고 요청하지 않음.
		// 다른 명영이 실행 되고 있는 상태이므로 한 번 빠져도 된다....
		return RE_ERR;
	}

}

int rs422_SendCmd()	// Master --> Slave
{
}

#else
int rs422_PacketParse_Slave()
{
	uint8_t RSTAT;
	uint32_t rxinfo;
	uint8_t rxid;
	uint8_t rxlen;
	uint8_t rxcmd;
	static int net_disable = 0;
	uint32_t rs422_CMD;
	LCAS_PACKET* pPacket;	
	static char rs422Data_in[256];

	unsigned int rs422if_cmd_index;

	RSTAT = FPGA_READ_WORD(RS422_RX_STATE_ADDR);
static int missing_cnt = 0;

	if(RSTAT&0x80)
	{
		FPGA_WR_RS422_RX_FLAG_CLEAR;
		rxinfo = FPGA_READ_WORD(RS422_RX_INFO_ADDR);
		rxid  = rxinfo; rxinfo = rxinfo>>8;
			rxlen = rxinfo; 
		rxlen = (rxlen - 1) << 2;
			rxinfo = rxinfo>>8;
		rxcmd = rxinfo;
//		slvid = rxid&0xF;

		rs422_CMD = FPGA_READ_WORD(RS422_RX_CMD_ADDR);

		switch(rs422_CMD) {
			case RS422_CMD_PACKET_SEND:		// Master에서 전송한 데이터를 받은 경우
//				memcpy(rs422Data_in, (uint8_t *)(pRxMem + 1), ((rxlen - 1) << 2));			// Packet Data
				memcpy(rs422Data_in, (uint8_t *)(pRxMem + 1), rxlen);			// Packet Data

				pPacket = (LCAS_PACKET*)rs422Data_in;
//ShowMemory(0, rs422Data_in, rxlen);
			
				devState.rs422if_packet = YES;
//				rs422_PacketParse(pPacket->nPacketCode, rs422Data_in);
				PacketParse(pPacket->nPacketCode, rs422Data_in);
devState.rs422if_cmd_index++;

//iprintf("##### devState.rs422if_cmd_index = %d \n",devState.rs422if_cmd_index);

				break;
			case STATE_REQUEST_CMD:
//memcpy(rs422Data_in, (uint8_t *)(pRxMem + 1), rxlen); 		// Packet Data
//iprintf("len = %d", rxlen);
				// 마스터에서 주기적으로 요청한다.
				// 요청에 대한 응답은 FPGA에서 자동으로 버퍼에 저장된 데이터를 보낸다.
				// 여기에서 특별히 할일은 없다.
				
				// rs422이 끊어진상태에서 큐에 상태데이터를 쌓는 것을 방지하기 위해서 플래그설정
				devState.rs422if_request = YES;
//rs422if_cmd_index = (unsigned int *)(rs422Data_in)[0];
//				iprintf("##### rs422if_cmd_index = %d rxlen[%d]\n",rs422if_cmd_index, rxlen);

rs422if_cmd_index = *(pRxMem + 1);
//				iprintf("pRxMem - ##### rs422if_cmd_index = %d rxlen[%d]\n",rs422if_cmd_index, rxlen);

				if(devState.rs422if_cmd_index != rs422if_cmd_index) {

				
					if(rs422if_cmd_index == 1) {
						devState.rs422if_cmd_index = 1;
						missing_cnt = 0;
					}
					else {
						missing_cnt++;
					}





					devState.rs422if_cmd_index = rs422if_cmd_index;
					memcpy(rs422Data_in, (uint8_t *)(pRxMem + 1), rxlen); 		// Packet Data

					pPacket = (LCAS_PACKET*)rs422Data_in;
					ShowMemory(0, rs422Data_in, rxlen);
								
					devState.rs422if_packet = YES;
					PacketParse(pPacket->nPacketCode, rs422Data_in);
		
					iprintf("xxxxxxxxxxxxxxxxxxxxxxxx##### rs422if_cmd_index[%d] missing_cnt[%d] rxlen[%d]\n",rs422if_cmd_index, missing_cnt,rxlen);
				};
				
				break;
		}



// RS422이 활성화 되면 Ethernet 통신을 막는다.
		if(net_disable == NO) {
//			rs422if.connected = YES;
			devState.rs422if_connected = YES;
			NVIC_DisableIRQ(EthernetMAC_IRQn); /*MSS_MAC_disable_intr();*/ /* (+) 210621 CJKIM, disable mac interrupt before sending. */
			net_disable = YES;
iprintf("net_disable\r\n");
		}

	}

	return RE_OK;

}

#endif

int rs422_SendData()
{
	uint8_t TSTAT;
	uint8_t packetInfo;

	LCAS_PACKET* pPacket;

//	char pBuffer[RS422_TX_DATA_BUF_SIZE];
	char pBuffer[PACKET_HEADER_SIZE];

	int data_cnt;
	int ret;

	if(tx.Head == tx.Tail) {
		return 0;
	}
	else {
		TSTAT = FPGA_READ_WORD(RS422_TX_STATE_ADDR);

		if(TSTAT&0x80) {
			data_cnt = r_get_available(&tx);
//iprintf("data_cnt=%d\n",data_cnt);

			if (data_cnt >= PACKET_HEADER_SIZE) {

				ret = r_peek(pBuffer,  PACKET_HEADER_SIZE);

				pPacket = (LCAS_PACKET*)(pBuffer);

//iprintf("rs422_SendData TSTAT=%x H=%d T=%d data_cnt=%d len=%d[%d] nPacketCode=%x\n",TSTAT,tx.Head ,tx.Tail, data_cnt , pPacket->nPacketLength, tx.Data[tx.Head],   pPacket->nPacketCode);

				if(data_cnt >= pPacket->nPacketLength) {
					FPGA_WR_RS422_TX_FLAG_CLEAR;

					pTxMem[0] = STATE_REQUEST_CMD;			// FPGA에서 자동으로 결정된다. (0xB0)

					ret = r_wite_TxMem(pPacket->nPacketLength);

					FPGA_WR_RS422_TX_LENGTH((pPacket->nPacketLength >> 2) + 1);
					FPGA_WR_RS422_TX_DATA_SAVED;			// 보낼데이터가 저장되었다. - Master에서 요청시 전송된다.
//					rs422if.TSTAT = 1;
				}
			}
		}
	}

}

void rs422_CPUModeSetting(void)
{
	FPGA_WR_RS422_MODE_CPU;
	rs422if.RunMode = RS422_RUN_CPUMODE;
}

int r_get_buffer_size()
{
	int ret = 0;

	ret = RS422_TX_DATA_BUF_SIZE;

	return ret;
}

int r_get_buffer_size4()
{
	int ret = 0;

	ret = RS422_TX_DATA_BUF_SIZE4;

	return ret;
}

//int r_get_available(RS422_BUF *bf)
int r_get_available()
{
	int ret = 0;

	ret = (tx.Head <= tx.Tail )? tx.Tail - tx.Head : r_get_buffer_size() + tx.Tail - tx.Head;

	return ret;
}

#if RS422_BUFFER_4BYTE
int r_enqueue(const char* pBuffer, int size)
{
	int ret = 0;
	int buf_size4;
	int size4;

//	MSS_MAC_disable_intr();

	buf_size4 = r_get_buffer_size4();
	size4 = size >> 2;

	if (tx.Tail4 + size4 >= buf_size4) {
		int copySize = ((buf_size4 - tx.Tail4) << 2);
		memcpy(&tx.iData[tx.Tail4], &pBuffer[0], copySize);
		memcpy(tx.iData, &pBuffer[copySize], size - copySize);
	}
	else {
		memcpy(&tx.iData[tx.Tail4], &pBuffer[0], size);
	}

	tx.Tail4 = (tx.Tail4 + size4) % buf_size4;

	ret = tx.Head4 <= tx.Tail4 ? tx.Tail4 - tx.Head4 : buf_size4 + tx.Tail4 - tx.Head4;

//	MSS_MAC_enable_intr();
	return ret;
}
#else
int r_enqueue(const char* pBuffer, int size)
{
	int ret = 0;
	int buf_size;

//	MSS_MAC_disable_intr();

	buf_size = r_get_buffer_size();

#if RS422_SIZE_4BYTE_ALIGN
	size = (((size + 3) >> 2) << 2);
//	iprintf("----------------------------------------------size[%d]\n",size);
#endif

	if (tx.Tail + size >= buf_size) {
		int copySize = buf_size - tx.Tail;
		memcpy(&tx.Data[tx.Tail], &pBuffer[0], copySize);
		memcpy(tx.Data, &pBuffer[copySize], size - copySize);
	}
	else {
		memcpy(&tx.Data[tx.Tail], &pBuffer[0], size);
	}

	tx.Tail = (tx.Tail + size) % buf_size;

	ret = tx.Head <= tx.Tail ? tx.Tail - tx.Head : buf_size + tx.Tail - tx.Head;

//	MSS_MAC_enable_intr();
	return ret;
}
#endif

// 20211123
#define RS422_TX_MEM_ADDR_FIX					1
int r_wite_TxMem(int size)
{
	int ret = 0;
	int buf_size,size4;

	uint32_t *pAddr; // = (pTxMem + 1);
	uint32_t *pData;
//	MSS_MAC_disable_intr();

	buf_size = r_get_buffer_size();

#if RS422_SIZE_4BYTE_ALIGN
		size = (((size + 3) >> 2) << 2);
//		iprintf("XXX----------------------------------------------size[%d]\n",size);
#endif

#if RS422_USE_MEMCPY
	if (tx.Head + size > buf_size) {
		int copy = buf_size - tx.Head;
		memcpy((uint8_t *)(pTxMem+1), (const void*)&tx.Data[tx.Head], copy);
		memcpy((uint8_t *)(pTxMem_byte+copy + 4), (const void*)tx.Data, size - copy); 	
	}
	else {
		memcpy((pTxMem + 1), (const void*)&tx.Data[tx.Head], size);
	}
#else
	if (tx.Head + size > buf_size) {
		int copy = buf_size - tx.Head;
		size4 = copy >> 2;
		pData = (uint32_t *)(&tx.Data[tx.Head]);
		pAddr = (pTxMem + 1);
		for(int i=0 ; i < size4 ;i++) {
			pAddr[i] =	pData[i];
//			iprintf("[%d]\t%x \t%x \t%x\n",i, pTxMem[i], pAddr[i], pData[i]);
		}
		size4 = (size - copy) >> 2;
		pData = (uint32_t *)(tx.Data);
		pAddr = (pTxMem_byte + copy + 4);
		for(int i=0 ; i < size4 ;i++) {
			pAddr[i] =	pData[i];
//			iprintf("[%d]\t%x \t%x \t%x\n",i, pTxMem[i], pAddr[i], pData[i]);
		}
	}
	else {
		size4 = size >> 2;
		pData = (uint32_t *)(&tx.Data[tx.Head]);
		pAddr = (pTxMem + 1);
		for(int i=0 ; i < size4 ;i++) {
			pAddr[i] =	pData[i];
//			iprintf("[%d]\t%x\t%x\t%x\n",i, pTxMem[i], pAddr[i]);
		}
	}
#endif

//iprintf("r_save_TxMem H[%d] T[%d] len[%d] nPacketCode[%x] addr[0x%x]\n",tx.Head,tx.Tail, tx.Data[tx.Head], pTxMem[3],pTxMem+1);

	tx.Head = (tx.Head + size) % buf_size;
	ret = size;

//	MSS_MAC_enable_intr();

	return ret;
}

#if RS422_BUFFER_4BYTE
int r_peek(char* pBuffer, int size)
{
	int ret = 0;
	int buf_size4;
	int size4;

//	if (tx.Head == tx.Tail)
//		return 0;

	buf_size4 = r_get_buffer_size4();
	size4 = size >> 2;

	if (tx.Head4 + size4 > buf_size4) {
		int copy = ((buf_size4 - tx.Head4) << 2);

		memcpy(&pBuffer[0], (const void*)&tx.iData[tx.Head4], copy);
		memcpy(&pBuffer[copy], (const void*)tx.iData, size - copy);
	}
	else {
		memcpy(&pBuffer[0], (const void*)&tx.Data[tx.Head], size);
	}

	ret = size;

	return ret;
}
#else
int r_peek(char* pBuffer, int size)
{
	int ret = 0;

//	if (tx.Head == tx.Tail)
//		return 0;


	if (tx.Head + size > r_get_buffer_size()) {
		int copy = r_get_buffer_size() - tx.Head;

		memcpy(&pBuffer[0], (const void*)&tx.Data[tx.Head], copy);
		memcpy(&pBuffer[copy], (const void*)tx.Data, size - copy);
	}
	else {
		memcpy(&pBuffer[0], (const void*)&tx.Data[tx.Head], size);
	}

	ret = size;

	return ret;
}
#endif

#if 0
void rs422_Test()
{

	uint8_t pBuffer[100];
	int i=0;
	//	pBuffer = (pTxMem + 1);

		for(i=0;i<48;i++) {
			pBuffer[i] = i;
		}

	pTxMem[0] = AUTO_MODE_CMD;
	pTxMem[1] = AUTO_MODE_CMD;

//	memcpy((uint8_t *)(pTxMem+2),&RS422_Main_State,sizeof(RS422_Main_State));

	memcpy((&pTxMem[1]), &pBuffer[0], 44);
	//memcpy((&pBuffer[0]), &LcasState, 48);

	//		memcpy((void *)(&pTxMem[1]), (const void*)&pBuffer[0], size);
	//			memcpy((pTxMem + 1), (const void*)&tx.Data[tx.Head], size);

//	pTxMem[1] = 0x12345678;//tx.Data[tx.Head];

	for(i=0;i<48;i++) {
	//pTxMem[i+1] = 0x12345678;//tx.Data[tx.Head];
	iprintf("AS 11 [%d]%x %x\n",i,pTxMem[i], pTxMem_byte[i]);

	}
	iprintf("===========================================================\n");
}
#endif




