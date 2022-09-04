/*******************************************************************************
 * (c) Copyright 2012-2013 Microsemi SoC Products Group.  All rights reserved.
 *
 *
 * SmartFusion2 In System Programming using UART interface example application to demonstrate the
 * SmartFusion2 In System Programming capabilities.
 * This Application is start up for the demo to copy the ISP appication to eSRAM to execute the code form eSRAM.
 *
 *
 *
 * SVN $Revision:  $
 * SVN $Date:  $
 */

#include "config.h"


int main()
{
	bool isFwUpgrade = true;

	init_system();

	isFwUpgrade = check_fwpdate();
	if(isFwUpgrade) {
		run_iap();
	} else {
		run_application();
	}

	while(1) {}		// execution never comes here

	return 0;
}
