/*
 * rs422_if.c
 */

#include "config.h"

#include "rs422_if.h"
#include "util.h"

#include "fpga_ChAS.h"

uint32_t * pTxMem = (uint32_t *)(0x50008000);	// 0x2000 (32bit) -- 0010 0000 0000 0000 << 2  --> 1000 0000 0000 0000 (0x8000) --> 0x50008000
uint32_t * pRxMem = (uint32_t *)(0x50009000);	// 0x2400 (32bit) -- 0010 0100 0000 0000 << 2  --> 1001 0000 0000 0000 (0x9000) --> 0x50009000

uint8_t * pTxMem_byte = (uint8_t *)(0x50008000);	// 0x2000 (32bit) -- 0010 0000 0000 0000 << 2  --> 1000 0000 0000 0000 (0x8000) --> 0x50008000
uint8_t * pRxMem_byte = (uint8_t *)(0x50009000);	// 0x2400 (32bit) -- 0010 0100 0000 0000 << 2  --> 1001 0000 0000 0000 (0x9000) --> 0x50009000

//static int totalCntI=0;	
//static int totalCntD=0;			

#define APC_RS422_PACKET_FUNCTION_COUNT		(rs422_MAX_PACKET_NO + 1)

void (*pfAPC_Rs422_Packet[APC_RS422_PACKET_FUNCTION_COUNT])(uint32_t slvid_addr);

RS422_ST rs422if;

extern LCAS_STATE_t	LcasState;


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

void rs422_init(void)
{
	uint8_t i=0,j=0;
	
	rs422if.RunMode = RS422_RUN_CPUMODE;
	rs422if.State = RS422_STATE_RUN;
	rs422if.RxAllAllow = 0;
	
	rs422if.TSTAT = 1;	// 처음엔 완료로 셋팅

/**
 *
 */
	rs422if.sendDataHead = 0;
	rs422if.sendDataTail = 0;

	rs422if.selfMsgBufHead = 0;
	rs422if.selfMsgBufTail = 0;
//	rs422if.stateDataBufHead = 0;
//	rs422if.stateDataBufTail = 0;
	rs422if.diagDataBufHead = 0;
	rs422if.diagDataBufTail = 0;
/*
	rs422if.scanDataBufHead = 0;
	rs422if.scanDataBufTail = 0;
	rs422if.acqDataBufHead = 0;
	rs422if.acqDataBufTail = 0;
*/
}

void rs422_PacketParse_init()
{
#if 0
	int i;
	
	for(i=0;i<APC_RS422_PACKET_FUNCTION_COUNT;i++) {
		pfAPC_Rs422_Packet[i] = RS422_BlankFunction;
	} 
// SOC --> APC
	pfAPC_Rs422_Packet[rs422_APC_CMD_CONNECT_CHECK] = rs422_APC_CMD_Connect_fn;
	pfAPC_Rs422_Packet[rs422_APC_CMD_START] = rs422_APC_CMD_Start_fn;
	pfAPC_Rs422_Packet[rs422_APC_CMD_STOP] = rs422_APC_CMD_Stop_fn;

	if(GCC_SLAVE == gcsvc.MasterSlave) {	// slave -- APC
#if 0		// APC
		pfAPC_Rs422_Packet[rs422_APC_CMD_REQUEST] = rs422_APC_CMD_REQUEST_fn;
		
pfAPC_Rs422_Packet[rs422_APC_CMD_INLET_CONFIG] = rs422_APC_CMD_INLET_CONFIG_fn;
		pfAPC_Rs422_Packet[rs422_APC_CMD_INLET_SETTING] = rs422_APC_CMD_INLET_SETTING_fn;
		pfAPC_Rs422_Packet[rs422_APC_CMD_INLET_FLOW_PRGM] = rs422_APC_CMD_INLET_FLOW_PRGM_fn;
		pfAPC_Rs422_Packet[rs422_APC_CMD_INLET_PRESS_PRGM] = rs422_APC_CMD_INLET_PRESS_PRGM_fn;
		
pfAPC_Rs422_Packet[rs422_APC_CMD_DET_CONFIG_1] = rs422_APC_CMD_DET_CONFIG_1_fn;
pfAPC_Rs422_Packet[rs422_APC_CMD_DET_CONFIG_2] = rs422_APC_CMD_DET_CONFIG_2_fn;
pfAPC_Rs422_Packet[rs422_APC_CMD_DET_CONFIG_3] = rs422_APC_CMD_DET_CONFIG_3_fn;

		pfAPC_Rs422_Packet[rs422_APC_CMD_DET_SETTING_1] = rs422_APC_CMD_DET_SETTING_1_fn;
		pfAPC_Rs422_Packet[rs422_APC_CMD_DET_SETTING_2] = rs422_APC_CMD_DET_SETTING_2_fn;
		pfAPC_Rs422_Packet[rs422_APC_CMD_DET_SETTING_3] = rs422_APC_CMD_DET_SETTING_3_fn;

		pfAPC_Rs422_Packet[rs422_APC_CMD_AUX_SETTING_1] = rs422_APC_CMD_AUX_SETTING_1_fn;
		pfAPC_Rs422_Packet[rs422_APC_CMD_AUX_SETTING_2] = rs422_APC_CMD_AUX_SETTING_2_fn;
		pfAPC_Rs422_Packet[rs422_APC_CMD_AUX_SETTING_3] = rs422_APC_CMD_AUX_SETTING_3_fn;

		pfAPC_Rs422_Packet[rs422_APC_CMD_CALIB_SETTING] = rs422_APC_CMD_CALIB_SETTING_fn;

		pfAPC_Rs422_Packet[rs422_APC_CMD_MAIN_STATE] = rs422_APC_CMD_MAIN_STATE_fn;
#endif		
	}
	else {	// Master
	// APC --> SOC
		pfAPC_Rs422_Packet[rs422_APC_CMD_INLET_DATA] = rs422_APC_CMD_INLET_DATA_fn;
		pfAPC_Rs422_Packet[rs422_APC_CMD_DET_DATA] = rs422_APC_CMD_DET_DATA_fn;
		pfAPC_Rs422_Packet[rs422_APC_CMD_AUX_DATA] = rs422_APC_CMD_AUX_DATA_fn;
		pfAPC_Rs422_Packet[rs422_APC_CMD_CALIB_DATA] = rs422_APC_CMD_CALIB_DATA_fn;
		pfAPC_Rs422_Packet[rs422_APC_CMD_VOLT_DATA] = rs422_APC_CMD_VOLT_DATA_fn;
	}
#endif
}

#if RS422_MASTER
int rs422_PacketParse_Master()
{
	uint8_t RSTAT;
	uint32_t rxinfo;
	uint8_t rxid;
	uint8_t slvid;
	uint8_t rxlen;
	uint8_t rxcmd;

	uint32_t packetID = 0;
	uint32_t slvid_addr;

	RSTAT = FPGA_READ_WORD(RS422_RX_STATE_ADDR);
//dp("s--\n");
	if(RSTAT&0x80)
	{
		FPGA_WR_RS422_RX_FLAG_CLEAR;
		rxinfo = FPGA_READ_WORD(RS422_RX_INFO_ADDR);
		rxid  = rxinfo; rxinfo = rxinfo>>8;
		rxlen = rxinfo; rxinfo = rxinfo>>8;
		rxcmd = rxinfo;
		slvid = rxid&0xF;
		rs422if.rx_slvid = rxid&0xF;

//dp("[RX]ID:%02X,LEN:%02X,CMD:%02X,RSTAT:%02X,D[0]:%08lX\r\n",rxid,rxlen,rxcmd,RSTAT, FPGA_READ_WORD(0x2401 + (APC_CONFIG_STASUS_SIZE*(slvid-1))));				

		if(rxcmd == AUTO_MODE_CMD) {
		dp("return  AUTO_MODE_CMD\n");
			return;
		}

		slvid_addr = (rs422if.rx_slvid-1)<<RS422_CONFIG_STASUS_SIZE_SHIFT;
		packetID = FPGA_READ_WORD(0x2401 + slvid_addr);

//dp("slvid = %x  packetID=%x rxLen=%d\n",slvid,packetID,rxlen);
//ShowMemoryWord((pRxMem + (APC_CONFIG_STASUS_SIZE*(slvid-1))), rxlen);

		if(packetID > rs422_MAX_PACKET_NO) return RE_ERR;
		
		pfAPC_Rs422_Packet[packetID](slvid_addr);
//		pfAPC_Rs422_Packet[packetID](apcif.rx_slvid);

	}

	return RE_OK;
}
#else
int rs422_PacketParse_Slave()
{
	uint8_t RSTAT;
	uint32_t rxinfo;
	uint8_t rxid;
//	uint8_t slvid;
	uint8_t rxlen;
	uint8_t rxcmd;

	uint32_t packetID = 0;
	uint32_t slvid_addr;
	
	RSTAT = FPGA_READ_WORD(RS422_RX_STATE_ADDR);

//	if(RSTAT != apcif.RSTAT) {
//		apcif.RSTAT = RSTAT;
//	}

	if(RSTAT&0x80)
	{
		FPGA_WR_RS422_RX_FLAG_CLEAR;
		rxinfo = FPGA_READ_WORD(RS422_RX_INFO_ADDR);
		rxid  = rxinfo; rxinfo = rxinfo>>8;
		rxlen = rxinfo; rxinfo = rxinfo>>8;
		rxcmd = rxinfo;
//		slvid = rxid&0xF;

		rs422if.rx_slvid = rxid&0xF;
		
		slvid_addr = 0;

		packetID = FPGA_READ_WORD(0x2401);

//dp("[RX]ID:%02X,LEN:%02X,CMD:%02X,RSTAT:%02X,D[0]:%08lX\r\n",rxid,rxlen,rxcmd,RSTAT, packetID);
//		ShowMemoryWord(pRxMem, rxlen);

		if(packetID > rs422_MAX_PACKET_NO) return RE_ERR;

		pfAPC_Rs422_Packet[packetID](0);

	}

	return RE_OK;

}
#endif






// ======================================================
void rs422_State_Send_buffer_blank(void) {}

// APC에서 버퍼에 저장한다. 전송
void rs422_State_Send_buffer(void)
{
	int i;
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;

		pTxMem[0] = RS422_STATE_SEND_CMD;
pTxMem[1] = PACKCODE_LCAS_STATE;

		memcpy((uint8_t *)(pTxMem+2),&LcasState,sizeof(LcasState));
		
		FPGA_WR_RS422_TX_LENGTH((sizeof(LcasState) >> 2) + 2); 	// tx_len : uint8_t --> uint32_t , 
//테스트 - master에서 요청시 자동으로 보낼 수 있도록 저장만 한다.  트리거신호는 없다.(FPGAㅔ서 자동으로)
// FPGA_WR_RS422_SEND_TRIG( RS422_ID_MASTER ); // tx_start
		
#if RS422_PROTOCOL_UP
// 20210713
		FPGA_WR_RS422_TX_DATA_SAVED;
#endif

		rs422if.TSTAT = 1;//TSTAT;			
	}
	else {
		rs422if.TSTAT = 0;//TSTAT;
	}

dp("TSTAT %02X\r\n",TSTAT);	

}




#if 0

RS422_Inlet_Config_t	RS422_Inlet_Config[3];
RS422_Inlet_Setting_t RS422_Inlet_Setting_XXX[3];
RS422_Inlet_Setting_t RS422_Inlet_Setting[3];
RS422_APC_FLOW_PRGM_t	RS422_APC_FLOW_PRGM[3];
RS422_APC_PRESS_PRGM_t	RS422_APC_PRESS_PRGM[3];
RS422_Det_Config_t	RS422_Det_Config[3];
RS422_Det_Setting_t	RS422_Det_Setting[3];
RS422_Aux_Setting_t	RS422_Aux_Setting[3];
RS422_Calibration_Set_t	RS422_Calibration_Set;

