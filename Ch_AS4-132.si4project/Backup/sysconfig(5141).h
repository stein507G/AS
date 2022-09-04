
#ifndef SYSCONFIG_H_
#define SYSCONFIG_H_


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
//	unsigned char	Model[32];		// Model 명.
	unsigned char	Model[12];		// Model 명.
	unsigned char	Reserved[50];
	unsigned int 	UsedENVM;	// STORAGE_LAMP_INFO_PKT : FRAM -> ENVM 복사 완료 or
								// STORAGE_WAVE_INFO_PKT :FRAM -> ENVM 사용 변경.
} __attribute__((packed))  STORAGE_SYS_INFO_T, *PSTORAGE_SYS_INFO_T;


void cpu_init(void);

void InterruptEnable_Main(void);

void init_Control_port(); // init_CoreGpio();

void init_system();

int rtcTime_Init();

void make_MACADDR(int NewMAC);
void saveEthernetConfig(int who);
void init_IpAddr(unsigned char sw);
void loadEthernetConfig(int who);

#if 0
void initSystemInform();
void saveSystemInform(int who);
#endif

void initEnvmData();

#endif /* SYSCONFIG_H_ */

