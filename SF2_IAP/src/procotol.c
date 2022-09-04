
#include "config.h"

#define	MAX_BUFFER		1024

#define	MAX_DATA_SIZE	(MAX_BUFFER / 2)

#define	STX_SIZE		1
#define	CHECKSUM_SIZE	1
#define	ETX_SIZE		1

unsigned char CalcChkSum(uint8_t* pBuffer, int offset, int size);

void uart0_rx_handler(mss_uart_instance_t * this_uart)
{
    uint8_t buffer[MAX_BUFFER] = {0};
    uint32_t nSize  = 0;

    nSize = MSS_UART_get_rx(this_uart, buffer, sizeof(buffer));
    enqueue(buffer, 0, nSize);

	NVIC_ClearPendingIRQ(UART0_IRQn);

}

#if 0
extern volatile size_t g_tx_size;
extern volatile const uint8_t * g_tx_buffer;

__attribute__((__interrupt__)) void uart_tx_handler(
		mss_uart_instance_t * this_uart)
{
	size_t size_in_fifo;

	if (g_tx_size > 0)
	{
		size_in_fifo = MSS_UART_fill_tx_fifo(this_uart,
				(const uint8_t *) g_tx_buffer, g_tx_size);

		if (size_in_fifo == g_tx_size)
		{
			g_tx_size = 0;
			MSS_UART_disable_irq(this_uart, MSS_UART_TBE_IRQ);
		}
		else
		{
			g_tx_buffer = &g_tx_buffer[size_in_fifo];
			g_tx_size = g_tx_size - size_in_fifo;
		}
	}
	else
	{
		g_tx_size = 0;
		MSS_UART_disable_irq(this_uart, MSS_UART_TBE_IRQ);
	}
}
#endif

int GenPacket(uint8_t* pBuffer, uint16_t cmd, uint8_t flag, uint8_t *pData, int size)
{
	int nPos =0;
	uint16_t chkSum = 0;

	pBuffer[nPos++] = PKT_STX;
	pBuffer[nPos++] = cmd & 0xFF;
	pBuffer[nPos++] = ( cmd >> 8 ) & 0xFF;
	pBuffer[nPos++] = flag;
	pBuffer[nPos++] = size & 0xFF;
	pBuffer[nPos++] = (size >> 8) & 0xFF;

	if (NULL != pData && size > 0) {
		memcpy(&pBuffer[nPos], pData, size);
		nPos += size;
	}

	chkSum = CalcChkSum(pBuffer, 1, nPos - 1);
	pBuffer[nPos++] = chkSum & 0xFF;
	//pBuffer[nPos++] = (chkSum >> 8) & 0xFF;
	pBuffer[nPos++] = PKT_ETX;

	return nPos;
}

int GenHeaderPacket(uint8_t* pBuffer, uint16_t cmd, uint8_t flag, uint8_t *pData, int size)
{
	int nPos =0;
	uint16_t chkSum = 0;

	pBuffer[nPos++] = PKT_STX;
	pBuffer[nPos++] = cmd & 0xFF;
	pBuffer[nPos++] = ( cmd >> 8 ) & 0xFF;
	pBuffer[nPos++] = flag;

	if (NULL != pData && size > 0) {
		pBuffer[nPos++] = size & 0xFF;
		pBuffer[nPos++] = (size >> 8) & 0xFF;

		memcpy(&pBuffer[nPos], pData, size);
		nPos += size;
	}

	return nPos;
}

int RecvData(uint8_t * pBuffer, int length)
{
	bool bRet = false;
	int nRet = 0;
	unsigned int packetLength = 0;
    int size = 0, hdrSize = sizeof(HEADER_PKT), dataSize = 0;

	PHEADER_PKT pPkt;

	do {

		if (get_available() >= hdrSize) {

			nRet = peek((uint8_t *)pBuffer, 0, hdrSize);
			pPkt = (PHEADER_PKT)(pBuffer);

			dataSize = pPkt->Length;
			size = hdrSize + dataSize + CHECKSUM_SIZE + ETX_SIZE;

			if (get_available() >= size ) {		// Packet Data

				nRet = dequeue((uint8_t *)pBuffer, 0, size);
				if(nRet != size) {
					nRet = 0;
					break;
				}
				nRet = size;
//				nRet = CheckPacket((uint8_t *)pBuffer, size);
//				if(nRet >= 0) {
//					nRet = size;
//				} else {
//					//nRet = 0;
//				}
				break;

			}
			else {
				nRet = 0;
				break;
			}
		}
		else {
			nRet = 0;
			break;
		}
	}while(get_available() >= hdrSize);

	return nRet;
}

