/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */

#ifndef _SH7264__USB_H_1
#define _SH7264__USB_H_1   1

#define USB_MAX_EP         8
#define USB_MAX_LOGICAL_EP 11
#define USB_DEVICE_ADDR      4

#define USB_REG_BASE_ADDR                           0xFFFFC000
#define USB_OFFSET_CFIFO                            0x14
#define USB_OFFSET_D0FIFO                           0x18
#define USB_OFFSET_D1FIFO                           0x1C
#define USB_OFFSET_CFIFOSEL                         0x20
#define USB_OFFSET_D0FIFOSEL                        0x28
#define USB_OFFSET_D1FIFOSEL                        0x2C
#define USB_OFFSET_CFIFOCTR                         0x22
#define USB_OFFSET_D0FIFOCTR                        0x2A
#define USB_OFFSET_D1FIFOCTR                        0x2E
#define USB_OFFSET_INTENB0                          0x30
#define USB_OFFSET_INTENB1                          0x32
#define USB_OFFSET_BRDYENB                          0x36
#define USB_OFFSET_NRDYENB                          0x38
#define USB_OFFSET_BEMPENB                          0x3A
#define USB_OFFSET_INTSTS0                          0x40
#define USB_OFFSET_BRDYSTS                          0x46
#define USB_OFFSET_NRDYSTS                          0x48
#define USB_OFFSET_BEMPSTS                          0x4A
#define USB_OFFSET_DCPCFG                           0x5C
#define USB_OFFSET_DCPMAXP                          0x5E
#define USB_OFFSET_DCPCTR                           0x60
#define USB_OFFSET_PIPESEL                          0x64
#define USB_OFFSET_PIPECFG                          0x68
#define USB_OFFSET_PIPEBUF                          0x6A
#define USB_OFFSET_PIPEMAXP                         0x6C
#define USB_OFFSET_PIPEPERI                         0x6E
#define USB_OFFSET_PIPE1CTR                         0x70
#define USB_OFFSET_PIPE5CTR                         0x78
#define USB_OFFSET_DEVADD0                          0xD0

#define USB_REG_BIT_MBW                         0x0C00   
#define USB_VAL_MBW_16                          0x0400
#define USB_REG_BIT_BIGEND                      0x0100
#define USB_REG_BIT_ISEL                        0x0020
#define USB_REG_BIT_CURPIPE                     0x000F
#define USB_VAL_CURPIPE_PIPE0                   0x0000
#define USB_REG_BIT_BVAL                        0x8000
#define USB_REG_BIT_BCLR                        0x4000
#define USB_REG_BIT_FRDY                        0x2000
#define USB_REG_BIT_BEMPE                       0x0400
#define USB_REG_BIT_NRDYE                       0x0200
#define USB_REG_BIT_BRDYE                       0x0100
#define USB_REG_BIT_BEMP                        0x0400
#define USB_REG_BIT_NRDY                        0x0200
#define USB_REG_BIT_BRDY                        0x0100
#define USB_REG_BIT_BCHG                        0x4000
#define USB_REG_BIT_CTRT                        0x0800
#define USB_REG_BIT_DTCH                        0x1000
#define USB_REG_BIT_ATTCH                       0x0800
#define USB_REG_BIT_SIGN                        0x0020
#define USB_REG_BIT_SACK                        0x0010
#define USB_REG_BIT_BRDY0                       0x0001
#define USB_REG_BIT_NRDY0                       0x0001
#define USB_REG_BIT_BEMP5                       0x0020
#define USB_REG_BIT_BEMP0                       0x0001
#define USB_REG_BIT_DIR                         0x0010
#define USB_REG_BIT_MAXP                        0x007F
#define USB_REG_BIT_PID                         0x0003
#define USB_VAL_TYPE_BULK                       0x4000
#define USB_VAL_TYPE_INT                        0x8000
#define USB_REG_BIT_DBLB                        0x0200
#define USB_REG_BIT_CNTMD                       0x0100
#define USB_REG_BIT_SHTNAK                      0x0080
#define USB_REG_BIT_EPNUM                       0x000F
#define USB_REG_BIT_MXPS                        0x07FF
#define USB_REG_BIT_INBUFM                      0x4000
#define USB_REG_BIT_ACLRM                       0x0200
#define USB_REG_BIT_SQCLR                       0x0100
#define USB_REG_BIT_SQSET                       0x0080
#define USB_REG_BIT_USBSPD                      0x00C0
#define USB_REG_BIT_UPPHUB                      0x7800
#define USB_REG_BIT_HUBPORT                     0x0700