RS422_Main_State_t RS422_Main_State;
//RS422_Main_State_t apcMainState;
//RS422_Main_State_t apcAPC_State;
//RS422_Inlet_Data_t	RS422_Inlet_Data;
//RS422_Det_Data_t	RS422_Det_Data;
//RS422_Aux_Data_t	RS422_Aux_Data;
RS422_Calibration_Data_t	RS422_Calibration_Data;
RS422_APC_Volt_data_t	RS422_APC_Volt_data;

//extern YL6700GC_STATE_t nsState;	// new State - YL6700GC
extern MS5611_PROM_t ms5611Cal;

extern GCSVC_ST gcsvc;
uint8_t pattern=0;

void apc_APCSend(uint8_t dest_addr, uint8_t len, uint8_t CmdRespCode, uint32_t * Packet);


void apc_CheckTxState(void)
{
	uint8_t TSTAT;
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

#if 0
	if(TSTAT != apcif.TSTAT)
	{
		apcif.TSTAT = TSTAT;
dp("TSTAT %02X\r\n",TSTAT);
		if(TSTAT&0x80) // TX END
		{
			FPGA_WR_RS422_TX_FLAG_CLEAR;
		}
	}
#endif	
		if(TSTAT&0x80) // TX END
		{
dp("TSTAT %02X\r\n",TSTAT);	
			FPGA_WR_RS422_TX_FLAG_CLEAR;
apcif.TSTAT = 1;//TSTAT;			
		}
		else {
apcif.TSTAT = 0;//TSTAT;
		}
//TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);		
dp("TSTAT %02X\r\n",TSTAT);	
}

void apc_CheckRxState(void)
{
	uint8_t RSTAT;
	uint32_t rxinfo;
	uint8_t rxid;
	uint8_t slvid;
	uint8_t rxlen;
	uint8_t rxcmd;

	RSTAT = FPGA_READ_WORD(RS422_RX_STATE_ADDR);

	if(RSTAT != apcif.RSTAT)
	{
		apcif.RSTAT = RSTAT;
		//dprintf("RSTAT %02X\r\n",RSTAT);
	}

	if(RSTAT&0x80)
	{
//		FPGA_WRITE_WORD(APC_RS422_RX_FLAG_RESET_ADDR,1); // flag clear
		FPGA_WR_RS422_RX_FLAG_CLEAR;		// flag clear
		rxinfo = FPGA_READ_WORD(RS422_RX_INFO_ADDR);
		rxid  = rxinfo; rxinfo = rxinfo>>8;
		rxlen = rxinfo; rxinfo = rxinfo>>8;
		rxcmd = rxinfo;
		slvid = rxid&0xF;

// master
dp("[RX]ID:%02X,LEN:%02X,CMD:%02X,RSTAT:%02X,D[0]:%08lX\r\n",rxid,rxlen,rxcmd,RSTAT, FPGA_READ_WORD(0x2401 + (APC_CONFIG_STASUS_SIZE*(slvid-1))));
		ShowMemoryWord((pRxMem + (APC_CONFIG_STASUS_SIZE*(slvid-1))), rxlen);

	}
}

// For Master
// Result[24:16]: CRC Error bit
// Result[8:0]:   PASS bit
inline void apc_CheckAutoResult(void)
{
	uint32_t Result;
	Result = FPGA_READ_WORD(RS422_RX_AUTO_RESULT_ADDR);
//	FPGA_WRITE_WORD(APC_RS422_RX_FLAG_RESET_ADDR,1); // flag clear
	FPGA_WR_RS422_RX_FLAG_CLEAR;
dp("[ Auto Result: %8lX ]\r\n",Result);
}

void apc_APCSend(uint8_t dest_addr, uint8_t len, uint8_t CmdRespCode, uint32_t * Packet)
{
	uint8_t i;
	pTxMem[0] = CmdRespCode;
pTxMem[1] = 0x10;
	
//	for(i=0;i<len;i++)
	for(i=1;i<len;i++)
		pTxMem[1+i] =  Packet[i];
	FPGA_WR_RS422_TX_LENGTH(len+1); 	// tx_len
	FPGA_WR_RS422_SEND_TRIG(dest_addr);// tx_start
}









inline void apc_APCAutoModeSetting(void)
{
	FPGA_WR_RS422_MODE_AUTO;
	apcif.RunMode = APC_RUN_AUTOMODE;
}

inline void apc_APCCPUModeSetting(void)
{
	FPGA_WR_RS422_MODE_CPU;
	apcif.RunMode = APC_RUN_CPUMODE;	
}



void rs422_APC_CMD_Connect_fn(uint32_t slvid_addr){}

int rs422_APC_Connect_Send(uint32_t xxx)
{
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;	

		pTxMem[0] = AUTO_MODE_CMD;		
		pTxMem[1] = rs422_APC_CMD_CONNECT_CHECK;		

		FPGA_WR_RS422_TX_LENGTH(2); 	// tx_len

		FPGA_WR_RS422_MODE_AUTO;
		apcif.RunMode = APC_RUN_AUTOMODE;
		apcif.TSTAT = 1;//TSTAT;			
	}
	else {
		// 보내지 못했ㅇ니 main에서 다시 보낸다.
		apcif.TSTAT = 0;//TSTAT;
	}		
	
	return apcif.TSTAT;
}

inline int rs422_APC_Connect_Send_busyCheck()
{
	if(apcif.busyTxHead == apcif.busyTxTail) {
		if(rs422_APC_Connect_Send(0)) {
			return RE_OK;
		}
		else {	// Tx ok
			apcif.busyFn[apcif.busyTxHead & 0x07] = 0;//FNO_rs422_APC_Connect_Send;
			apcif.busyTxHead++;
		}
	}
	else {
		apcif.busyFn[apcif.busyTxHead & 0x07] = 0;//FNO_rs422_APC_Connect_Send;
			apcif.busyTxHead++;		
	}
	return RE_ERR;
}

inline void rs422_APC_CMD_Start_fn(uint32_t slvid_addr) {}

inline int rs422_APC_Start_Send(uint32_t xxx)
{
	uint8_t TSTAT;

	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;
	
		pTxMem[0] = AUTO_MODE_CMD;		
		pTxMem[1] = rs422_APC_CMD_START;		

		FPGA_WR_RS422_TX_LENGTH(2); 	// tx_len

		FPGA_WR_RS422_SEND_TRIG(RS422_ID_GLOBAL); // tx_start - RS422_ID_GLOBAL 은 응답없음

		apcif.TSTAT = 1;//TSTAT;			
	}
	else {
		// 보내지 못했ㅇ니 main에서 다시 보낸다.
		apcif.TSTAT = 0;//TSTAT;
//dp("---TSTAT %02X\r\n",TSTAT);			
//		apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_Start_Send;
//			apcif.busyTxHead++;
	}

	return apcif.TSTAT;	
}

int rs422_APC_Start_Send_busyCheck()
{
	if(apcif.busyTxHead == apcif.busyTxTail) {
		if(rs422_APC_Start_Send(0)) { // busy
			return RE_OK;
		}
		else {	// Tx ok
			apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_Start_Send;
			apcif.busyTxHead++;
		}
	}
	else {
		apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_Start_Send;
			apcif.busyTxHead++;
	}

	return RE_ERR;
}

//inline void APC_Stop_Command(uint32_t slvid_addr)
inline void rs422_APC_CMD_Stop_fn(uint32_t slvid_addr) {}

//inline void APC_Stop_Command(uint32_t slvid_addr)
int rs422_APC_Stop_Send(uint32_t xxx)
{
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;
	
		pTxMem[0] = AUTO_MODE_CMD;		
		pTxMem[1] = rs422_APC_CMD_STOP;		

		FPGA_WR_RS422_TX_LENGTH(2); 	// tx_len

		FPGA_WR_RS422_SEND_TRIG(RS422_ID_GLOBAL); // tx_start - RS422_ID_GLOBAL 은 응답없음
		apcif.TSTAT = 1;//TSTAT;			
	}
	else {
		// 보내지 못했ㅇ니 main에서 다시 보낸다.
		apcif.TSTAT = 0;//TSTAT;
	}

	return apcif.TSTAT;	
}

int rs422_APC_Stop_Send_busyCheck()
{
	if(apcif.busyTxHead == apcif.busyTxTail) {
		if(rs422_APC_Stop_Send(0)) { // busy
			return RE_OK;
		}
		else {	// Tx ok
			apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_Stop_Send;
			apcif.busyTxHead++;
		}
	}
	else {
		apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_Stop_Send;
		apcif.busyTxHead++;
	}
	return RE_ERR;	
}

inline void rs422_APC_REQUEST_Send(uint32_t slvid)
{
#if RYU_TCP_TEST_MAIN		// RYU_TCP			



// APC에서는 FPGA에 주기적으로 쓴다.
// 상태값을 요청하고 , FPGA에서 자동으로 읽어 들인다.	
// GLOBAL ID로 설정하지 말라
// 
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;
				
		pTxMem[0] = STATE_REQUEST_CMD;
		pTxMem[1] = rs422_APC_CMD_REQUEST;

		FPGA_WR_RS422_TX_LENGTH(2); 	// tx_len

		// APC_RUN_AUTOMODE 는 너무 많이 기다려서 하나씩 요청한다.
apc_APCCPUModeSetting();	// 없어도 Default CPU Mode
		FPGA_WR_RS422_SEND_TRIG(slvid); // tx_start - RS422_ID_GLOBAL 은 응답없음
	}
	else {
		// 현재 보낼 수 없는 상태일 경우는 무시하고 요청하지 않음.
		// 다른 명영이 실행 되고 있는 상태이므로 한 번 빠져도 된다....
	}

	
#endif	
}

inline void rs422_APC_REQUEST_Send_Auto()
{
// APC에서는 FPGA에 주기적으로 쓴다.
// 상태값을 요청하고 , FPGA에서 자동으로 읽어 들인다.	
// GLOBAL ID로 설정하지 말라
// 

	pTxMem[0] = STATE_REQUEST_CMD;
	pTxMem[1] = rs422_APC_CMD_REQUEST;

	FPGA_WR_RS422_TX_LENGTH(2); 	// tx_len

// APC_RUN_AUTOMODE 는 너무 많이 기다려서 하나씩 요청한다.
	FPGA_WR_RS422_MODE_AUTO;
	apcif.RunMode = APC_RUN_AUTOMODE;
}
void rs422_APC_REQUEST_Send_blank() {}

void rs422_APC_INLET_CONFIG_Send() {}
void rs422_APC_INLET_CONFIG_Send_blank() {}



