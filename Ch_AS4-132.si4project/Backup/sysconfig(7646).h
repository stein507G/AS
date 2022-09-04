
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


#define FLAG_SERIAL_NO		0
#define FLAG_IP_ADDR			1
#define FLAG_MAC_ADDR			2


#define INFO_COMMON_VER_SER_ENVM_ADDR				0x3F000 	// ChroZen-LC 공통부분 
//	#define FLAG_COMMON_VER_SER_ENVM_ADDR_OFFSET		0xFC 	// 
//	#define USED_ENVM_STORAGE_OFFSET	0xFC	

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

#define INFO_SPECIAL_ENVM_ADDR					0x3F390
	#define INFO_SPECIAL_ENVM_LEN					16

#define INFO_RTC_INIT_SAVE_ADDR					0x3F3B0
	#define INFO_RTC_INIT_SAVE_LEN				4

#define INFO_TEMP_CAL_ENVM_ADDR					0x3F3C0
	#define INFO_TEMP_CAL_ENVM_LEN				20

// ==========================================================================================================
// EEPROM Data 여기에 복사 저장 
// ==========================================================================================================
#define INFO_EEPROM_DATA_ENVM_ADDR			0x3F400		// eeprom_data 를 eNVM영역에 저장하였는지?            0xAA5555AA
	#define INFO_EEPROM_DATA_ENVM_FLAG_ADDR 	0xFC		//	0xAA5555AA

// ==========================================================================================================
	#define SYSTEM_CONFIG_DATA_OFFSET			0x0
	#define USED_TIME_DATA_OFFSET					0x40
	#define HOME_ADJUST_DATA_OFFSET 			0x80
	#define HOME_ADJUST_DATA_OFFSET_2nd 			0xC0

#define ENVM_SYSTEM_CONFIG_DATA_ADDR					(INFO_EEPROM_DATA_ENVM_ADDR + SYSTEM_CONFIG_DATA_OFFSET)
	#define ENVM_SYSTEM_CONFIG_DATA_LEN		48

#define ENVM_USED_TIME_DATA_ADDR					(INFO_EEPROM_DATA_ENVM_ADDR + USED_TIME_DATA_OFFSET)
	#define ENVM_USED_TIME_DATA_LEN				40

#define ENVM_HOME_ADJUST_DATA_ADDR			(INFO_EEPROM_DATA_ENVM_ADDR + HOME_ADJUST_DATA_OFFSET) 	// 0x3F480	43byte
	#define ENVM_HOME_ADJUST_DATA_LEN			44	// 43

#define ENVM_HOME_ADJUST_DATA_ADDR_2nd			(INFO_EEPROM_DATA_ENVM_ADDR + HOME_ADJUST_DATA_OFFSET_2nd) // 0x3F4C0	43byte
	#define ENVM_HOME_ADJUST_DATA_LEN			44	// 43

void cpu_init(void);

void InterruptEnable_Main(void);

void init_Control_port(); // init_CoreGpio();

void init_system();

int rtcTime_Init();

int save_MAC_ADDR_envm();

void make_MACADDR(int NewMAC);
void saveEthernetConfig(int who);
void init_IpAddr(unsigned char sw);
void loadEthernetConfig(int who);

#if 0
void initSystemInform();
void saveSystemInform(int who);
#endif

int isEnvmDataAvailable();
int isFirmwareUpdate();


int save_IPADDR();

int checkSpecialValue();
int save_Special_Envm();
int initSpecialValue();

int save_Storage_INFO_Envm();

#endif /* SYSCONFIG_H_ */

