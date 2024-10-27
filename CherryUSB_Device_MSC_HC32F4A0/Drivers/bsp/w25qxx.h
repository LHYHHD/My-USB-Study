#ifndef _W25QXX_H__
#define _W25QXX_H__

#include "spi.h"
#include "hc32_ll.h"

#ifdef LL_SPI_ENABLE

//W25Xϵ��/Qϵ��оƬ�б�
#define W25Q80 	    0XEF13          /* W25Q80   оƬID */
#define W25Q16 	    0XEF14          /* W25Q16   оƬID */
#define W25Q32 	    0XEF15          /* W25Q32   оƬID */
#define W25Q64 	    0XEF16          /* W25Q64   оƬID */
#define W25Q128	    0XEF17          /* W25Q128  оƬID */
#define NM25Q16     0X6814          /* NM25Q16  оƬID */
#define NM25Q64     0X5216          /* NM25Q64  оƬID */
#define NM25Q128    0X5217          /* NM25Q128 оƬID */
#define BY25Q64     0X6816          /* BY25Q64  оƬID */
#define BY25Q128    0X6817          /* BY25Q128 оƬID */

extern uint16_t NORFLASH_TYPE;//��������ʹ�õ�flashоƬ�ͺ�

#define FLASH_SECTOR_SIZE 	512		//������С,��λ�ֽ�			    
extern uint16_t FLASH_SECTOR_COUNT;
#define FLASH_BLOCK_SIZE   	16     	//ÿ��BLOCK��16������

extern uint8_t W25QXX_BUFFER[4096];

//ָ���
#define FLASH_WriteEnable       0x06
#define FLASH_WriteDisable      0x04
#define FLASH_ReadStatusReg     0x05
#define FLASH_WriteStatusReg    0x01
#define FLASH_ReadData          0x03
#define FLASH_FastReadData      0x0B
#define FLASH_FastReadDual      0x3B
#define FLASH_PageProgram       0x02
#define FLASH_BlockErase        0xD8
#define FLASH_SectorErase       0x20
#define FLASH_ChipErase         0xC7
#define FLASH_PowerDown         0xB9
#define FLASH_ReleasePowerDown  0xAB
#define FLASH_DeviceID          0xAB
#define FLASH_ManufactDeviceID  0x90
#define FLASH_JedecDeviceID     0x9F

void Norflash_Init(void);
uint16_t  Norflash_ReadID(void);  	      //��ȡFLASH ID
uint8_t	 Norflash_ReadSR(void);         //��ȡ״̬�Ĵ���
void Norflash_Write_SR(uint8_t sr);  	  //д״̬�Ĵ���
void Norflash_Write_Enable(void);   //дʹ��
void Norflash_Write_Disable(void);  //д����
void Norflash_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead); //��ȡflash
void Norflash_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite); //д��flash
void Norflash_Erase_Chip(void);    	   //��Ƭ����
void Norflash_Erase_Sector(uint32_t Dst_Addr);//��������
void Norflash_Wait_Busy(void);           //�ȴ�����
void Norflash_PowerDown(void);           //�������ģʽ
void Norflash_WAKEUP(void);			   //����

#endif

#endif

