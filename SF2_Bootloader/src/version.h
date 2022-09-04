#ifndef		_CHROGEN_LC_UVD_VERSION_H_
#define		_CHROGEN_LC_UVD_VERSION_H_

/*
================================================================================================
					< SmartFusion2 BootLoader Firmware History >
================================================================================================
Version	Test	Date		Author		Description 
------------------------------------------------------------------------------------------------
v0.90			21.10.05	CJKIM		1. IAP or Application ���� ��� �߰�.
											- 3�� �� PC ���� ���� ���� ������ Application ����.
            								- 3�� �� PC ���� ���� �� IAP ����.

v1.00			21.10.18	CJKIM		1. Data Storage ó��  �߰�.
											- ���α׷� ���� �� Version ���� ����.

v1.00			21.10.22	CJKIM		1. Data Storage ó��  �߰�.
											- System, Lamp Time, Wave ���� ���� �߰�. ( �������� ��� ���� ������ )
										2. LoadStorageInfo() ���� ��  3ȸ Retry �߰�.
										3. ClearWaveInfo(), ClearLampTime() �Լ� ���� ����.
										4. ���α׷� ���� Packet ���� ����.
										5. �ҽ� ����

v1.00			21.10.28	CJKIM		1. Memory Map ����.
											- BootLoader 8KB -> 12KB�� ����.
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

