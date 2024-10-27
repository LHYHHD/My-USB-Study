#ifndef _SPI_H__
#define _SPI_H__

#include "hc32f4xx_conf.h"
#include "hc32_ll.h"
#include "gpio_bit.h"


#ifdef LL_SPI_ENABLE

#define BSP_SPI_CS_PORT                 (GPIO_PORT_A)
#define BSP_SPI_CS_PIN                  (GPIO_PIN_04)
#define BSP_SPI_CS_ACTIVE()             PAout(4) = 0
#define BSP_SPI_CS_INACTIVE()           PAout(4) = 1

#define BSP_SPI_SCK_PORT                (GPIO_PORT_A)
#define BSP_SPI_SCK_PIN                 (GPIO_PIN_05)
#define BSP_SPI_SCK_PIN_FUNC            (GPIO_FUNC_40)          /*!< SPI1 SCK */

#define BSP_SPI_MOSI_PORT               (GPIO_PORT_A)           /*!< W25Qxx IO0 */
#define BSP_SPI_MOSI_PIN                (GPIO_PIN_07)
#define BSP_SPI_MOSI_PIN_FUNC           (GPIO_FUNC_41)          /*!< SPI1 MOSI */

#define BSP_SPI_MISO_PORT               (GPIO_PORT_A)           /*!< W25Qxx IO1 */
#define BSP_SPI_MISO_PIN                (GPIO_PIN_06)
#define BSP_SPI_MISO_PIN_FUNC           (GPIO_FUNC_42)          /*!< SPI1 MISO */


#define BSP_SPI_UNIT                    CM_SPI1
#define BSP_SPI_PERIPH_CLK              FCG1_PERIPH_SPI1

void spi1_init(void);
uint8_t SPI1_ReadWriteByte(uint8_t TxData);

#endif


#endif