// LCD나 GC제어기에서 들어온 데이터를 APC에 적용하도록 데이터 대입  
inline void RS422_Inlet_Setting_fn(uint32_t portNo)
{
#if APC_PACKET_REDUCE	
//	RS422_Inlet_Setting[portNo].iInletType = czSysConfig.btInlet[portNo];
	RS422_Inlet_Setting[portNo].iCarriergas = czInlet[portNo].btCarriergas;
	RS422_Inlet_Setting[portNo].iApcMode = czInlet[portNo].btApcMode;
	RS422_Inlet_Setting[portNo].iConnection = czInlet[portNo].btConnection;
	RS422_Inlet_Setting[portNo].iGasSaverMode = czInlet[portNo].bGasSaverMode;
	RS422_Inlet_Setting[portNo].iPressCorrectOnoff = czInlet[portNo].bPressCorrect;
	RS422_Inlet_Setting[portNo].iVacuumCorrectOnoff = czInlet[portNo].bVacuumCorrect;
	RS422_Inlet_Setting[portNo].fPressCorrect = czInlet[portNo].fPressCorrect;
	RS422_Inlet_Setting[portNo].fLength = czInlet[portNo].fLength;
	RS422_Inlet_Setting[portNo].fDiameter = czInlet[portNo].fDiameter;
	RS422_Inlet_Setting[portNo].fThickness = czInlet[portNo].fThickness;
	RS422_Inlet_Setting[portNo].fGasSaverTime = czInlet[portNo].fGasSaverTime;
	RS422_Inlet_Setting[portNo].fGasSaverFlow = czInlet[portNo].fGasSaverFlow;
#endif	

// settings	
	RS422_Inlet_Setting[portNo].iInjectionMode = czInlet[portNo].btInjMode;
	RS422_Inlet_Setting[portNo].iPressureOnoff = czInlet[portNo].fPressureOnoff;
	RS422_Inlet_Setting[portNo].iColumnFlowOnoff = czInlet[portNo].fColumnFlowOnoff;
	RS422_Inlet_Setting[portNo].iSplitratio = (int)(czInlet[portNo].iSplitratio);
	
	RS422_Inlet_Setting[portNo].fPulsedPress = czInlet[portNo].fPulsed_FlowPressSet;
//	RS422_Inlet_Setting[portNo].fPulsedPress = (int)(czInlet[portNo].btCarriergas);	
	RS422_Inlet_Setting[portNo].fPulsedTime = czInlet[portNo].fPulsed_Time;
	RS422_Inlet_Setting[portNo].fColumnFlowSet = czInlet[portNo].fColumnFlowSet;
	RS422_Inlet_Setting[portNo].fPressureSet = czInlet[portNo].fPressureSet;
	RS422_Inlet_Setting[portNo].fSplitFlowSet = czInlet[portNo].fSplitFlowSet;
	RS422_Inlet_Setting[portNo].fSplitOnTime = czInlet[portNo].fSplitOnTime;
	RS422_Inlet_Setting[portNo].fTotalFlowSet = czInlet[portNo].fTotalFlowSet;

//	RS422_Inlet_Setting[portNo].iSplitratio = (int)(czInlet[portNo].iSplitratio);	

memset(&RS422_Inlet_Setting_XXX[portNo], 0, sizeof(RS422_Inlet_Setting_t));

//dprintf("iSplitRatio = %d iSplitRatio = %d\n",RS422_Inlet_Setting[portNo].iSplitratio, RS422_Inlet_Setting[portNo].fPulsedPress);		
}

inline int rs422_APC_INLET_SETTING_Send(uint32_t portNo)
{
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;

RS422_Inlet_Setting_fn(portNo);

		pTxMem[0] = APC_CMD_INLET_SET;
		pTxMem[1] = rs422_APC_CMD_INLET_SETTING_1 + portNo;

//		memcpy((uint8_t *)(pTxMem+2),&RS422_Inlet_Setting_XXX[portNo],sizeof(RS422_Inlet_Setting_t));
		memcpy((uint8_t *)(pTxMem+2),&RS422_Inlet_Setting[portNo],sizeof(RS422_Inlet_Setting_t));

		FPGA_WR_RS422_TX_LENGTH((sizeof(RS422_Inlet_Setting_t) >> 2) + 2); 	// tx_len : uint8_t --> uint32_t , 

apc_APCCPUModeSetting();	// 없어도 Default CPU Mode
		FPGA_WR_RS422_SEND_TRIG(portNo+1); // ID
		apcif.TSTAT = 1;//TSTAT;			
	}
	else {
		apcif.TSTAT = 0;//// 보내지 못했ㅇ니 main에서 다시 보낸다.
	}

	return apcif.TSTAT;		
}

inline int rs422_APC_INLET_SETTING_1_Send(uint32_t xxx)
{
	rs422_APC_INLET_SETTING_Send(0);
}

inline int rs422_APC_INLET_SETTING_2_Send(uint32_t xxx)
{
	rs422_APC_INLET_SETTING_Send(1);
}

inline int rs422_APC_INLET_SETTING_3_Send(uint32_t xxx)
{
	rs422_APC_INLET_SETTING_Send(2);
}

int rs422_APC_INLET_SETTING_Send_busyCheck(uint32_t portNo)
{
	if(apcif.busyTxHead == apcif.busyTxTail) {
		if(rs422_APC_INLET_SETTING_Send(portNo)) { // busy
			return RE_OK;
		}
		else {	// Tx ok
			apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_INLET_SETTING_Send + portNo;
			apcif.busyTxHead++;
		}
	}
	else {
		apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_INLET_SETTING_Send + portNo;
		apcif.busyTxHead++;
	}
	return RE_ERR;	
}
void rs422_APC_INLET_SETTING_Send_blank() {}


inline void RS422_INLET_FLOW_PRGM_fn(uint32_t portNo)
{
	int i;
	
	for(i=0 ; i<6 ; i++) {
		RS422_APC_FLOW_PRGM[portNo].fRate[i] = czInlet[portNo].flowPrgm[i].fRate;
		RS422_APC_FLOW_PRGM[portNo].fFinalFlow[i] = czInlet[portNo].flowPrgm[i].fFinalFlow;
		RS422_APC_FLOW_PRGM[portNo].fFinalTime[i] = czInlet[portNo].flowPrgm[i].fFinalTime;
	}
dp("=== RS422_INLET_FLOW_PRGM_fn portNo = %d\n",portNo);		
}
inline int rs422_APC_INLET_FLOW_PRGM_Send(uint32_t portNo)
{
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;

RS422_INLET_FLOW_PRGM_fn(portNo);

		pTxMem[0] = APC_CMD_INLET_SET;
		pTxMem[1] = rs422_APC_CMD_INLET_FLOW_PRGM_1 + portNo; 

		memcpy((uint8_t *)(pTxMem+2),&RS422_APC_FLOW_PRGM[portNo],sizeof(RS422_APC_FLOW_PRGM_t));
		FPGA_WR_RS422_TX_LENGTH((sizeof(RS422_APC_FLOW_PRGM_t) >> 2) + 2); 	// tx_len : uint8_t --> uint32_t , 

apc_APCCPUModeSetting();	// 없어도 Default CPU Mode
		FPGA_WR_RS422_SEND_TRIG(portNo+1); // tx_start - RS422_ID_GLOBAL 은 응답없음
		apcif.TSTAT = 1;//TSTAT;			
dp("=== rs422_APC_INLET_FLOW_PRGM_Send portNo = %d\n",portNo);				
	}
	else {
		apcif.TSTAT = 0;//// 보내지 못했ㅇ니 main에서 다시 보낸다.
	}

	return apcif.TSTAT;			
}

inline int rs422_APC_INLET_FLOW_PRGM_1_Send(uint32_t xxx)
{
	rs422_APC_INLET_FLOW_PRGM_Send(0);
}
inline int rs422_APC_INLET_FLOW_PRGM_2_Send(uint32_t xxx)
{
	rs422_APC_INLET_FLOW_PRGM_Send(1);
}
inline int rs422_APC_INLET_FLOW_PRGM_3_Send(uint32_t xxx)
{
	rs422_APC_INLET_FLOW_PRGM_Send(2);
}

int rs422_APC_INLET_FLOW_PRGM_Send_busyCheck(uint32_t portNo)
{
	if(apcif.busyTxHead == apcif.busyTxTail) {
		if(rs422_APC_INLET_FLOW_PRGM_Send(portNo)) { 
dp("OK = rs422_APC_INLET_FLOW_PRGM_Send_busyCheck = %d\n",apcif.busyTxHead);		
			return RE_OK;
		}
		else {
			apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_INLET_FLOW_PRGM_Send + portNo;
			apcif.busyTxHead++;	
dp("OK xx = apcif.busyTxHead = %d\n",apcif.busyTxHead);
		}
	}
	else {
		apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_INLET_FLOW_PRGM_Send + portNo;
		apcif.busyTxHead++;	
dp("xxx apcif.busyTxHead = %d\n",apcif.busyTxHead);		
	}
	return RE_ERR;	
}
void rs422_APC_INLET_FLOW_PRGM_Send_blank() {}

inline void RS422_INLET_PRESS_PRGM_fn(uint32_t portNo)
{
	int i;
	
	for(i=0 ; i<6 ; i++) {
		RS422_APC_PRESS_PRGM[portNo].fRate[i] = czInlet[portNo].presPrgm[i].fRate;
		RS422_APC_PRESS_PRGM[portNo].fFinalPress[i] = czInlet[portNo].presPrgm[i].fFinalPress;
		RS422_APC_PRESS_PRGM[portNo].fFinalTime[i] = czInlet[portNo].presPrgm[i].fFinalTime;
	}

}

inline int rs422_APC_INLET_PRESS_PRGM_Send(uint32_t portNo)
{
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;

RS422_INLET_PRESS_PRGM_fn(portNo);

		pTxMem[0] = APC_CMD_INLET_SET;
		pTxMem[1] = rs422_APC_CMD_INLET_PRESS_PRGM_1 + portNo; 

		memcpy((uint8_t *)(pTxMem+2),&RS422_APC_PRESS_PRGM[portNo],sizeof(RS422_APC_PRESS_PRGM_t));
		FPGA_WR_RS422_TX_LENGTH((sizeof(RS422_APC_PRESS_PRGM_t) >> 2) + 2); 	// tx_len : uint8_t --> uint32_t , 

apc_APCCPUModeSetting();	// 없어도 Default CPU Mode
		FPGA_WR_RS422_SEND_TRIG(portNo+1); // tx_start - RS422_ID_GLOBAL 은 응답없음
		apcif.TSTAT = 1;//TSTAT;			
	}
	else {
		apcif.TSTAT = 0;//// 보내지 못했ㅇ니 main에서 다시 보낸다.
	}

	return apcif.TSTAT;		
}


//inline 
int rs422_APC_INLET_PRESS_PRGM_1_Send(uint32_t xxx)
{
//	rs422_APC_INLET_FLOW_PRGM_Send(0);
	rs422_APC_INLET_PRESS_PRGM_Send(0);
}
//inline 
int rs422_APC_INLET_PRESS_PRGM_2_Send(uint32_t xxx)
{
//	rs422_APC_INLET_FLOW_PRGM_Send(1);
	rs422_APC_INLET_PRESS_PRGM_Send(1);
}
//inline 
int rs422_APC_INLET_PRESS_PRGM_3_Send(uint32_t xxx)
{
//	rs422_APC_INLET_FLOW_PRGM_Send(2);
	rs422_APC_INLET_PRESS_PRGM_Send(2);
}

int rs422_APC_INLET_PRESS_PRGM_Send_busyCheck(uint32_t portNo)
{
	if(apcif.busyTxHead == apcif.busyTxTail) {
		if(rs422_APC_INLET_PRESS_PRGM_Send(portNo)) { // busy
			return RE_OK;
		}
		else {	// Tx ok
			apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_INLET_PRESS_PRGM_Send + portNo;
			apcif.busyTxHead++;
		}
	}
	else {
		apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_INLET_PRESS_PRGM_Send +portNo;
		apcif.busyTxHead++;
	}
	return RE_ERR;	
}

void rs422_APC_INLET_PRESS_PRGM_Send_blank(){}

void rs422_APC_DET_CONFIG_1_Send(){}
void rs422_APC_DET_CONFIG_1_Send_blank(){}
void rs422_APC_DET_CONFIG_2_Send(){}
void rs422_APC_DET_CONFIG_2_Send_blank(){}
void rs422_APC_DET_CONFIG_3_Send(){}
void rs422_APC_DET_CONFIG_3_Send_blank(){}

inline void RS422_Det_Setting_fn(uint32_t portNo)
{
#if APC_PACKET_REDUCE
//	RS422_Det_Setting[portNo].iDetType = czSysConfig.btDet[portNo];
		RS422_Det_Setting[portNo].iMakeupgas = czDet[portNo].btMakeupgas;
		RS422_Det_Setting[portNo].iConnection = czDet[portNo].btConnection;

		RS422_Det_Setting[portNo].fLitoffset = czDet[portNo].fLitoffset;
		RS422_Det_Setting[portNo].fIgnitedelay = czDet[portNo].fIgnitedelay;
		RS422_Det_Setting[portNo].fIgniteflow = czDet[portNo].fIgniteflow;
		RS422_Det_Setting[portNo].fIgnitetemp = czDet[portNo].fIgnitetemp;
#endif

	// settings	
		RS422_Det_Setting[portNo].iAutoIgnition = czDet[portNo].bAutoIgnition;

		RS422_Det_Setting[portNo].iFlowOnoff1 = czDet[portNo].fFlowOnoff1;
		RS422_Det_Setting[portNo].iFlowOnoff2 = czDet[portNo].fFlowOnoff2;
		RS422_Det_Setting[portNo].iFlowOnoff3 = czDet[portNo].fFlowOnoff3;
	
		RS422_Det_Setting[portNo].fFlowSet1 = czDet[portNo].fFlowSet1;
		RS422_Det_Setting[portNo].fFlowSet2 = czDet[portNo].fFlowSet2;
		RS422_Det_Setting[portNo].fFlowSet3 = czDet[portNo].fFlowSet3;
}

