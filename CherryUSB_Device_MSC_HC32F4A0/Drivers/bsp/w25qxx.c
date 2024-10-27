#include "w25qxx.h"
#include "delay_systick.h"
#include "./MALLOC/malloc.h"

#ifdef LL_SPI_ENABLE




uint16_t NORFLASH_TYPE = W25Q32;
uint16_t FLASH_SECTOR_COUNT=2048*4;	//��������

//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25X16
//����Ϊ2M�ֽ�,����32��Block,512��Sector

//��ʼ��w25q64��CS��
void Norflash_Init(void){
	spi1_init();
    NORFLASH_TYPE = Norflash_ReadID();	      //��ȡFLASH ID.
}

//��ȡSPI_FLASH��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
uint8_t Norflash_ReadSR(void)
{
    uint8_t byte = 0;
    BSP_SPI_CS_ACTIVE();                          //ʹ������
    SPI1_ReadWriteByte(FLASH_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������
    byte = SPI1_ReadWriteByte(0Xff);           //��ȡһ���ֽ�
    BSP_SPI_CS_INACTIVE();                          //ȡ��Ƭѡ
    return byte;
}
//дSPI_FLASH״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void Norflash_Write_SR(uint8_t sr)
{
    BSP_SPI_CS_ACTIVE();                          //ʹ������
    SPI1_ReadWriteByte(FLASH_WriteStatusReg);   //����дȡ״̬�Ĵ�������
    SPI1_ReadWriteByte(sr);               //д��һ���ֽ�
    BSP_SPI_CS_INACTIVE();                          //ȡ��Ƭѡ
}
//SPI_FLASHдʹ��
//��WEL��λ
void Norflash_Write_Enable(void)
{
    BSP_SPI_CS_ACTIVE();                          //ʹ������
    SPI1_ReadWriteByte(FLASH_WriteEnable);      //����дʹ��
    BSP_SPI_CS_INACTIVE();                          //ȡ��Ƭѡ
}
//SPI_FLASHд��ֹ
//��WEL����
void Norflash_Write_Disable(void)
{
    BSP_SPI_CS_ACTIVE();                          //ʹ������
    SPI1_ReadWriteByte(FLASH_WriteDisable);     //����д��ָֹ��
    BSP_SPI_CS_INACTIVE();                          //ȡ��Ƭѡ
}
//��ȡоƬID W25X16��ID:0XEF14
uint16_t Norflash_ReadID(void)
{
    uint16_t Temp = 0;
    BSP_SPI_CS_ACTIVE();
    SPI1_ReadWriteByte(0x90);//���Ͷ�ȡID����
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    Temp |= SPI1_ReadWriteByte(0xFF) << 8;
    Temp |= SPI1_ReadWriteByte(0xFF);
    BSP_SPI_CS_INACTIVE();
    return Temp;
}
//��ȡSPI FLASH
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void Norflash_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    uint16_t i;
    BSP_SPI_CS_ACTIVE();                          //ʹ������
    SPI1_ReadWriteByte(FLASH_ReadData);         //���Ͷ�ȡ����
    SPI1_ReadWriteByte((uint8_t)((ReadAddr) >> 16)); //����24bit��ַ
    SPI1_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
    SPI1_ReadWriteByte((uint8_t)ReadAddr);

    for (i = 0; i < NumByteToRead; i++)
    {
        pBuffer[i] = SPI1_ReadWriteByte(0XFF); //ѭ������
    }

    BSP_SPI_CS_INACTIVE();                          //ȡ��Ƭѡ
}
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
void Norflash_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t i;
    Norflash_Write_Enable();                  //SET WEL
    BSP_SPI_CS_ACTIVE();                          //ʹ������
    SPI1_ReadWriteByte(FLASH_PageProgram);      //����дҳ����
    SPI1_ReadWriteByte((uint8_t)((WriteAddr) >> 16)); //����24bit��ַ
    SPI1_ReadWriteByte((uint8_t)((WriteAddr) >> 8));
    SPI1_ReadWriteByte((uint8_t)WriteAddr);

    for (i = 0; i < NumByteToWrite; i++)SPI1_ReadWriteByte(pBuffer[i]); //ѭ��д��

    BSP_SPI_CS_INACTIVE();                          //ȡ��Ƭѡ
    Norflash_Wait_Busy();					   //�ȴ�д�����
}
//�޼���дSPI FLASH
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ����
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void Norflash_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain = 256 - WriteAddr % 256; //��ҳʣ����ֽ���

    if (NumByteToWrite <= pageremain)pageremain = NumByteToWrite; //������256���ֽ�

    while (1)
    {
        Norflash_Write_Page(pBuffer, WriteAddr, pageremain);

        if (NumByteToWrite == pageremain)break; //д�������
        else //NumByteToWrite>pageremain
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain;			 //��ȥ�Ѿ�д���˵��ֽ���

            if (NumByteToWrite > 256)pageremain = 256; //һ�ο���д��256���ֽ�
            else pageremain = NumByteToWrite; 	 //����256���ֽ���
        }
    };
}
//дSPI FLASH
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//�ڴ���������£�flash��Ҫ���ڴ������Ϊ�ٶȻ����
#ifndef SRAMIN
uint8_t W25QXX_BUFFER[4096];
#endif
void Norflash_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
#ifdef SRAMIN
		uint8_t *W25QXX_BUFFER = mymalloc(SRAMIN,4096);
