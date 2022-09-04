#include "config.h"

#define	MAX_RETRY			3

STORAGE_INFO_T	g_tStorageInfo;

int InitStorageInfo()
{
	int nRet = 0, i = 0;
	memset((uint8_t *)&g_tStorageInfo, 0, sizeof(STORAGE_INFO_T));
	for(i = 0; i < MAX_RETRY ; i++) {
		nRet = LoadStorageInfo();
		if(NVM_SUCCESS == nRet) {
			break;
		} else {
			//DEBUG_E("() - Failed to read data storage [%d]\r\n", nRet);
		}
	}
	return nRet;
}

PSTORAGE_INFO_T GetStorageInfo()
{
	return (PSTORAGE_INFO_T)&g_tStorageInfo;
}

int LoadStorageInfo()
{
	nvm_status_t status = NVM_SUCCESS;
	int nRet = NVM_SUCCESS, nRetry = 0, nSize = sizeof(STORAGE_INFO_T);
	volatile uint32_t address = ENVM_DATA_STORAGE_READ_BASE_ADDR;

	for(nRetry = 0 ; nRetry < MAX_RETRY ; nRetry++) {
		status = MSS_NVM_read( (uint8_t *)address, (uint8_t *)&g_tStorageInfo, nSize);
		if(NVM_SUCCESS == status)
		{
			break;
		}
//		else if(NVM_PAGE_LOCK_ERROR == status) {
//			// 메모리 Unlock 후  writing 재시도  or send NAK
//			if( NVM_unlock(address, pPktHeader->Length) != NVM_SUCCESS ) {
//
//			}
//		}
		else {
			nRet = status;
			break;
		}
	}

	return nRet;
}

int SaveStorageInfo(int type)
{
	nvm_status_t status = NVM_SUCCESS;
	int nRet = NVM_SUCCESS, nRetry = 0, nSize = sizeof(STORAGE_INFO_T);
	volatile uint32_t address = 0, baseAddress = ENVM_DATA_STORAGE_WRITE_BASE_ADDR, offset = 0 ;
	uint8_t* pData = NULL;

	if( DS_TYPE_SYSTEM == type)
	{
		offset = 0;
		nSize = sizeof(g_tStorageInfo.SystemInfo);
		pData = (uint8_t*)&g_tStorageInfo.SystemInfo;
	}
	else if( DS_TYPE_LAMP_TIME == type)
	{
		offset = sizeof(g_tStorageInfo.SystemInfo);
		nSize = sizeof(g_tStorageInfo.LampTime);
		pData = (uint8_t*)&g_tStorageInfo.LampTime;
	}
	else if( DS_TYPE_WAVE == type)
	{
		offset = sizeof(g_tStorageInfo.SystemInfo) + sizeof(g_tStorageInfo.LampTime);
		nSize = sizeof(g_tStorageInfo.WaveInfo);
		pData = (uint8_t*)&g_tStorageInfo.WaveInfo;
	} else {
		//DEBUG_E("() - Invalid Storage Type[%d]", type);
		return -1;
	}

	address = baseAddress + offset;
	for(nRetry = 0 ; nRetry < MAX_RETRY ; nRetry++)
	{
		status = NVM_write( address, pData, nSize, NVM_DO_NOT_LOCK_PAGE);
		if(NVM_SUCCESS == status || NVM_WRITE_THRESHOLD_WARNING == status)
		{
			status = NVM_verify( address, (uint8_t *)&g_tStorageInfo, nSize );
			if(status != NVM_SUCCESS) {
				nRet = status;
			}
//			else {
//				address = ENVM_DATA_STORAGE_READ_BASE_ADDR;
//				status = MSS_NVM_read( (uint8_t *)address, (uint8_t *)&g_tStorageInfo, nSize);
//				if(NVM_SUCCESS != status)
//				{
//					nRet = status;
//				}
//			}
			break;
		}
		else if(NVM_PAGE_LOCK_ERROR == status) {
			// 메모리 Unlock 후  writing 재시도  or send NAK
			if( NVM_unlock(address, nSize) != NVM_SUCCESS ) {

			}
		}
		else {
			nRet = status;
			break;
		}
	}
	return nRet;
}

