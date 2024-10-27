#ifndef _SD_DISKIO_H__
#define _SD_DISKIO_H__

#include "hc32_ll.h"
#include "sd.h"
#include "./FATFS/source/ff.h"
#include "./FATFS/source/diskio.h"

/* SDIOC configuration define */
#define SDIOC_SD_UINT                   (CM_SDIOC1)
#define SDIOC_SD_CLK                    (FCG1_PERIPH_SDIOC1)
//#define SIDOC_SD_INT_SRC                (INT_SRC_SDIOC1_SD)
//#define SIDOC_SD_IRQ                    (INT006_IRQn)
//����������ǰ����ϵ�SD Card��������
/* CK = PC12 */
#define SDIOC_CK_PORT                   (GPIO_PORT_C)//����ѡ�����ȥ�����ֲ��Pin function table����HXSH HXCode���
#define SDIOC_CK_PIN                    (GPIO_PIN_12)
/* CMD = PD02 */
#define SDIOC_CMD_PORT                  (GPIO_PORT_D)
#define SDIOC_CMD_PIN                   (GPIO_PIN_02)
/* D0 = PC08 */
#define SDIOC_D0_PORT                   (GPIO_PORT_C)
#define SDIOC_D0_PIN                    (GPIO_PIN_08)
/* D1 = PC09 */
#define SDIOC_D1_PORT                   (GPIO_PORT_C)
#define SDIOC_D1_PIN                    (GPIO_PIN_09)
/* D2 = PC10 */
#define SDIOC_D2_PORT                   (GPIO_PORT_C)
#define SDIOC_D2_PIN                    (GPIO_PIN_10)
/* D3 = PC11 */
#define SDIOC_D3_PORT                   (GPIO_PORT_C)
#define SDIOC_D3_PIN                    (GPIO_PIN_11)
/* CD = PD03 */
#define SDIOC_CD_PORT                   (GPIO_PORT_D)//�����������ǿ����Զ����
#define SDIOC_CD_PIN                    (GPIO_PIN_03)




/*�ݸ壺
//CSD��CID�Ĵ�����sd.h�����ֳɵĽ�������(SD_GetCardCSD��SD_GetCardCID��
//SCR�Ĵ�������Ҫ�����Լ�����SdHandle.u32SCR������
//CSD.CSD_STRUCTURE CSD�汾��������SDSC����SDHC/SDXC����SECURED [127:126]
//CSD.WRITE_BL_LEN [25:22] ���15��Ϊ9���ʹ���2^9=512
//CSD.READ_BL_LEN [83:80] Ϊ9���ʹ���2^9
//CSD.C_SIZE 2.0�汾[69:48] 1.0�汾[73:62]
//CSD.TRAN_SPEED ��������� [103:96]
//CID.MDT �������� [19:8] ��4λΪ�£���8λ+2000Ϊ��
//CID.PRV ��Ʒ�汾
//SCR.SPEC ���淶�汾 [59:56]
//SCR.SECURITY ֧�ֵİ�ȫЭ�� [54:52]
1.1������(��CSD register�У���ͬ��CSD�Ĵ����汾�����㷽ʽ��ͬ��1.0�汾 ��������(C_SIZE+1)*C_SIZEMULT*READ_BL_LEN)��2.0�汾��������C_SIZE*READ_BL_LEN
1.2CSD�Ĵ����汾(CSD register bit[127:126]��CSD_STRUCTURE������ CSD register�İ汾��0x0Ϊ Version 1.0��Ӧ SDSC��0x1Ϊ Version 2.0��Ӧ SDHC/SDXC��0x2Ϊ Version 3.0��Ӧ SDUC��)
1.3SCR�Ĵ����е�SD���淶�汾(SCR register bit[59:56]��SD_SPEC����bit[47]��SD_SPEC3����bit[42]��SD_SPEC4����bit[41:38]��SD_SPECX�������������ж� SD���İ汾���������õ� SDXC/UHS-I mode�Ŀ�ֻ������ SD_SPEC�� SD_SPEC3��)
1.4�Ƿ�֧�ְ�ȫ�㷨(SCR register bit[54:52] 0����,1����ȫЭ��1.0��ȫ˵���汾0.96,2����ȫЭ��2.0��ȫ˵���汾1.0-1.01)
1.5��������
1.6�߼����С

*/
//���û���˵SD���������Ϣ
typedef struct {
	uint8_t CSD_STRUCTURE;//CSD�Ĵ����汾��0��1.0 SDSC��1��2.0 SDHC/SDXC��2��3.0 SECURED  CSD[127:126]
	uint32_t u32BlockNum;//�������
	uint32_t u32BlockSize;//��Ĵ�С����λ�ֽ�
	uint32_t u32LogBlockNum;//�߼�������
	uint32_t u32LogBlockSize;//�߼����С����λ�ֽ�
	uint8_t TRAN_SPEED;//0x32���25M,0x5A���50Mhz
	uint8_t Year;//�������0����2000��
	uint8_t Month;//�����·�
	uint8_t SPEC;//���淶�汾������λ��Ч 0000->1.0x��0001->1.10��0010->2.0��0110->3.0x��1110->4.xx
}stc_SDCardMustInfo;

extern stc_sd_handle_t SdHandle;
extern stc_SDCardMustInfo SDCardMustInfo;

uint8_t GetSdCardInsertState(void);
int32_t SdCard_Config(void);

DSTATUS SD_Status(BYTE lun);
DSTATUS SD_Initialize(BYTE lun);
DRESULT SD_Read(BYTE lun, BYTE *buff, DWORD sector, UINT count);
DRESULT SD_Write(BYTE lun, const BYTE *buff, DWORD sector, UINT count);
DRESULT SD_Ioctl(BYTE lun, BYTE cmd, void *buff);

#endif


