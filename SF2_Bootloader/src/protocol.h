#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

enum {
	ERROR_NO_ERROR	=0x0,

	ERROR_INVALID_STX = 0x20,
	ERROR_INVALID_CHKSUM,
	ERROR_FLASH_WRITE,
	ERROR_FLASH_VERIFY,
	ERROR_READ_DATA_SIZE,
	ERROR_READ_DATA,

};

#define		PKT_STX		0x02
#define		PKT_ETX		0x03

#define		FLAG_SEND		0x00
#define		FLAG_ENQ		0x05
#define		FLAG_ACK		0x06
#define		FLAG_NAK		0x15

enum {
	PROGRAM_BL,
	PROGRAM_IAP,
	PROGRAM_APP,
};

enum {

	// Host -> Device
	PKT_CMD_RESET_SYSTEM = 0x7000,

	PKT_CMD_SAVE_STORAGE_INFO = 0x7100,
	PKT_CMD_SEND_SERIAL_NO,
	PKT_CMD_SEND_VERSION,


	// FW Update Command
	PKT_CMD_UPDATE_BEGIN = 0x8000,
	PKT_CMD_UPDATE_BASE_ADDR,			//0x8001 , Intel Hex PKT_TYPE_EXT_LINEAR_ADDR
	PKT_CMD_UPDATE_BASE_OFFSET_ADDR,	//0x8002 , Intel Hex PKT_TYPE_EXT_SEG_ADDR
	PKT_CMD_UPDATE_OFFSET_ADDR,			//0x8003 , Intel Hex PKT_TYPE_DATA && Hex Data Length < 16 일 경우 다음 데이터에 offset address 먼저 전송함.
	PKT_CMD_UPDATE_OFFSET_ADDR_2,
	PKT_CMD_UPDATE_DATA,				//0x8004
	PKT_CMD_READ_DATA,
	PKT_CMD_UPDATE_END,

	//  Host <- Device
	PKT_CMD_RUN_PROGRAM = 0x8100,
	PKT_CMD_UPDATE_CHECK,
	PKT_CMD_UPDATE_READY,

	//  Host <-> Device
	PKT_CMD_REQ_SERIAL_NO = 0x9000,
	PKT_CMD_REQ_VERSION,
};

/*
 * STX(1) Cmd(2) Flag(1) Length(2) Data(N) CheckSum(1) ETX(1)
 */

typedef struct
{
	unsigned char	Stx;
	unsigned short	Cmd;
	unsigned char	Flag;
	unsigned short	Length;
} __attribute__((packed)) HEADER_PKT, *PHEADER_PKT;

typedef struct
{
	unsigned char	Stx;
	unsigned short	Cmd;
	unsigned char	Flag;
	unsigned short	Length;
	unsigned char	ChkSum;
	unsigned char	Etx;
} __attribute__((packed)) UPDATE_COMMON_PKT, *PUPDATE_COMMON_PKT;

typedef struct
{
	unsigned char	Stx;
	unsigned short	Cmd;
	unsigned char	Flag;
	unsigned short	Length;
	unsigned int	Data;
	unsigned char	ChkSum;
	unsigned char	Etx;
} __attribute__((packed))  UPDATE_ADDR_PKT, * PUPDATE_ADDR_PKT;

typedef struct
{
	unsigned char	Stx;
	unsigned short	Cmd;
	unsigned char	Flag;
	unsigned short	Length;
	unsigned int	Data;
	unsigned char	ChkSum;
	unsigned char	Etx;
} __attribute__((packed)) UPDATE_RESP_PKT, *PUPDATE_RESP_PKT;

typedef struct
{
	unsigned int	Kind;					// Program Kind : BL (0) , IAP (1) , Application(2)
	unsigned char	Version[32];			// Program Version , String
} __attribute__((packed)) RUN_PROGRAM_T, *PRUN_PROGRAM_T;


typedef struct
{
	unsigned char	Stx;
	unsigned short	Cmd;
	unsigned char	Flag;
	unsigned short	Length;
	unsigned int	Kind;					// Program Kind : BL (0) , IAP (1) , Application(2)
	unsigned char	Version[32];			// Program Version , String
	unsigned char	ChkSum;
	unsigned char	Etx;
} __attribute__((packed)) RUN_PROGRAM_PKT, *PRUN_PROGRAM_PKT;

typedef struct
{
	unsigned char	Stx;
	unsigned short	Cmd;
	unsigned char	Flag;
	unsigned short	Length;
	unsigned char	SerialNo[32];			// Serial No , String
	unsigned char	ChkSum;
	unsigned char	Etx;
} __attribute__((packed)) SERIAL_NO_PKT, * PRUN_SERIAL_NO_PKT;

typedef struct
{
	unsigned int	BL;
	unsigned int	IAP;
	unsigned int	APP;
	unsigned int	FPGA;
} __attribute__((packed)) VERSION_T, *PVERSION_T;

typedef struct
{
	unsigned char	Stx;
	unsigned short	Cmd;
	unsigned char	Flag;
	unsigned short	Length;
	unsigned int	BL;
	unsigned int	IAP;
	unsigned int	APP;
	unsigned int	FPGA;
	unsigned char	ChkSum;
	unsigned char	Etx;
} __attribute__((packed)) VERSION_PKT, * PVERSION_PKT;

typedef struct
{
	unsigned char	Stx;
	unsigned short	Cmd;
	unsigned char	Flag;
	unsigned short	Length;
	unsigned int	Address;
	unsigned int	DataLen;
	unsigned char	ChkSum;
	unsigned char	Etx;
} __attribute__((packed)) READ_DATA_PKT, * PREAD_DATA_PKT;

void uart0_rx_handler(mss_uart_instance_t * this_uart);

bool check_fwpdate();
int send_update_check();
int post_prog_run();

#endif /* __PROTOCOL_H__ */