int SaveStorageInfoALL()
{
	nvm_status_t status = NVM_SUCCESS;
	int nRet = NVM_SUCCESS, nRetry = 0, nSize = sizeof(STORAGE_INFO_T);
	volatile uint32_t address = ENVM_DATA_STORAGE_WRITE_BASE_ADDR;

	for(nRetry = 0 ; nRetry < MAX_RETRY ; nRetry++)
	{
		status = NVM_write( address, (uint8_t *)&g_tStorageInfo, nSize, NVM_DO_NOT_LOCK_PAGE);
		if(NVM_SUCCESS == status || NVM_WRITE_THRESHOLD_WARNING == status)
		{
			status = NVM_verify( address, (uint8_t *)&g_tStorageInfo, nSize );
			if(status != NVM_SUCCESS) {
				nRet = status;
			}
//			else {
//				address = ENVM_DATA_STORAGE_READ_BASE_ADDR;
//				status = MSS_NVM_read( (uint8_t *)address, (uint8_t *)&g_tStorageInfo, nSize);
//				if(NVM_SUCCESS != status)
//				{
//					nRet = status;
//				}
//			}
			break;
		}
		else if(NVM_PAGE_LOCK_ERROR == status) {
			// 메모리 Unlock 후  writing 재시도  or send NAK
			if( NVM_unlock(address, nSize) != NVM_SUCCESS ) {

			}
		}
		else {
			nRet = status;
			break;
		}
	}
	return nRet;
}

int SaveStorageInfoALL_Param(PSTORAGE_INFO_T pInfo)
{
	nvm_status_t status = NVM_SUCCESS;
	int nRet = NVM_SUCCESS, nRetry = 0, nSize = sizeof(STORAGE_INFO_T);
	volatile uint32_t address = ENVM_DATA_STORAGE_WRITE_BASE_ADDR;

	for(nRetry = 0 ; nRetry < MAX_RETRY ; nRetry++)
	{
		status = NVM_write( address, (uint8_t *)pInfo, nSize, NVM_DO_NOT_LOCK_PAGE);
		if(NVM_SUCCESS == status || NVM_WRITE_THRESHOLD_WARNING == status)
		{
			status = NVM_verify( address, (uint8_t *)pInfo, nSize );
			if(status != NVM_SUCCESS) {
				nRet = status;
			}
//			else {
//				address = ENVM_DATA_STORAGE_READ_BASE_ADDR;
//				status = MSS_NVM_read( (uint8_t *)address, (uint8_t *)&g_tStorageInfo, nSize);
//				if(NVM_SUCCESS != status)
//				{
//					nRet = status;
//				}
//			}
			break;
		}
		else if(NVM_PAGE_LOCK_ERROR == status) {
			// 메모리 Unlock 후  writing 재시도  or send NAK
			if( NVM_unlock(address, nSize) != NVM_SUCCESS ) {

			}
		}
		else {
			nRet = status;
			break;
		}
	}
	return nRet;
}

// System Info
int GetBLVer()
{
	int nRet = 0;
	nRet = g_tStorageInfo.SystemInfo.BLVer;
	return nRet;
}

void SetBLVer(int value)
{
	g_tStorageInfo.SystemInfo.BLVer = value;
}

int GetIAPVer()
{
	int nRet = 0;
	nRet = g_tStorageInfo.SystemInfo.IAPVer;
	return nRet;
}

void SetIAPVer(int value)
{
	g_tStorageInfo.SystemInfo.IAPVer = value;
}

int GetAPPVer()
{
	int nRet = 0;
	nRet = g_tStorageInfo.SystemInfo.APPVer;
	return nRet;
}

void SetAPPVer(int value)
{
	g_tStorageInfo.SystemInfo.APPVer = value;
}

int GetFPGAVer()
{
	int nRet = 0;
	nRet = g_tStorageInfo.SystemInfo.FPGAVer;
	return nRet;
}

void SetFPGAVer(int value)
{
	g_tStorageInfo.SystemInfo.FPGAVer = value;
}

int GetSerialNo(unsigned char *pBuffer)
{
	int nRet = 0;
	nRet = sizeof(g_tStorageInfo.SystemInfo.SerialNo);
	memcpy(pBuffer, g_tStorageInfo.SystemInfo.SerialNo, nRet);

	return nRet;
}