#define CTRL_GET_DEVICEDESCRIPTOR       0x0680      
#define CTRL_SET_ADDRESS                        0x0500
#define CTRL_SET_CONFIGURATION                  0x0900

#define USB_PIPE_CTRL     (0x0) // (PIPE) Control
#define USB_PIPE_ISO_IN   (0x1) // (PIPE) Isochronous IN
#define USB_PIPE_ISO_OUT  (0x2) // (PIPE) Isochronous OUT
#define USB_PIPE_BULK_IN  (0x3) // (PIPE) Bulk IN
#define USB_PIPE_BULK_OUT (0x4) // (PIPE) Bulk OUT
#define USB_PIPE_INTR_OUT (0x7) // (PIPE) Interrupt OUT
#define USB_PIPE_INTR_IN  (0x6) // (PIPE) Interrupt IN

#define USB_PIPE_TYPE_CTL           (0x0 << 14)
#define USB_PIPE_TYPE_BULK          (0x1 << 14)
#define USB_PIPE_TYPE_INTR          (0x2 << 14)
#define USB_PIPE_TYPE_ISO           (0x3 << 14)

#define USB_INTSTS1_BCHG            (0x1 << 14)
#define USB_INTSTS1_DTCH            (0x1 << 12)
#define USB_INTSTS1_ATTCH           (0x1 << 11)
#define USB_INTSTS1_SOFERR          (0x1 << 6)
#define USB_INTSTS1_SIGN            (0x1 << 5)
#define USB_INTSTS1_SACK            (0x1 << 4)

#define USB_INTSTS0_BRDY            (0x1 << 8)
#define USB_INTSTS0_NRDY            (0x1 << 9)
#define USB_INTSTS0_BEMP            (0x1 << 10)

#define USB_ENDPOINT_ATTRIBUTE_BULK             2

#define USB_STOP_TRANSFER                       0
#define USB_IN_TRANSFER                         1
#define USB_OUT_TRANSFER                        2
#define USB_STATUS_TRANSFER                     3

#define USB_TEST_UNIT_READY_CMD                 0x00
#define USB_REQUEST_SENSE_CMD                   0x03
#define USB_INQUIRY_CMD                         0x12
#define USB_MODE_SENSE_CMD                      0x1A
#define USB_MODE_SENSE_CMD_5A                   0x5A
#define USB_READ_CAPACITY_CMD                   0x25
#define USB_READ10_CMD                          0x28
#define USB_PREVENT_ALLOW_REMOVAL_CMD           0x1E

#define CTRL_GET_HUB_STATUS                     0x00A0
#define CTRL_GET_STATUS                         0x0080
#define CTRL_GET_PORT_STATUS                    0x00A3
#define CTRL_SET_ADDRESS                        0x0500
#define CTRL_SET_CONFIGURATION                  0x0900
#define CTRL_SET_PORT_FEATURE                   0x0323
#define CTRL_CLEAR_PORT_FEATURE                 0x0123

#define CTRL_CLASS_INTERFACE                    0xFEA1
#define MAX_CLUSTERS_READABLE                   0xF00

//Bulk transfer pipes definition.
#define USB_PIPE_IN                             3
#define USB_PIPE_OUT                            4
#define USB_PIPE_INT_IN                         6


typedef struct _USBCOMMAND_
{
    UINT16              request;
    UINT16              value;
    UINT16              index;
    UINT16              length;
}USBCOMMAND;

typedef struct _USB_DEVICE_DESCRIPTOR_INFO_
{ 
  UINT8                 bLength;
    UINT8               bDescriptorType;
    UINT16              bcdUSB;
    UINT8               bDeviceClass;
    UINT8               bDeviceSubClass;
    UINT8               bDeviceProtocol;
    UINT8               bMaxPacketSize;
    UINT16              idVendor;
    UINT16              idProduct;
    UINT16              bcdDevice;
    UINT8               iManufacturer;
    UINT8               iProduct;
    UINT8               iSerialNumber;
    UINT8               bNumConfigurations;
    char                    ManufacturerName[9];
    char                    ProductName[30];
    char                    SerialNumber[10];
} USB_DEVICE_DESCRIPTOR_INFO;

