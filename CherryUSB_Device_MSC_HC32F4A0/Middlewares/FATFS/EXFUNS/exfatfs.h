#ifndef _EXFATFS_H__
#define _EXFATFS_H__

#include "./FATFS/source/ff.h"
//#include "sd_diskio.h"

FRESULT get_FreeDiskSpace(const TCHAR* path,uint32_t * all,uint32_t *last);

#endif

