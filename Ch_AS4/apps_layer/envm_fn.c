#include "config.h"
#include "envm_fn.h"

extern uint32_t LocalTime;

// envm에서 프로그램 실행하면서 저장가능함.
// ram으로 프로그램 옮기지 않아도 됨.
// envm 영역 0x0 ~ 0x3ffff  // 사용되지 않는 영역사용

nvm_status_t MSS_NVM_read (uint8_t * addr, uint8_t * podata, uint32_t  len);

void test_envm()
{
	uint32_t addr,len;
	uint8_t dtype,idata[1024] = "111-test_envm-xx-wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww";
	uint8_t rdata[1024];

	addr = 0x35000;		// envm 영역 0x0 ~ 0x3ffff  // 사용되지 않는 영역사용
	//idata = "----test_envm----";
	len = 1024;		// write : 3.5msec , read 1msec 소요됨
//	len = 256;		// write : 1msec , read 0.3msec 소요됨

	iprintf("1%d \n", LocalTime);
//	NVM_write( (uint32_t *)addr, idata, len, NVM_DO_NOT_LOCK_PAGE);
	NVM_write( addr, idata, len, NVM_DO_NOT_LOCK_PAGE);
	iprintf("2%d \n", LocalTime);
//  status =
	MSS_NVM_read( (uint8_t *)addr, rdata, len );
	iprintf("3%d \n", LocalTime);

	iprintf("##################read_nvm ; [%s] \n", rdata);
		
}



