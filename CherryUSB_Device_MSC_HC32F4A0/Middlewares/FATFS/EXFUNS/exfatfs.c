#include "./FATFS/EXFUNS/exfatfs.h"


/*
��������FR_OK��ʾ��ȡ�ɹ�����������ʧ��
path:���̱��
all�������ܿռ䣬��λKB
last������ʣ��ռ䣬��λKB
*/
FRESULT get_FreeDiskSpace(const TCHAR* path,uint32_t * all,uint32_t *last)//��ȡʣ����̿ռ�
{
	FATFS *fs;
	DWORD fre_clust, fre_sect, tot_sect;
	FRESULT temp;
	temp = f_getfree(path, &fre_clust, &fs);
	if(temp==FR_OK)
	{
    tot_sect =( (fs->n_fatent - 2) * fs->csize)/2;//�ܵ� ��λKb
    fre_sect = (fre_clust * fs->csize)/2;//���е� ��λKb
		
		*all=tot_sect;	*last=fre_sect;
	}
	return temp;
}


