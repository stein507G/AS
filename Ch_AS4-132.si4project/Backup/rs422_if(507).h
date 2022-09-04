/**
 * rs422_if.h
 */

#ifndef RS422_IF_H_
#define RS422_IF_H_



#define RS422_CHROZEN_LC_SLAVE_COUNT    		3
#define RS422_CONFIG_STASUS_SIZE    	64 // 256 bytes
#define RS422_CONFIG_STASUS_SIZE_SHIFT    	6

#define RS422_RUN_CPUMODE    		0
#define RS422_RUN_AUTOMODE    	1
#define RS422_STATE_IDLE   0
#define RS422_STATE_RUN    1



//=================================================================
// RS422
// ----------------------------------
//#define RS422_MASTER			0
//#define RS422_SLAVE				1

	#define RS422_ID_MASTER			0x0E
	#define RS422_ID_GLOBAL			0x0A

	#define RS422_ID_UVD			0x01
	#define RS422_ID_AS				0x02
	#define RS422_ID_OVEN			0x03
//	#define RS422_ID_PDA				0x04
//	#define RS422_ID_AUX				0x05
//-----------------------------------------------------------------

#define RS422_TX_STATE_ADDR			0x2200
#define RS422_MY_ID_ADDR			0x220F
#define RS422_SEND_TRIG_ADDR 		0x2210
#define RS422_TX_LENGTH_ADDR 		0x2211
#define RS422_TX_CRC_ADDR 			0x2212
#define RS422_MASTER_MODE_ADDR			0x2213			// 0:cpuode,1:Auto mode
#define RS422_TX_FLAG_RESET_ADDR		0x2220
// 20210713
#define RS422_TX_DATA_SAVED_FLAG_ADDR		0x2222

#define RS422_RX_STATE_ADDR		0x2600
#define RS422_RCV_GALLOW_ADDR		0x2602				// 0:Off(자신의 ID만 응답), 1:ON(ID에 상관없이 응답)
#define RS422_RX_INFO_ADDR			0x2610	
#define RS422_RX_AUTO_RESULT_ADDR		0x2613	
#define RS422_RX_FLAG_RESET_ADDR		0x2620

#define FPGA_WR_RS422_SEND_TRIG(X)		*(volatile uint32_t *) (FPGA_BASE_ADDR + (RS422_SEND_TRIG_ADDR<<2)) = (X)
#define FPGA_WR_RS422_TX_LENGTH(X)		*(volatile uint32_t *) (FPGA_BASE_ADDR + (RS422_TX_LENGTH_ADDR<<2)) = (X)

#define FPGA_WR_RS422_MODE_AUTO				FPGA_WRITE_WORD(RS422_MASTER_MODE_ADDR,1)
#define FPGA_WR_RS422_MODE_CPU				FPGA_WRITE_WORD(RS422_MASTER_MODE_ADDR,0)
#define FPGA_WR_RS422_TX_FLAG_CLEAR			FPGA_WRITE_WORD(RS422_TX_FLAG_RESET_ADDR,1)
//#define FPGA_WR_RS422_RX_FLAG_CLEAR		*(volatile uint32_t *) (FPGA_BASE_ADDR + (RS422_RX_FLAG_RESET_ADDR<<2)) = 1
#define FPGA_WR_RS422_RX_FLAG_CLEAR			FPGA_WRITE_WORD(RS422_RX_FLAG_RESET_ADDR,1)

// 20210713
#define FPGA_WR_RS422_TX_DATA_SAVED			FPGA_WRITE_WORD(RS422_TX_DATA_SAVED_FLAG_ADDR,1)

#define RS422_RX_CMD_ADDR						0x2400
	#define AUTO_MODE_CMD    				0x000000C0		// Auto Mode Master -> Slave
	//#define CPU_MODE_SOC_CMD    	0x000000A0		// Auto Mode Master -> Slave
	//#define CPU_MODE_APC_CMD    	0x000000B0		// Auto Mode Slave -> Master
	#define STATE_REQUEST_CMD    		0x000000B0		// Auto Mode Slave -> Master (자동응답 버퍼에 쌓인것을 읽는다.)
	//#define STATE_SEND_RET_LEN   	0x00000072		// 112 + 2
	#define RS422_STATE_SEND_CMD    0x000000B0		// Auto Mode Slave -> Master 
	#define RS422_CMD_PACKET_SEND		0x00000009		// Master -> Slave ( CMD Packet - Ethernet통신에서 사용하던 패킷과 동일하게 전송 )