#if 0
unsigned short CalcChkSum(char* pBuffer, int offset, int size)
{
	short word16 ,  i;
	unsigned int sum = 0;
	for( i = 0; i < size; i = i+2 )
	{
		word16 = ( ( pBuffer[offset + i]<<8) & 0xFF00 )+( pBuffer[offset + i+1] & 0xFF );
		sum = sum + (unsigned int) word16;
	}

	while( sum >> 16 )
			sum = ( sum & 0xFFFF ) + ( sum >> 16 );

	sum = ~sum;
	return ((u_short) sum);

}
#else
unsigned char CalcChkSum(uint8_t* pBuffer, int offset, int size)
{
	int sum = 0, temp = 0, result = 0;

	int i = 0;
	for (i = 0; i < size; i++) {
		temp = pBuffer[offset + i] & 0xFF;
		sum += temp;
	}
	result = (~(sum & 0xFF) + 1)& 0xFF;

	return (unsigned char)result ;

}
#endif

int CheckPacket(uint8_t* pBuffer, int size)
{
	int nRet = ERROR_NO_ERROR;
	unsigned char chkSum = 0, calcChkSum = 0;
	PHEADER_PKT pPktHeader = (PHEADER_PKT) pBuffer;

	if(pPktHeader->Stx == PKT_STX) {
		calcChkSum = CalcChkSum(pBuffer, 1, size - (STX_SIZE + CHECKSUM_SIZE + ETX_SIZE)) ;
		chkSum = (pBuffer[size - 2]  & 0xFF) ;

		if (chkSum != calcChkSum) {
#ifdef __DEBUG_CONSOLE__
			//printf("Invalid CheckSum [%X, %X]\n", chkSum, calcChkSum);
#endif

			nRet = ERROR_INVALID_CHKSUM;
		}

	} else {
		nRet = ERROR_INVALID_STX;
	}

	return nRet;
}

