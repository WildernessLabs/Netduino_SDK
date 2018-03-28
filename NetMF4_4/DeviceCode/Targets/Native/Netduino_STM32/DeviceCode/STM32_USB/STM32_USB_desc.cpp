#include <pal\com\usb\USB.h>
#if defined(PLATFORM_ARM_STM32F4_ANY)
#include "..\stm32f4xx.h"
#elif defined(PLATFORM_ARM_STM32F2_ANY)
#include "..\stm32f2xx.h"
#endif
#include "usbd_netmf_core.h"

#define USBD_VID                      0x22B1          // Wilderness Labs LLC
#define USBD_PID                      0x1000          // Netduino (MFUSB)

#define USBD_LANGID_STRING            USB_LANGID_ENG_US
#define USBD_MANUFACTURER_STRING      "Wilderness Labs LLC"

#if defined(PLATFORM_ARM_Netduino2)
#define USBD_PRODUCT_FS_STRING        "Netduino 2"
#elif defined(PLATFORM_ARM_NetduinoPlus2)
#define USBD_PRODUCT_FS_STRING        "Netduino Plus 2"
#elif defined(PLATFORM_ARM_NetduinoGo)
#define USBD_PRODUCT_FS_STRING        "Netduino Go"
#elif defined(PLATFORM_ARM_Netduino3)
#define USBD_PRODUCT_FS_STRING        "Netduino 3"
#elif defined(PLATFORM_ARM_Netduino3Ethernet)
#define USBD_PRODUCT_FS_STRING        "Netduino 3 Ethernet"
#elif defined(PLATFORM_ARM_Netduino3Wifi)
#define USBD_PRODUCT_FS_STRING        "Netduino 3 Wi-Fi"
#endif
#define USBD_SERIALNUMBER_FS_STRING   "000000000000"  // TODO: Unique Device ID

#if defined(PLATFORM_ARM_Netduino2)
#define USBD_CONFIGURATION_FS_STRING  "Netduino2"
#elif defined(PLATFORM_ARM_NetduinoPlus2)
#define USBD_CONFIGURATION_FS_STRING  "NetduinoPlus2"
#elif defined(PLATFORM_ARM_NetduinoGo)
#define USBD_CONFIGURATION_FS_STRING  "NetduinoGo"
#elif defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
#define USBD_CONFIGURATION_FS_STRING  "Netduino3"
#endif
#define USBD_INTERFACE_FS_STRING      "Netduino"


/* USB Standard Device Descriptor */
UINT8 USBD_DeviceDesc[USB_SIZ_DEVICE_DESC] =
{
    0x12,                       /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType */
    0x10,                       /* bcdUSB */
    0x01,
    0x00,                       /* bDeviceClass */
    0x00,                       /* bDeviceSubClass */
    0x00,                       /* bDeviceProtocol */
    USB_OTG_MAX_EP0_SIZE,       /* bMaxPacketSize */
    LOBYTE(USBD_VID),           /* idVendor */
    HIBYTE(USBD_VID),           /* idVendor */
    LOBYTE(USBD_PID),           /* idProduct */
    HIBYTE(USBD_PID),           /* idProduct */
    0x00,                       /* bcdDevice */
    0x01,
    USBD_IDX_MFC_STR,           /* iManufacturer */
    USBD_IDX_PRODUCT_STR,       /* iProduct */
    USBD_IDX_SERIAL_STR,        /* iSerialNumber */
    USBD_CFG_MAX_NUM            /* bNumConfigurations */
};

/* USB Standard Device Descriptor */
UINT8 USBD_LangIDDesc[USB_SIZ_STRING_LANGID] =
{
  USB_SIZ_STRING_LANGID,
  USB_STRING_DESCRIPTOR_TYPE,
  LOBYTE(USBD_LANGID_STRING),
  HIBYTE(USBD_LANGID_STRING),
};

/////////////////////////////////////////////////////////////////////////////
// Descriptor callbacks

UINT8 *     USBD_USR_DeviceDescriptor( UINT8 speed , UINT16 *length);
UINT8 *     USBD_USR_LangIDStrDescriptor( UINT8 speed , UINT16 *length);
UINT8 *     USBD_USR_ManufacturerStrDescriptor ( UINT8 speed , UINT16 *length);
UINT8 *     USBD_USR_ProductStrDescriptor ( UINT8 speed , UINT16 *length);
UINT8 *     USBD_USR_SerialStrDescriptor( UINT8 speed , UINT16 *length);
UINT8 *     USBD_USR_ConfigStrDescriptor( UINT8 speed , UINT16 *length);
UINT8 *     USBD_USR_InterfaceStrDescriptor( UINT8 speed , UINT16 *length);

USBD_DEVICE USR_desc =
{
  USBD_USR_DeviceDescriptor,
  USBD_USR_LangIDStrDescriptor,
  USBD_USR_ManufacturerStrDescriptor,
  USBD_USR_ProductStrDescriptor,
  USBD_USR_SerialStrDescriptor,
  USBD_USR_ConfigStrDescriptor,
  USBD_USR_InterfaceStrDescriptor,
};

UINT8* USBD_USR_DeviceDescriptor(UINT8 speed, UINT16 *length)
{
  *length = sizeof(USBD_DeviceDesc);
  
  return USBD_DeviceDesc;
}

UINT8* USBD_USR_LangIDStrDescriptor(UINT8 speed, UINT16 *length)
{
    *length = sizeof(USBD_LangIDDesc);
    
    return USBD_LangIDDesc;
}

UINT8* USBD_USR_ProductStrDescriptor(UINT8 speed, UINT16 *length)
{
    STM32_USB_GetString((UINT8 *)USBD_PRODUCT_FS_STRING, USBD_StrDesc, length);

    return USBD_StrDesc;
}

UINT8* USBD_USR_ManufacturerStrDescriptor(UINT8 speed, UINT16 *length)
{
    STM32_USB_GetString((UINT8 *)USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
  
    return USBD_StrDesc;
}

UINT8* USBD_USR_SerialStrDescriptor(UINT8 speed, UINT16 *length)
{
    STM32_USB_GetString((UINT8 *)USBD_SERIALNUMBER_FS_STRING, USBD_StrDesc, length);

    return USBD_StrDesc;
}

UINT8* USBD_USR_ConfigStrDescriptor(UINT8 speed, UINT16 *length)
{
    STM32_USB_GetString((UINT8 *)USBD_CONFIGURATION_FS_STRING, USBD_StrDesc, length);
    
    return USBD_StrDesc;
}

UINT8* USBD_USR_InterfaceStrDescriptor(UINT8 speed, UINT16 *length)
{
    STM32_USB_GetString((UINT8 *)USBD_INTERFACE_FS_STRING, USBD_StrDesc, length);
    
    return USBD_StrDesc;
}

