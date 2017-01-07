#ifndef COMMON_H
#define	COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "os_defs.h"

#define WUP_GX2_VERSION			"v0.1"

/* WUP Modes */
#define WUP_MODE_MII_MAKER	0
#define WUP_MODE_HBC		1

/* homebrew launcher return codes */
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS                0
#endif
#define EXIT_RELAUNCH_ON_LOAD       0xFFFFFFFD

#ifndef MEM_BASE
#define MEM_BASE                (0x00800000)
#endif

#define ELF_DATA_ADDR           (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x00))
#define ELF_DATA_SIZE           (*(volatile unsigned int*)(MEM_BASE + 0x1300 + 0x04))
#define MAIN_ENTRY_ADDR         (*(volatile unsigned int*)(MEM_BASE + 0x1400 + 0x00))
#define OS_FIRMWARE             (*(volatile unsigned int*)(MEM_BASE + 0x1400 + 0x04))

#define OS_SPECIFICS            ((OsSpecifics*)(MEM_BASE + 0x1500))

#ifdef __cplusplus
}
#endif

#endif	/* COMMON_H */

