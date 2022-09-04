#ifndef _STORAGE_H_
#define _STORAGE_H_

//------------------ [ Memory MAP ] -----------------------
/*
	BootLoader		0x00000000	12KB
	IAP				0x00003000	24KB
	Application		0x00009000	216KB
	Data Storage	0x0003F000	2KB
	Reserved		0x0003F800	2KB
*/
#define ESRAM_IAP_RUN_ADDR 						0x20000000

#define ENVM_BOOTLOADER_IMAGE_ADDR 				0x00000000
#define ENVM_BOOTLOADER_UPDATE_BASE_ADDR 		0x60000000

#define ENVM_IAP_IMAGE_ADDR 					0x60003000
#define ENVM_APP_IMAGE_ADDR 					0x00009000

#define ENVM_DATA_STORAGE_READ_BASE_ADDR 		0x0003F000
#define ENVM_DATA_STORAGE_WRITE_BASE_ADDR 		0x6003F000
//---------------------------------------------------------

#define	COPY_DONE_FRAM_TO_ENVM		0xAA5555AA

// Data Storage Type
enum {
	DS_TYPE_SYSTEM = 1,
	DS_TYPE_LAMP_TIME,
	DS_TYPE_WAVE,
};

// System Info  [128Byte]
typedef struct
{
	unsigned int	BLVer;			// Major(2) Minor(2)
	unsigned int	IAPVer;
	unsigned int	APPVer;
	unsigned int	FPGAVer;
	unsigned char	SerialNo[32];
	unsigned char	IPAddress[4];
	unsigned char	Gateway[4];
	unsigned char	MacAddress[6];
	unsigned char	Model[12];		// Model 명.
	unsigned char	Reserved[50];
	unsigned int 	UsedENVM;	// STORAGE_LAMP_INFO_T : FRAM -> ENVM 복사 완료 or
								// STORAGE_WAVE_INFO_T : FRAM -> ENVM 사용 변경.
} __attribute__((packed))  STORAGE_SYS_INFO_T, *PSTORAGE_SYS_INFO_T;

// Lamp 사용시간 저장  [64Byte]
typedef struct
{
	unsigned int D2LampElapse;
	unsigned int WLampElapse;
	unsigned char	Reserved[56];
} __attribute__((packed)) STORAGE_LAMP_INFO_T, *PSTORAGE_LAMP_INFO_T;

// Calibration 후 파장 저장	 [64Byte]
typedef struct
{
	// 파장 캘리브레이션에서 사용하는 변수들
	unsigned short wave[3];			// 파장별 (0 486 656) Step
	unsigned short GM_wave[3];		// 파장별 (0 486 656) zero Step + Step(wave)

	float fGM_ZeroWaveStep;			// Found Step
	float fGM_486WaveStep;			// Found Step
	float fGM_656WaveStep;			// Found Step
	unsigned char	Reserved[40];
} __attribute__((packed)) STORAGE_WAVE_INFO_T, *PSTORAGE_WAVE_INFO_T;

typedef struct
{
	STORAGE_SYS_INFO_T	SystemInfo;
	STORAGE_LAMP_INFO_T	LampTime;
	STORAGE_WAVE_INFO_T	WaveInfo;
}__attribute__((packed)) STORAGE_INFO_T, *PSTORAGE_INFO_T;

int InitStorageInfo();
PSTORAGE_INFO_T GetStorageInfo();

int LoadStorageInfo();
int SaveStorageInfoALL();
int SaveStorageInfoALL_Param(PSTORAGE_INFO_T pInfo);
int SaveStorageInfo(int type);

// System Info
int GetBLVer();
void SetBLVer(int value);
int GetIAPVer();
void SetIAPVer(int value);
int GetAPPVer();
void SetAPPVer(int value);
int GetFPGAVer();
void SetFPGAVer(int value);

int GetSerialNo(unsigned char *pBuffer);
void SetSerialNo(unsigned char *pBuffer, int size);

int GetIPAddress(unsigned char *pBuffer);
void SetIPAddress(unsigned char *pBuffer, int size);

int GetGateway(unsigned char *pBuffer);
void SetGateway(unsigned char *pBuffer, int size);

int GetMacAddress(unsigned char *pBuffer);
void SetMacAddress(unsigned char *pBuffer, int size);

int GetSerialNo(unsigned char *pBuffer);
void SetSerialNo(unsigned char *pBuffer, int size);

int GetModelName(unsigned char *pBuffer);
void SetModelName(unsigned char *pBuffer, int size);

int GetUseStorageENVM();
void SetUseStorageENVM(int value);

// Lamp 사용시간 저장
int GetD2LampTime();
void SetD2LampTime(int value);
int GetWLampTime();
void SetWLampTime(int value);

int GetLampTime(PSTORAGE_LAMP_INFO_T pValue);
void SetLampTime(PSTORAGE_LAMP_INFO_T pValue, int size);
void CleraLampTime();

// Calibration 후 파장 저장
int GetWaveInfo(PSTORAGE_WAVE_INFO_T pValue);
void SetWaveInfo(PSTORAGE_WAVE_INFO_T pValue, int size);
void ClearWaveInfo();

#endif
