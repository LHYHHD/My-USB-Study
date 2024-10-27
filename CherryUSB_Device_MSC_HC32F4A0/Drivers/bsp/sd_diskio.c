#include "sd_diskio.h"
#include "sd.h"
#include "hc32_ll_fcg.h"
#include "hc32_ll_gpio.h"
#include "delay_systick.h"
#include "./MALLOC/malloc.h"
#include "./DEBUG/debug.h"

stc_sd_handle_t SdHandle;
stc_SDCardMustInfo SDCardMustInfo;


//���SD������״̬������Ϊ1������Ϊ0
uint8_t GetSdCardInsertState(void){
	if(GPIO_ReadInputPins(SDIOC_CD_PORT,SDIOC_CD_PIN) == SET){
		return 0;//û�в���SD��
	}
	return 1;
}

//��ʼ���ɹ�����0�����򷵻�����
//SDIOC�����ʱ��Ƶ�ʻ��Զ����ã����ݵĲ���SD�����ڲ��Ĵ��������õ�
int32_t SdCard_Config(void){
	int32_t i32Ret;
	#if	MALLOC_ENABLE
	/* �ٷ���CSD�Ĵ�����������Ľṹ�� */
	stc_sd_card_csd_t *stcSdCardCSD = mymalloc(SRAMIN,sizeof(stc_sd_card_csd_t));
	/* �ٷ���SD��������Ϣ�Ľṹ�� */
	stc_sd_card_info_t *stcSdCardInfo = mymalloc(SRAMIN,sizeof(stc_sd_card_info_t));
	/* �ٷ���CID�Ĵ�����������Ľṹ�� */
	stc_sd_card_cid_t *stcSdCardCID = mymalloc(SRAMIN,sizeof(stc_sd_card_cid_t));
	#else
	stc_sd_card_csd_t stcSdCardCSD;
	stc_sd_card_info_t stcSdCardInfo;
	stc_sd_card_cid_t stcSdCardCID;
	#endif
    /* Enable SDIOC clock */
    FCG_Fcg1PeriphClockCmd(SDIOC_SD_CLK, ENABLE);
	


	 /* SDIOC pins configuration */
    GPIO_SetFunc(SDIOC_CK_PORT,  SDIOC_CK_PIN,  GPIO_FUNC_9);
    GPIO_SetFunc(SDIOC_CMD_PORT, SDIOC_CMD_PIN, GPIO_FUNC_9);
    GPIO_SetFunc(SDIOC_D0_PORT,  SDIOC_D0_PIN,  GPIO_FUNC_9);
    GPIO_SetFunc(SDIOC_D1_PORT,  SDIOC_D1_PIN,  GPIO_FUNC_9);
    GPIO_SetFunc(SDIOC_D2_PORT,  SDIOC_D2_PIN,  GPIO_FUNC_9);
    GPIO_SetFunc(SDIOC_D3_PORT,  SDIOC_D3_PIN,  GPIO_FUNC_9);
	
	/* CD������ų�ʼ��������Ϊ���룬�����������ǿ����Զ���� */
	stc_gpio_init_t stcGpioInit;
	(void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_IN;
    stcGpioInit.u16PinAttr = PIN_ATTR_DIGITAL;
	stcGpioInit.u16PullUp = PIN_PU_OFF;//�ⲿ��47k��������
    (void)GPIO_Init(SDIOC_CD_PORT,SDIOC_CD_PIN,&stcGpioInit);


	
	/* PLCK2Ϊ120MHZ������£���ʼ��SD��ǰ�������Ƚ����С�����ｫ����Ϊ60Mhz */
	//��ΪSD����ʼ��ʱCLK����С�ڵ���400K,PCLK1=120Mhzʱ����ʼ��ʱ�Զ���Ƶ���Ƶ�ʻ��Ǵ���400k���ᵼ�³�ʼ��ʧ��
	CLK_SetClockDiv(CLK_BUS_PCLK1,  CLK_PCLK1_DIV4);//�Ƚ�PCLK1��Ϊϵͳʱ�ӵ�4��Ƶ�õ�60MHz
	//��ʼ��SD��ʱ,SDIOC���Զ���Ƶ(С�ڵ���400k)��ʼ��,������PCLK1Ϊ120Mhz����ô��Ƶ��ʼ����ʧ�ܣ�ֻ���ڳ�ʼ��ǰ����PCLK1�ķ�Ƶϵ������ʼ������ٸ�Ϊ2
	
    /* Configure structure initialization */
    SdHandle.SDIOCx                     = SDIOC_SD_UINT;
    SdHandle.stcSdiocInit.u32Mode       = SDIOC_MD_SD;
    SdHandle.stcSdiocInit.u8CardDetect  = SDIOC_CARD_DETECT_CD_PIN_LVL;//CD���ģʽ
    SdHandle.stcSdiocInit.u8SpeedMode   = SDIOC_SPEED_MD_HIGH;//����SDģʽ������SD�ٶȿ��Դﵽ25Mhz������SD�ٶȿ��Դﵽ50Mhz
	SdHandle.stcSdiocInit.u8BusWidth    = SDIOC_BUS_WIDTH_4BIT;//�ĸ�������
    SdHandle.stcSdiocInit.u16ClockDiv   = SDIOC_CLK_DIV4;//�����Ƶϵ���ڳ�ʼ��SD��ʱ�������ã�ֻ����������ʱ������
    SdHandle.DMAx    = NULL;

    /* Reset and init SDIOC */
	SDIOC_SWReset(SdHandle.SDIOCx, SDIOC_SW_RST_ALL);
	delay_us(5);

	i32Ret = SD_Init(&SdHandle);
	
	if(i32Ret == LL_OK && GetSdCardInsertState()==1){
		#if MALLOC_ENABLE
		SD_GetCardCSD(&SdHandle,stcSdCardCSD);//���ùٷ���������CSD�Ĵ���
		SD_GetCardCID(&SdHandle,stcSdCardCID);//���ùٷ���������CID�Ĵ���
		SD_GetCardInfo(&SdHandle,stcSdCardInfo);//��ȡ������Ϣ
		SDCardMustInfo.CSD_STRUCTURE = stcSdCardCSD->u8CSDStruct;//CSD�Ĵ����汾��0��1.0 SDSC��1��2.0 SDHC/SDXC��2��3.0 SECURED  CSD[127:126]
		SDCardMustInfo.u32BlockNum=stcSdCardInfo->u32BlockNum;//�������
		SDCardMustInfo.u32BlockSize=stcSdCardInfo->u32BlockSize;//��Ĵ�С����λ�ֽ�
		SDCardMustInfo.u32LogBlockNum=stcSdCardInfo->u32LogBlockNum;//�߼�������
		SDCardMustInfo.u32LogBlockSize=stcSdCardInfo->u32LogBlockSize;//�߼����С����λ�ֽ�
		SDCardMustInfo.TRAN_SPEED = stcSdCardCSD->u8MaxBusClkFreq;//0x32���25M,0x5A���50Mhz
		SDCardMustInfo.Year=((stcSdCardCID->u16ManufactDate&0xfff0)>>4);//�������0~255����2000��~2255��
		SDCardMustInfo.Month=(stcSdCardCID->u16ManufactDate&0xf);//�����·�
		#else
		SD_GetCardInfo(&SdHandle,&stcSdCardInfo);
		SD_GetCardCSD(&SdHandle,&stcSdCardCSD);//���ùٷ���������CSD�Ĵ���
		SD_GetCardCID(&SdHandle,&stcSdCardCID);//���ùٷ���������CID�Ĵ���
		SDCardMustInfo.CSD_STRUCTURE = stcSdCardCSD.u8CSDStruct;//CSD�Ĵ����汾��0��1.0 SDSC��1��2.0 SDHC/SDXC��2��3.0 SECURED  CSD[127:126]
		SDCardMustInfo.u32BlockNum=stcSdCardInfo.u32BlockNum;//�������
		SDCardMustInfo.u32BlockSize=stcSdCardInfo.u32BlockSize;//��Ĵ�С����λ�ֽ�
		SDCardMustInfo.u32LogBlockNum=stcSdCardInfo.u32LogBlockNum;//�߼�������
		SDCardMustInfo.u32LogBlockSize=stcSdCardInfo.u32LogBlockSize;//�߼����С����λ�ֽ�
		SDCardMustInfo.TRAN_SPEED = stcSdCardCSD.u8MaxBusClkFreq;//0x32���25M,0x5A���50Mhz
		SDCardMustInfo.Year=((stcSdCardCID.u16ManufactDate&0xfff0)>>4);//�������0~255����2000��~2255��
		SDCardMustInfo.Month=(stcSdCardCID.u16ManufactDate&0xf);//�����·�
		#endif
		SDCardMustInfo.SPEC=(SdHandle.u32SCR[1]&0x00000400)>>7| \
							(SdHandle.u32SCR[1]&0x00008000)>> 13| \
							(SdHandle.u32SCR[1]&0x0F000000)>> 24;//���淶�汾
		if(SDCardMustInfo.TRAN_SPEED == 0x32)//���ֻ��25M
			SDIOC_SetClockDiv(SdHandle.SDIOCx,SDIOC_CLK_DIV8);//��Ϊ15M���������30M��
		delay_us(20);
	}
	//��PCLK1��Ƶϵ���Ļ�2
	CLK_SetClockDiv(CLK_BUS_PCLK1,  CLK_PCLK1_DIV2);
	delay_us(5);
	return i32Ret;
}

/* Block size is 512 bytes */
#define SD_DEFAULT_BLOCK_SIZE (512U)
#define SD_RW_TIMEOUT_TIME     (9000UL)
/* Disk status */
static volatile DSTATUS SdStat = STA_NOINIT;

/**
* @brief Get SD card state.
* @param None
* @retval An en_result_t enumeration value:
* - 1: �������ڴ���
* - 0: û�����ݴ���
*/
static uint8_t SDCard_GetCardTransState(void){
	en_sd_card_state_t enCardState;
	if(LL_OK == SD_GetCardState(&SdHandle, &enCardState)){
		if(SD_CARD_STAT_TRANS == enCardState)
			return 1;
		else{
			return 0;
		}
	}
	return 0;
}

/**
* @brief Gets Disk Status
* @param lun: Not used
* @retval DSTATUS: Operation status
*/
DSTATUS SD_Status(BYTE lun){
	SdStat=STA_NOINIT;
	if(SDCard_GetCardTransState() == 1){
		SdStat &= (DSTATUS)(~(DSTATUS)STA_NOINIT);
	}
	return SdStat;
}


/**
* @brief Initializes a Drive
* @param lun: Not used
* @retval DSTATUS: Operation status
*/
DSTATUS SD_Initialize(BYTE lun){
	
	if(LL_OK == SdCard_Config()){
		return SD_Status(lun);
	}
	return STA_NOINIT;
}


/**
* @brief Reads Sector(s)
* @param lun: Not used
* @param buff: Pointer to data buffer used to store read data
* @param sector: Sector address (LBA)
* @param count: Number of sectors to read (1..128)
* @retval DRESULT: Operation result
*/
DRESULT SD_Read(BYTE lun, BYTE *buff, DWORD sector, UINT count){
	DRESULT res = RES_ERROR;
	en_sd_card_state_t enCardState;
	if(LL_OK==SD_ReadBlocks(&SdHandle,(uint32_t)sector,(uint16_t)count,(uint8_t *)buff,SD_RW_TIMEOUT_TIME)){
		/* Wait until the read operation is finished */
		while(enCardState == SD_CARD_STAT_TRANS){
			SD_GetCardState(&SdHandle, &enCardState);
		}
		res = RES_OK;

	}
	return res;
}

/**
* @brief Writes Sector(s)
* @param lun: Not used
* @param buff: Pointer to data to be written
* @param sector: Sector address (LBA)
* @param count: Number of sectors to write (1..128)
* @retval DRESULT: Operation result
*/
DRESULT SD_Write(BYTE lun, const BYTE *buff, DWORD sector, UINT count){
	DRESULT res = RES_ERROR;
	en_sd_card_state_t enCardState;
	if(LL_OK==SD_WriteBlocks(&SdHandle,(uint32_t)sector,(uint16_t)count,(uint8_t *)buff,SD_RW_TIMEOUT_TIME)){
		/* Wait until the Write operation is finished */
		while(enCardState == SD_CARD_STAT_TRANS){
			SD_GetCardState(&SdHandle, &enCardState);
		}
		res = RES_OK;
	}
	return res;
}

/**
* @brief I/O control operation
* @param lun: Not used
* @param cmd: Control code
* @param buff: Pointer to buffer used to send/receive data
* @retval DRESULT: Operation result
*/
DRESULT SD_Ioctl(BYTE lun, BYTE cmd, void *buff){
	if (0U != (SdStat & (DSTATUS)STA_NOINIT)){
		return RES_NOTRDY;
	}else{
		switch (cmd){
			/* Make sure that no pending write process */
			case CTRL_SYNC :
				return RES_OK;
			/* Get number of sectors on the disk (DWORD) */
			case GET_SECTOR_COUNT :
				*(DWORD*)buff = SDCardMustInfo.u32LogBlockNum;
				return RES_OK;
			/* Get R/W sector size (WORD) */
			case GET_SECTOR_SIZE :
				*(WORD*)buff = (uint16_t)SDCardMustInfo.u32LogBlockSize;
				return RES_OK;
			/* Get erase block size in unit of sector (DWORD) */
			case GET_BLOCK_SIZE :
				*(DWORD*)buff = SDCardMustInfo.u32LogBlockSize / SD_DEFAULT_BLOCK_SIZE;
				return RES_OK;
			default:
				return RES_PARERR;
		}
	}
}