inline int rs422_APC_DET_SETTING_Send(uint32_t portNo)
{
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;

//RS422_Det_Setting_fn(portNo); // LCD_if , PC_ 실행 에서 설정

		pTxMem[0] = APC_CMD_DET_SET;
		pTxMem[1] = rs422_APC_CMD_DET_SETTING_1 + portNo;

		memcpy((uint8_t *)(pTxMem+2),&RS422_Det_Setting[portNo],sizeof(RS422_Det_Setting_t));
		FPGA_WR_RS422_TX_LENGTH((sizeof(RS422_Det_Setting_t) >> 2) + 2); 	// tx_len : uint8_t --> uint32_t , 

apc_APCCPUModeSetting();	// 없어도 Default CPU Mode
		FPGA_WR_RS422_SEND_TRIG(RS422_ID_DET); // tx_start - RS422_ID_GLOBAL 은 응답없음
		apcif.TSTAT = 1;//TSTAT;			
	}
	else {
		apcif.TSTAT = 0;//// 보내지 못했ㅇ니 main에서 다시 보낸다.
	}

	return apcif.TSTAT;		
}

//inline int rs422_APC_DET_SETTING_1_Send(uint32_t portNo)
inline int rs422_APC_DET_SETTING_1_Send(uint32_t xxx)
{
	rs422_APC_DET_SETTING_Send(0);
}
inline int rs422_APC_DET_SETTING_2_Send(uint32_t xxx)
{
	rs422_APC_DET_SETTING_Send(1);
}
inline int rs422_APC_DET_SETTING_3_Send(uint32_t xxx)
{
	rs422_APC_DET_SETTING_Send(2);
}

inline int rs422_APC_DET_SETTING_Send_busyCheck(uint32_t portNo)
{
dp("apcif.busyTxHead =%d = apcif.busyTxTail= %d\n",apcif.busyTxHead,apcif.busyTxTail);
	if(apcif.busyTxHead == apcif.busyTxTail) {	// 
		if(rs422_APC_DET_SETTING_Send(portNo)) {
			return RE_OK;
		}
		else {	// Tx ok
			apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_DET_SETTING_1_Send + portNo;
			apcif.busyTxHead++;
		}
	}
	else {
		apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_DET_SETTING_1_Send + portNo;
		apcif.busyTxHead++;
	}
	return RE_ERR;	
}

void rs422_APC_DET_SETTING_1_Send_blank(){}

inline void RS422_Aux_Setting_fn(uint32_t portNo)
{
#if APC_PACKET_REDUCE
//	RS422_Aux_Setting[portNo].iDetType = czSysConfig.bAuxAPC[portNo];
#endif
	// settings	

		RS422_Aux_Setting[portNo].iAuxGas = czAuxApc[portNo].lcdAuxApc.btAuxGas;
	
		RS422_Aux_Setting[portNo].iFlowOnoff1 = czAuxApc[portNo].lcdAuxApc.fFlowOnoff1;
		RS422_Aux_Setting[portNo].iFlowOnoff2 = czAuxApc[portNo].lcdAuxApc.fFlowOnoff2;
		RS422_Aux_Setting[portNo].iFlowOnoff3 = czAuxApc[portNo].lcdAuxApc.fFlowOnoff3;
		
		RS422_Aux_Setting[portNo].fFlowSet1 = czAuxApc[portNo].lcdAuxApc.fFlowSet1;
		RS422_Aux_Setting[portNo].fFlowSet2 = czAuxApc[portNo].lcdAuxApc.fFlowSet2;
		RS422_Aux_Setting[portNo].fFlowSet3 = czAuxApc[portNo].lcdAuxApc.fFlowSet3;
}

inline int rs422_APC_AUX_SETTING_Send(uint32_t portNo)
{
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;

RS422_Aux_Setting_fn(portNo);

		pTxMem[0] = APC_CMD_AUX_SET;
		pTxMem[1] = rs422_APC_CMD_AUX_SETTING_1 + portNo;

		memcpy((uint8_t *)(pTxMem+2),&RS422_Aux_Setting[portNo],sizeof(RS422_Aux_Setting_t));
		FPGA_WR_RS422_TX_LENGTH((sizeof(RS422_Aux_Setting_t) >> 2) + 2); 	// tx_len : uint8_t --> uint32_t , 

apc_APCCPUModeSetting();	// 없어도 Default CPU Mode
		FPGA_WR_RS422_SEND_TRIG(RS422_ID_AUX); // tx_start - RS422_ID_GLOBAL 은 응답없음
		apcif.TSTAT = 1;//TSTAT;			
	}
	else {
		apcif.TSTAT = 0;//// 보내지 못했ㅇ니 main에서 다시 보낸다.
	}

	return apcif.TSTAT;	
}

inline int rs422_APC_AUX_SETTING_1_Send(uint32_t xxx)
{
	rs422_APC_AUX_SETTING_Send(0);
}
inline int rs422_APC_AUX_SETTING_2_Send(uint32_t xxx)
{
	rs422_APC_AUX_SETTING_Send(1);
}
inline int rs422_APC_AUX_SETTING_3_Send(uint32_t xxx)
{
	rs422_APC_AUX_SETTING_Send(2);
}

inline int rs422_APC_AUX_SETTING_Send_busyCheck(uint32_t portNo)
{
	if(apcif.busyTxHead == apcif.busyTxTail) {
		if(rs422_APC_AUX_SETTING_1_Send(portNo)) { // busy
			return RE_OK;
		}
		else {	// Tx ok
			apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_AUX_SETTING_1_Send + portNo;
			apcif.busyTxHead++;
		}
	}
	else {
		apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_AUX_SETTING_1_Send + portNo;
		apcif.busyTxHead++;
	}
	return RE_ERR;
}
void rs422_APC_AUX_SETTING_Send_blank(){}

inline void RS422_Calib_Setting_fn(uint32_t iPortNO, uint32_t iCalibType, uint32_t iStartStop, uint32_t iFlowCalNo)
//inline void RS422_Calib_Setting_fn(uint32_t portNo)
{
/*	
typedef struct _RS422_Calibration_Set_t
{
	unsigned char iPortNo;		// Det1~3, Aux1~3
	unsigned char iCalibType;		// SensorZero, ValveCal, FlowCal
	unsigned char iStartStop;		// Start/Stop/  stop ~ Reset
	unsigned char iReset;
	unsigned char iApply;
	
	unsigned char iFlowCalNo;	// Sen 1~3 // Calib 적용할 센서.
	
	float fFlowSet[3];	// FlowCalib Set 3
	float fMeasure[3];	// FlowCalib Measure 3
} RS422_Calibration_Set_t;
*/

#if APC_PACKET_REDUCE
//	RS422_Det_Setting[portNo].iDetType = czSysConfig.btDet[portNo];
#endif
	// portNo = 0:Inlet1 , 1:Inlet2 , 2:Inlet3 , 4:Det , 5:AuxApc
	// iPortNo
	// settings		
		RS422_Calibration_Set.iPortNo =  iPortNO;
		RS422_Calibration_Set.iCalibType =  iCalibType;
		
		RS422_Calibration_Set.iStartStop =  iStartStop;
//		RS422_Calibration_Set.iReset =  czDet[portNo].btMakeupgas;
//		RS422_Calibration_Set.iApply =  czDet[portNo].btMakeupgas;

		RS422_Calibration_Set.iFlowCalNo =  iFlowCalNo;
		
		RS422_Calibration_Set.fFlowSet[0];// =  czDet[portNo].btMakeupgas;
		RS422_Calibration_Set.fFlowSet[1];// =  czDet[portNo].btMakeupgas;
		RS422_Calibration_Set.fFlowSet[2];// =  czDet[portNo].btMakeupgas;
		RS422_Calibration_Set.fMeasure[0];// =  czDet[portNo].btMakeupgas;
		RS422_Calibration_Set.fMeasure[1];// =  czDet[portNo].btMakeupgas;
		RS422_Calibration_Set.fMeasure[2];// =  czDet[portNo].btMakeupgas;



}

inline int rs422_APC_CALIB_SETTING_Send(uint32_t portNo)
{
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;

//RS422_Calib_Setting_fn(...............);

		pTxMem[0] = APC_CMD_CALIB_SET;
		pTxMem[1] = rs422_APC_CMD_CALIB_SETTING + portNo;

		memcpy((uint8_t *)(pTxMem+2),&RS422_Calibration_Set,sizeof(RS422_Calibration_Set_t));
		FPGA_WR_RS422_TX_LENGTH((sizeof(RS422_Calibration_Set_t) >> 2) + 2); 	// tx_len : uint8_t --> uint32_t , 

apc_APCCPUModeSetting();	// 없어도 Default CPU Mode
		FPGA_WR_RS422_SEND_TRIG(RS422_ID_AUX); // tx_start - RS422_ID_GLOBAL 은 응답없음
		apcif.TSTAT = 1;//TSTAT;			
	}
	else {
		apcif.TSTAT = 0;//// 보내지 못했ㅇ니 main에서 다시 보낸다.
	}

	return apcif.TSTAT;		
}

inline int rs422_APC_CALIB_SETTING_Send_busyCheck(uint32_t portNo)
{
	if(apcif.busyTxHead == apcif.busyTxTail) {
		if(rs422_APC_CALIB_SETTING_Send(portNo)) { // busy
			return RE_OK;
		}
		else {	// Tx ok
			apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_CALIB_SETTING_Send;
			apcif.busyTxHead++;
		}
	}
	else {
		apcif.busyFn[apcif.busyTxHead & 0x07] = FNO_rs422_APC_CALIB_SETTING_Send;
		apcif.busyTxHead++;
	}
	return RE_ERR;	
}
void rs422_APC_CALIB_SETTING_Send_blank(){}

// =============================================================================================
// =============================================================================================
// =============================================================================================
void rs422_APC_Set_Main_State()
{
//printf("ucRequest = %d oven= %f\n",RS422_Main_State.ucRequest,RS422_Main_State.fOvenTemp);
	RS422_Main_State.fOvenTemp 		= czState.ActTemp.fOven;
	RS422_Main_State.fDetTemp[0] 	= czState.ActTemp.fDet[0];	// FID
	RS422_Main_State.fDetTemp[1] 	= czState.ActTemp.fDet[1];	// FID
	RS422_Main_State.fDetTemp[2] 	= czState.ActTemp.fDet[2];	// FID	
	RS422_Main_State.fPout 				= ms5611Cal.PRESS_act;// = 14.696;	// 대기압
	RS422_Main_State.fSignal[0] 	= czState.fSignal[0];// Ignite
	RS422_Main_State.fSignal[1] 	= czState.fSignal[1];	// Ignite
	RS422_Main_State.fSignal[2] 	= czState.fSignal[2];	// Ignite	
	RS422_Main_State.iGCstate 		= czState.btState;		// 삭제??

//	RS422_Main_State.ucRequest    = ~RS422_Main_State.ucRequest;
//	RS422_Main_State.cAutoReadyRun = 0;		// 새로 설정 - readyRun 
}

void rs422_APC_Main_State_Send()
{
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;
// 전송전 셋팅 - 이디에 위치 시킬것인지?
rs422_APC_Set_Main_State();

		pTxMem[0] = AUTO_MODE_CMD;	
		pTxMem[1] = rs422_APC_CMD_MAIN_STATE;

		memcpy((uint8_t *)(pTxMem+2),&RS422_Main_State,sizeof(RS422_Main_State));

		FPGA_WR_RS422_TX_LENGTH((sizeof(RS422_Main_State) >> 2) + 2); 	// tx_len : uint8_t --> uint32_t , 

#if 1
		FPGA_WR_RS422_SEND_TRIG(RS422_ID_GLOBAL); // tx_start
#else
		FPGA_WR_RS422_MODE_AUTO;
		apcif.RunMode = APC_RUN_AUTOMODE;
#endif
	}
	else {
		// 현재 보낼 수 없는 상태일 경우는 무시하고 요청하지 않음.
		// 다른 명영이 실행 되고 있는 상태이므로 한 번 빠져도 된다....
	}
	
}

