#include "spi.h"
#include "hc32_ll_fcg.h"
#include "hc32_ll_gpio.h"
#include "hc32_ll_spi.h"

#ifdef LL_SPI_ENABLE

void spi1_init(void){
	stc_gpio_init_t stcGpioInit;
    stc_spi_init_t stcSpiInit;
    stc_spi_delay_t stcSpiDelayCfg;

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDrv = PIN_HIGH_DRV;
    stcGpioInit.u16PinInputType = PIN_IN_TYPE_CMOS;
    (void)GPIO_Init(BSP_SPI_CS_PORT,  BSP_SPI_CS_PIN,  &stcGpioInit);
    (void)GPIO_Init(BSP_SPI_SCK_PORT, BSP_SPI_SCK_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_SPI_MOSI_PORT, BSP_SPI_MOSI_PIN, &stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinState = PIN_STAT_SET;
    (void)GPIO_Init(BSP_SPI_CS_PORT,  BSP_SPI_CS_PIN,  &stcGpioInit);

    GPIO_SetFunc(BSP_SPI_SCK_PORT, BSP_SPI_SCK_PIN, BSP_SPI_SCK_PIN_FUNC);
    GPIO_SetFunc(BSP_SPI_MOSI_PORT, BSP_SPI_MOSI_PIN, BSP_SPI_MOSI_PIN_FUNC);
    GPIO_SetFunc(BSP_SPI_MISO_PORT, BSP_SPI_MISO_PIN, BSP_SPI_MISO_PIN_FUNC);

    /* Clear initialize structure */
    (void)SPI_StructInit(&stcSpiInit);
    (void)SPI_DelayStructInit(&stcSpiDelayCfg);

    /* Configure peripheral clock */
    FCG_Fcg1PeriphClockCmd(BSP_SPI_PERIPH_CLK, ENABLE);

    /* SPI De-initialize */
    (void)SPI_DeInit(BSP_SPI_UNIT);
    /* Configuration SPI structure */
    stcSpiInit.u32WireMode          = SPI_3_WIRE;//三线SPI模式
    stcSpiInit.u32TransMode         = SPI_FULL_DUPLEX;//全双工
    stcSpiInit.u32MasterSlave       = SPI_MASTER;//主机
    stcSpiInit.u32ModeFaultDetect   = SPI_MD_FAULT_DETECT_DISABLE;//关闭模式故障检测
    stcSpiInit.u32Parity            = SPI_PARITY_INVD;//不用奇偶校验
    stcSpiInit.u32SpiMode           = SPI_MD_0;//
    stcSpiInit.u32BaudRatePrescaler = SPI_BR_CLK_DIV4;//PCLK1 8分频
    stcSpiInit.u32DataBits          = SPI_DATA_SIZE_8BIT;//8位数据
    stcSpiInit.u32FirstBit          = SPI_FIRST_MSB;//
    (void)SPI_Init(BSP_SPI_UNIT, &stcSpiInit);

    stcSpiDelayCfg.u32IntervalDelay = SPI_INTERVAL_TIME_8SCK;
    stcSpiDelayCfg.u32ReleaseDelay  = SPI_RELEASE_TIME_8SCK;
    stcSpiDelayCfg.u32SetupDelay    = SPI_SETUP_TIME_1SCK;
    (void)SPI_DelayTimeConfig(BSP_SPI_UNIT, &stcSpiDelayCfg);
    SPI_Cmd(BSP_SPI_UNIT, ENABLE);
}
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
    uint8_t Rxdata;
    SPI_TransReceive(BSP_SPI_UNIT,&TxData,&Rxdata,1, 1000);       
 	return Rxdata;          		    //返回收到的数据		
}
#endif

