/*
 * Copyright (c) 2024, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "usbd_core.h"
#include "usbd_msc.h"

#include "w25qxx.h"
#include "sd_diskio.h"

#define MSC_IN_EP  0x81
#define MSC_OUT_EP 0x02

#define USBD_VID           0xFFFF
#define USBD_PID           0xFFFF
#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

#define USB_CONFIG_SIZE (9 + MSC_DESCRIPTOR_LEN)

#ifdef CONFIG_USB_HS
#define MSC_MAX_MPS 512
#else
#define MSC_MAX_MPS 64
#endif


const uint8_t msc_storage_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_1_1, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0200, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x01, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    MSC_DESCRIPTOR_INIT(0x00, MSC_OUT_EP, MSC_IN_EP, MSC_MAX_MPS, 0x02),
    ///////////////////////////////////////
    /// string0 descriptor
    ///////////////////////////////////////
    USB_LANGID_INIT(USBD_LANGID_STRING),
    ///////////////////////////////////////
    /// string1 descriptor
    ///////////////////////////////////////
    0x14,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'C', 0x00,                  /* wcChar0 */
    'h', 0x00,                  /* wcChar1 */
    'e', 0x00,                  /* wcChar2 */
    'r', 0x00,                  /* wcChar3 */
    'r', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    'U', 0x00,                  /* wcChar6 */
    'S', 0x00,                  /* wcChar7 */
    'B', 0x00,                  /* wcChar8 */
    ///////////////////////////////////////
    /// string2 descriptor
    ///////////////////////////////////////
    0x26,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'C', 0x00,                  /* wcChar0 */
    'h', 0x00,                  /* wcChar1 */
    'e', 0x00,                  /* wcChar2 */
    'r', 0x00,                  /* wcChar3 */
    'r', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    'U', 0x00,                  /* wcChar6 */
    'S', 0x00,                  /* wcChar7 */
    'B', 0x00,                  /* wcChar8 */
    ' ', 0x00,                  /* wcChar9 */
    'M', 0x00,                  /* wcChar10 */
    'S', 0x00,                  /* wcChar11 */
    'C', 0x00,                  /* wcChar12 */
    ' ', 0x00,                  /* wcChar13 */
    'D', 0x00,                  /* wcChar14 */
    'E', 0x00,                  /* wcChar15 */
    'M', 0x00,                  /* wcChar16 */
    'O', 0x00,                  /* wcChar17 */
    ///////////////////////////////////////
    /// string3 descriptor
    ///////////////////////////////////////
    0x16,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    '2', 0x00,                  /* wcChar0 */
    '0', 0x00,                  /* wcChar1 */
    '2', 0x00,                  /* wcChar2 */
    '2', 0x00,                  /* wcChar3 */
    '1', 0x00,                  /* wcChar4 */
    '2', 0x00,                  /* wcChar5 */
    '3', 0x00,                  /* wcChar6 */
    '4', 0x00,                  /* wcChar7 */
    '5', 0x00,                  /* wcChar8 */
    '6', 0x00,                  /* wcChar9 */
#ifdef CONFIG_USB_HS
    ///////////////////////////////////////
    /// device qualifier descriptor
    ///////////////////////////////////////
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x00,
    0x00,
#endif
    0x00
};

struct usbd_interface intf0;

/* assume the block device is 512M */
//#define BLOCK_DEV_NAME      "sd0"
//#define BLOCK_SIZE          512U
//#define BLOCK_COUNT         0x1024U * 0x1024U
//static rt_device_t blk_dev = RT_NULL;

static void usbd_event_handler(uint8_t busid, uint8_t event)
{
    switch (event) {
        case USBD_EVENT_RESET:
            break;
        case USBD_EVENT_CONNECTED:
            break;
        case USBD_EVENT_DISCONNECTED:
            break;
        case USBD_EVENT_RESUME:
            break;
        case USBD_EVENT_SUSPEND:
            break;
        case USBD_EVENT_CONFIGURED:
            break;
        case USBD_EVENT_SET_REMOTE_WAKEUP:
            break;
        case USBD_EVENT_CLR_REMOTE_WAKEUP:
            break;

        default:
            break;
    }
}