void rs422_APC_Main_State_Send_blank(void){}

// APC --> SOC에서 수신
void rs422_APC_CMD_INLET_DATA_fn(uint32_t slvid_addr)
{
	RS422_Inlet_Data_t tmp;
	uint8_t portNo;
	static int cnt=0;
	
//	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Inlet_Data));
	memcpy(&tmp,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Inlet_Data_t));

	portNo = slvid_addr >> RS422_CONFIG_STASUS_SIZE_SHIFT;
	
	rs422_APC_CMD_INLET_DATA_check(portNo, &tmp);

//	memcpy(&RS422_Inlet_Data,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Inlet_Data));
//	memcpy(&RS422_Inlet_Data,&tmp,sizeof(RS422_Inlet_Data_t));
/*
dp("rs422_APC_CMD_INLET_DATA_fn ucType[%d] = %d\n",portNo,RS422_Inlet_Data.ucType);
	czSysConfig.btInlet[slvid] = RS422_Inlet_Data.ucType;
//		RS422_Inlet_Config[i].iInletType = czSysConfig.btInlet[i];
*/	
/*
totalCntI++;	
if(++cnt == 100) {
	cnt = 0;
dp("rs422_APC_CMD_INLET_DATA_fn = %d %d\n",tmp.iErrorCode,totalCntI);
}
*/
//dp("2rs422_APC_CMD_INLET_DATA_fn = iSet_splitRatio %d \n",RS422_Inlet_Data.iSet_splitRatio);
/*
dprintf("Error = %d fAct_Press = %f\n fAct_Flow[0] = %f\n fAct_Velocity = %f\n fSet_Flow[2] = %f\n iReady = %d\n",
RS422_Inlet_Data.iErrorCode,
RS422_Inlet_Data.fAct_Press,
RS422_Inlet_Data.fAct_Flow[0],
RS422_Inlet_Data.fAct_Velocity,
RS422_Inlet_Data.fSet_Flow[2],
RS422_Inlet_Data.iReady);	
*/
}

//void rs422_APC_CMD_INLET_DATA_check(uint32_t portNo, char *rcvData)
void rs422_APC_CMD_INLET_DATA_check(uint32_t portNo, RS422_Inlet_Data_t *tmp)
{
// 받은 데이터 검사
	static int splitRatio_old = 0;

//dp("rs422_APC_CMD_INLET_DATA_fn ucType[%d] = %d\n", portNo, tmp->ucType);
	if(tmp->ucType > 0 && tmp->ucType <= ONCOLUMN) {
		czSysConfig.btInlet[portNo] = tmp->ucType;
	}

#if 1
	switch(portNo) {
		case 0:	
			czState.FlowReady.bColFlow1 = tmp->iReady;
			mainHeaterCmd.tempMode.InjType1 = (czSysConfig.btInlet[0] & 0x0003);		
		break;
		case 1:	
			czState.FlowReady.bColFlow2 = tmp->iReady; 
			mainHeaterCmd.tempMode.InjType2 = (czSysConfig.btInlet[1] & 0x0003);			
		break;
		case 2:	
			czState.FlowReady.bColFlow3 = tmp->iReady; 
			mainHeaterCmd.tempMode.InjType3 = (czSysConfig.btInlet[2] & 0x0003);			
		break;
	}

	mainDevStt.apcInletError[portNo] = tmp->iErrorCode;
	
	czState.ActFlow.Disp_InjFlow[portNo][0] = tmp->fAct_Flow[0];	// tot
	czState.ActFlow.Disp_InjFlow[portNo][3] = tmp->fAct_Flow[1];	// spl
	czState.ActFlow.Disp_InjFlow[portNo][2] = tmp->fAct_Flow[2];	// col (cap,pak,on)

	
//czState.ActFlow.Disp_InjFlow[0][3] = tmp->fAct_Flow[0] - tmp->fAct_Flow[1] - tmp->fAct_Flow[2];	// spl

	czState.ActFlow.Disp_Press[portNo] = tmp->fAct_Press;

	czInlet[portNo].fColumnFlowSet =
		czState.ActFlow.Disp_Setflow[portNo] = tmp->fSet_Flow[2];
		
	czInlet[portNo].fPressureSet =
		czState.ActFlow.Disp_Setpress[portNo] = tmp->fSet_Press;

//rintf("czState.ActFlow.Disp_Setflow[portNo]-tot-[0]=%.2lf spl-[3]=%.2lf col-[2]=%.2lf fSet_Press=%.2lf\n",
 

	czState.ActFlow.Disp_Velocity_Inj[portNo] = tmp->fAct_Velocity;						
//			tmp->fSet_Velocity;
/*
	czInlet[portNo].iSplitratio = (short int)(tmp->iSet_splitRatio);

	if(splitRatio_old != tmp->iSet_splitRatio) {
		splitRatio_old = tmp->iSet_splitRatio;

		LCD_Send_Data(sizeof(CHROZEN_INLET_SETTING_t), (YL6700GC_LCD_COMM_INLET_SETTING_1 + portNo) , &czInlet[portNo]);
	}
*/	
#else

	switch(portNo) {
		case 0:
			czState.FlowReady.bColFlow1 = tmp->iReady;
			
			czState.ActFlow.Disp_InjFlow[0][0] = tmp->fAct_Flow[0];	// tot
			czState.ActFlow.Disp_InjFlow[0][3] = tmp->fAct_Flow[1];	// spl
			czState.ActFlow.Disp_InjFlow[0][2] = tmp->fAct_Flow[2];	// col (cap,pak,on)
//czState.ActFlow.Disp_InjFlow[0][3] = tmp->fAct_Flow[0] - tmp->fAct_Flow[1] - tmp->fAct_Flow[2];	// spl

			czState.ActFlow.Disp_Press[0] = tmp->fAct_Press;

			czState.ActFlow.Disp_Setflow[0][0] = 
			czInlet[portNo].fTotalFlowSet = tmp->fSet_Flow[0];
			czState.ActFlow.Disp_Setflow[0][3] = 
			czInlet[portNo].fSplitFlowSet = tmp->fSet_Flow[1];
			czState.ActFlow.Disp_Setflow[0][2] = 
			czInlet[portNo].fColumnFlowSet = tmp->fSet_Flow[2];

			czState.ActFlow.Disp_Setpress[0] = 
			czInlet[portNo].fPressureSet = tmp->fSet_Press;

			czState.ActFlow.Disp_Velocity_Inj[0] = tmp->fAct_Velocity;						
//			tmp->fSet_Velocity;
			czInlet[portNo].iSplitratio = (short int)(tmp->iSet_splitRatio);
			break;
		case 1:
			czState.FlowReady.bColFlow2 = tmp->iReady;			
			
			czState.ActFlow.Disp_InjFlow[1][0] = tmp->fAct_Flow[0];	// tot
			czState.ActFlow.Disp_InjFlow[1][3] = tmp->fAct_Flow[1];	// spl
			czState.ActFlow.Disp_InjFlow[1][2] = tmp->fAct_Flow[2];	// col (cap,pak,on)
//czState.ActFlow.Disp_InjFlow[1][3] = tmp->fAct_Flow[0] - tmp->fAct_Flow[1] - tmp->fAct_Flow[2];	// spl

			czState.ActFlow.Disp_Press[1] = tmp->fAct_Press;		

			czState.ActFlow.Disp_Setflow[1][0] = 
			czInlet[portNo].fTotalFlowSet = tmp->fSet_Flow[0];
			czState.ActFlow.Disp_Setflow[1][3] = 
			czInlet[portNo].fSplitFlowSet = tmp->fSet_Flow[1];
			czState.ActFlow.Disp_Setflow[1][2] = 
			czInlet[portNo].fColumnFlowSet = tmp->fSet_Flow[2];

			czState.ActFlow.Disp_Setpress[1] = 
			czInlet[portNo].fPressureSet = tmp->fSet_Press;

			czState.ActFlow.Disp_Velocity_Inj[1] = tmp->fAct_Velocity;			
//			tmp->fSet_Velocity;

			czInlet[portNo].iSplitratio = (short int)(tmp->iSet_splitRatio);
			break;
		case 2:
			czState.FlowReady.bColFlow3 = tmp->iReady;			
			
			czState.ActFlow.Disp_InjFlow[2][0] = tmp->fAct_Flow[0];	// tot
			czState.ActFlow.Disp_InjFlow[2][3] = tmp->fAct_Flow[1];	// spl
			czState.ActFlow.Disp_InjFlow[2][2] = tmp->fAct_Flow[2];	// col (cap,pak,on)
			
//czState.ActFlow.Disp_InjFlow[2][3] = tmp->fAct_Flow[0] - tmp->fAct_Flow[1] - tmp->fAct_Flow[2];	// spl

			czState.ActFlow.Disp_Press[2] = tmp->fAct_Press;			

			czState.ActFlow.Disp_Setflow[2][0] = 
			czInlet[portNo].fTotalFlowSet = tmp->fSet_Flow[0];
			czState.ActFlow.Disp_Setflow[2][3] = 
			czInlet[portNo].fSplitFlowSet = tmp->fSet_Flow[1];
			czState.ActFlow.Disp_Setflow[2][2] = 
			czInlet[portNo].fColumnFlowSet = tmp->fSet_Flow[2];

			czState.ActFlow.Disp_Setpress[2] = 
			czInlet[portNo].fPressureSet = tmp->fSet_Press;

			czState.ActFlow.Disp_Velocity_Inj[2] = tmp->fAct_Velocity;
//			tmp->fSet_Velocity;

			czInlet[portNo].iSplitratio = (short int)(tmp->iSet_splitRatio);
dprintf("tot = %.3lf pur = %.3lf col = %.3lf\n", tmp->fAct_Flow[0], tmp->fAct_Flow[1], tmp->fAct_Flow[2]);			
dprintf("Settot = %.3lf pur = %.3lf col = %.3lf\n", tmp->fSet_Flow[0], tmp->fSet_Flow[1], tmp->fSet_Flow[2]);			
			break;
	}
#endif
	tmp->iErrorCode;
	tmp->iStartStop;

//		RS422_Inlet_Config[i].iInletType = czSysConfig.btInlet[i];

//	memcpy(&RS422_Inlet_Data,&tmp,sizeof(RS422_Inlet_Data));
}

void rs422_APC_CMD_DET_DATA_fn(uint32_t slvid_addr)
{
	RS422_Det_Data_t tmp;
	static int cnt=0;

	memcpy(&tmp, (uint8_t *)(pRxMem + slvid_addr + 2), sizeof(RS422_Det_Data_t));
	
	rs422_APC_CMD_DET_DATA_check(&tmp);

//	memcpy(&RS422_Det_Data,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Det_Data));
//	memcpy(&RS422_Det_Data, &tmp, sizeof(RS422_Det_Data_t));

/*

totalCntD++;
if(++cnt == 100) {
	cnt = 0;
dp("rs422_APC_CMD_DET_DATA_fn = %d %d\n",RS422_Det_Data.iF_ErrorCode,totalCntD);
}


dprintf("Det Error= %d %d %d fAct_Flow[0][0] = %f\n fAct_Flow[2][2] = %f\n fSet_Flow[1][1] = %f\n fSet_Flow[2][1] = %f\n iReady[0] = %d\n",
RS422_Det_Data.iF_ErrorCode,
RS422_Det_Data.iC_ErrorCode,
RS422_Det_Data.iR_ErrorCode,
RS422_Det_Data.fDet_ActFlow[0][0],
RS422_Det_Data.fDet_ActFlow[1][1],
RS422_Det_Data.fDet_ActFlow[2][2],
RS422_Det_Data.iReady[0]);		
*/
}

