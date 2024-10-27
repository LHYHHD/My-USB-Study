#include "./FATFS/EXFUNS/exfatfs.h"


/*
函数返回FR_OK表示获取成功，返回其他失败
path:磁盘编号
all：返回总空间，单位KB
last：返回剩余空间，单位KB
*/
FRESULT get_FreeDiskSpace(const TCHAR* path,uint32_t * all,uint32_t *last)//获取剩余磁盘空间
{
	FATFS *fs;
	DWORD fre_clust, fre_sect, tot_sect;
	FRESULT temp;
	temp = f_getfree(path, &fre_clust, &fs);
	if(temp==FR_OK)
	{
    tot_sect =( (fs->n_fatent - 2) * fs->csize)/2;//总的 单位Kb
    fre_sect = (fre_clust * fs->csize)/2;//空闲的 单位Kb
		
		*all=tot_sect;	*last=fre_sect;
	}
	return temp;
}


