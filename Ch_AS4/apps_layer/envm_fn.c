#include "config.h"
#include "envm_fn.h"

extern uint32_t LocalTime;

// envm���� ���α׷� �����ϸ鼭 ���尡����.
// ram���� ���α׷� �ű��� �ʾƵ� ��.
// envm ���� 0x0 ~ 0x3ffff  // ������ �ʴ� �������

nvm_status_t MSS_NVM_read (uint8_t * addr, uint8_t * podata, uint32_t  len);

void test_envm()
{
	uint32_t addr,len;
	uint8_t dtype,idata[1024] = "111-test_envm-xx-wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww";
	uint8_t rdata[1024];

	addr = 0x35000;		// envm ���� 0x0 ~ 0x3ffff  // ������ �ʴ� �������
	//idata = "----test_envm----";
	len = 1024;		// write : 3.5msec , read 1msec �ҿ��
//	len = 256;		// write : 1msec , read 0.3msec �ҿ��

	iprintf("1%d \n", LocalTime);
//	NVM_write( (uint32_t *)addr, idata, len, NVM_DO_NOT_LOCK_PAGE);
	NVM_write( addr, idata, len, NVM_DO_NOT_LOCK_PAGE);
	iprintf("2%d \n", LocalTime);
//  status =
	MSS_NVM_read( (uint8_t *)addr, rdata, len );
	iprintf("3%d \n", LocalTime);

	iprintf("##################read_nvm ; [%s] \n", rdata);
		
}