typedef struct _USB_DEVICE_CONFIGURATION_INFO_
{ 
  UINT8                 bLength;
    UINT8               bDescriptorType;
    UINT16              wTotalLength;
    UINT8               bNumInterfaces;
    UINT8               bConfigurationValue;
    UINT8               iConfiguration;
    UINT8               bmAttributes;
    UINT8               bMaxPower;
} USB_DEVICE_CONFIGURATION_INFO;

typedef struct _USB_INTERFACE_DESCRIPTOR_INFO_
{ 
  UINT8                 bLength;
    UINT8               bDescriptorType;
    UINT8               bInterfaceNumber;
    UINT8               bAlternateSetting;
    UINT8               bNumEndpoints;
    UINT8               bInterfaceClass;
    UINT8               bInterfaceSubClass;
    UINT8               bInterfaceProtocol;
    UINT8               iInterface;
} USB_INTERFACE_DESCRIPTOR_INFO;

typedef struct _USB_ENDPOINT_DESCRIPTOR_INFO_
{ 
  UINT8                 bLength;
    UINT8               bDescriptorType;
    UINT8               bEndpointAddress;
    UINT8               bmAttibutes;
    UINT16              wMaxPacketSize;
    UINT8               bInterval;
} USB_ENDPOINT_DESCRIPTOR_INFO;

typedef struct  _USB_DESCRIPTOR_INFO_
{
    USB_DEVICE_DESCRIPTOR_INFO      descriptor;
    USB_DEVICE_CONFIGURATION_INFO   configuration;
    USB_INTERFACE_DESCRIPTOR_INFO   interface;
    USB_ENDPOINT_DESCRIPTOR_INFO        endpoints[2];
} USB_DESCRIPTOR_INFO;


typedef struct _USB_INFO_{
    BlockDeviceInfo*            BlockDeviceInformation;
    BlockStorageDevice*     Device;
    USBCOMMAND              cmd;            
    USB_DESCRIPTOR_INFO     disk;
    UINT16                  buff[18];
    UINT32                  recv_data_len;
    UINT32                  recv_total_len;
    UINT8                   compound_device;
    UINT8                   device_address;
    UINT16                  max_packet_size;
    UINT8                   device_speed;
    char                    error;
    UINT32                  packetnum;
    UINT32                  filladdress;
    UINT8                   pid;
    UINT8                   initialized;
} USB_CONFIGURATION_INFO;

struct SH7264_USB_BL_Driver
{
private:
    static UINT16 reg_read(UINT32 offset);
    static void reg_write(UINT16    val,UINT32  offset);
    static void reg_bclr(UINT16 val, UINT32 offset);
    static void reg_bset(UINT16 val, UINT32 offset);
    static void reg_mdfy(UINT16 val, UINT16 pat,UINT32  offset);
    static void enable_irq_ready(UINT16 pipenum);
    static void clear_irq_ready_status(UINT16 pipenum);
    static void disable_irq_ready(UINT16 pipenum);
    static void enable_irq_empty(UINT16 pipenum);
    static void clear_irq_empty_status(UINT16 pipenum);
    static void disable_irq_empty(UINT16 pipenum);
    static void enable_irq_nrdy(UINT16 pipenum);
    static void disable_irq_nrdy(UINT16 pipenum);
    static void pipe_irq_disable(UINT16 pipenum);   
    static void pipe_irq_enable(UINT16 pipenum);
    static UINT32 get_fifosel_addr(UINT16 pipenum);
    static UINT32 get_fifoctr_addr(UINT16   pipenum);
    static UINT32 get_pipectr_addr(UINT16   pipenum);
    static UINT32 get_devadd_addr(UINT16    pipenum);
    static UINT32 get_fifo_addr(UINT16  pipenum);
    static void set_pid(UINT16  pipenum, UINT16 pid);
    static void pipe_start(UINT16   pipenum);   //Set the corresponding pipe's PID to 1
    static void pipe_stop(UINT16 pipenum);                          //Set the corresponding pipe's PID to 0
    static void pipe_change(UINT16  pipenum, UINT8  mbw, UINT8  endian);
    static UINT8 irq_pipe_ready(UINT16 status);                  //Processing the BRDY irq when it come
    static void irq_pipe_empty(UINT16 status);                      //Processing the BEMP irq when it come
    static void irq_pipe_nrdy(UINT16    status);            //Processing the NRDY irq when it come
    static void wait_4_transmit_completed(UINT16 pipenum);
    static void Clear_Pipe_Buffer(UINT16    pipenum);
    static void Clear_SQ_Control_Reg(UINT16 pipenum);
    static void Set_SQ_Control_Reg(UINT16   pipenum);
    static void Write_FIFO_Reg(UINT32   offset, UINT16  *buf, UINT32    len);