void rs422_APC_CMD_DET_DATA_check(/*uint32_t portNo,*/ RS422_Det_Data_t *tmp)
{
//	dp("rs422_APC_CMD_DET_DATA_check ucType = %d\n", tmp->ucType[0], tmp->ucType[1], tmp->ucType[2]);
/*	
	if(tmp->ucType > 0 && tmp->ucType <= ONCOLUMN) {
		czSysConfig.btDet[portNo] = tmp->ucType;
	}
*/
// 하위 3bit // sen1~3 On/Off
	czState.FlowOnoff.nFlowReady = (tmp->iOnOff[0] & 0x07) << 3 | (tmp->iOnOff[1] & 0x07) << 6 | (tmp->iOnOff[2] & 0x07) << 9;
/*	
	czState.FlowOnoff.bDetFlow11 = tmp->iOnOff[0] & 0x01;
	czState.FlowOnoff.bDetFlow12 = tmp->iOnOff[0] & 0x02;
	czState.FlowOnoff.bDetFlow13 = tmp->iOnOff[0] & 0x04;
//	tmp->iOnOff[0];
	czState.FlowOnoff.bDetFlow21 = tmp->iOnOff[1] & 0x01;
	czState.FlowOnoff.bDetFlow22 = tmp->iOnOff[1] & 0x02;
	czState.FlowOnoff.bDetFlow23 = tmp->iOnOff[1] & 0x04;
//	tmp->iOnOff[1];
	czState.FlowOnoff.bDetFlow11 = tmp->iOnOff[2] & 0x01;
	czState.FlowOnoff.bDetFlow12 = tmp->iOnOff[2] & 0x02;
	czState.FlowOnoff.bDetFlow13 = tmp->iOnOff[2] & 0x04;
//	tmp->iOnOff[2];
*/	
	mainDevStt.apcDetError[0] = tmp->iF_ErrorCode;

	czState.FlowReady.nFlowReady = (tmp->iReady[0] & 0x07) << 3 | (tmp->iReady[1] & 0x07) << 6 | (tmp->iReady[2] & 0x07) << 9;
/*
	tmp->iReady[0];
	tmp->iReady[1];
	tmp->iReady[2];

	czState.FlowReady.bColFlow1 = tmp->iReady;
*/	
	mainDevStt.apcDetError[1] = tmp->iC_ErrorCode;
	
	tmp->iStartStop;
	mainDevStt.apcDetError[2] = tmp->iR_ErrorCode;

	memcpy(&czState.ActFlow.Disp_DetFlow, &tmp->fDet_ActFlow, sizeof(czState.ActFlow.Disp_DetFlow));
//	czState.ActFlow.Disp_DetFlow[0][0] = tmp->fDet_ActFlow[0][0];
	
}

void rs422_APC_CMD_AUX_DATA_fn(uint32_t slvid_addr)
{
	RS422_Aux_Data_t tmp;

//	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Aux_Data_t));
	memcpy(&tmp, (uint8_t *)(pRxMem + slvid_addr + 2), sizeof(RS422_Aux_Data_t));
	
	rs422_APC_CMD_AUX_DATA_check(&tmp);

//	memcpy(&RS422_Aux_Data,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Aux_Data));
//	memcpy(&RS422_Aux_Data, &tmp, sizeof(RS422_Aux_Data));
}

void rs422_APC_CMD_AUX_DATA_check(RS422_Aux_Data_t *tmp)
{
//	dp("rs422_APC_CMD_AUX_DATA_check ucType = %d %d %d \n", tmp->ucType[0], tmp->ucType[1], tmp->ucType[2]);	
// 하위 3bit // sen1~3 On/Off
	czState.FlowOnoff.nFlowReady = (tmp->iOnOff[0] & 0x07) << 3 | (tmp->iOnOff[1] & 0x07) << 6 | (tmp->iOnOff[2] & 0x07) << 9;

	mainDevStt.apcAuxError[0] = tmp->iF_ErrorCode;

	czState.FlowReady.nFlowReady = (tmp->iReady[0] & 0x07) << 3 | (tmp->iReady[1] & 0x07) << 6 | (tmp->iReady[2] & 0x07) << 9;
			
	mainDevStt.apcAuxError[1] = tmp->iC_ErrorCode;
	
	tmp->iStartStop;
	mainDevStt.apcAuxError[2] = tmp->iR_ErrorCode;
	
	memcpy(&czState.ActFlow.Disp_AuxFlow, &tmp->fAux_ActFlow, sizeof(czState.ActFlow.Disp_AuxFlow));
}

void rs422_APC_CMD_CALIB_DATA_fn(uint32_t slvid_addr)
{
	RS422_Calibration_Data_t tmp;
	uint8_t portNo;

//	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Calibration_Data_t));
	memcpy(&tmp, (uint8_t *)(pRxMem + slvid_addr + 2), sizeof(RS422_Calibration_Data_t));

	portNo = slvid_addr >> RS422_CONFIG_STASUS_SIZE_SHIFT;
	
	rs422_APC_CMD_CALIB_DATA_check(portNo, &tmp);	// portNo (0~4) : inlet1~3(0~2) / Det(3) / Aux(4)

//	memcpy(&RS422_Calibration_Data,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Calibration_Data));
	memcpy(&RS422_Calibration_Data, &tmp, sizeof(RS422_Calibration_Data_t));

}

void rs422_APC_CMD_CALIB_DATA_check(uint32_t portNo, RS422_Calibration_Data_t *tmp)
{
//dp("rs422_APC_CMD_CALIB_DATA_check portNo= %d iCalibNo = %d\n", portNo, tmp->iCalibNo);	

//extern YL6700GC_APC_Calib_Read_t APC_Calib_Read;	
//	unsigned char iCalibNo;		// (0~2) : inlet3 det3 aux3
//tmp->iCalibNo;
//nsigned char iSenZeroState; // stop, ing, pass, fail, error, complete, reset
//	unsigned char iValveCalibState;
//	unsigned char iFlowCalibState;

//	unsigned char iErrorCode[3];	// [3]으로 변경. 
	
//	float fCalib_ValveVolt[3];
//	float fCalib_ActFlow[3];

//	tmp->iSenZeroState

	switch(portNo) {
		case 0:	// inlet 1
			APC_Calib_Read.InjSenZeroState[0] = tmp->iSenZeroState;
			APC_Calib_Read.InjValCalState[0] = tmp->iValveCalibState;			
			tmp->iFlowCalibState;	
			APC_Calib_Read.APC_InjErrorCode[0][0] = tmp->iErrorCode[0];
			APC_Calib_Read.APC_InjErrorCode[0][1] = tmp->iErrorCode[1];
			APC_Calib_Read.APC_InjErrorCode[0][2] = tmp->iErrorCode[2];			

			APC_Calib_Read.Disp_InjValcalib[0][0] = tmp->fCalib_ValveVolt[0];				
			APC_Calib_Read.Disp_InjValcalib[0][1] = tmp->fCalib_ValveVolt[1];				
			APC_Calib_Read.Disp_InjValcalib[0][2] = tmp->fCalib_ValveVolt[2];	

			czState.ActFlow.Disp_InjFlow[0][0] = tmp->fCalib_ActFlow[0];	// tot
			czState.ActFlow.Disp_InjFlow[0][1] = tmp->fCalib_ActFlow[1];	// purge
			czState.ActFlow.Disp_InjFlow[0][2] = tmp->fCalib_ActFlow[2];	// col (cap,pak,on)
			
			break;
		case 1:	// inlet 2
			APC_Calib_Read.InjSenZeroState[1] = tmp->iSenZeroState;		
			APC_Calib_Read.InjValCalState[1] = tmp->iValveCalibState;						
			tmp->iFlowCalibState;					
			APC_Calib_Read.APC_InjErrorCode[1][0] = tmp->iErrorCode[0];
			APC_Calib_Read.APC_InjErrorCode[1][1] = tmp->iErrorCode[1];
			APC_Calib_Read.APC_InjErrorCode[1][2] = tmp->iErrorCode[2];			

			APC_Calib_Read.Disp_InjValcalib[1][0] = tmp->fCalib_ValveVolt[0];				
			APC_Calib_Read.Disp_InjValcalib[1][1] = tmp->fCalib_ValveVolt[1];				
			APC_Calib_Read.Disp_InjValcalib[1][2] = tmp->fCalib_ValveVolt[2];	

			czState.ActFlow.Disp_InjFlow[1][0] = tmp->fCalib_ActFlow[0];	// tot
			czState.ActFlow.Disp_InjFlow[1][1] = tmp->fCalib_ActFlow[1];	// purge
			czState.ActFlow.Disp_InjFlow[1][2] = tmp->fCalib_ActFlow[2];	// col (cap,pak,on)
			
			break;
		case 2:	// inlet 3
			APC_Calib_Read.InjSenZeroState[2] = tmp->iSenZeroState;		
			APC_Calib_Read.InjValCalState[2] = tmp->iValveCalibState;						
			tmp->iFlowCalibState;					
			APC_Calib_Read.APC_InjErrorCode[2][0] = tmp->iErrorCode[0];
			APC_Calib_Read.APC_InjErrorCode[2][1] = tmp->iErrorCode[1];
			APC_Calib_Read.APC_InjErrorCode[2][2] = tmp->iErrorCode[2];			

			APC_Calib_Read.Disp_InjValcalib[2][0] = tmp->fCalib_ValveVolt[0];				
			APC_Calib_Read.Disp_InjValcalib[2][1] = tmp->fCalib_ValveVolt[1];				
			APC_Calib_Read.Disp_InjValcalib[2][2] = tmp->fCalib_ValveVolt[2];	

			czState.ActFlow.Disp_InjFlow[2][0] = tmp->fCalib_ActFlow[0];	// tot
			czState.ActFlow.Disp_InjFlow[2][1] = tmp->fCalib_ActFlow[1];	// purge
			czState.ActFlow.Disp_InjFlow[2][2] = tmp->fCalib_ActFlow[2];	// col (cap,pak,on)
			
			break;
		case 3:	// Det
			switch(tmp->iCalibNo) {
				case 0:	// Det1
					APC_Calib_Read.DetSenZeroState[0] = tmp->iSenZeroState;					
					APC_Calib_Read.DetValCalState[0] = tmp->iValveCalibState;								
					tmp->iFlowCalibState;		
					APC_Calib_Read.APC_DetErrorCode[0][0] = tmp->iErrorCode[0];
					APC_Calib_Read.APC_DetErrorCode[0][1] = tmp->iErrorCode[1];
					APC_Calib_Read.APC_DetErrorCode[0][2] = tmp->iErrorCode[2];			

					APC_Calib_Read.Disp_DetValcalib[0][0] = tmp->fCalib_ValveVolt[0];				
					APC_Calib_Read.Disp_DetValcalib[0][1] = tmp->fCalib_ValveVolt[1];				
					APC_Calib_Read.Disp_DetValcalib[0][2] = tmp->fCalib_ValveVolt[2];	

					czState.ActFlow.Disp_DetFlow[0][0] = tmp->fCalib_ActFlow[0];	// tot
					czState.ActFlow.Disp_DetFlow[0][1] = tmp->fCalib_ActFlow[1];	// purge
					czState.ActFlow.Disp_DetFlow[0][2] = tmp->fCalib_ActFlow[2];	// col (cap,pak,on)
					break;
				case 1: // Det2
					APC_Calib_Read.DetSenZeroState[1] = tmp->iSenZeroState;						
					APC_Calib_Read.DetValCalState[1] = tmp->iValveCalibState;		
					tmp->iFlowCalibState;							
					APC_Calib_Read.APC_DetErrorCode[1][0] = tmp->iErrorCode[0];
					APC_Calib_Read.APC_DetErrorCode[1][1] = tmp->iErrorCode[1];
					APC_Calib_Read.APC_DetErrorCode[1][2] = tmp->iErrorCode[2];			

					APC_Calib_Read.Disp_DetValcalib[1][0] = tmp->fCalib_ValveVolt[0];				
					APC_Calib_Read.Disp_DetValcalib[1][1] = tmp->fCalib_ValveVolt[1];				
					APC_Calib_Read.Disp_DetValcalib[1][2] = tmp->fCalib_ValveVolt[2];	

					czState.ActFlow.Disp_DetFlow[1][0] = tmp->fCalib_ActFlow[0];	// tot
					czState.ActFlow.Disp_DetFlow[1][1] = tmp->fCalib_ActFlow[1];	// purge
					czState.ActFlow.Disp_DetFlow[1][2] = tmp->fCalib_ActFlow[2];	// col (cap,pak,on)
					break;
				case 2:	// Det3
					APC_Calib_Read.DetSenZeroState[2] = tmp->iSenZeroState;										
					APC_Calib_Read.DetValCalState[2] = tmp->iValveCalibState;													
					tmp->iFlowCalibState;			
					APC_Calib_Read.APC_DetErrorCode[2][0] = tmp->iErrorCode[0];
					APC_Calib_Read.APC_DetErrorCode[2][1] = tmp->iErrorCode[1];
					APC_Calib_Read.APC_DetErrorCode[2][2] = tmp->iErrorCode[2];			

					APC_Calib_Read.Disp_DetValcalib[2][0] = tmp->fCalib_ValveVolt[0];				
					APC_Calib_Read.Disp_DetValcalib[2][1] = tmp->fCalib_ValveVolt[1];				
					APC_Calib_Read.Disp_DetValcalib[2][2] = tmp->fCalib_ValveVolt[2];	

					czState.ActFlow.Disp_DetFlow[2][0] = tmp->fCalib_ActFlow[0];	// tot
					czState.ActFlow.Disp_DetFlow[2][1] = tmp->fCalib_ActFlow[1];	// purge
					czState.ActFlow.Disp_DetFlow[2][2] = tmp->fCalib_ActFlow[2];	// col (cap,pak,on)
					break;
			}
			break;
		case 4:	// Aux
			switch(tmp->iCalibNo) {
				case 0:	// Aux1
					APC_Calib_Read.AuxSenZeroState[0] = tmp->iSenZeroState;					
					APC_Calib_Read.AuxValCalState[0] = tmp->iValveCalibState;		
					tmp->iFlowCalibState;	
					APC_Calib_Read.APC_AuxErrorCode[0][0] = tmp->iErrorCode[0];
					APC_Calib_Read.APC_AuxErrorCode[0][1] = tmp->iErrorCode[1];
					APC_Calib_Read.APC_AuxErrorCode[0][2] = tmp->iErrorCode[2];			

					APC_Calib_Read.Disp_AuxValcalib[0][0] = tmp->fCalib_ValveVolt[0];				
					APC_Calib_Read.Disp_AuxValcalib[0][1] = tmp->fCalib_ValveVolt[1];				
					APC_Calib_Read.Disp_AuxValcalib[0][2] = tmp->fCalib_ValveVolt[2];	

					czState.ActFlow.Disp_AuxFlow[0][0] = tmp->fCalib_ActFlow[0];	// tot
					czState.ActFlow.Disp_AuxFlow[0][1] = tmp->fCalib_ActFlow[1];	// purge
					czState.ActFlow.Disp_AuxFlow[0][2] = tmp->fCalib_ActFlow[2];	// col (cap,pak,on)
					break;
				case 1:	// Aux2
					APC_Calib_Read.AuxSenZeroState[1] = tmp->iSenZeroState;														
					APC_Calib_Read.AuxValCalState[1] = tmp->iValveCalibState;																							
					tmp->iFlowCalibState;							
					APC_Calib_Read.APC_AuxErrorCode[1][0] = tmp->iErrorCode[0];
					APC_Calib_Read.APC_AuxErrorCode[1][1] = tmp->iErrorCode[1];
					APC_Calib_Read.APC_AuxErrorCode[1][2] = tmp->iErrorCode[2];			

					APC_Calib_Read.Disp_AuxValcalib[1][0] = tmp->fCalib_ValveVolt[0];				
					APC_Calib_Read.Disp_AuxValcalib[1][1] = tmp->fCalib_ValveVolt[1];				
					APC_Calib_Read.Disp_AuxValcalib[1][2] = tmp->fCalib_ValveVolt[2];	

					czState.ActFlow.Disp_AuxFlow[1][0] = tmp->fCalib_ActFlow[0];	// tot
					czState.ActFlow.Disp_AuxFlow[1][1] = tmp->fCalib_ActFlow[1];	// purge
					czState.ActFlow.Disp_AuxFlow[1][2] = tmp->fCalib_ActFlow[2];	// col (cap,pak,on)
					
					break;
				case 2:	// Aux3
					APC_Calib_Read.AuxSenZeroState[2] = tmp->iSenZeroState;														
					APC_Calib_Read.AuxValCalState[2] = tmp->iValveCalibState;																							
					tmp->iFlowCalibState;							
					APC_Calib_Read.APC_AuxErrorCode[2][0] = tmp->iErrorCode[0];
					APC_Calib_Read.APC_AuxErrorCode[2][1] = tmp->iErrorCode[1];
					APC_Calib_Read.APC_AuxErrorCode[2][2] = tmp->iErrorCode[2];			

					APC_Calib_Read.Disp_AuxValcalib[2][0] = tmp->fCalib_ValveVolt[0];				
					APC_Calib_Read.Disp_AuxValcalib[2][1] = tmp->fCalib_ValveVolt[1];				
					APC_Calib_Read.Disp_AuxValcalib[2][2] = tmp->fCalib_ValveVolt[2];	

					czState.ActFlow.Disp_AuxFlow[2][0] = tmp->fCalib_ActFlow[0];	// tot
					czState.ActFlow.Disp_AuxFlow[2][1] = tmp->fCalib_ActFlow[1];	// purge
					czState.ActFlow.Disp_AuxFlow[2][2] = tmp->fCalib_ActFlow[2];	// col (cap,pak,on)
					
					break;
			}
		
			break;
	}

}

