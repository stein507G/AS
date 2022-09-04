/*
 * config.h
 *
 *  Created on: 2015. 4. 7.
 *      Author: chanii
 */

#ifndef CONFIG_H_
#define CONFIG_H_


#include <ctype.h>

#include <stdarg.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <string.h>

#include <limits.h>
#include <stdbool.h>


/***************************** CMSIS includes ************************/
#include "m2sxxx.h"
#include "system_m2sxxx.h"
#include "sys_config.h"

/***************************** Driver includes **********************/
#include "mss_uart.h"
#include "mss_watchdog.h"
#include "mss_timer.h"
#include "mss_nvm.h"

/***************************** Application **********************/
#include "debug.h"
#include "protocol.h"
#include "run_program.h"
#include "storage.h"
#include "system.h"
#include "timer.h"
#include "util.h"
#include "version.h"

#endif /* CONFIG_H_ */
