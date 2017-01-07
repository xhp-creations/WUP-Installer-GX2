/****************************************************************************
 * Copyright (C) 2015
 * by Dimok
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 ***************************************************************************/
#include "exports.h"
#include "os_functions.h"

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! MCP functions
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
EXPORT_DECL(int, MCP_GetLastRawError, void);
EXPORT_DECL(int, IOS_IoctlvAsync, unsigned int fd, unsigned int command, int cnt_in, int cnt_out, void *ioctlv, void *ipc_callback, void *usrdata);

void InitOSFunctionPointers(void)
{
    unsigned int *funcPointer = 0;
    OSDynLoadModule coreinit_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);

    //!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //! MCP functions
    //!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    OS_FIND_EXPORT(coreinit_handle, MCP_GetLastRawError);
    OS_FIND_EXPORT(coreinit_handle, IOS_IoctlvAsync);
}

