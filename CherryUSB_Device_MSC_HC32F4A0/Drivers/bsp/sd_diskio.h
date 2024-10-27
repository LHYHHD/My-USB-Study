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
//以下是天空星板子上的SD Card引脚连接
/* CK = PC12 */
#define SDIOC_CK_PORT                   (GPIO_PORT_C)//引脚选择可以去数据手册的Pin function table或者HXSH HXCode软件
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
#define SDIOC_CD_PORT                   (GPIO_PORT_D)//这个检测引脚是可以自定义的
#define SDIOC_CD_PIN                    (GPIO_PIN_03)




/*草稿：
//CSD和CID寄存器，sd.h中有现成的解析函数(SD_GetCardCSD和SD_GetCardCID）
//SCR寄存器，需要我们自己根据SdHandle.u32SCR来解析
//CSD.CSD_STRUCTURE CSD版本决定了是SDSC还是SDHC/SDXC还是SECURED [127:126]
//CSD.WRITE_BL_LEN [25:22] 最大15，为9，就代表2^9=512
//CSD.READ_BL_LEN [83:80] 为9，就代表2^9
//CSD.C_SIZE 2.0版本[69:48] 1.0版本[73:62]
//CSD.TRAN_SPEED 最大传输速率 [103:96]
//CID.MDT 出场年月 [19:8] 低4位为月，高8位+2000为年
//CID.PRV 产品版本
//SCR.SPEC 卡规范版本 [59:56]
//SCR.SECURITY 支持的安全协议 [54:52]
1.1总容量(由CSD register中，不同的CSD寄存器版本，计算方式不同，1.0版本 容量等于(C_SIZE+1)*C_SIZEMULT*READ_BL_LEN)，2.0版本容量等于C_SIZE*READ_BL_LEN
1.2CSD寄存器版本(CSD register bit[127:126]，CSD_STRUCTURE，描述 CSD register的版本，0x0为 Version 1.0对应 SDSC，0x1为 Version 2.0对应 SDHC/SDXC，0x2为 Version 3.0对应 SDUC。)
1.3SCR寄存器中的SD卡规范版本(SCR register bit[59:56]（SD_SPEC）、bit[47]（SD_SPEC3）、bit[42]（SD_SPEC4）、bit[41:38]（SD_SPECX），可以用于判断 SD卡的版本，不过常用的 SDXC/UHS-I mode的卡只需用上 SD_SPEC和 SD_SPEC3。)
1.4是否支持安全算法(SCR register bit[54:52] 0：无,1：安全协议1.0安全说明版本0.96,2：安全协议2.0安全说明版本1.0-1.01)
1.5出厂年月
1.6逻辑块大小

*/
//对用户来说SD卡必须的信息
typedef struct {
	uint8_t CSD_STRUCTURE;//CSD寄存器版本，0：1.0 SDSC，1：2.0 SDHC/SDXC，2：3.0 SECURED  CSD[127:126]
	uint32_t u32BlockNum;//块的数量
	uint32_t u32BlockSize;//块的大小，单位字节
	uint32_t u32LogBlockNum;//逻辑块数量
	uint32_t u32LogBlockSize;//逻辑块大小，单位字节
	uint8_t TRAN_SPEED;//0x32最高25M,0x5A最高50Mhz
	uint8_t Year;//出厂年份0就是2000年
	uint8_t Month;//出厂月份
	uint8_t SPEC;//卡规范版本，低四位有效 0000->1.0x，0001->1.10，0010->2.0，0110->3.0x，1110->4.xx
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