#endif
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
	//secpos = WriteAddr / 4096; //������ַ 0~511 for w25x16
    secpos = WriteAddr >> 12; //������ַ 0~511 for w25x16
    secoff = WriteAddr % 4096; //�������ڵ�ƫ��
    secremain = 4096 - secoff; //����ʣ��ռ��С

    if (NumByteToWrite <= secremain)secremain = NumByteToWrite; //������4096���ֽ�

    while (1)
    {
        Norflash_Read(W25QXX_BUFFER, secpos * 4096, 4096); //������������������

        for (i = 0; i < secremain; i++) //У������
        {
            if (W25QXX_BUFFER[secoff + i] != 0XFF)break; //��Ҫ����
        }

        if (i < secremain) //��Ҫ����
        {
            Norflash_Erase_Sector(secpos);//�����������

            for (i = 0; i < secremain; i++)	 //����
            {
                W25QXX_BUFFER[i + secoff] = pBuffer[i];
            }

            Norflash_Write_NoCheck(W25QXX_BUFFER, secpos * 4096, 4096); //д����������

        }
        else Norflash_Write_NoCheck(pBuffer, WriteAddr, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.

        if (NumByteToWrite == secremain)break; //д�������
        else//д��δ����
        {
            secpos++;//������ַ��1
            secoff = 0; //ƫ��λ��Ϊ0

            pBuffer += secremain; //ָ��ƫ��
            WriteAddr += secremain; //д��ַƫ��
            NumByteToWrite -= secremain;				//�ֽ����ݼ�

            if (NumByteToWrite > 4096)secremain = 4096;	//��һ����������д����
            else secremain = NumByteToWrite;			//��һ����������д����
        }
    }
#ifdef MALLOC_ENABLE
		myfree(SRAMIN,W25QXX_BUFFER);
#endif
}


//��������оƬ
//��Ƭ����ʱ��:
//W25X16:25s
//W25X32:40s
//W25X64:40s
//�ȴ�ʱ�䳬��...
void Norflash_Erase_Chip(void)
{
    Norflash_Write_Enable();                  //SET WEL
    Norflash_Wait_Busy();
    BSP_SPI_CS_ACTIVE();                          //ʹ������
    SPI1_ReadWriteByte(FLASH_ChipErase);        //����Ƭ��������
    BSP_SPI_CS_INACTIVE();//ȡ��Ƭѡ
    Norflash_Wait_Busy();   				   //�ȴ�оƬ��������
}
//����һ������
//Dst_Addr:������ַ 0~511 for w25x16
//����һ��ɽ��������ʱ��:150ms
void Norflash_Erase_Sector(uint32_t Dst_Addr)
{
    Dst_Addr *= 4096;
    Norflash_Write_Enable();                  //SET WEL
    Norflash_Wait_Busy();
    BSP_SPI_CS_ACTIVE();                          //ʹ������
    SPI1_ReadWriteByte(FLASH_SectorErase);      //������������ָ��
    SPI1_ReadWriteByte((uint8_t)((Dst_Addr) >> 16)); //����24bit��ַ
    SPI1_ReadWriteByte((uint8_t)((Dst_Addr) >> 8));
    SPI1_ReadWriteByte((uint8_t)Dst_Addr);
    BSP_SPI_CS_INACTIVE();                          //ȡ��Ƭѡ
    Norflash_Wait_Busy();   				   //�ȴ��������
}
//�ȴ�����
void Norflash_Wait_Busy(void)
{
    while ((Norflash_ReadSR() & 0x01) == 0x01); // �ȴ�BUSYλ���
}
//�������ģʽ
void Norflash_PowerDown(void)
{
    BSP_SPI_CS_ACTIVE();                          //ʹ������
    SPI1_ReadWriteByte(FLASH_PowerDown);        //���͵�������
    BSP_SPI_CS_INACTIVE();                          //ȡ��Ƭѡ
    delay_ms(1);                               //�ȴ�TPD
}
//����
void Norflash_WAKEUP(void)
{
    BSP_SPI_CS_ACTIVE();                          //ʹ������
    SPI1_ReadWriteByte(FLASH_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB
    BSP_SPI_CS_INACTIVE();                          //ȡ��Ƭѡ
		delay_ms(1);                                //�ȴ�TRES1
}

#endif