void SetSerialNo(unsigned char *pBuffer, int size)
{
	memcpy(g_tStorageInfo.SystemInfo.SerialNo, pBuffer, size);
}

int GetIPAddress(unsigned char *pBuffer)
{
	int nRet = 0;
	nRet = sizeof(g_tStorageInfo.SystemInfo.IPAddress);
	memcpy(pBuffer, g_tStorageInfo.SystemInfo.IPAddress, nRet);
	return nRet;
}

void SetIPAddress(unsigned char *pBuffer, int size)
{
	memcpy(g_tStorageInfo.SystemInfo.IPAddress, pBuffer, size);
}

int GetGateway(unsigned char *pBuffer)
{
	int nRet = 0;
	nRet = sizeof(g_tStorageInfo.SystemInfo.Gateway);
	memcpy(pBuffer, g_tStorageInfo.SystemInfo.Gateway, nRet);
	return nRet;
}

void SetGateway(unsigned char *pBuffer, int size)
{
	memcpy(g_tStorageInfo.SystemInfo.Gateway, pBuffer, size);
}

int GetMacAddress(unsigned char *pBuffer)
{
	int nRet = 0;
	nRet = sizeof(g_tStorageInfo.SystemInfo.MacAddress);
	memcpy(pBuffer, g_tStorageInfo.SystemInfo.MacAddress, nRet);
	return nRet;
}

void SetMacAddress(unsigned char *pBuffer, int size)
{
	memcpy(g_tStorageInfo.SystemInfo.MacAddress, pBuffer, size);
}

int GetModelName(unsigned char *pBuffer)
{
	int nRet = 0;
	nRet = sizeof(g_tStorageInfo.SystemInfo.Model);
	memcpy(pBuffer, g_tStorageInfo.SystemInfo.Model, nRet);

	return nRet;
}

void SetModelName(unsigned char *pBuffer, int size)
{
	memcpy(g_tStorageInfo.SystemInfo.Model, pBuffer, size);
}

// Lamp 사용시간 저장
int GetD2LampTime()
{
	int nRet = 0;
	nRet = g_tStorageInfo.LampTime.D2LampElapse;
	return nRet;
}

void SetD2LampTime(int value)
{
	g_tStorageInfo.LampTime.D2LampElapse = value;
}

int GetWLampTime()
{
	int nRet = 0;
	nRet = g_tStorageInfo.LampTime.WLampElapse;
	return nRet;
}

void SetWLampTime(int value)
{
	g_tStorageInfo.LampTime.WLampElapse = value;
}

int GetLampTime(PSTORAGE_LAMP_INFO_T pValue)
{
	int nRet = 0;
	nRet = sizeof(g_tStorageInfo.LampTime);
	memcpy(pValue, &g_tStorageInfo.LampTime, nRet);
	return nRet;
}

void SetLampTime(PSTORAGE_LAMP_INFO_T pValue, int size)
{
	memcpy(&g_tStorageInfo.LampTime, pValue, size);
}

void ClearLampTime()
{
	memset(&g_tStorageInfo.LampTime, 0, sizeof(g_tStorageInfo.LampTime));
}

// Calibration 후 파장 저장
int GetWaveInfo(PSTORAGE_WAVE_INFO_T pValue)
{
	int nRet = 0;
	nRet = sizeof(g_tStorageInfo.WaveInfo);
	memcpy(pValue, &g_tStorageInfo.WaveInfo, nRet);
	return nRet;
}

void SetWaveInfo(PSTORAGE_WAVE_INFO_T pValue, int size)
{
	memcpy(&g_tStorageInfo.WaveInfo, (uint8_t *)pValue, size);
}

void ClearWaveInfo()
{
	memset(&g_tStorageInfo.WaveInfo, 0, sizeof(g_tStorageInfo.WaveInfo));
}

int GetUseStorageENVM()
{
	int nRet = 0;
	nRet = g_tStorageInfo.SystemInfo.UsedENVM;
	return nRet;
}

void SetUseStorageENVM(int value)
{
	g_tStorageInfo.SystemInfo.UsedENVM = value;
}


