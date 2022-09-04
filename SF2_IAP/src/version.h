#ifndef		_CHROGEN_LC_UVD_VERSION_H_
#define		_CHROGEN_LC_UVD_VERSION_H_

/*
================================================================================================
					< SmartFusion2 IAP( In-Application Programming ) Firmware History >
================================================================================================
Version	Test	Date		Author		Description 
------------------------------------------------------------------------------------------------
v0.90			21.10.05	CJKIM		1. PC�� �����Ͽ� FW Update ��� �߰�.
										2. Unchanged Data �޸𸮿� ���� �ʵ��� ����.
										3. NVM_PAGE_LOCK_ERROR �߻� �� Unlock ����.

v0.99			21.10.15	CJKIM		1. UART RX Interrupt�� ó�� ����.
										2. IAP Memory 16K -> 24 KB �� ����.

v1.00			21.10.18	CJKIM		1. Data Storage ó��  �߰�.
											- ���α׷� ���� �� Version ���� ����.

v1.00			21.10.22	CJKIM		1. Data Storage ó��  �߰�.
											- System, Lamp Time, Wave ���� ���� �߰�. ( �������� ��� ���� ������ )
										2. Serial No Packet ���� ��  Storage�� �ٷ� �����ϵ��� ����.
										3. LoadStorageInfo() ���� ��  3ȸ Retry �߰�.
										4. ClearWaveInfo(), ClearLampTime() �Լ� ���� ����.
										5. ���α׷� ���� Packet ���� ����.
										6. �ҽ� ����

v1.00			21.10.28	CJKIM		1. Memory Map ����.
											- BootLoader 8KB -> 12KB�� ����.
											BootLoader		0x00000000	12KB
											IAP				0x00003000	24KB
											Application		0x00009000	216KB
											Data Storage	0x0003F000	2KB
											Reserved		0x0003F800	2KB

										* FW Release.

v1.00			21.11.08	CJKIM		1. Model ��  ���� / �б� ��� �߰�.

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