#define RS422_RX_PACKET_LENGTH_ADDR		0x2401
#define RS422_RX_PACKET_CODE_ADDR			0x2403
#define RS422_RX_SLOT_OFFSET_ADDR			0x2405
#define RS422_RX_SLOT_SIZE_ADDR				0x2406












#if 0


#define rs422_APC_CMD_CONNECT_CHECK   	0x00000003

#define rs422_APC_CMD_START							0x05
#define rs422_APC_CMD_STOP							0x07

#define rs422_APC_CMD_REQUEST   			0x09

#define rs422_APC_CMD_INLET_CONFIG		0x10
//#define APC_CMD_INLET_CONFIG_1		0x10
//#define APC_CMD_INLET_CONFIG_2		0x11
//#define APC_CMD_INLET_CONFIG_3		0x12

#define rs422_APC_CMD_INLET_SETTING		0x11
//#define APC_CMD_INLET_SETTING_1		0x13
//#define APC_CMD_INLET_SETTING_2		0x14
//#define APC_CMD_INLET_SETTING_3		0x15

//#define rs422_APC_CMD_INLET_FLOW_PRGM		0x12
//#define APC_CMD_INLET_FLOW_PRGM_1		0x16
//#define APC_CMD_INLET_FLOW_PRGM_2		0x17
//#define APC_CMD_INLET_FLOW_PRGM_3		0x18

//#define rs422_APC_CMD_INLET_PRESS_PRGM		0x13
//#define APC_CMD_INLET_PRESS_PRGM_1		0x19
//#define APC_CMD_INLET_PRESS_PRGM_2		0x1A
//#define APC_CMD_INLET_PRESS_PRGM_3		0x1B

#define rs422_APC_CMD_DET_CONFIG_1		0x14
#define rs422_APC_CMD_DET_CONFIG_2		0x15
#define rs422_APC_CMD_DET_CONFIG_3		0x16

#define rs422_APC_CMD_DET_SETTING_1		0x17
#define rs422_APC_CMD_DET_SETTING_2		0x18
#define rs422_APC_CMD_DET_SETTING_3		0x19

#define rs422_APC_CMD_AUX_SETTING_1		0x1A
#define rs422_APC_CMD_AUX_SETTING_2		0x1B
#define rs422_APC_CMD_AUX_SETTING_3		0x1C

#define rs422_APC_CMD_CALIB_SETTING		0x1D

#define rs422_APC_CMD_MAIN_STATE		0x1F

//================================
// APC --> SOC
#define rs422_APC_CMD_INLET_DATA		0x20
#define rs422_APC_CMD_DET_DATA		0x21
#define rs422_APC_CMD_AUX_DATA		0x22
#define rs422_APC_CMD_CALIB_DATA		0x23
#define rs422_APC_CMD_VOLT_DATA		0x24

#define rs422_APC_CMD_INLET_SETTING_1		0x25
#define rs422_APC_CMD_INLET_SETTING_2		0x26
#define rs422_APC_CMD_INLET_SETTING_3		0x27


//				rs422_APC_CMD_INLET_FLOW_PRGM
#define rs422_APC_CMD_INLET_FLOW_PRGM_1		0x28
#define rs422_APC_CMD_INLET_FLOW_PRGM_2		0x29
#define rs422_APC_CMD_INLET_FLOW_PRGM_3		0x2A

#define rs422_APC_CMD_INLET_PRESS_PRGM_1	0x2B
#define rs422_APC_CMD_INLET_PRESS_PRGM_2	0x2C
#define rs422_APC_CMD_INLET_PRESS_PRGM_3	0x2D

#define rs422_MAX_PACKET_NO    	0x2D		// Auto Mode APC -> SOC

#endif
// 아래는 사용안함. - 테스트용임.
#define APC_CMD_CON_STATUS    	0xC0
#define APC_CMD_START    		0xB0
#define APC_CMD_STOP    		0xBF
/*
#define APC_CMD_INLET_SET    	0xD0
#define APC_CMD_DET_SET    		0xD1
#define APC_CMD_AUX_SET    		0xD2
#define APC_CMD_CALIB_SET    	0xD3
*/
// ==================================




// TCP_WRITE_PACKET
#define RS422_ENQUEUE_PACKET { \
		r_enqueue(pPacketBuff, ((LCAS_PACKET *)pPacketBuff)->nPacketLength, &tx); \
	}

#define RS422_SEND_LCAS_PACKET(Code,full_data,entry_data)	{ \
	MAKE_LCAS_PACKET(Code,full_data,entry_data); \
	RS422_ENQUEUE_PACKET; \
}