void rs422_APC_CMD_VOLT_DATA_fn(uint32_t slvid_addr)	// 전원 전압 
{
	RS422_APC_Volt_data_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_APC_Volt_data));
	
	rs422_APC_CMD_VOLT_DATA_check();

	memcpy(&RS422_APC_Volt_data,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_APC_Volt_data));
}	

void rs422_APC_CMD_VOLT_DATA_check()
{
}

//================================================================================
// APC
//================================================================================
int APC_Rs422_Parse_blank() {}
void RS422_BlankFunction(uint32_t slvid_addr){}
void rs422_APC_Det_Data_Send_blank(void){}


#if 0		// APC

void rs422_APC_CMD_REQUEST_fn(uint32_t slvid_addr){}
void rs422_APC_CMD_REQUEST_check() {}

void rs422_APC_CMD_INLET_CONFIG_fn(uint32_t slvid_addr)
{
	RS422_Inlet_Config_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Inlet_Config_t));
	
	rs422_APC_CMD_INLET_CONFIG_check();

	memcpy(&RS422_Inlet_Config,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Inlet_Config_t));
}

void rs422_APC_CMD_INLET_CONFIG_check() {}

void rs422_APC_CMD_INLET_SETTING_fn(uint32_t slvid_addr)
{
	RS422_Inlet_Setting_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Inlet_Setting_t));
	
	rs422_APC_CMD_INLET_SETTING_check();

	memcpy(&RS422_Inlet_Setting,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Inlet_Setting_t));
}

void rs422_APC_CMD_INLET_SETTING_check() {}

void rs422_APC_CMD_INLET_FLOW_PRGM_fn(uint32_t slvid_addr)
{
	RS422_APC_FLOW_PRGM_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_APC_FLOW_PRGM_t));
	
	rs422_APC_CMD_INLET_FLOW_PRGM_check();

	memcpy(&RS422_APC_FLOW_PRGM,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_APC_FLOW_PRGM_t));
}

void rs422_APC_CMD_INLET_FLOW_PRGM_check() {}

void rs422_APC_CMD_INLET_PRESS_PRGM_fn(uint32_t slvid_addr)
{
	RS422_APC_PRESS_PRGM_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_APC_PRESS_PRGM_t));
	
	rs422_APC_CMD_INLET_PRESS_PRGM_check();

	memcpy(&RS422_APC_PRESS_PRGM,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_APC_PRESS_PRGM_t));
}

void rs422_APC_CMD_INLET_PRESS_PRGM_check(){}

void rs422_APC_CMD_DET_CONFIG_1_fn(uint32_t slvid_addr)
{
	RS422_Det_Config_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Det_Config_t));
	
	rs422_APC_CMD_DET_CONFIG_1_check();

	memcpy(&RS422_Det_Config,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Det_Config_t));
}

void rs422_APC_CMD_DET_CONFIG_1_check(){}

void rs422_APC_CMD_DET_CONFIG_2_fn(uint32_t slvid_addr)
{
	RS422_Det_Config_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Det_Config_t));
	
	rs422_APC_CMD_DET_CONFIG_2_check();

	memcpy(&RS422_Det_Config,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Det_Config_t));
}

void rs422_APC_CMD_DET_CONFIG_2_check(){}

void rs422_APC_CMD_DET_CONFIG_3_fn(uint32_t slvid_addr)
{
	RS422_Det_Config_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Det_Config_t));
	
	rs422_APC_CMD_DET_CONFIG_3_check();

	memcpy(&RS422_Det_Config,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Det_Config_t));
}
	
void rs422_APC_CMD_DET_CONFIG_3_check(){}

void rs422_APC_CMD_DET_SETTING_1_fn(uint32_t slvid_addr)
{
	RS422_Det_Setting_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Det_Setting));
	
	rs422_APC_CMD_DET_SETTING_1_check();

	memcpy(&RS422_Det_Setting,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Det_Setting));
}

void rs422_APC_CMD_DET_SETTING_1_check(){}

void rs422_APC_CMD_DET_SETTING_2_fn(uint32_t slvid_addr)
{
	RS422_Det_Setting_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Det_Setting));
	
	rs422_APC_CMD_DET_SETTING_2_check();

	memcpy(&RS422_Det_Setting,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Det_Setting));
}

void rs422_APC_CMD_DET_SETTING_2_check(){}

void rs422_APC_CMD_DET_SETTING_3_fn(uint32_t slvid_addr)
{
	RS422_Det_Setting_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Det_Setting));
	
	rs422_APC_CMD_DET_SETTING_3_check();

	memcpy(&RS422_Det_Setting,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Det_Setting));
}
	
void rs422_APC_CMD_DET_SETTING_3_check(){}

void rs422_APC_CMD_AUX_SETTING_1_fn(uint32_t slvid_addr)
{
	RS422_Aux_Setting_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Aux_Setting_t));
	
	rs422_APC_CMD_AUX_SETTING_1_check();

	memcpy(&RS422_Aux_Setting,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Aux_Setting_t));
}

void rs422_APC_CMD_AUX_SETTING_1_check(){}

void rs422_APC_CMD_AUX_SETTING_2_fn(uint32_t slvid_addr)
{
	RS422_Aux_Setting_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Aux_Setting_t));
	
	rs422_APC_CMD_AUX_SETTING_2_check();

	memcpy(&RS422_Aux_Setting,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Aux_Setting_t));
}

void rs422_APC_CMD_AUX_SETTING_2_check(){}

void rs422_APC_CMD_AUX_SETTING_3_fn(uint32_t slvid_addr)
{
	RS422_Aux_Setting_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Aux_Setting_t));
	
	rs422_APC_CMD_AUX_SETTING_3_check();

	memcpy(&RS422_Aux_Setting,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Aux_Setting_t));
}
	
void rs422_APC_CMD_AUX_SETTING_3_check(){}

void rs422_APC_CMD_CALIB_SETTING_fn(uint32_t slvid_addr)
{
	RS422_Calibration_Set_t tmp;

	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Calibration_Set_t));
	
	rs422_APC_CMD_CALIB_SETTING_check();

	memcpy(&RS422_Calibration_Set,(uint8_t *)(pRxMem + slvid_addr + 2),sizeof(RS422_Calibration_Set_t));
}

void rs422_APC_CMD_CALIB_SETTING_check(){}