    static UINT16 Get_Control_Reg(UINT16 pipenum);              //Get the corresponding pipe's control resgister value
    static UINT32 Get_Data_FIFO(UINT16    pipenum, UINT32 buff_addr);         //Get data from the corresponding FIFO resgister
    static UINT32 Put_Data_FIFO(UINT16  pipenum, UINT32 buff_addr, UINT32 size);
    
    static UINT8    Start_Control_Command(UINT16 command, UINT16 wvalue, UINT16 windex, UINT16 wlength);
    static void USB_Time_Sleep_MicroSeconds(int micros);
    static void Initialize_Flash_Device(USB_DESCRIPTOR_INFO *device, UINT8  device_address);
    static void Start_Control_Transfer(UINT16   pipenum);
    static void Check_Status_Control_Packet(UINT16 pipenum);
    static void In_Control_Packet_read(UINT16   pipenum);                   //Red the incoming packet data
    static void In_Control_Packet(UINT16    pipenum);
    static void Out_Command_Packet(UINT16   pipenum);
    static void Out_Control_Packet(UINT16   pipenum);
    static void Status_Control_Packet();
    static void highspeed_control_status_stage();   
    static UINT8 is_CSW_packet();                                    //Looking for the bulk transfer CSW packet
    static void Get_Descriptor(USB_DEVICE_DESCRIPTOR_INFO   *descriptor);
    static void Get_Configuration(USB_DESCRIPTOR_INFO   *device);
    static void Start_Bulk_Transfer(UINT8   cmdIndex, UINT32    secNum, UINT16  totalSecs, UINT32   address);   
    static void Setup_INQ_Command(UINT32    packetnum);
    static void Setup_TUR_Command(UINT32    packetnum);
    static void Setup_RS_Command(UINT32 packetnum);
    static void Setup_RC_Command(UINT32 packetnum);
    static void Setup_MS_Command(UINT32 packetnum);
    static void Setup_PAR_Command(UINT32    packetnum);
    static void Setup_R10_Command(UINT32    packetnum, UINT32   secnum, UINT16  totalsecs);
    static void Setup_W10_Command(UINT32 packetnum, UINT32 secnum, UINT16   totalsecs);
    static void Reset_Command_Buffer();
    static void Reset_Bus(void);
    static void Configure_Pipe5();
    static void Setup_Connection(void);
    static void Setup_Control_Command(UINT16 command, UINT16 wvalue, UINT16 windex, UINT16 wlength);
    static void get_string(char *str, UINT16    len);
    static void Configure_Bulk_Transfer(UINT16  pipenum);
    static void Transfer_Write_Data(UINT16 pipenum, UINT32 size);

public:
    static HRESULT Initialize( );
    static BOOL USB_Read_Sectors(UINT32 Sector, UINT32  NumSector, UINT32 Buf);
    static BOOL USB_Write_Sectors(UINT32 Sector, UINT32     NumSector, UINT32 Buf);
    static BOOL USB_Erase_Sectors(UINT32 Address, INT32 SectorCount);
    static void USB_IRQ_Enable(void);
    static void USB_IRQ_Disable(void);      
    static HRESULT USB_Initialize( );
    static void USB_Global_ISR  ( );
    };

struct SH7264_USB_CONFIG
{
    HAL_DRIVER_CONFIG_HEADER Header;

    //--//

    static LPCSTR GetDriverName() { return "SH7264_USB"; }
};

#endif //_SH7264__USB_H_1

