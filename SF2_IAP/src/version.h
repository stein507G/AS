#ifndef		_CHROGEN_LC_UVD_VERSION_H_
#define		_CHROGEN_LC_UVD_VERSION_H_

/*
================================================================================================
					< SmartFusion2 IAP( In-Application Programming ) Firmware History >
================================================================================================
Version	Test	Date		Author		Description 
------------------------------------------------------------------------------------------------
v0.90			21.10.05	CJKIM		1. PC와 연동하여 FW Update 기능 추가.
										2. Unchanged Data 메모리에 쓰지 않도록 수정.
										3. NVM_PAGE_LOCK_ERROR 발생 시 Unlock 실행.

v0.99			21.10.15	CJKIM		1. UART RX Interrupt로 처리 변경.
										2. IAP Memory 16K -> 24 KB 로 변경.

v1.00			21.10.18	CJKIM		1. Data Storage 처리  추가.
											- 프로그램 실행 시 Version 정보 저장.

v1.00			21.10.22	CJKIM		1. Data Storage 처리  추가.
											- System, Lamp Time, Wave 개별 저장 추가. ( 기존에는 모든 영역 저장함 )
										2. Serial No Packet 수신 시  Storage에 바로 저장하도록 변경.
										3. LoadStorageInfo() 실패 시  3회 Retry 추가.
										4. ClearWaveInfo(), ClearLampTime() 함수 오류 수정.
										5. 프로그램 실행 Packet 전송 제거.
										6. 소스 정리

v1.00			21.10.28	CJKIM		1. Memory Map 변경.
											- BootLoader 8KB -> 12KB로 변경.
											BootLoader		0x00000000	12KB
											IAP				0x00003000	24KB
											Application		0x00009000	216KB
											Data Storage	0x0003F000	2KB
											Reserved		0x0003F800	2KB

										* FW Release.

v1.00			21.11.08	CJKIM		1. Model 명  저장 / 읽기 기능 추가.

										* FW Release.

*/

#define VERSION_YEAR    	2021
#define VERSION_MONTH    	11
#define VERSION_DATE    	08

#define VERSION_MAJOR      	1
#define VERSION_MINOR_1    	0
#define VERSION_MINOR_2    	0


#define VERSION_STRING2(x) #x
#define VERSION_STRING(x) VERSION_STRING2(x)

#define IAP_FIRMWARE_VERSION   ( (VERSION_MAJOR * 100) + (VERSION_MINOR_1 * 10) + VERSION_MINOR_2)

#define IAP_FIRMWARE_VERSION_STRING     VERSION_STRING(VERSION_MAJOR) "." VERSION_STRING(VERSION_MINOR_1) \
										VERSION_STRING(VERSION_MINOR_2)

#define IAP_FIRMWARE_DATE_STRING     VERSION_STRING(VERSION_YEAR) "." VERSION_STRING(VERSION_MONTH) \
										"." VERSION_STRING(VERSION_DATE)

#endif	// _CHROGEN_LC_UVD_VERSION_H_

