#include "dynamic_libs/os_functions.h"
#include <string.h>
#include <coreinit/title.h>
#include <sysapp/launch.h>
#include "common/retain_vars.h"
#include "common/common.h"
#include "utils/logger.h"
#include "main.h"

int main(int argc, char **argv)
{
	//! do OS (for acquire) and sockets first so we got logging
	InitOSFunctionPointers();
	
//	log_init(WUP_LOGGER_IP);
//	log_print("Starting WUP Installer" WUP_GX2_VERSION "\n");
	
	//! *******************************************************************
	//! *           Check from where our application is started           *
	//! *******************************************************************
	if(!gInstallMiimakerAsked)
	{
		gCurrentTitleId = OSGetTitleID();
		
		if (gCurrentTitleId == 0x000500101004A200 || // mii maker eur
			gCurrentTitleId == 0x000500101004A100 || // mii maker usa
			gCurrentTitleId == 0x000500101004A000)	 // mii maker jpn
			gMode = WUP_MODE_MII_MAKER;
		else				 //0x0005000013374842	 // hbl channel
			gMode = WUP_MODE_HBC;
	}
	
	//! *******************************************************************
	//! *                        Call our Main                            *
	//! *******************************************************************
	Menu_Main();
	
	if(gInstallMiimakerAsked)
	{
		if(!gInstallMiimakerFinished)
		{
			SYSLaunchMenu();
		}
		else
		{
			gInstallMiimakerAsked = false;
			gInstallMiimakerFinished = false;
			
			SYSLaunchTitle(gCurrentTitleId);
		}
		
		log_deinit();
		return EXIT_RELAUNCH_ON_LOAD;
	}
	
	//! *******************************************************************
	//! *                 Jump to homebrew launcher                       *
	//! *******************************************************************
	log_deinit();
	
	return EXIT_SUCCESS;
}