void usbd_msc_get_cap(uint8_t busid, uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{
	if(lun==0){//Flash
		*block_num = 4*1024*1024/512U;
		*block_size = 512U;
	}else if(lun==1){//SD卡
		*block_num = SDCardMustInfo.u32BlockNum - 1;
		*block_size = SDCardMustInfo.u32BlockSize;
	}
}

//sector:扇区地址
//length:以字为节单位
int usbd_msc_sector_read(uint8_t busid, uint8_t lun, uint32_t sector, uint8_t *buffer, uint32_t length)
{
	if(lun==0){//Flash
		Norflash_Read(buffer,sector*512U,length);
	}else if(lun==1){//SD卡
		SD_Read(lun,buffer,sector,length/512U);
	}
    return 0;
}


//sector:扇区地址
//length:以字为节单位
int usbd_msc_sector_write(uint8_t busid, uint8_t lun, uint32_t sector, uint8_t *buffer, uint32_t length)
{
	if(lun==0){//Flash
		Norflash_Write(buffer,sector*512U,length);
	}else if(lun==1){//SD卡
		SD_Write(lun,buffer,sector,length/512U);
	}
    return 0;
}

void msc_storage_init(uint8_t busid, uintptr_t reg_base)
{
//    rt_err_t res;

//    blk_dev = rt_device_find(BLOCK_DEV_NAME);
//    RT_ASSERT(blk_dev);

//    res = rt_device_open(blk_dev, RT_DEVICE_OFLAG_RDWR);
//    RT_ASSERT(res == RT_EOK);

    usbd_desc_register(busid, msc_storage_descriptor);
    usbd_add_interface(busid, usbd_msc_init_intf(busid, &intf0, MSC_OUT_EP, MSC_IN_EP));

    usbd_initialize(busid, reg_base, usbd_event_handler);
}

void USB_IRQ_Handler(void){
	extern void USBD_IRQHandler(uint8_t busid);
	USBD_IRQHandler(0);
}
/* USBFS Core*/
#define USBF_DP_PORT            (GPIO_PORT_A)
#define USBF_DP_PIN             (GPIO_PIN_12)
#define USBF_DM_PORT            (GPIO_PORT_A)
#define USBF_DM_PIN             (GPIO_PIN_11)
void usb_dc_low_level_init(void){
	LL_PERIPH_WE(LL_PERIPH_ALL);
    stc_gpio_init_t stcGpioCfg;
	//在系统时钟为240MHZ的前提下，USB时钟源选择CLK_USBCLK_SYSCLK_DIV5同样也能达到48MHZ
    //设置USB时钟来源为PLLAR(在systemClockConfig240MHZ()中已设置为48MHZ)
    CLK_SetUSBClockSrc(CLK_USBCLK_PLLXR);
	(void)GPIO_StructInit(&stcGpioCfg);
	stcGpioCfg.u16PinAttr = PIN_ATTR_ANALOG;
	(void)GPIO_Init(USBF_DM_PORT, USBF_DM_PIN, &stcGpioCfg);
	(void)GPIO_Init(USBF_DP_PORT, USBF_DP_PIN, &stcGpioCfg);
//	GPIO_SetFunc(USBF_VBUS_PORT, USBF_VBUS_PIN, GPIO_FUNC_10); /* VBUS */
	FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_USBFS, ENABLE);
	
	stc_irq_signin_config_t stcIrqRegiConf;
    /* Register INT_SRC_USBFS_GLB Int to Vect.No.030 */
//    stcIrqRegiConf.enIRQn = INT030_IRQn;
	stcIrqRegiConf.enIRQn = INT003_IRQn;
    /* Select interrupt function */
    stcIrqRegiConf.enIntSrc = INT_SRC_USBFS_GLB;
	
    /* Callback function */
    stcIrqRegiConf.pfnCallback = &USB_IRQ_Handler;
    /* Registration IRQ */
    (void)INTC_IrqSignIn(&stcIrqRegiConf);
    /* Clear Pending */
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    /* Set priority */
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIO_15);
    /* Enable NVIC */
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
	
	LL_PERIPH_WP(LL_PERIPH_ALL);
}
void usb_dc_low_level_deinit(void){
	LL_PERIPH_WE(LL_PERIPH_ALL);
	FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_USBFS, DISABLE);
	stc_irq_signin_config_t stcIrqRegiConf;
	stcIrqRegiConf.enIRQn = INT003_IRQn;
    stcIrqRegiConf.enIntSrc = INT_SRC_USBFS_GLB;
    stcIrqRegiConf.pfnCallback = &USB_IRQ_Handler;
    (void)INTC_IrqSignIn(&stcIrqRegiConf);
    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIO_15);
    NVIC_DisableIRQ(stcIrqRegiConf.enIRQn);
	LL_PERIPH_WP(LL_PERIPH_ALL);
}
