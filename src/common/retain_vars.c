#include <wut_types.h>

u8 gMode __attribute__((section(".data"))) = 0;
u8 gTarget __attribute__((section(".data"))) = 0;
bool gInstallMiimakerAsked __attribute__((section(".data"))) = false;
bool gInstallMiimakerFinished __attribute__((section(".data"))) = false;
u64 gCurrentTitleId  __attribute__((section(".data"))) = 0;
char gFolderPath[1024][256] __attribute__((section(".data"))) = { "\0" };