#define RS422_SEND_LCAS_TABLE_PACKET(Code,TableNo,full_data,entry_data)	{ \
	MAKE_LCAS_TABLE_PACKET(Code,TableNo,full_data,entry_data); \
	RS422_ENQUEUE_PACKET; \
}

#define RS422_SEND_REQUEST_LCAS_PACKET(Code,full_data,entry_data)	{ \
	REQUEST_LCAS_PACKET(Code,full_data,entry_data); \
	RS422_ENQUEUE_PACKET; \
}

#define RS422_SEND_REQUEST_LCAS_TABLE_PACKET(Code,TableNo,full_data,entry_data)	{ \
	REQUEST_LCAS_TABLE_PACKET(Code,TableNo,full_data,entry_data); \
	RS422_ENQUEUE_PACKET; \
}

#define RS422_SEND_RESPONSE_LCAS_PACKET(full_data,ReqHeader)	{ \
	RESPONSE_LCAS_PACKET(full_data,ReqHeader); \
	RS422_ENQUEUE_PACKET; \
}

#define RS422_SEND_RESPONSE_LCAS_TABLE_PACKET(TableNo,full_data,ReqHeader)	{ \
	RESPONSE_LCAS_TABLE_PACKET(TableNo,full_data,ReqHeader); \
	RS422_ENQUEUE_PACKET; \
}

#define RS422_SEND_ACK_LCAS_PACKET(ReqHeader)	{ \
	ACK_LCAS_PACKET(ReqHeader); \
	RS422_ENQUEUE_PACKET; \
}



#if 0
#define RS422_WRITE_PACKET		// TCP_WRITE_PACKET
	//MAKE_LCAS_PACKET

#define RS422_SEND_LCAS_PACKET(Code,full_data,entry_data)	{ \
	MAKE_LCAS_PACKET(Code,full_data,entry_data); \
	RS422_WRITE_PACKET; \
}

	//MAKE_LCAS_TABLE_PACKET
RS422_SEND_LCAS_TABLE_PACKET
	//REQUEST_LCAS_PACKET


RS422_SEND_REQUEST_LCAS_PACKET
	//REQUEST_LCAS_TABLE_PACKET

RS422_SEND_REQUEST_LCAS_TABLE_PACKET

//IS_REQUEST_PACKET
USE_LCAS_PACKET
	//IS_ACK_PACKET
	//RESPONSE_LCAS_PACKET
RS422_SEND_RESPONSE_LCAS_PACKET




	//RESPONSE_LCAS_TABLE_PACKET
RS422_SEND_RESPONSE_LCAS_TABLE_PACKET

ACK_LCAS_PACKET
RS422_SEND_ACK_LCAS_PACKET



#define RS422_WRITE_PACKET(Code,full_data,entry_data)	{ \
#define RS422_BUFF_LCAS_PACKET(Code,full_data,entry_data)	{ \
	MAKE_LCAS_PACKET(Code,full_data,entry_data); \
	RS422_WRITE_PACKET; \
}

#define SEND_LCAS_PACKET(Code,full_data,entry_data)	{ \
	MAKE_LCAS_PACKET(Code,full_data,entry_data); \
	TCP_WRITE_PACKET; \
}

#define RS422_BUFF_LCAS_PACKET(Code,full_data,entry_data)	{ \

if(rs422if.connected) {
	memcpy(responseDataBuf[rs422if.responseDataBufHead], diagData, sizeof(diagData));
	rs422if.responseDataBufHead = (++rs422if.responseDataBufHead) & 0x7;
}

responseDataBufHead
responseDataBuf


#endif





