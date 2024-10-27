/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "sd_diskio.h"
#include "w25qxx.h"
#include "./MALLOC/malloc.h"
#include "rtc.h"
#include "./DEBUG/debug.h"

/* Definitions of physical drive number for each drive */
#define DEV_SDCARD		0	/* 设备0，SD卡 */
#define DEV_FLASH		1	/* 设备1，外部flash */





/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
){
	DSTATUS status;
	if(pdrv == DEV_SDCARD){//对SD卡操作
		return SD_Status(pdrv);
		
	}else if(pdrv == DEV_FLASH){//对flash操作
		/* SPI Flash状态检测：读取SPI Flash 设备ID */
		if (NORFLASH_TYPE == W25Q32) {
			/* 设备ID读取结果正确 */
			status &= ~STA_NOINIT;
			
		} else {
			/* 设备ID读取结果错误 */
			status = STA_NOINIT;
		}
	} 
	return status;//驱动器未初始化
}


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
/* 此函数不需要用户手动调用，挂载文件系统时会自动调用 */
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
){
	DSTATUS status;
	if(pdrv == DEV_SDCARD){//对sdcard操作
		return SD_Initialize(pdrv);
		
	}else if(pdrv == DEV_FLASH){//对flash操作
		
		Norflash_Init();
		status &= ~STA_NOINIT;
	} 
	return status;
}




/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
){

	if(pdrv == DEV_SDCARD){
		return SD_Read(pdrv, buff, sector, count);

	}else if(pdrv == DEV_FLASH){
		for(;count>0;count--){										    
			Norflash_Read((uint8_t*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
			sector++;
			buff+=FLASH_SECTOR_SIZE;
		}
		return RES_OK;
	} 
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{

	if(pdrv == DEV_SDCARD){
		return SD_Write(pdrv, buff, sector, count);

	}else if(pdrv == DEV_FLASH){
		for(;count>0;count--){										    
			Norflash_Write((uint8_t*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
			sector++;
			buff+=FLASH_SECTOR_SIZE;
		}

		return RES_OK;
	}
	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
){

	if(pdrv == DEV_SDCARD){
		return SD_Ioctl(pdrv, cmd, buff);
	}else if(pdrv == DEV_FLASH){
		switch(cmd)
	    {
		    case CTRL_SYNC:
				return RES_OK;  
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = FLASH_SECTOR_SIZE;
		        return RES_OK;
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = FLASH_BLOCK_SIZE;
		        return RES_OK;
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = FLASH_SECTOR_COUNT;
		        return RES_OK;
		    default:
		        return RES_PARERR;
	    }
	} 
	return RES_PARERR;
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
//User defined function to give a current time to fatfs module  
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31)                                                                                                                                                                                                                                         
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) 
*/
__weak DWORD get_fattime (void){
	DWORD d=0;
	d = (sys_data.u8Year<<25)+(sys_data.u8Month<<21)+(sys_data.u8Day<<16);
	if(sys_hour_flg==0){//当前RTC时钟为24小时制
		d += (sys_time.u8Hour<<11)+(sys_time.u8Minute<<5)+(sys_time.u8Second);
	}else{//当前RTC时钟为12小时制度
		d += ((sys_time.u8Hour+12*sys_time.u8AmPm)<<11)+(sys_time.u8Minute<<5)+(sys_time.u8Second);
	}
	return d;
}

//动态分配内存
void *ff_memalloc (UINT size){
	return (void*)mymalloc(SRAMIN,size);
}
//释放内存
void ff_memfree (void* mf){
	myfree(SRAMIN,mf);
}