int ParseUpdateData(uint8_t *pBuffer, int size)
{
#define	BYTE_PER_PAGE		128
#define	PAGE_PER_BLOCK		32
#define	MAX_RETRY			3

	bool bResponse = false;
	uint8_t flag = FLAG_ACK;
	int nRet = 0, nPos = 0, nLen = 0 , nRetry = 0;
	nvm_status_t status = NVM_SUCCESS;
	static volatile uint32_t address = 0x00000000 , baseAddress = ENVM_BOOTLOADER_UPDATE_BASE_ADDR, offset = 0;

	uint8_t pktBuffer[MAX_BUFFER]={0};

	uint8_t dataBuffer[MAX_DATA_SIZE]={0};
	int dataSize = sizeof(int);

	PSTORAGE_INFO_T pStorageInfo = GetStorageInfo();
	PHEADER_PKT pPktHeader = (PHEADER_PKT) pBuffer;
	PUPDATE_RESP_PKT pPktResp;

	nRet = CheckPacket(pBuffer, size);
	if(nRet == ERROR_NO_ERROR) {

		switch(pPktHeader->Cmd) {
			case PKT_CMD_UPDATE_DATA :						// data record.
			{
				bResponse = true;
				nPos = sizeof(HEADER_PKT);
				address = baseAddress + offset;
				for(nRetry = 0 ; nRetry < MAX_RETRY ; nRetry++) {
//					status = NVM_verify( address, &pBuffer[nPos], pPktHeader->Length );
//					if(status != NVM_SUCCESS) {

						status = NVM_write( address, &pBuffer[nPos], pPktHeader->Length, NVM_DO_NOT_LOCK_PAGE);
						if(NVM_SUCCESS == status || NVM_WRITE_THRESHOLD_WARNING == status)
						{
							status = NVM_verify( address, &pBuffer[nPos], pPktHeader->Length );
							if(status != NVM_SUCCESS) {
								nRet = status;
							} else {
								offset += pPktHeader->Length;
							}
							break;
						}
						else if(NVM_PAGE_LOCK_ERROR == status) {
							// 메모리 Unlock 후  writing 재시도  or send NAK
							status = NVM_unlock(address, pPktHeader->Length);
							if( status != NVM_SUCCESS ) {
								nRet = status;
							}
						}
						else {
							nRet = status;
							break;
						}
//					}
//					else {		// Unchanged Data
//						offset += pPktHeader->Length;
//						break;
//					}

				}
			}
				break;
			case PKT_CMD_READ_DATA :						// end of file record.
			{
				bResponse = true;

				PREAD_DATA_PKT pktData = (PREAD_DATA_PKT) pBuffer;
				dataSize = pktData->DataLen;
				if( dataSize > MAX_DATA_SIZE) {
					nRet = ERROR_READ_DATA_SIZE;
				} else {
					int readAddr = pktData->Address - ENVM_BOOTLOADER_UPDATE_BASE_ADDR;
					status = MSS_NVM_read( (uint8_t *)readAddr, (uint8_t *)dataBuffer, dataSize);
					if(status != NVM_SUCCESS) {
						nRet = ERROR_READ_DATA;
					}
				}
			}
				break;

			case PKT_CMD_UPDATE_END :						// end of file record.
			{
				LED_OnOff(LED_OFF);
				bResponse = true;
			}
				break;

			case PKT_CMD_UPDATE_BEGIN :				// start
			{
				baseAddress = ENVM_BOOTLOADER_UPDATE_BASE_ADDR;
				offset = 0;
				LED_OnOff(LED_ON);

				bResponse = true;
			}
				break;
			case PKT_CMD_UPDATE_BASE_ADDR :			// extended linear address record. (  Upper 4byte 0xFFFF0000 )
			{
				PUPDATE_ADDR_PKT pktData = (PUPDATE_ADDR_PKT) pBuffer;
				baseAddress = pktData->Data;

				bResponse = true;
			}
				break;
			case PKT_CMD_UPDATE_BASE_OFFSET_ADDR :			// extended segment address record. ( Addr x 16 )
			{
				PUPDATE_ADDR_PKT pktData = (PUPDATE_ADDR_PKT) pBuffer;
				offset = pktData->Data;

				bResponse = true;

			}
				break;
			case PKT_CMD_UPDATE_OFFSET_ADDR :			// extended segment address record. ( Addr x 16 )
			{
				PUPDATE_ADDR_PKT pktData = (PUPDATE_ADDR_PKT) pBuffer;
				offset &= 0xFFFF0000;
				offset |= pktData->Data;

				bResponse = true;
			}
				break;

			case PKT_CMD_UPDATE_OFFSET_ADDR_2 :			// extended segment address record. ( Addr x 16 )
			{
				PUPDATE_ADDR_PKT pktData = (PUPDATE_ADDR_PKT) pBuffer;
				offset &= 0xFFFFF000;
				offset |= pktData->Data;

				bResponse = true;
			}
				break;

			case PKT_CMD_SEND_SERIAL_NO :			//
			{
				PRUN_SERIAL_NO_PKT pktData = (PRUN_SERIAL_NO_PKT) pBuffer;

				SetSerialNo(pktData->SerialNo, pktData->Length);
				SaveStorageInfo(DS_TYPE_SYSTEM);
				bResponse = true;
			}
				break;

			case PKT_CMD_REQ_SERIAL_NO :			//
			{
				//dataSize = sizeof(pStorageInfo->SystemInfo.SerialNo);
				//memcpy(dataBuffer, (uint8_t*)pStorageInfo->SystemInfo.SerialNo, dataSize);

				dataSize = GetSerialNo(dataBuffer);

				bResponse = true;
			}
				break;

			case PKT_CMD_SEND_MODEL_NAME :			//
			{
				PMODEL_NAME_PKT pktData = (PMODEL_NAME_PKT) pBuffer;

				SetModelName(pktData->ModelName, pktData->Length);
				SaveStorageInfo(DS_TYPE_SYSTEM);
				bResponse = true;
			}
				break;

			case PKT_CMD_REQ_MODEL_NAME :			//
			{
				dataSize = GetModelName(dataBuffer);

				bResponse = true;
			}
				break;

			case PKT_CMD_SAVE_STORAGE_INFO :			//
			{
				status = SaveStorageInfoALL();
				if (NVM_SUCCESS != status) {
					nRet = status;
				}

				bResponse = true;
			}
				break;

			case PKT_CMD_REQ_VERSION :			// extended segment address record. ( Addr x 16 )
			{
				VERSION_T tVersion;
				tVersion.BL = GetBLVer();
				tVersion.IAP = GetIAPVer();
				tVersion.APP = GetAPPVer();
				tVersion.FPGA = GetFPGAVer();

				dataSize = sizeof(tVersion);
				memcpy(&dataBuffer[nPos], (uint8_t*)&tVersion, dataSize);

				bResponse = true;
			}
				break;
		}

		if(nRet !=  ERROR_NO_ERROR) {
			flag = FLAG_NAK;
			dataBuffer[0] = nRet & 0xFF;
			dataBuffer[1] = (nRet >> 8) & 0xFF;
			dataBuffer[2] = (nRet >> 16) & 0xFF;
			dataBuffer[3] = (nRet >> 24) & 0xFF;
			dataSize = sizeof(int);
			bResponse = true;
		}

	} else {
		flag = FLAG_NAK;
		dataBuffer[0] = nRet & 0xFF;
		dataBuffer[1] = (nRet >> 8) & 0xFF;
		dataBuffer[2] = (nRet >> 16) & 0xFF;
		dataBuffer[3] = (nRet >> 24) & 0xFF;
		dataSize = sizeof(int);
		bResponse = true;
	}

	if(bResponse) {
		nLen = GenPacket((uint8_t*)pktBuffer, pPktHeader->Cmd, flag, (uint8_t*)dataBuffer, dataSize);
		MSS_UART_polled_tx( get_uart(), (uint8_t*)pktBuffer, nLen );
	}

	// Reboot system
	if (PKT_CMD_RESET_SYSTEM == pPktHeader->Cmd) {
		delay_ms(1000);
		reset_system();
	}

	return nRet;
}


