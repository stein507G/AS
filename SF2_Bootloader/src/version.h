#ifndef		_CHROGEN_LC_UVD_VERSION_H_
#define		_CHROGEN_LC_UVD_VERSION_H_

/*
================================================================================================
					< SmartFusion2 BootLoader Firmware History >
================================================================================================
Version	Test	Date		Author		Description 
------------------------------------------------------------------------------------------------
v0.90			21.10.05	CJKIM		1. IAP or Application 실행 기능 추가.
											- 3초 내 PC 에서 응답 하지 않으면 Application 실행.
            								- 3초 내 PC 에서 응답 시 IAP 실행.

v1.00			21.10.18	CJKIM		1. Data Storage 처리  추가.
											- 프로그램 실행 시 Version 정보 저장.

v1.00			21.10.22	CJKIM		1. Data Storage 처리  추가.
											- System, Lamp Time, Wave 개별 저장 추가. ( 기존에는 모든 영역 저장함 )
										2. LoadStorageInfo() 실패 시  3회 Retry 추가.
										3. ClearWaveInfo(), ClearLampTime() 함수 오류 수정.
										4. 프로그램 실행 Packet 전송 제거.
										5. 소스 정리

v1.00			21.10.28	CJKIM		1. Memory Map 변경.
											- BootLoader 8KB -> 12KB로 변경.
											BootLoader		0x00000000	12KB
											IAP				0x00003000	24KB
											Application		0x00009000	216KB
											Data Storage	0x0003F000	2KB
											Reserved		0x0003F800	2KB
*/

#define VERSION_YEAR    	2021
#define VERSION_MONTH    	10
#define VERSION_DATE    	28

#define VERSION_MAJOR      	1
#define VERSION_MINOR_1    	0
#define VERSION_MINOR_2    	0


#define VERSION_STRING2(x) #x
#define VERSION_STRING(x) VERSION_STRING2(x)

#define BL_FIRMWARE_VERSION   ( (VERSION_MAJOR * 100) + (VERSION_MINOR_1 * 10) + VERSION_MINOR_2)

#define BL_FIRMWARE_VERSION_STRING     VERSION_STRING(VERSION_MAJOR) "." VERSION_STRING(VERSION_MINOR_1) \
										VERSION_STRING(VERSION_MINOR_2)

#define BL_FIRMWARE_DATE_STRING     VERSION_STRING(VERSION_YEAR) "." VERSION_STRING(VERSION_MONTH) \
										"." VERSION_STRING(VERSION_DATE)

#endif	// _CHROGEN_LC_UVD_VERSION_H_

