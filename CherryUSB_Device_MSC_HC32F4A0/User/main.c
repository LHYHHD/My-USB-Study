#include "main.h"
#include "systemClockConfig.h"
#include "delay_systick.h"
//#include "dma1.h"
//#include "tmr2.h"
//#include "tmrA.h"
#include "tmr0.h"
//#include "adc1.h"
//#include "adc2.h"
//#include "dma2.h"

#include "ots.h"
#include "rtc.h"

//#include "sd.h"
#include "uart1.h"
#include "w25qxx.h"
#include "sd_diskio.h"

#include "./DEBUG/debug.h"
#include "./FATFS/EXFUNS/exfatfs.h"
#include "./MALLOC/malloc.h"


volatile uint32_t count_i = 0;//计时

uint8_t retFlash;    /* Return value for Flash */
FATFS FlashFatFS;    /* File system object for Flash logical drive */
FIL FlashFile;       /* File object for Flash */
uint8_t retSD;    /* Return value for Flash */
FATFS SDFatFS;    /* File system object for Flash logical drive */
FIL SDFile;       /* File object for Flash */


int32_t main(void){
	
    /* Register write enable for some required peripherals. */
    LL_PERIPH_WE(LL_PERIPH_ALL);

	systemClockConfig240MHZ();
	delay_init(240);
	
	my_mem_init(SRAMIN);
	
	CountTimer_Tmr0Unit2CHB_Init(TMR0_CLK_DIV256);
	
	uart1_init();//115200
	RTC_Config();
	OtsInit();
	
	/* LED GPIO初始化 */
	stc_gpio_init_t stcGpioInit;
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
	stcGpioInit.u16PullUp = PIN_PU_ON;
    (void)GPIO_Init(GPIO_PORT_B,GPIO_PIN_02,&stcGpioInit);
	
	
	Norflash_Init();
	SD_Initialize(0);
	
	/* 挂载Flash文件系统 */
	retFlash=f_mount(&FlashFatFS,"Flash:",1);//这里要指定你需要在哪个物理磁盘上挂载文件系统
	if(retFlash!=FR_OK){
		if(retFlash==FR_NO_FILESYSTEM){//没有文件系统，那就格式化一个
			debugOut(USER_APP,"Flash no FATFS file system,formating...\r\n");
			BYTE *work_temp = mymalloc(SRAMIN,FLASH_SECTOR_SIZE);//格式化文件系统时，需要一个较大的临时工作区，至少与扇区大小一样大
			MKFS_PARM *opt_temp = mymalloc(SRAMIN,sizeof(MKFS_PARM));
			opt_temp->fmt = FM_FAT|FM_SFD;     // Flash 不支持 FAT32 
			opt_temp->n_fat = 1;          // FAT 表的数量
			opt_temp->align = 0;          // 数据区域对齐（0 表示自动）
			opt_temp->n_root = 0;         // 根目录项数量（0 表示自动）
			opt_temp->au_size = 0;        // 簇大小（0 表示自动）
			retFlash=f_mkfs("Flash:",opt_temp,work_temp,FLASH_SECTOR_SIZE);//指定你需要在哪个物理磁盘上格式化文件系统
			myfree(SRAMIN,work_temp);
			myfree(SRAMIN,opt_temp);//释放内存
			if(retFlash==FR_OK){
				debugOut(USER_APP,"format FATFS file system success in the Flash\r\n");
				//挂载文件系统
				retFlash=f_mount(&FlashFatFS,"Flash:",1);//这里要指定你需要在哪个物理磁盘上挂载文件系统
				if(retFlash!=FR_OK){debugOut(USER_APP,"retSD:%d\r\n",retFlash);}
			}else debugOut(USER_APP,"format FATFS file system error in the Flash：%d\r\n",retFlash);
		}else{
			debugOut(USER_APP,"retSD:%d\r\n",retFlash);
		}
	}else debugOut(USER_APP,"FATFS file system load success in the Flash\r\n");
	uint32_t all=0,last=0;
	retFlash = get_FreeDiskSpace("Flash:",&all,&last);
	debugOut(USER_APP,"Flash all:%d kb , last:%d kb\r\n",all,last);
	
	/* 挂载SD文件系统 */
	retSD=f_mount(&SDFatFS,"SD:",1);//根目录路径会存放到SDPat中，在ffconf.h中设置
	if(retSD!=FR_OK){
		if(retSD==FR_NO_FILESYSTEM){//没有文件系统，那就格式化一个
			debugOut(USER_APP,"SD no FATFS file system,formating...\r\n");
			BYTE *work_temp = mymalloc(SRAMIN,SDCardMustInfo.u32LogBlockSize);//格式化文件系统时，需要一个较大的临时工作区，至少与扇区大小一样大
			MKFS_PARM *opt_temp = mymalloc(SRAMIN,sizeof(MKFS_PARM));
			opt_temp->fmt = FM_FAT32;     // 指定格式化为 FAT32 文件系统
			opt_temp->n_fat = 1;          // FAT 表的数量
			opt_temp->align = 0;          // 数据区域对齐（0 表示自动）
			opt_temp->n_root = 0;         // 根目录项数量（0 表示自动）
			opt_temp->au_size = 0;        // 簇大小（0 表示自动）
			retSD=f_mkfs("SD:",opt_temp,work_temp,SDCardMustInfo.u32LogBlockSize);//格式化文件系统
			myfree(SRAMIN,work_temp);
			myfree(SRAMIN,opt_temp);//释放内存
			if(retSD==FR_OK){debugOut(USER_APP,"format FATFS file system success in the SD\r\n");}
			else debugOut(USER_APP,"format FATFS file system error in the SD\r\n");
		}else{
			debugOut(USER_APP,"retSD:%d\r\n",retSD);
		}
	}else debugOut(USER_APP,"FATFS file system load success in the SD\r\n");
	
	
	retFlash = get_FreeDiskSpace("SD:",&all,&last);
	debugOut(USER_APP,"SD all:%d kb , last:%d kb\r\n",all,last);
	
    /* Register write protected for some required peripherals. */
    LL_PERIPH_WP(LL_PERIPH_ALL);
	
	extern void msc_storage_init(uint8_t busid, uintptr_t reg_base);
	msc_storage_init(0,CM_USBFS_BASE);

    for (;;) {
		delay_ms(100);
		GPIO_TogglePins(GPIO_PORT_B,GPIO_PIN_02);

    }
}