int processFwUpdate()
{
#define		MAX_LED_FLASH_INTERVAL		3
#define		LED_FLASH_INTERVAL 		200	//ms

	uint8_t buffer[MAX_BUFFER] = {0};
	int nRet = 0, nSize = 0;
	uint8_t ledOn = LED_ON , ledFlashInterval = 1;

	uint32_t time = 0;
	time = get_mili_time();
	while(1)
	{
		nSize = RecvData ( buffer, MAX_BUFFER );
		if(nSize > 0) {
			if((diff_mili_time(time) >= LED_FLASH_INTERVAL)) {
				time = get_mili_time();
				LED_OnOff(ledOn);
				ledOn ^=1;
			}

			nRet = ParseUpdateData(buffer, nSize);
			memset(buffer, 0, MAX_BUFFER);
		}
	}

	return nRet;
}

int post_update_ready()
{
	int nRet = 0;
	UPDATE_COMMON_PKT pkt;

	nRet = GenPacket((uint8_t*)&pkt, PKT_CMD_UPDATE_READY, FLAG_SEND, NULL, 0 );
	MSS_UART_polled_tx( get_uart(), (uint8_t*)&pkt, nRet );

	return nRet;
}

int post_prog_run()
{
	int nRet = 0;
	RUN_PROGRAM_T tProgram;
	RUN_PROGRAM_PKT pkt;

	tProgram.Kind = PROGRAM_IAP;
	memcpy(tProgram.Version , IAP_FIRMWARE_VERSION_STRING, sizeof(IAP_FIRMWARE_VERSION_STRING));

	nRet = GenPacket((uint8_t*)&pkt, PKT_CMD_RUN_PROGRAM, FLAG_SEND, (uint8_t*)&tProgram, sizeof(tProgram));
	MSS_UART_polled_tx( get_uart(), (uint8_t*)&pkt, nRet );

	return nRet;
}
