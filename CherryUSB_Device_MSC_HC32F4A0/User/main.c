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


volatile uint32_t count_i = 0;//��ʱ

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
	
	/* LED GPIO��ʼ�� */
	stc_gpio_init_t stcGpioInit;
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
	stcGpioInit.u16PullUp = PIN_PU_ON;
    (void)GPIO_Init(GPIO_PORT_B,GPIO_PIN_02,&stcGpioInit);
	
	
	Norflash_Init();
	SD_Initialize(0);
	
	/* ����Flash�ļ�ϵͳ */
	retFlash=f_mount(&FlashFatFS,"Flash:",1);//����Ҫָ������Ҫ���ĸ���������Ϲ����ļ�ϵͳ
	if(retFlash!=FR_OK){
		if(retFlash==FR_NO_FILESYSTEM){//û���ļ�ϵͳ���Ǿ͸�ʽ��һ��
			debugOut(USER_APP,"Flash no FATFS file system,formating...\r\n");
			BYTE *work_temp = mymalloc(SRAMIN,FLASH_SECTOR_SIZE);//��ʽ���ļ�ϵͳʱ����Ҫһ���ϴ����ʱ��������������������Сһ����
			MKFS_PARM *opt_temp = mymalloc(SRAMIN,sizeof(MKFS_PARM));
			opt_temp->fmt = FM_FAT|FM_SFD;     // Flash ��֧�� FAT32 
			opt_temp->n_fat = 1;          // FAT �������
			opt_temp->align = 0;          // ����������루0 ��ʾ�Զ���
			opt_temp->n_root = 0;         // ��Ŀ¼��������0 ��ʾ�Զ���
			opt_temp->au_size = 0;        // �ش�С��0 ��ʾ�Զ���
			retFlash=f_mkfs("Flash:",opt_temp,work_temp,FLASH_SECTOR_SIZE);//ָ������Ҫ���ĸ���������ϸ�ʽ���ļ�ϵͳ
			myfree(SRAMIN,work_temp);
			myfree(SRAMIN,opt_temp);//�ͷ��ڴ�
			if(retFlash==FR_OK){
				debugOut(USER_APP,"format FATFS file system success in the Flash\r\n");
				//�����ļ�ϵͳ
				retFlash=f_mount(&FlashFatFS,"Flash:",1);//����Ҫָ������Ҫ���ĸ���������Ϲ����ļ�ϵͳ
				if(retFlash!=FR_OK){debugOut(USER_APP,"retSD:%d\r\n",retFlash);}
			}else debugOut(USER_APP,"format FATFS file system error in the Flash��%d\r\n",retFlash);
		}else{
			debugOut(USER_APP,"retSD:%d\r\n",retFlash);
		}
	}else debugOut(USER_APP,"FATFS file system load success in the Flash\r\n");
	uint32_t all=0,last=0;
	retFlash = get_FreeDiskSpace("Flash:",&all,&last);
	debugOut(USER_APP,"Flash all:%d kb , last:%d kb\r\n",all,last);
	
	/* ����SD�ļ�ϵͳ */
	retSD=f_mount(&SDFatFS,"SD:",1);//��Ŀ¼·�����ŵ�SDPat�У���ffconf.h������
	if(retSD!=FR_OK){
		if(retSD==FR_NO_FILESYSTEM){//û���ļ�ϵͳ���Ǿ͸�ʽ��һ��
			debugOut(USER_APP,"SD no FATFS file system,formating...\r\n");
			BYTE *work_temp = mymalloc(SRAMIN,SDCardMustInfo.u32LogBlockSize);//��ʽ���ļ�ϵͳʱ����Ҫһ���ϴ����ʱ��������������������Сһ����
			MKFS_PARM *opt_temp = mymalloc(SRAMIN,sizeof(MKFS_PARM));
			opt_temp->fmt = FM_FAT32;     // ָ����ʽ��Ϊ FAT32 �ļ�ϵͳ
			opt_temp->n_fat = 1;          // FAT �������
			opt_temp->align = 0;          // ����������루0 ��ʾ�Զ���
			opt_temp->n_root = 0;         // ��Ŀ¼��������0 ��ʾ�Զ���
			opt_temp->au_size = 0;        // �ش�С��0 ��ʾ�Զ���
			retSD=f_mkfs("SD:",opt_temp,work_temp,SDCardMustInfo.u32LogBlockSize);//��ʽ���ļ�ϵͳ
			myfree(SRAMIN,work_temp);
			myfree(SRAMIN,opt_temp);//�ͷ��ڴ�
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