typedef struct
{
	uint8_t		RunMode; // 0:CPU Mode, 1:else Auto Mode
	uint8_t		State;
	uint8_t		slvch; 	// slave channel number
	
	uint8_t		RSTAT;
	uint8_t		rx_slvid;	

//	uint8_t		firstSend;	// 맨처음에는 
	uint8_t		TSTAT;			// 송신완료 면 1 , 아니면 0  // (1)송신버퍼에저장 , (0)송신버퍼에저장안됨 - 전송할 데이터가 있다.
	uint8_t 	busyTxHead; // 저장할 펑션 번지
	uint8_t 	busyTxTail; // 전송할 펑션 번지 마지막
	uint8_t 	busyFn[8];	// 전송할 펑션번호 
	
	uint16_t	TxTime;
	
	uint8_t		AutoEnd;
uint8_t		rx_intr;		// ryu 추가 
	uint8_t		RxAllAllow; // when this bit is set, receive the frame no matter what the destination id

	uint8_t		sendCmd[5];
	uint8_t		sendStart;
	uint8_t		sendSop;
	
//	uint32_t	Config[RS422_CHROZEN_LC_SLAVE_COUNT][RS422_CONFIG_STASUS_SIZE];
	uint32_t	MainStatus[RS422_CONFIG_STASUS_SIZE];
//	uint32_t	SlaveStatus[RS422_CHROZEN_LC_SLAVE_COUNT][RS422_CONFIG_STASUS_SIZE];

	uint8_t		TST_patternA;

	uint32_t	reSendData[5];

	uint8_t		enable; 	//  alive;

#if 0
	// 보낼 데이터의 종류별로 따로 버퍼를 둔다.
	// 전송시 우선적으로 SelfMsg를 먼저 보낸다.
	// 전송시 SelfMsgBuf에 보낼 메시지가 있는지 검사하여 있으면 보내고, 없으면 다른 보낼 데이터가 있는지 검사하여 보낸다.
	uint32_t	selfMsgBufHead;	// selfMsg 데이터의 저장할 위치
	uint32_t	selfMsgBufTail;	// 데이터의 전송할 위치



	uint32_t	stateDataBufHead;	// state 데이터의 저장할 위치
	uint32_t	stateDataBufTail;	// 데이터의 전송할 위치

	uint32_t	diagDataBufHead;	// diag 데이터의 저장할 위치
	uint32_t	diagDataBufTail;	// 데이터의 전송할 위치

	uint32_t	scanDataBufHead;	// scan 데이터의 저장할 위치
	uint32_t	scanDataBufTail;	// 데이터의 전송할 위치

	uint32_t	acqDataBufHead;	// acq 데이터의 저장할 위치
	uint32_t	acqDataBufTail;	// 데이터의 전송할 위치

uint32_t	responseDataBufHead; // response 데이터의 저장할 위치
uint32_t	responseDataBufTail; // 데이터의 전송할 위치


/*
#define RS422_BUF_SELFMSG_DATA				(1)
#define RS422_BUF_STATE_DATA					(2)
#define RS422_BUF_DIAG_DATA						(3)
#define RS422_BUF_ACQ_DATA						(4)
#define RS422_BUF_SCAN_DATA						(5)

#define RS422_BUF_RESPONSE_DATA				(6)	// 요청한 데이터에 대한 응답 
*/
	uint32_t	sendDataInfo[32];	// 데이터의 종류
	uint32_t	sendDataHead;	// 저장할 데이터 위치
	uint32_t	sendDataTail;	// 보내야할 데이터 위치
#endif

	uint32_t	connected;

} RS422_ST; // GC Service struct

#define RS422_TX_DATA_BUF_SIZE		1024
#define RS422_TX_DATA_BUF_SIZE4		256

typedef struct
{
	int			Head;		// 읽을 데이터 위치
	int			Tail;

	int 		Head4; 	// 4byte 단위 인덱싱
	int 		Tail4;
	
//	char*		pBuf;
	char		Data[RS422_TX_DATA_BUF_SIZE];
	int			iData[RS422_TX_DATA_BUF_SIZE4];
} RS422_BUF;





#define RS422_BUF_STATE_DATA					(1)
#define RS422_BUF_DIAG_DATA						(2)
#define RS422_BUF_ACQ_DATA						(3)
#define RS422_BUF_SCAN_DATA						(4)
#define RS422_BUF_SELFMSG_DATA				(5)

#define RS422_BUF_RESPONSE_DATA				(6)	// 요청한 데이터에 대한 응답 



void rs422_SetMyBoardId(uint8_t Id);
void InterruptEnable_rs422(void);
void rs422_init(void);
//void rs422_PacketParse_init();

//void rs422_make_SendData_Header();

#if RS422_MASTER
int rs422_PacketParse_Master();
int rs422_REQUEST_Send(uint32_t slvid);
int rs422_SendCmd();	// Master --> Slave
#else
int rs422_PacketParse_Slave();
#endif

int rs422_PacketParse(unsigned int nPacketCode, char *pData);
int rs422_SendData();


void rs422_State_Send_buffer(void);
void rs422_State_Send_buffer_blank(void);



//==============================================================================

void rs422_CPUModeSetting(void);


//==============================================================================
int r_get_buffer_size();
int r_get_buffer_size4();


int r_get_available(RS422_BUF *bf);
int r_enqueue(const char* pBuffer, int size, RS422_BUF *bf);

//int r_save_TxMem(int size, RS422_BUF *bf);
int r_wite_TxMem(int size, RS422_BUF *bf);
int r_peek(char* pBuffer, int size, RS422_BUF *bf);


void rs422_Test();

#endif // #ifndef RS422_IF_H_