void rs422_APC_CMD_MAIN_STATE_fn(uint32_t slvid_addr)
{
	RS422_Main_State_t tmp;
	
	memcpy(&tmp,(uint8_t *)(pRxMem + 2),sizeof(RS422_Main_State));
		
	rs422_APC_CMD_MAIN_STATE_check();		
//	dp("rs422_APC_CMD_MAIN_STATE_fn\n");
	
	memcpy(&RS422_Main_State,(uint8_t *)(pRxMem + 2),sizeof(RS422_Main_State));
//ShowMemory(1, (uint8_t *)pRxMem, sizeof(RS422_Main_State) + 4);
/*
dprintf(" fOvenTemp = %f\n fDetTemp[0] = %f\n fPout = %f\n fSignal[2] = %f\n iGCstate = %d\n",
RS422_Main_State.fOvenTemp,
RS422_Main_State.fDetTemp[0],
RS422_Main_State.fPout,
RS422_Main_State.fSignal[2],
RS422_Main_State.iGCstate);	
*/
}

void rs422_APC_CMD_MAIN_STATE_check()
{
	// 받은 데이처 검사
}



void rs422_APC_Det_Data_Send_buffer(void)
{
	int i;
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
//dp("TSTAT %02X\r\n",TSTAT);	
		FPGA_WR_RS422_TX_FLAG_CLEAR;

		pTxMem[0] = APC_STATE_SEND_CMD;	
		pTxMem[1] = rs422_APC_CMD_DET_DATA;

#if 1	// 테스트
RS422_Det_Data.iStartStop = 0x0;

for(i=0;i<3;i++) {
	RS422_Det_Data.iOnOff[i] = ON;
	RS422_Det_Data.iReady[i] = 0xff;

	RS422_Det_Data.iF_ErrorCode = 0x0;
	RS422_Det_Data.iC_ErrorCode = 0x0;
	RS422_Det_Data.iR_ErrorCode = 0x0;
	
	RS422_Det_Data.fDet_ActFlow[i][0] = 33.3;
//	RS422_Det_Data.fDet_SetFlow[i][0] = 44.4;
}
#endif
		memcpy((uint8_t *)(pTxMem+2),&RS422_Det_Data,sizeof(RS422_Det_Data));
		
		FPGA_WR_RS422_TX_LENGTH((sizeof(RS422_Det_Data) >> 2) + 2); 	// tx_len : uint8_t --> uint32_t , 
//테스트 - master에서 요청시 자동으로 보낼 수 있도록 저장만 한다.  트리거신호는 없다.(FPGAㅔ서 자동으로)	
//	FPGA_WR_RS422_SEND_TRIG(RS422_ID_MASTER); // tx_start

		apcif.TSTAT = 1;//TSTAT;			
//dp("OK---TSTAT %02X\r\n",TSTAT);						
	}
	else {
		apcif.TSTAT = 0;//TSTAT;
//dp("---TSTAT %02X\r\n",TSTAT);				
	}

}

void rs422_APC_Det_Data_Send_blank(void){}

void rs422_APC_Aux_Data_Send_buffer(void)
{
	uint8_t TSTAT;

	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
//dp("TSTAT %02X\r\n",TSTAT);	
		FPGA_WR_RS422_TX_FLAG_CLEAR;
	
		pTxMem[0] = APC_STATE_SEND_CMD;	
		pTxMem[1] = rs422_APC_CMD_AUX_DATA;

		memcpy((uint8_t *)(pTxMem+2),&RS422_Aux_Data,sizeof(RS422_Aux_Data));
		
		FPGA_WR_RS422_TX_LENGTH((sizeof(RS422_Aux_Data) >> 2) + 2); 	// tx_len : uint8_t --> uint32_t , 
//테스트 - master에서 요청시 자동으로 보낼 수 있도록 저장만 한다.  트리거신호는 없다.(FPGAㅔ서 자동으로)		
//	FPGA_WR_RS422_SEND_TRIG(RS422_ID_MASTER); // tx_start
		apcif.TSTAT = 1;//TSTAT;			
	}
	else {
		apcif.TSTAT = 0;//TSTAT;
	}	
}

void rs422_APC_Aux_Data_Send_blank(void){}

void rs422_APC_Calib_Data_Send_buffer(void)
{
	uint8_t TSTAT;
	
	TSTAT = FPGA_READ_WORD(APC_RS422_TX_STATE_ADDR);

	if(TSTAT&0x80) // TX END
	{
		FPGA_WR_RS422_TX_FLAG_CLEAR;
	
		pTxMem[0] = APC_STATE_SEND_CMD;	
		pTxMem[1] = rs422_APC_CMD_CALIB_DATA;

		memcpy((uint8_t *)(pTxMem+2),&RS422_Calibration_Data,sizeof(RS422_Calibration_Data));
		
		FPGA_WR_RS422_TX_LENGTH((sizeof(RS422_Calibration_Data) >> 2) + 2); 	// tx_len : uint8_t --> uint32_t , 
//	FPGA_WR_RS422_SEND_TRIG(RS422_ID_MASTER); // tx_start
//테스트 - master에서 요청시 자동으로 보낼 수 있도록 저장만 한다.  트리거신호는 없다.(FPGAㅔ서 자동으로)		
		apcif.TSTAT = 1;//TSTAT;			
	}
	else {
		apcif.TSTAT = 0;//TSTAT;
	}
}

void rs422_APC_Calib_Data_Send_blank(void){}

void rs422_APC_Volt_Data_Send_buffer(void)
{
	pTxMem[0] = APC_STATE_SEND_CMD;	
	pTxMem[1] = rs422_APC_CMD_VOLT_DATA;

	memcpy((uint8_t *)(pTxMem+2),&RS422_APC_Volt_data,sizeof(RS422_APC_Volt_data));
	
	FPGA_WR_RS422_TX_LENGTH((sizeof(RS422_APC_Volt_data) >> 2) + 2); 	// tx_len : uint8_t --> uint32_t , 
//	FPGA_WR_RS422_SEND_TRIG(RS422_ID_MASTER); // tx_start	
//테스트 - master에서 요청시 자동으로 보낼 수 있도록 저장만 한다.  트리거신호는 없다.(FPGAㅔ서 자동으로)		
}

void rs422_APC_Volt_Data_Send_blank(void){}

#endif









//================================================================================
//================================================================================
// This function may be called every 1 MS
void apc_process(void)
{
	if(apcif.State == APC_STATE_RUN)
	{
		if(GCC_MASTER == gcsvc.MasterSlave)
		{
			if(apcif.RunMode == APC_RUN_AUTOMODE)
			{
				if(apcif.AutoEnd == 1)
				{
//					apc_APCMainStatusUpdate();
					apcif.AutoEnd = 0;
					apc_CheckAutoResult();
apc_APCCPUModeSetting();
				}
			}
			else
			{
				apc_CheckRxState();
			}
		}
		else
		{
			apc_CheckRxState();
		}

		apc_CheckTxState();

	}

}

void apc_APCSendStaus(uint8_t dest_addr)
{
	apc_APCSend(dest_addr,APC_CONFIG_STASUS_SIZE,APC_CMD_CON_STATUS,apcif.MainStatus);
}

void apc_APCSendStausAuto(void)
{
	apcif.slvch++;
	if(apcif.slvch >= APC_SLAVE_NUMBER) apcif.slvch=0;
	apc_APCSendStaus(apcif.slvch+1);
}

void apc_APCSendConfig(uint8_t dest_addr)
{
	uint8_t CmdRespCode=0;
	uint8_t DestinationAddr=0;

	if(GCC_MASTER == gcsvc.MasterSlave) // Configuration Set Command
	{
		if((dest_addr == 0)||(dest_addr > 9)) return;

		if(dest_addr < 4)  		CmdRespCode =	rs422_APC_CMD_INLET_CONFIG;
		else if(dest_addr == 4)  CmdRespCode =	rs422_APC_CMD_DET_CONFIG_1;
		else if(dest_addr == 5) CmdRespCode =	rs422_APC_CMD_AUX_SETTING_1;

		DestinationAddr = dest_addr;
//		apc_APCSend(DestinationAddr,APC_CONFIG_STASUS_SIZE,CmdRespCode, apcif.Config[DestinationAddr-1]);
		apc_APCSend(DestinationAddr,63,CmdRespCode, apcif.Config[DestinationAddr-1]);
		
	}
	else // Configuration Response
	{
		if(gcsvc.ApcMyId < 4)  		CmdRespCode =	rs422_APC_CMD_INLET_CONFIG;
		else if(gcsvc.ApcMyId == 4)  CmdRespCode =	rs422_APC_CMD_DET_CONFIG_1;
		else if(gcsvc.ApcMyId == 5) CmdRespCode =	rs422_APC_CMD_AUX_SETTING_1;
 
//apc_APCSend(0xE,APC_CONFIG_STASUS_SIZE,CmdRespCode, apcif.Config[gcsvc.ApcMyId]);
apc_APCSend(0xE,63                   ,CmdRespCode, apcif.Config[gcsvc.ApcMyId - 1]);

	}
}

// this function is available in Slave mode
void apc_APCConfigSendBack(void)
{
	uint8_t CmdRespCode=0;
	uint8_t i;
	uint8_t dest_addr = 0xE;

	if(GCC_SLAVE == gcsvc.MasterSlave)
	{
		if(gcsvc.ApcMyId < 4)  		CmdRespCode =	APC_CMD_INLET_SET;
		else if(gcsvc.ApcMyId < 7)  CmdRespCode =	APC_CMD_DET_SET;
		else if(gcsvc.ApcMyId < 10) CmdRespCode =	APC_CMD_AUX_SET;

		pTxMem[0] = CmdRespCode;

		for(i=0;i<APC_CONFIG_STASUS_SIZE;i++)
			pTxMem[1+i] =  pRxMem[1+i];
		FPGA_WR_RS422_TX_LENGTH(APC_CONFIG_STASUS_SIZE+1); 	// tx_len
		FPGA_WR_RS422_SEND_TRIG(dest_addr);// tx_start
	}
}

// AUTO Mode 일 경우는 CMD는 0xC0가 되어야 한다.
void apc_APCMainStatusUpdate(void)
{
	uint8_t i;
	uint32_t tdata;
	apcif.TST_patternA++;

#if 0
	if(apcif.TST_patternA&0x01)
		tdata = 0x1234;
	else
		tdata = 0x5678;

	// 1. define control word
//	pTxMem[0] = APC_CMD_CON_STATUS;
	pTxMem[0] = AUTO_MODE_CMD;
	
	// 2. define information
	for(i=0;i<APC_CONFIG_STASUS_SIZE;i++)
	{
		pTxMem[i+1] = tdata;
	}
	// 3. define length = control(1) + information(APC_CONFIG_STASUS_SIZE)
	FPGA_WR_RS422_TX_LENGTH(APC_CONFIG_STASUS_SIZE+1); 	// tx_len
#else
//	pTxMem[0] = APC_CMD_CON_STATUS;		
		pTxMem[0] = AUTO_MODE_CMD;		

	memcpy((uint8_t *)(pTxMem+1),&RS422_Main_State,sizeof(RS422_Main_State));
	
	FPGA_WR_RS422_TX_LENGTH((sizeof(RS422_Main_State) >> 2) + 1 + 1); 	// tx_len : uint8_t --> uint32_t , 
#endif	
}

void apc_APCSlaveStatusUpdate(void)
{
	uint8_t i;
	uint32_t tdata;
	apcif.TST_patternA++;

	if(apcif.TST_patternA&0x01)
		tdata = 0xABCDE;
	else
		tdata = 0xEF012;

	// 1. define control word
	pTxMem[0] = APC_CMD_CON_STATUS;

	// 2. define information
	for(i=0;i<APC_CONFIG_STASUS_SIZE;i++)
	{
		pTxMem[i+1] = tdata;
	}

	// 3. define length = control(1) + information(APC_CONFIG_STASUS_SIZE)
	FPGA_WR_RS422_TX_LENGTH(APC_CONFIG_STASUS_SIZE+1); 	// tx_len
}


#endif




