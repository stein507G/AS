
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




#define INFO_COMMON_VER_SER_ENVM_ADDR				0x3F000 	// ChroZen-LC 공통부분 
//	#define FLAG_COMMON_VER_SER_ENVM_ADDR_OFFSET		0xFC 	// 
	#define USED_ENVM_STORAGE_OFFSET	0xFC	

//	#define APPVER_COMMON_VER_SER_ENVM_ADDR_OFFSET		8
	#define APP_VER_STORAGE_OFFSET		8	
	#define APP_VER_COMMON_VER_SER_ENVM_LEN						66	// appVer ~ Model[12]

	#define FPGA_VER_STORAGE_OFFSET		12	
	#define SERIOL_NO_STORAGE_OFFSET 	16
	#define INSTALL_DATE_STORAGE_OFFSET		76	
	#define IP_ADDR_STORAGE_OFFSET 	48	
	#define MAC_ADDR_STORAGE_OFFSET 	56		
	#define USED_ENVM_STORAGE_OFFSET 	0xFC 	

#if INFORM_COMMON_STORAGE_USE
// saveFlag : 공통
// 데이터가 저장되어있는 지에 대한 플래그
// LC공통으로 사용하기 위한 - STORAGE_SYS_INFO_T  ---> // Model[12]까지 // UsedENVM 사용
#define INFO_ENVM_SAVE_FLAG_ADDR							0x3F300			
	// 0x3F300 : serial
	// 0x3F301 : ip_addr
	// 0x3F302 : mac_addr  
#define INFO_ENVM_SAVE_FLAG_LEN								3

#define INFO_SERIAL_NO_ENVM_ADDR				(INFO_COMMON_VER_SER_ENVM_ADDR + SERIOL_NO_STORAGE_OFFSET) 	// saveFlag : 0x3F300
	#define INFO_SERIAL_NO_ENVM_LEN				32
#define INFO_INSTALL_DATE_ENVM_ADDR			(INFO_COMMON_VER_SER_ENVM_ADDR + INSTALL_DATE_STORAGE_OFFSET) 	// saveFlag : 0x3F300	
	#define INFO_INSTALL_DATE_ENVM_LEN		4
	// serial(32) + inst Date(4)
#define INFO_IP_ADDR_ENVM_ADDR					(INFO_COMMON_VER_SER_ENVM_ADDR + IP_ADDR_STORAGE_OFFSET)		// saveFlag : 0x3F301
	#define INFO_IP_ADDR_ENVM_LEN					8	// 12  - netmask, port no 고정 
#define INFO_MACADDR_ENVM_ADDR					(INFO_COMMON_VER_SER_ENVM_ADDR + MAC_ADDR_STORAGE_OFFSET)		// saveFlag : 0x3F302
	#define INFO_MACADDR_ENVM_LEN					6
#endif



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

int isEnvmDataAvailable();
int isFirmwareUpdate();

int save_InstallDate_Envm();

int save_IPADDR();


#endif /* SYSCONFIG_H_ */

