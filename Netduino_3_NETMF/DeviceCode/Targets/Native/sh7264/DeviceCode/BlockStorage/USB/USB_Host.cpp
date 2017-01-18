/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */

#include <tinyhal.h>
#include "USB_Host.h"
//#include "..\..\Include\iodefine.h"

USB_CONFIGURATION_INFO g_SH7264_USB_Info;

char Data_buff[526];
#define DataAddress ((unsigned long)Data_buff)//0x0C800000

//--//

HRESULT SH7264_USB_BL_Driver::USB_Initialize()
{
    GLOBAL_LOCK(irq);

    if(g_SH7264_USB_Info.initialized == 0)
    {
        USB.SYSCFG.WORD &= ~(0x1 << 10);
        USB_Time_Sleep_MicroSeconds(1000);
        
        USB.SYSCFG.WORD |= (0x1 << 10);
    
        USB.SYSCFG.WORD |=  (0x1 << 7);         //High speed is enable
        USB.SYSCFG.WORD &= ~(0x1);          //USB module operation is disable
        USB.SYSCFG.WORD &= ~(0x7 <<4);          //USB function controller function is selected  
    
        USB.SYSCFG.WORD |= (0x1);           //USB module operation is enable
        
        USB.BRDYENB.WORD = 0x0001;          //BRDY interrupt enable for PIPE0
        USB.BEMPENB.WORD = 0x0001;          //BEMP interrupt enable for PIPE0
        USB.NRDYENB.WORD = 0x0001;          //BEMP interrupt enable for PIPE0
        
        USB.SYSCFG.WORD |= (0x3 <<5);           //USB host controller function is selected  
        
        USB_Time_Sleep_MicroSeconds(400);   
    
        Configure_Pipe5();
        
        USB.INTENB1.WORD |= (0x1 << 14);        //USB bus change interrupt enable
        USB.INTENB1.WORD &=~(0x1 << 12);        //Disconnection detection interrupt disable
        
        USB_Time_Sleep_MicroSeconds(500);
        g_SH7264_USB_Info.device_address = 0;
        g_SH7264_USB_Info.packetnum = 0;
        g_SH7264_USB_Info.max_packet_size = 0x40;       
    }

    return S_OK;
}

void    SH7264_USB_BL_Driver::Setup_Connection(void)
{   
    USB.SOFCFG.WORD &= ~(0x1 << 8);
    switch (USB.DVSTCTR.WORD & 0x7)
    {
        case 0:
            g_SH7264_USB_Info.device_speed = 0;         //Connection off
            break;
        case 1:
            g_SH7264_USB_Info.device_speed = 0x1;           //Lowspeed device pluged
            USB.SOFCFG.WORD |= (0x1 << 8);
            USB.DEVADD0.WORD = (USB.DEVADD0.WORD & ~(0x3 << 6)) | (0x1 << 6);   
            
            break;
        case 2:
            g_SH7264_USB_Info.device_speed = 0x2;           //Fullspeed device pluged
            USB.SOFCFG.WORD |= (0x1 << 8);
            USB.DEVADD0.WORD = (USB.DEVADD0.WORD & ~(0x3 << 6)) | (0x2 << 6);
            
            break;
        case 3:
            g_SH7264_USB_Info.device_speed = 0x3;           //Highspeed device pluged               
            USB.DEVADD0.WORD |= (0x3 << 6);
            break;
    }
}

void    SH7264_USB_BL_Driver::USB_Global_ISR(void)                      //Interrupt handler controller
{
    UINT16  USB_INTR;

    USB_INTR = (USB.INTENB1.WORD) & (USB.INTSTS1.WORD);

    if(USB_INTR)
    {
        if(USB_INTR & USB_REG_BIT_BCHG)                                 //USB bus changing irq
        {
            USB.INTSTS1.WORD &= ~(0x1 << 14);
            USB_Time_Sleep_MicroSeconds(1000);
            USB.INTENB1.WORD &= ~(01 << 14);
            USB.INTSTS1.WORD &= ~(0x1 << 12);
        USB.INTENB1.WORD |= (01 << 12);
            if(USB.INTSTS1.WORD & (0x1 << 11))
            {
                USB.INTSTS1.WORD &= ~(0x1 << 11);
                
                Reset_Bus();
                
                USB_Time_Sleep_MicroSeconds(10000);
    
                Setup_Connection();
                if(g_SH7264_USB_Info.device_speed > 0)
                {
                    Initialize_Flash_Device(&g_SH7264_USB_Info.disk, USB_DEVICE_ADDR);
                    if(g_SH7264_USB_Info.error == 0) {
                    }
                }
            }
        }
        else if(USB_INTR & USB_REG_BIT_DTCH)                                //USB device unpluging irq
        {
            USB.INTSTS1.WORD &= ~(0x1 << 12);//BIT.DTCH = 0;
            USB.INTENB1.WORD |=  (0x1 << 12);//BIT.DTCHE = 1;
            USB.INTSTS1.WORD &= ~(0x1 << 14);//BIT.BCHG = 0;
            USB.INTENB1.WORD |=  (0x1 << 14);//BIT.BCHGE = 1;
            g_SH7264_USB_Info.initialized = 0;  
            USB_Initialize();       
        }
    }
}

UINT16  SH7264_USB_BL_Driver::reg_read(UINT32   offset)
{
    return *(UINT16 *)(USB_REG_BASE_ADDR + offset);
}

void    SH7264_USB_BL_Driver::reg_write(UINT16  val,UINT32  offset)
{
    *(UINT16 *)(USB_REG_BASE_ADDR + offset) = val;
}

void    SH7264_USB_BL_Driver::reg_bclr(UINT16   val, UINT32 offset)
{
    UINT16  tmp;
    
    tmp = reg_read(offset);
    tmp = tmp & (~val);
    reg_write(tmp, offset);
}

void    SH7264_USB_BL_Driver::reg_bset(UINT16   val, UINT32 offset)
{
    UINT16  tmp;
    
    tmp = reg_read(offset);
    tmp = tmp | val;
    reg_write(tmp, offset);
}

void    SH7264_USB_BL_Driver::reg_mdfy(UINT16   val, UINT16 pat,UINT32  offset)
{
  UINT16    tmp;
  
  tmp = reg_read(offset);
  tmp = tmp & (~pat);
  tmp = tmp | val;
  reg_write(tmp, offset);
}

void    SH7264_USB_BL_Driver::enable_irq_ready(UINT16 pipenum)
{
    reg_bset((1 << pipenum), USB_OFFSET_BRDYENB);           //Enable the corresponding pipe's BRDY interrupt    
}

void SH7264_USB_BL_Driver::clear_irq_ready_status(UINT16 pipenum)
{
    reg_bclr((1 << pipenum), USB_OFFSET_BRDYSTS);           //Clear the corresponding pipe's BRDY interrupt status  
}

void    SH7264_USB_BL_Driver::disable_irq_ready(UINT16 pipenum)
{
    reg_bclr((1 << pipenum), USB_OFFSET_BRDYENB);           //Disable the corresponding pipe's BRDY interrupt
}

void    SH7264_USB_BL_Driver::enable_irq_empty(UINT16 pipenum)
{
    reg_bset((1 << pipenum), USB_OFFSET_BEMPENB);           //Enable the corresponding pipe's BEMP interrup
}

void SH7264_USB_BL_Driver::clear_irq_empty_status(UINT16 pipenum)
{
    reg_bclr((1 << pipenum), USB_OFFSET_BEMPSTS);           //Clear the corresponding pipe's BRDY interrupt status  
}

void SH7264_USB_BL_Driver::disable_irq_empty(UINT16 pipenum)
{
    reg_bclr((1 << pipenum), USB_OFFSET_BEMPENB);           //Disable the corresponding pipe's BEMP interrupt
}

void SH7264_USB_BL_Driver::enable_irq_nrdy(UINT16 pipenum)
{
    reg_bset((1 << pipenum), USB_OFFSET_NRDYENB);           //Enable the corresponding pipe's NRDY interrup
}

void SH7264_USB_BL_Driver::disable_irq_nrdy(UINT16 pipenum)
{
    reg_bclr((1 << pipenum), USB_OFFSET_NRDYENB);           //Disable the corresponding pipe's NRDY interrupt
}

void SH7264_USB_BL_Driver::pipe_irq_disable(UINT16  pipenum)
{
    disable_irq_ready(pipenum);
    disable_irq_nrdy(pipenum);
}

void SH7264_USB_BL_Driver::pipe_irq_enable(UINT16 pipenum)
{
    enable_irq_ready(pipenum);
    enable_irq_nrdy(pipenum);
}

UINT32  SH7264_USB_BL_Driver::get_fifosel_addr(UINT16 pipenum)
{
    if (pipenum == 0)
        return USB_OFFSET_CFIFOSEL;             //Return CFIFOSEL register address

    if ((pipenum == 3) || (pipenum == 5) || (pipenum == 1))
        return USB_OFFSET_D1FIFOSEL;                //Return D1FIFOSEL register address
    else
        return USB_OFFSET_D0FIFOSEL;                //Return D0FIFOSEL register address

}

UINT32  SH7264_USB_BL_Driver::get_fifoctr_addr(UINT16   pipenum)
{
    if (pipenum == 0)
        return USB_OFFSET_CFIFOCTR;             //Return CFIFOCTR register address

    if ((pipenum == 3) || (pipenum == 5) || (pipenum == 1))
        return USB_OFFSET_D1FIFOCTR;                //Return D1FIFOCTR register address
    else
        return USB_OFFSET_D0FIFOCTR;                //Return D0FIFOCTR register address
}

UINT32  SH7264_USB_BL_Driver::get_pipectr_addr(UINT16   pipenum)
{
    if(pipenum == 0)
        return USB_OFFSET_PIPE1CTR - 0x10;          //Return DCPCTR register address
 
    return USB_OFFSET_PIPE1CTR + (pipenum - 1) * 2;//Return PIPEnCTR register address
}

UINT32 SH7264_USB_BL_Driver::get_devadd_addr(UINT16 pipenum)
{ 
    return USB_OFFSET_DEVADD0 + (pipenum * 2);      //Return DEVADDn register address
}

UINT32 SH7264_USB_BL_Driver::get_fifo_addr(UINT16   pipenum)
{
    if (pipenum == 0)
        return USB_OFFSET_CFIFO;                    //Return CFIFO register address

    if ((pipenum == 3) || (pipenum == 5) || (pipenum == 1))
        return USB_OFFSET_D1FIFO;                   //Return D1FIFO register address
    else    
        return USB_OFFSET_D0FIFO;                   //Return D0FIFO register address
}

void    SH7264_USB_BL_Driver::set_pid(UINT16    pipenum, UINT16 pid)
{
    UINT32  offset;
    
    if (pipenum == 0)
        reg_mdfy(pid, 0x0003, USB_OFFSET_DCPCTR);   //Set the PID bits with pid on DCPCTR
    else 
    {   
        offset = get_pipectr_addr(pipenum);     //Get the corresponding PIPECTR address
        reg_mdfy(pid, 0x0003, offset);          //Set the PID bits with pid
    }
}

void    SH7264_USB_BL_Driver::pipe_start(UINT16 pipenum)    //Set the corresponding pipe's PID to 1
{
    set_pid(pipenum, 1);                                    
}

void    SH7264_USB_BL_Driver::pipe_stop(UINT16 pipenum)                         //Set the corresponding pipe's PID to 0
{
    set_pid(pipenum, 0);
}

void    SH7264_USB_BL_Driver::pipe_change(UINT16    pipenum, UINT8  mbw, UINT8  endian)
{
    reg_mdfy(pipenum, USB_REG_BIT_CURPIPE, get_fifosel_addr(pipenum));      //Set the CURPIPE bits with pipenum on corresponding FIFOSEL register
    USB_Time_Sleep_MicroSeconds(100);
    
    reg_mdfy((UINT16)(mbw << 10), USB_REG_BIT_MBW, get_fifosel_addr(pipenum));  //Set MBW (16-bit width access) on corresponding FIFOSEL register
    if(endian)
        reg_bset(USB_REG_BIT_BIGEND, get_fifosel_addr(pipenum));    //Big endian
    else
        reg_bclr(USB_REG_BIT_BIGEND, get_fifosel_addr(pipenum));    //Little endian
}

UINT16 SH7264_USB_BL_Driver::Get_Control_Reg(UINT16 pipenum)                //Get the corresponding pipe's control resgister value
{
    UINT16      ret = 0;
    UINT32      offset;

    if (pipenum == 0)
        ret = USB.DCPCTR.WORD;
    else if (pipenum < 8)
    {
        offset = get_pipectr_addr(pipenum);
        ret = reg_read(offset);
    }

    return ret;
}

UINT32  SH7264_USB_BL_Driver::Get_Data_FIFO(UINT16  pipenum, UINT32 buff_addr)          //Get data from the corresponding FIFO resgister
{
    UINT16      *temp, i;
    UINT32      offset, size;
            
    i = 0;
    temp = (UINT16 *)buff_addr;
    
    offset = USB_REG_BASE_ADDR + get_fifo_addr(pipenum);
    
    while (1)
    {
        if(pipenum == 0)
        {
            size = USB.CFIFOCTR.WORD & 0x0FFF;
            for(i=0; (i*2)<size; i++)
            {
                *(temp + i) = *(UINT16 *)offset;
            }
            return (i*2);
        }
        else
        {
            size = USB.D1FIFOCTR.WORD & 0x0FFF;;
            for (i=0; (i*2)<size; i++)
            {
                *(temp + i) = *(UINT16 *)offset;
            }
            return (i*2);   
        }
    }
    
    return 0;
}

UINT32 SH7264_USB_BL_Driver::Put_Data_FIFO(UINT16   pipenum, UINT32 buff_addr, UINT32 size)
{
    UINT16  *dataBuf, i, check;
    UINT32  regBuf;
            
    i = 0;
    dataBuf = (UINT16 *)buff_addr;
    regBuf = USB_REG_BASE_ADDR + get_fifo_addr(pipenum);
    check = 1 << pipenum;
    if (USB.BRDYSTS.WORD & check)
    {       
        USB.BRDYSTS.WORD &= ~check;
        for (i=0; ((2*i) < size); i++)
        {
            *(UINT16 *)regBuf = *(dataBuf + i);
        }
        reg_write(USB_REG_BIT_BVAL, get_fifoctr_addr(pipenum));
    }

    return i*2; 
}

UINT8 SH7264_USB_BL_Driver::irq_pipe_ready(UINT16 status)                 //Processing the BRDY irq when it come
{
    UINT16  fifosel, tmp, check, pipenum;
    UINT8 ret = 0;
    
    if (status & USB_REG_BIT_BRDY0)
    {
        reg_write(~USB_REG_BIT_BRDY0, USB_OFFSET_BRDYSTS);
        reg_mdfy(USB_VAL_CURPIPE_PIPE0, USB_REG_BIT_CURPIPE, USB_OFFSET_CFIFOSEL);
        fifosel = USB.DCPCFG.WORD;
        if ((fifosel & USB_REG_BIT_DIR) == 0x00)
        {
            In_Control_Packet_read(0);  
            ret = 1;
        }
    }
    else
    {
        for (pipenum = 1; pipenum < 8; pipenum++)
        {
            check = 1 << pipenum;
            if (status & check)
            {
                USB.BRDYSTS.WORD &= ~check;
                USB.PIPESEL.WORD = pipenum;
                tmp = USB.PIPECFG.WORD;
                if ((tmp & USB_REG_BIT_DIR) == 0)
                {
                    ret = 1;
                    In_Control_Packet_read(pipenum);        
                }
            }
        }
    }
    return ret;
}

void SH7264_USB_BL_Driver::irq_pipe_empty(UINT16 status)                        //Processing the BEMP irq when it come
{
    UINT16  tmp, check, pipenum;

    if (status & USB_REG_BIT_BEMP0)
    {
        USB.BEMPSTS.WORD &= ~(0x1);
    USB.CFIFOSEL.WORD &= ~(0xF);
        disable_irq_empty(0);
        disable_irq_ready(0);
        disable_irq_nrdy(0);
    }
    else
    {
        for (pipenum = 1; pipenum < 8; pipenum++)
        {
            check = 1 << pipenum;
            if (status & check)
            {
                USB.BEMPSTS.WORD &= ~check;
                tmp = Get_Control_Reg(pipenum);
                if ((tmp & USB_REG_BIT_INBUFM) == 0)
                {
                    disable_irq_empty(pipenum);
                    disable_irq_ready(0);
                    disable_irq_nrdy(0);
                }
            }
        }
    }
}

void SH7264_USB_BL_Driver::irq_pipe_nrdy(UINT16 status)         //Processing the NRDY irq when it come
{
    UINT16  check;
    UINT16  pipenum;

    if (status & USB_REG_BIT_NRDY0)
    {
        reg_write(~USB_REG_BIT_NRDY0, USB_OFFSET_NRDYSTS);
        reg_mdfy(USB_VAL_CURPIPE_PIPE0, USB_REG_BIT_CURPIPE, USB_OFFSET_CFIFOSEL);
        pipe_irq_disable(0);
    }
    else
    {
        for (pipenum = 1; pipenum < 8; pipenum++)
        {
            check = 1 << pipenum;
            if (status & check)
            {
                reg_write(~check, USB_OFFSET_NRDYSTS);
                pipe_irq_disable(pipenum);
                pipe_stop( pipenum);
            }
        }
    }
}

void SH7264_USB_BL_Driver::wait_4_transmit_completed(UINT16 pipenum)
{
    int cnt = 0;
    enable_irq_ready(pipenum);
    enable_irq_empty(pipenum);

    while(1)
    {
        if(USB.INTSTS0.WORD & 0x0400)   
        {
            disable_irq_empty(pipenum);
            clear_irq_empty_status(pipenum);
        }
        if(USB.INTSTS0.WORD & 0x0100)
        {   
            disable_irq_ready(pipenum);
            clear_irq_ready_status(pipenum);
            break;
        }  
        else
        {
            cnt += 1;
            if (cnt > 0x10000) break;
        }
    }   
}

void SH7264_USB_BL_Driver::Clear_Pipe_Buffer(UINT16 pipenum)
{
    UINT32 offset;

    if (pipenum == 0)
        return;
    else
    {
        pipe_stop(pipenum);
        offset = get_pipectr_addr(pipenum);
        reg_bset(USB_REG_BIT_ACLRM, offset);
    USB_Time_Sleep_MicroSeconds(100000);
        reg_bclr(USB_REG_BIT_ACLRM, offset);    
    }   
}

void SH7264_USB_BL_Driver::Clear_SQ_Control_Reg(UINT16  pipenum)
{
    pipe_stop(pipenum);

    if (pipenum == 0)
        reg_bset(USB_REG_BIT_SQCLR, USB_OFFSET_DCPCTR);
    else if (pipenum == 3)
    {
        USB.PIPE3CTR.WORD |= (0x1 << 8);
    }
    else if (pipenum == 4)
    {
        USB.PIPE4CTR.WORD |= (0x1 << 8);
    }
}

void SH7264_USB_BL_Driver::Set_SQ_Control_Reg(UINT16    pipenum)
{
    UINT32 offset;

    pipe_stop(pipenum);

    if (pipenum == 0)
        USB.DCPCTR.WORD |= (0x1 << 7);
    else if (pipenum < 8)
    {
        reg_write(pipenum, USB_OFFSET_PIPESEL);
        offset = get_pipectr_addr(pipenum);
        reg_bset(USB_REG_BIT_SQSET, offset);
    }
}

void SH7264_USB_BL_Driver::Write_FIFO_Reg(UINT32    offset, UINT16  *buf, UINT32    len)
{
    UINT32 fifoaddr = USB_REG_BASE_ADDR + offset;
    UINT32 odd = len & 0x0001;
    int i;
    
    len = len / 2;

    for ( i = 0; i < len; i++ )
        *(UINT16 *)(fifoaddr) = buf[i];
    if (odd) 
        *(UINT8 *)(fifoaddr) = buf[len];
}

void    SH7264_USB_BL_Driver::Setup_Control_Command(UINT16 command, UINT16 wvalue, UINT16 windex, UINT16 wlength)
{
    USB.USBREQ.WORD = command;  
    USB.USBVAL = wvalue;
    USB.USBINDX = windex;
    USB.USBLENG = wlength;
    
    g_SH7264_USB_Info.cmd.request = command;
    g_SH7264_USB_Info.cmd.value = wvalue;
    g_SH7264_USB_Info.cmd.index = windex;
    g_SH7264_USB_Info.cmd.length = wlength;
    g_SH7264_USB_Info.recv_total_len = 0;
}

UINT8   SH7264_USB_BL_Driver::Start_Control_Command(UINT16 command, UINT16 wvalue, UINT16 windex, UINT16 wlength)
{       
    UINT32 cnt;
    g_SH7264_USB_Info.filladdress = DataAddress;
    Setup_Control_Command(command, wvalue, windex, wlength);

    USB.DCPMAXP.WORD = (g_SH7264_USB_Info.device_address << 12) | g_SH7264_USB_Info.max_packet_size;

    USB.INTSTS1.WORD &= ~(0x3 << 4);

    USB.DCPCTR.WORD |= (0x1 << 14);
    
    Set_SQ_Control_Reg(0);
    
    g_SH7264_USB_Info.error = 0;
    
    cnt = 0;
    
    while(1)
    {
        if(USB.INTSTS1.WORD & (0x1 << 4))
        {   
            USB.INTSTS1.WORD &= ~(0x1 << 4);
            if ((USB.USBREQ.WORD & 0x80) == 0x80)
                g_SH7264_USB_Info.pid = USB_IN_TRANSFER;
            else
                g_SH7264_USB_Info.pid = USB_STATUS_TRANSFER;
            Start_Control_Transfer(0);
            break;  
        }   
        if(USB.INTSTS1.WORD & (0x1 << 5))
        {
            USB.INTSTS1.WORD &= ~(0x1 << 5);
            break;
        }
        cnt++;
        if(cnt == 0x100000)
        {
            g_SH7264_USB_Info.error = 1;
            break;
        }
    }
    if (g_SH7264_USB_Info.error )
        return 0; //Not Ok
    else
        return 1;   //Ok
}

void SH7264_USB_BL_Driver::USB_Time_Sleep_MicroSeconds(int micros)
{
        volatile int i;
        volatile UINT32 temp;
        for (i = 0; i < micros; i++)
        {
            temp = USB.INTSTS1.WORD;
        }
}

void    SH7264_USB_BL_Driver::Initialize_Flash_Device(USB_DESCRIPTOR_INFO   *device, UINT8  device_address)
{
    UINT16  i, len, windex;
    
    USB.DEVADD5.WORD = (USB.DEVADD5.WORD & ~(0x3 << 6)) | (0x3 << 6);
    
    if (!Start_Control_Command(CTRL_SET_ADDRESS, device_address, 0, 0)) {
        return;
    }
    g_SH7264_USB_Info.device_address = device_address;
    USB_Time_Sleep_MicroSeconds(10000);
    reg_write(0, get_devadd_addr(device_address));
    reg_mdfy((USB.DVSTCTR.WORD & 0x7) << 6, USB_REG_BIT_USBSPD, get_devadd_addr(device_address));
    
    if (!Start_Control_Command(CTRL_GET_DEVICEDESCRIPTOR, 0x100, 0, 0x12)) {
         return;
    }
    Get_Descriptor(&device->descriptor);
    USB_Time_Sleep_MicroSeconds(1000);
    g_SH7264_USB_Info.max_packet_size = device->descriptor.bMaxPacketSize;

    if (!Start_Control_Command(CTRL_GET_DEVICEDESCRIPTOR, 0x200, 0, 10)) {
     return;
    }
    len = *(UINT8 *)(DataAddress + 2);
    if(len == 0)    
    {
        g_SH7264_USB_Info.error = 1;
        return;
    }
    else
    {
        if(len < 32)    
        {
            g_SH7264_USB_Info.error = 1;
            return;
        }   
    }
    
    USB_Time_Sleep_MicroSeconds(100);
    if (!Start_Control_Command(CTRL_GET_DEVICEDESCRIPTOR, 0x200, 0, 0xFF)) {
        return;
    }
    Get_Configuration(device);
    
    if (!Start_Control_Command(CTRL_GET_DEVICEDESCRIPTOR, 0x300, 0, 0xFF)) {
     return;
    }
    len = *(UINT8 *)DataAddress;
    windex = (*(UINT8 *)(DataAddress + 2) >> 8) | (*(UINT8 *)(DataAddress + 3) << 8);
    
    if (!Start_Control_Command(CTRL_GET_DEVICEDESCRIPTOR, 0x300 | device->descriptor.iManufacturer, windex, 0xFF)) {
     return;
    }
    if (((((UINT8 *)DataAddress)[0])>0) &&((((UINT8 *)DataAddress)[1])==0x03)) {
        len = (UINT16)(((*(UINT8 *)DataAddress) - 2) / 2);
        get_string(device->descriptor.ManufacturerName, len);
    }
    if (!Start_Control_Command(CTRL_GET_DEVICEDESCRIPTOR, 0x300 | device->descriptor.iProduct, windex, 0xFF)) {
     return;
    }
    if (((((UINT8 *)DataAddress)[0])>0) &&((((UINT8 *)DataAddress)[1])==0x03)) {
        len = (UINT16)(((*(UINT8 *)DataAddress) - 2) / 2);
        get_string(device->descriptor.ProductName, len);
    }
    if (!Start_Control_Command(CTRL_GET_DEVICEDESCRIPTOR, 0x300 | device->descriptor.iSerialNumber, windex, 0xFF)){
     return;
    }
    if (((((UINT8 *)DataAddress)[0])>0) &&((((UINT8 *)DataAddress)[1])==0x03)) {    
        len = (UINT16)(((*(UINT8 *)DataAddress) - 2) / 2);
        get_string(device->descriptor.SerialNumber, len);
    }
    if (!Start_Control_Command(CTRL_SET_CONFIGURATION, 1, 0, 0)) {
     return;
    }

    for (i = 0; i < g_SH7264_USB_Info.disk.interface.bNumEndpoints; i++)
    {
        if ((g_SH7264_USB_Info.disk.endpoints[i].bLength == 0x07) && (g_SH7264_USB_Info.disk.endpoints[i].bDescriptorType == 0x05))
        {
            if ((g_SH7264_USB_Info.disk.endpoints[i].bEndpointAddress & 0x80) == 0x80)
            {
                if ((g_SH7264_USB_Info.disk.endpoints[i].bmAttibutes & 0x03) == 0x03) {             
                    Configure_Bulk_Transfer(USB_PIPE_INT_IN);
                }
                else {
                    Configure_Bulk_Transfer(USB_PIPE_IN);
                }
            }
            else if ((g_SH7264_USB_Info.disk.endpoints[i].bEndpointAddress & 0x80) == 0x00)
            {
                Configure_Bulk_Transfer(USB_PIPE_OUT);
            }           
        }
    }
    
    if (!Start_Control_Command(CTRL_GET_STATUS, 0, 0, 2)) {
     return;
    }
    
    if (!Start_Control_Command(CTRL_GET_STATUS, 0, 0, 2)){
     return;
    }
    
    if (!Start_Control_Command(CTRL_CLASS_INTERFACE, 0, 0, 1)){
     return;
    }
    
    Start_Bulk_Transfer(USB_INQUIRY_CMD, 0, 0, DataAddress);
    
    Start_Bulk_Transfer(USB_TEST_UNIT_READY_CMD, 0, 0, DataAddress);
    Start_Bulk_Transfer(USB_REQUEST_SENSE_CMD, 0, 0, DataAddress);
    Start_Bulk_Transfer(USB_TEST_UNIT_READY_CMD, 0, 0, DataAddress);
    Start_Bulk_Transfer(USB_READ_CAPACITY_CMD, 0, 0, DataAddress);
    Start_Bulk_Transfer(USB_TEST_UNIT_READY_CMD, 0, 0, DataAddress);
    Start_Bulk_Transfer(USB_PREVENT_ALLOW_REMOVAL_CMD, 0, 0, DataAddress);
    Start_Bulk_Transfer(USB_REQUEST_SENSE_CMD, 0, 0, DataAddress);
    Start_Bulk_Transfer(USB_TEST_UNIT_READY_CMD, 0, 0, DataAddress);
    Start_Bulk_Transfer(USB_READ_CAPACITY_CMD, 0, 0, DataAddress);
    
    Start_Control_Command(CTRL_GET_STATUS, 0, 0, 2);
    
    g_SH7264_USB_Info.initialized = 1;
}

void SH7264_USB_BL_Driver::Start_Control_Transfer(UINT16    pipenum)
{
    switch (g_SH7264_USB_Info.pid)
    {
        case USB_IN_TRANSFER:
            In_Control_Packet(pipenum);
            break;
        case USB_OUT_TRANSFER:
            Out_Control_Packet(pipenum);
            break;
        case USB_STATUS_TRANSFER:
            Status_Control_Packet();
            break;
    }
}

void    SH7264_USB_BL_Driver::Check_Status_Control_Packet(UINT16 pipenum)
{
    UINT16  actual_length;
    
    if (pipenum == 0)
    {   
        g_SH7264_USB_Info.pid = USB_STATUS_TRANSFER;
        if((g_SH7264_USB_Info.recv_total_len == 0) && ((g_SH7264_USB_Info.cmd.request & 0x80) == 0x80))
        {
            if(g_SH7264_USB_Info.cmd.value != 0x200)
                actual_length = *(UINT8 *)(DataAddress);    
            else
            {
                actual_length = *(UINT16 *)(DataAddress + 2);
                actual_length = (actual_length << 8) | (actual_length >> 8);
            }
            if(g_SH7264_USB_Info.cmd.length > actual_length)
                g_SH7264_USB_Info.cmd.length = actual_length;
        }
        g_SH7264_USB_Info.recv_total_len += g_SH7264_USB_Info.recv_data_len;
        
        if(g_SH7264_USB_Info.recv_total_len < g_SH7264_USB_Info.cmd.length)
            g_SH7264_USB_Info.pid = USB_IN_TRANSFER;    
    }
    else
    {
        g_SH7264_USB_Info.pid = USB_IN_TRANSFER;
    
        if(is_CSW_packet())
            g_SH7264_USB_Info.pid = USB_STOP_TRANSFER;
    }
}

void SH7264_USB_BL_Driver::In_Control_Packet_read(UINT16    pipenum)                    //Red the incoming packet data
{
    UINT16  tmp;
    UINT32  offset;

    if(pipenum == 0)
    {
        pipe_change(0, 1, 1);
    }

    tmp = reg_read(get_fifoctr_addr(pipenum));
    if ((tmp & USB_REG_BIT_FRDY) == 0)
    {
        pipe_stop(pipenum);
        pipe_irq_disable(pipenum);
    
        return;
    }
    
    pipe_stop(pipenum);
    pipe_irq_disable(pipenum);
    
    g_SH7264_USB_Info.recv_data_len = Get_Data_FIFO(pipenum, g_SH7264_USB_Info.filladdress);
    
    if(g_SH7264_USB_Info.recv_data_len & 0x00000001)
        g_SH7264_USB_Info.filladdress += g_SH7264_USB_Info.recv_data_len + 1;
    else
        g_SH7264_USB_Info.filladdress += g_SH7264_USB_Info.recv_data_len;
        
    if(g_SH7264_USB_Info.recv_data_len == 0)
    {
        offset = USB_REG_BASE_ADDR + get_fifoctr_addr(pipenum);
        *(UINT16 *)offset |= 0x4000;
        g_SH7264_USB_Info.pid = USB_STOP_TRANSFER;
    }
}

void SH7264_USB_BL_Driver::In_Control_Packet(UINT16 pipenum)
{
    UINT32 cnt;
    if (pipenum == 0)
    {
        USB.DCPCFG.WORD &= ~(0x1<<4);
        reg_mdfy(USB_VAL_CURPIPE_PIPE0, (USB_REG_BIT_ISEL | USB_REG_BIT_CURPIPE), USB_OFFSET_CFIFOSEL);
        reg_write(USB_REG_BIT_BCLR, USB_OFFSET_CFIFOCTR);
    
        pipe_irq_disable(pipenum);
        pipe_start(pipenum);
        pipe_irq_enable(pipenum);
    }
    else
    {
        pipe_change(pipenum, 1, 1);
            
        pipe_irq_disable(pipenum);
    
        pipe_start(pipenum);  
        
        pipe_irq_enable(pipenum);

    }
    cnt = 0;
    while(g_SH7264_USB_Info.pid != USB_STOP_TRANSFER)
    {
        if(USB.INTSTS0.WORD & 0x0100)
        {
            irq_pipe_ready(USB.BRDYSTS.WORD);
            if (g_SH7264_USB_Info.pid != USB_STOP_TRANSFER){
                Check_Status_Control_Packet(pipenum);
                Start_Control_Transfer(pipenum);
            }
        }
        if(USB.INTSTS0.WORD & 0x0200)   
        {
            irq_pipe_nrdy(USB.NRDYSTS.WORD);
            g_SH7264_USB_Info.pid = USB_STOP_TRANSFER;
            reg_mdfy(0, 0x0003, get_pipectr_addr(pipenum));
        }
        if(USB.INTSTS0.WORD & 0x0400)   
        {
            irq_pipe_empty(USB.BEMPSTS.WORD);
        }
        cnt++;
        if (cnt >= 0x100000) 
        {
            g_SH7264_USB_Info.error = 1;
            break;
        }
    }
}

void SH7264_USB_BL_Driver::Out_Command_Packet(UINT16    pipenum)
{
    UINT16  tmp;
    
    pipe_change(pipenum, 1, 1);
    tmp = reg_read(get_fifoctr_addr(pipenum));
    if ((tmp & USB_REG_BIT_FRDY) == 0)
    {
        pipe_stop(pipenum);
        
        Clear_SQ_Control_Reg(USB_PIPE_OUT);
        Clear_Pipe_Buffer(USB_PIPE_OUT);
        
        g_SH7264_USB_Info.pid = 0;
    }

    Write_FIFO_Reg(get_fifo_addr(pipenum), g_SH7264_USB_Info.buff, 0x1F);
    reg_write(USB_REG_BIT_BVAL, get_fifoctr_addr(pipenum));

    enable_irq_ready(pipenum);
    enable_irq_empty(pipenum);
}

void    SH7264_USB_BL_Driver::Out_Control_Packet(UINT16 pipenum)
{
    pipe_change(pipenum, 1, 1);

    if(pipenum == 0)
    {
        reg_bset(USB_REG_BIT_DIR, USB_OFFSET_DCPCFG);
        reg_mdfy(USB_REG_BIT_ISEL | USB_VAL_CURPIPE_PIPE0, (USB_REG_BIT_ISEL | USB_REG_BIT_CURPIPE), USB_OFFSET_CFIFOSEL);
        Set_SQ_Control_Reg(0);
        reg_write(USB_REG_BIT_BCLR, USB_OFFSET_CFIFOCTR);
    }
    else
    {
        reg_write(USB_REG_BIT_BCLR, get_fifoctr_addr(pipenum));
    }

    pipe_start(pipenum);

    reg_write(~(1 << pipenum), USB_OFFSET_BRDYSTS);
    
    Out_Command_Packet(pipenum);
    
    g_SH7264_USB_Info.pid = USB_STOP_TRANSFER;
    
    wait_4_transmit_completed(pipenum);
}

void SH7264_USB_BL_Driver::Status_Control_Packet()
{
    UINT32 cnt;
    pipe_stop(0);
    Set_SQ_Control_Reg(0);
    
    if ((USB.USBREQ.WORD & 0x80) == 0x80)
    {
        if (g_SH7264_USB_Info.device_speed == 3)
            highspeed_control_status_stage();
        else
        {
            USB.DCPCFG.WORD |= (0x1 << 4);
            reg_mdfy((USB_REG_BIT_ISEL | USB_VAL_CURPIPE_PIPE0), (USB_REG_BIT_ISEL | USB_REG_BIT_CURPIPE), USB_OFFSET_CFIFOSEL);
            reg_write((USB_REG_BIT_BVAL | USB_REG_BIT_BCLR), USB_OFFSET_CFIFOCTR);
            reg_write(~USB_REG_BIT_BEMP0, USB_OFFSET_BEMPSTS);
            enable_irq_empty(0);
            enable_irq_nrdy(0);
            pipe_start(0);
        }
    }
    else
    {
        USB.DCPCFG.WORD &= ~(0x1 << 4);
        reg_mdfy(USB_VAL_CURPIPE_PIPE0, (USB_REG_BIT_ISEL | USB_REG_BIT_CURPIPE), USB_OFFSET_CFIFOSEL);
        reg_write(USB_REG_BIT_BCLR, USB_OFFSET_CFIFOCTR);
        reg_write(~USB_REG_BIT_BRDY0, USB_OFFSET_BRDYSTS);
        reg_write(~USB_REG_BIT_BEMP0, USB_OFFSET_BEMPSTS);
        enable_irq_ready(0);
        enable_irq_nrdy(0);
        pipe_start(0);
    }
    cnt = 0;
    while(g_SH7264_USB_Info.pid != USB_STOP_TRANSFER)
    {
        if(USB.INTSTS0.WORD & 0x0100)
        {
            irq_pipe_ready(USB.BRDYSTS.WORD);
            Start_Control_Transfer(0);
        }
        if(USB.INTSTS0.WORD & 0x0200)   
        {
            irq_pipe_nrdy(USB.NRDYSTS.WORD);
        }
        if(USB.INTSTS0.WORD & 0x0400)   
        {
            irq_pipe_empty(USB.BEMPSTS.WORD);
            g_SH7264_USB_Info.pid = USB_STOP_TRANSFER;
            reg_mdfy(0, 0x0003, get_pipectr_addr(0));
        }
        cnt++;
        if (cnt >= 0x100000) break;
    }
}

void SH7264_USB_BL_Driver::highspeed_control_status_stage()                             
{
    UINT16  tmp;

    pipe_stop(5);
    
    reg_write(5, USB_OFFSET_PIPESEL);
    reg_write((g_SH7264_USB_Info.device_address << 12) | 64, USB_OFFSET_PIPEMAXP);
    reg_bset(USB_REG_BIT_SQSET, USB_OFFSET_PIPE5CTR);
    pipe_change(5, 1, 1);

    tmp = reg_read(get_fifoctr_addr(5));

    if ((tmp & USB_REG_BIT_FRDY) == 0)
    {
        g_SH7264_USB_Info.error = -1;
        return;
    }

    reg_write(USB_REG_BIT_BVAL | USB_REG_BIT_BCLR, get_fifoctr_addr(5));
    reg_write(~USB_REG_BIT_BEMP5, USB_OFFSET_BEMPSTS);
    enable_irq_empty(5); 
    pipe_start(5);
}

UINT8   SH7264_USB_BL_Driver::is_CSW_packet()                                   //Looking for the bulk transfer CSW packet
{
    UINT32  value;
    UINT32  address;

    address = g_SH7264_USB_Info.filladdress - 0xE;
    
    value = (*(UINT16   *)(address)) << 16;
    value |= *(UINT16   *)(address + 2);
    if(value == 0x55534253)
    {
        value = 0;
        value = *(UINT16 *)(address + 4) << 16;
        value = *(UINT16 *)(address + 6);
        if(value == g_SH7264_USB_Info.packetnum)
            return 1;   
    }
    return 0;
}

void SH7264_USB_BL_Driver::Transfer_Write_Data(UINT16 pipenum, UINT32 size)
{
    UINT16  tmp;
    UINT32  offset;

    if(pipenum == 0)
    {
        pipe_change(0, 1, 1);
    }

    tmp = reg_read(get_fifoctr_addr(pipenum));
    if ((tmp & USB_REG_BIT_FRDY) == 0)
    {
        pipe_stop(pipenum);
        disable_irq_empty(pipenum);
        disable_irq_nrdy(pipenum);
    
        return;
    }
    
    g_SH7264_USB_Info.recv_data_len = Put_Data_FIFO(pipenum, g_SH7264_USB_Info.filladdress, size);
    
    if(g_SH7264_USB_Info.recv_data_len & 0x00000001)
        g_SH7264_USB_Info.filladdress += g_SH7264_USB_Info.recv_data_len + 1;
    else
        g_SH7264_USB_Info.filladdress += g_SH7264_USB_Info.recv_data_len;
        
    if(g_SH7264_USB_Info.recv_data_len == 0)
    {
        offset = USB_REG_BASE_ADDR + get_fifoctr_addr(pipenum);
        *(unsigned short *)offset |= 0x4000;
        g_SH7264_USB_Info.pid = USB_STOP_TRANSFER;
    }
    
    wait_4_transmit_completed(pipenum);
    
    g_SH7264_USB_Info.pid = USB_STOP_TRANSFER;
}

BOOL SH7264_USB_BL_Driver::USB_Read_Sectors(UINT32 Sector, UINT32   NumSector, UINT32 Buf)
{
    g_SH7264_USB_Info.recv_data_len = 0;
    Start_Bulk_Transfer(USB_READ10_CMD, Sector, NumSector, Buf);
    if(g_SH7264_USB_Info.filladdress == Buf)
    {
        g_SH7264_USB_Info.error = 1;
        return 0;
    }
    if(g_SH7264_USB_Info.filladdress < (Buf + (NumSector * 0x200)))
        return 1;
    return 1;
}

BOOL SH7264_USB_BL_Driver::USB_Write_Sectors(UINT32 Sector, UINT32  NumSector, UINT32 Buf)
{
    UINT16 pipenum, size, tmp, i, check;
    UINT32 offset, cnt;
    BOOL Ret;

    Ret = TRUE;     

    //return Ret;
    
    /* Send CBW */
    pipenum = USB_PIPE_OUT;
    g_SH7264_USB_Info.recv_data_len = 0;
    
    USB.PIPESEL.BIT.PIPESEL = pipenum;

    Setup_W10_Command(g_SH7264_USB_Info.packetnum, Sector, NumSector); //Prepare CBW

    reg_write(USB_REG_BIT_BCLR, get_fifoctr_addr(pipenum)); //Clear FIFO

    pipe_change(pipenum, 1, 1);
    pipe_start(pipenum);
    pipe_irq_enable(pipenum);
    
    reg_write(~(1 << pipenum), USB_OFFSET_BRDYSTS);
    
    Out_Command_Packet(pipenum);    //Send CBW

    /* Send Data */
    g_SH7264_USB_Info.filladdress = Buf;

    pipe_change(pipenum, 1, 1);
    
    pipe_irq_disable(pipenum);

    pipe_start(pipenum);  
    
    pipe_irq_enable(pipenum);

    g_SH7264_USB_Info.pid = USB_OUT_TRANSFER;
        
    USB_Time_Sleep_MicroSeconds(1000);
        
    while(g_SH7264_USB_Info.pid != USB_STOP_TRANSFER)
    {       
        if(USB.INTSTS0.WORD & 0x0100)   
        {
            check = 1 << pipenum;
            if (USB.BRDYSTS.WORD & check)
            {
                tmp = Get_Control_Reg(pipenum);
                USB.PIPESEL.WORD = pipenum;
                tmp = USB.PIPECFG.WORD;
                size = NumSector * 0x200;
                if ((tmp & USB_REG_BIT_DIR)) {
                    Transfer_Write_Data(pipenum, size);
                }
            }           
        }
        if(USB.INTSTS0.WORD & 0x0400)
        {
            irq_pipe_empty(USB.BEMPSTS.WORD);
        }
        if(USB.INTSTS0.WORD & 0x0200)   
        {
            irq_pipe_nrdy(USB.NRDYSTS.WORD);
            g_SH7264_USB_Info.pid = USB_STOP_TRANSFER;
            reg_mdfy(0, 0x0003, get_pipectr_addr(pipenum));
        }
    }
    
    /* Read CSW */
    g_SH7264_USB_Info.pid = USB_IN_TRANSFER;
    pipenum = USB_PIPE_IN;

    pipe_change(pipenum, 1, 1);
    
    USB_Time_Sleep_MicroSeconds(1000);
    
    pipe_irq_disable(pipenum);

    pipe_start(pipenum);  
    
    pipe_irq_enable(pipenum);

    cnt = 0;
    
    while(1)
    {
        if(USB.INTSTS0.WORD & 0x0100)
        {
            offset = USB_REG_BASE_ADDR + get_fifo_addr(pipenum);
            g_SH7264_USB_Info.filladdress = DataAddress;
            size = 0x0FFF & USB.D1FIFOCTR.WORD;

            for (i=0; (i*2)<size; i++)
            {
                *(UINT16 *)(g_SH7264_USB_Info.filladdress + i*2) = *(UINT16 *)offset;
            }
            //USB_Time_Sleep_MicroSeconds(1000);
            if (size & 1) size += 1;
            g_SH7264_USB_Info.filladdress += size;//0xE;
            if(is_CSW_packet()) {
                break;
            }
            USB_Time_Sleep_MicroSeconds(1000);
        }
        else
        {
            cnt++;
            if (cnt > 0x10000) 
            {
                Ret = FALSE;
                break;
            }
        }           
    }

    /* Clear status and Check error and Return */
    USB.BRDYENB.WORD = 0x0000;
    USB.NRDYENB.WORD = 0x0000;
    USB.BEMPENB.WORD = 0x0000;
    
    if(g_SH7264_USB_Info.packetnum == 0xFFFFFFFF)
        g_SH7264_USB_Info.packetnum = 1;
    else
        g_SH7264_USB_Info.packetnum++;
    
    return Ret;
}

BOOL SH7264_USB_BL_Driver::USB_Erase_Sectors(UINT32 Address, INT32 SectorCount)
{
    return TRUE;
}


void    SH7264_USB_BL_Driver::Get_Descriptor(USB_DEVICE_DESCRIPTOR_INFO *descriptor)
{   
    memcpy(descriptor,(void *)(DataAddress),0x12);
    descriptor->bcdUSB = (descriptor->bcdUSB << 8) | (descriptor->bcdUSB >> 8);
    descriptor->idVendor = (descriptor->idVendor << 8) | (descriptor->idVendor >> 8);
    descriptor->idProduct = (descriptor->idProduct << 8) | (descriptor->idProduct >> 8);
    descriptor->bcdDevice = (descriptor->bcdDevice << 8) | (descriptor->bcdDevice >> 8);
    USB_Time_Sleep_MicroSeconds(100);
}

void    SH7264_USB_BL_Driver::Get_Configuration(USB_DESCRIPTOR_INFO *device)
{
    UINT8   i, j, k;
    UINT32 DataAdd;
    
    memcpy(&device->configuration,(void *)(DataAddress),0x09);
    device->configuration.wTotalLength = (device->configuration.wTotalLength << 8) | (device->configuration.wTotalLength >> 8);
    memcpy(&device->interface,(void *)(DataAddress+9),0x9);
    
    DataAdd = DataAddress + 0x12;
    
    i = 0;
    while ((i < device->interface.bNumEndpoints) && (DataAdd < DataAddress + g_SH7264_USB_Info.recv_total_len))
    {
        j = *(volatile BYTE *)(DataAdd);
        k = *(volatile BYTE *)(DataAdd + 1);
        if ((j == 0x07) && (k == 0x05))
        {
            memcpy(&device->endpoints[i],(void *)(DataAdd),0x7);
            device->endpoints[i].wMaxPacketSize = (device->endpoints[i].wMaxPacketSize << 8) | (device->endpoints[i].wMaxPacketSize >> 8);
            DataAdd += 7;
            i += 1;
        }
        else {
            DataAdd += 1;
        }
    }
        
    USB_Time_Sleep_MicroSeconds(100);
}

void    SH7264_USB_BL_Driver::Start_Bulk_Transfer(UINT8 cmdIndex, UINT32    secNum, UINT16  totalSecs, UINT32   address)
{
    switch (cmdIndex)
    {
        case USB_TEST_UNIT_READY_CMD:
            Setup_TUR_Command(g_SH7264_USB_Info.packetnum);
            break;
        case USB_REQUEST_SENSE_CMD:
            Setup_RS_Command(g_SH7264_USB_Info.packetnum);
            break;
        case USB_INQUIRY_CMD:
            Setup_INQ_Command(g_SH7264_USB_Info.packetnum);
            break;
        case USB_MODE_SENSE_CMD:
            Setup_MS_Command(g_SH7264_USB_Info.packetnum);
            break;
        case USB_PREVENT_ALLOW_REMOVAL_CMD:
            Setup_PAR_Command(g_SH7264_USB_Info.packetnum);
            break;
        case USB_READ_CAPACITY_CMD:
            Setup_RC_Command(g_SH7264_USB_Info.packetnum);
            break;
        case USB_READ10_CMD:
            Setup_R10_Command(g_SH7264_USB_Info.packetnum, secNum, totalSecs);
            break;
    }
    if(g_SH7264_USB_Info.packetnum == 0)
    {
        Clear_SQ_Control_Reg(USB_PIPE_OUT);
        Clear_Pipe_Buffer(USB_PIPE_OUT);
    }
    Out_Control_Packet(USB_PIPE_OUT);
        
    g_SH7264_USB_Info.filladdress = address;

    if(g_SH7264_USB_Info.packetnum == 0)
    {
        Clear_SQ_Control_Reg(USB_PIPE_IN);
        Clear_Pipe_Buffer(USB_PIPE_IN);
    }
    g_SH7264_USB_Info.pid = 1;
    In_Control_Packet(USB_PIPE_IN);
    
    if(g_SH7264_USB_Info.packetnum == 0xFFFFFFFF)
        g_SH7264_USB_Info.packetnum = 1;
    else
        g_SH7264_USB_Info.packetnum++;
}

void    SH7264_USB_BL_Driver::Reset_Command_Buffer()
{
    UINT8   i;
    
    g_SH7264_USB_Info.buff[0] = 0x5553;
    g_SH7264_USB_Info.buff[1] = 0x4243;
    for(i=2; i<18; i++)
    {
        g_SH7264_USB_Info.buff[i] = 0x0000; 
    }
}

void    SH7264_USB_BL_Driver::Setup_INQ_Command(UINT32  packetnum)
{
    Reset_Command_Buffer();
    
    g_SH7264_USB_Info.buff[2] = packetnum >> 16;
    g_SH7264_USB_Info.buff[3] = packetnum;
    g_SH7264_USB_Info.buff[4] = 0x2400;
    g_SH7264_USB_Info.buff[5] = 0x0000;
    g_SH7264_USB_Info.buff[6] = 0x8000;
    g_SH7264_USB_Info.buff[7] = 0x0612;
    g_SH7264_USB_Info.buff[8] = 0x0000;
    g_SH7264_USB_Info.buff[9] = 0x0024;
}

void    SH7264_USB_BL_Driver::Setup_TUR_Command(UINT32  packetnum)
{
    Reset_Command_Buffer();
    
    g_SH7264_USB_Info.buff[2] = packetnum >> 16;
    g_SH7264_USB_Info.buff[3] = packetnum;
    g_SH7264_USB_Info.buff[4] = 0x0000;
    g_SH7264_USB_Info.buff[5] = 0x0000;
    g_SH7264_USB_Info.buff[6] = 0x0000;
    g_SH7264_USB_Info.buff[7] = 0x0600;
}

void    SH7264_USB_BL_Driver::Setup_RS_Command(UINT32   packetnum)
{
    Reset_Command_Buffer();
    
    g_SH7264_USB_Info.buff[2] = packetnum >> 16;
    g_SH7264_USB_Info.buff[3] = packetnum;
    g_SH7264_USB_Info.buff[4] = 0x1200;
    g_SH7264_USB_Info.buff[5] = 0x0000;
    g_SH7264_USB_Info.buff[6] = 0x8000;
    g_SH7264_USB_Info.buff[7] = 0x0603;
    g_SH7264_USB_Info.buff[8] = 0x0000;
    g_SH7264_USB_Info.buff[9] = 0x0012;
}

void    SH7264_USB_BL_Driver::Setup_RC_Command(UINT32   packetnum)
{
    Reset_Command_Buffer();
    
    g_SH7264_USB_Info.buff[2] = packetnum >> 16;
    g_SH7264_USB_Info.buff[3] = packetnum;
    g_SH7264_USB_Info.buff[4] = 0x0800;
    g_SH7264_USB_Info.buff[5] = 0x0000;
    g_SH7264_USB_Info.buff[6] = 0x8000;
    g_SH7264_USB_Info.buff[7] = 0x0A25;
}

void    SH7264_USB_BL_Driver::Setup_MS_Command(UINT32   packetnum)
{
    Reset_Command_Buffer();
    
    g_SH7264_USB_Info.buff[2] = packetnum >> 16;
    g_SH7264_USB_Info.buff[3] = packetnum;
    g_SH7264_USB_Info.buff[4] = 0xC000;
    g_SH7264_USB_Info.buff[5] = 0x0000;
    g_SH7264_USB_Info.buff[6] = 0x8000;
    g_SH7264_USB_Info.buff[7] = 0x061A;
    g_SH7264_USB_Info.buff[8] = 0x003F;
    g_SH7264_USB_Info.buff[9] = 0x00C0;
}

void    SH7264_USB_BL_Driver::Setup_PAR_Command(UINT32  packetnum)
{
    Reset_Command_Buffer();
    
    g_SH7264_USB_Info.buff[2] = packetnum >> 16;
    g_SH7264_USB_Info.buff[3] = packetnum;
    g_SH7264_USB_Info.buff[4] = 0x0000;
    g_SH7264_USB_Info.buff[5] = 0x0000;
    g_SH7264_USB_Info.buff[6] = 0x0000;
    g_SH7264_USB_Info.buff[7] = 0x061E;
    g_SH7264_USB_Info.buff[8] = 0x0000;
    g_SH7264_USB_Info.buff[9] = 0x0001;
}

void    SH7264_USB_BL_Driver::Setup_R10_Command(UINT32  packetnum, UINT32   secnum, UINT16  totalsecs)
{
    UINT32  size;
    
    size = totalsecs * 0x200;
    
    Reset_Command_Buffer();
    
    g_SH7264_USB_Info.buff[2] = packetnum >> 16;
    g_SH7264_USB_Info.buff[3] = packetnum;
    g_SH7264_USB_Info.buff[4] = (((size & 0x000000FF) << 8) | ((size & 0x0000FF00) >> 8));
    g_SH7264_USB_Info.buff[5] = (((size & 0x00FF0000) >> 8) | ((size & 0xFF000000) >> 24));
    g_SH7264_USB_Info.buff[6] = 0x8000;
    g_SH7264_USB_Info.buff[7] = 0x0A28;
    g_SH7264_USB_Info.buff[8] = ((secnum & 0xFF000000) >> 24);
    g_SH7264_USB_Info.buff[9] = (((secnum & 0x00FF0000) >> 8) | ((secnum & 0x0000FF00) >> 8));
    g_SH7264_USB_Info.buff[10] = ((secnum & 0x000000FF) << 8);
    g_SH7264_USB_Info.buff[11] = totalsecs;
}

void    SH7264_USB_BL_Driver::Setup_W10_Command(UINT32 packetnum, UINT32 secnum, UINT16 totalsecs)
{
    UINT32  size;
    
    size = totalsecs * 0x200;
    
    Reset_Command_Buffer();
    
    g_SH7264_USB_Info.buff[2] = packetnum >> 16;
    g_SH7264_USB_Info.buff[3] = packetnum;
    g_SH7264_USB_Info.buff[4] = (((size & 0x000000FF) << 8) | ((size & 0x0000FF00) >> 8));
    g_SH7264_USB_Info.buff[5] = (((size & 0x00FF0000) >> 8) | ((size & 0xFF000000) >> 24));
    g_SH7264_USB_Info.buff[6] = 0x0000;
    g_SH7264_USB_Info.buff[7] = 0x0A2A;
    g_SH7264_USB_Info.buff[8] = ((secnum & 0xFF000000) >> 24);
    g_SH7264_USB_Info.buff[9] = (((secnum & 0x00FF0000) >> 8) | ((secnum & 0x0000FF00) >> 8));
    g_SH7264_USB_Info.buff[10] = ((secnum & 0x000000FF) << 8);
    g_SH7264_USB_Info.buff[11] = totalsecs;
}

void    SH7264_USB_BL_Driver::Reset_Bus(void)
{
    USB.DVSTCTR.WORD |=  (0x1 << 6);     //USB bus reset signal is output
    USB.DVSTCTR.WORD &= ~(0x1 << 4);    //Down port is disabled (SOF/・OF transmission is disabled)
    while(!(USB.DVSTCTR.WORD & 0x0004));
    
    USB_Time_Sleep_MicroSeconds(100000);
    
    USB.DVSTCTR.WORD |=  (0x1 << 4);    //Downstream port is enable (SOF/・OF transmission is enabled)  
    USB.DVSTCTR.WORD &= ~(0x1 << 6);    //USB bus reset signal output is stopped 
    while(USB.DVSTCTR.WORD & 0x0004);

    if((USB.DVSTCTR.WORD & 0x0003) == 0x0002){
        USB.DVSTCTR.WORD |=  (0x1 << 6);     //USB bus reset signal is output
        USB.DVSTCTR.WORD &= ~(0x1 << 4);    //Down port is disabled (SOF/・OF transmission is disabled)
        while(!(USB.DVSTCTR.WORD & 0x0004));
    }
    for(int i=0; i<150; i++) USB_Time_Sleep_MicroSeconds(100000);
    
    USB.DVSTCTR.WORD |=  (0x1 << 4);    //Downstream port is enable (SOF/・OF transmission is enabled)  
    USB.DVSTCTR.WORD &= ~(0x1 << 6);    //USB bus reset signal output is stopped 
    USB_Time_Sleep_MicroSeconds(100000);
}

void    SH7264_USB_BL_Driver::Configure_Pipe5()
{
    USB.PIPESEL.WORD = 5;               //Select pipe 5
    USB.D1FIFOSEL.WORD = (USB.D1FIFOSEL.WORD & ~(0x000F)) | 0x5;        //Select pipe 5 for D1FIFO
    USB_Time_Sleep_MicroSeconds(10000);
    USB.D1FIFOSEL.WORD = (USB.D1FIFOSEL.WORD & ~(0x3 << 10)) | (0x1 << 10);     //Select 16 bits width for FIFO accessing   
    USB.PIPECFG.WORD |= (0x1 << 4);         //Sending (IN transfer)
    USB.PIPECFG.WORD = (USB.PIPECFG.WORD & ~(0x3 << 14)) | (0x2 << 14);     //Interrupt transfer selected
                                                                                                                                            
    USB.PIPEBUF.WORD = 0x4F;            //Select buffer size and buffer number
    USB.PIPEMAXP.WORD = 0x40;           //Select max packet size
    USB.PIPEPERI.WORD = 0;
    USB.D1FIFOCTR.WORD |= (0x1 << 14);      //Clear the buffer on D1FIFO
    
    Clear_Pipe_Buffer(5);
    
    USB.BUSWAIT.WORD = 0x04;        //4 wait cycle (6 access cycles)
}

void    SH7264_USB_BL_Driver::USB_IRQ_Enable(void)
{
    INTC.IPR10.WORD |= (0xA<<12);           //Enable USB interrupt controller
}

void    SH7264_USB_BL_Driver::USB_IRQ_Disable(void)
{
    INTC.IPR10.WORD &= ~(0x9<<12);  //Disable USB interrupt controller
}

void    SH7264_USB_BL_Driver::get_string(char *str, UINT16  len)
{
    UINT16  i;
    UINT32  offset;
    
    offset = DataAddress + 2;
    
    for(i=0; i<len; i++)
    {
        str[i] = *(UINT8 *)(offset + i*2);  
    }
}

void    SH7264_USB_BL_Driver::Configure_Bulk_Transfer(UINT16    pipenum)
{
    UINT8       endpoint, dir, bufnum, index;
    UINT16      TransferType;
    
    if ((pipenum == USB_PIPE_IN)||(pipenum == USB_PIPE_INT_IN))
    {
        if(g_SH7264_USB_Info.disk.endpoints[0].bEndpointAddress & 0x80)
            index = 0;
        else
            index = 1;
    }
    else
    {
        if(g_SH7264_USB_Info.disk.endpoints[0].bEndpointAddress & 0x80)
            index = 1;
        else
            index = 0;  
    }
    
    if ((pipenum == USB_PIPE_IN)||(pipenum == USB_PIPE_OUT))
    {
        TransferType = USB_VAL_TYPE_BULK;
    }
    else
    {
        TransferType = USB_VAL_TYPE_INT | (1 << 10);
    }

    endpoint = g_SH7264_USB_Info.disk.endpoints[index].bEndpointAddress & 0x0F;
    dir = g_SH7264_USB_Info.disk.endpoints[index].bEndpointAddress & 0x80;
    
    USB.PIPESEL.WORD = pipenum;

        pipe_change(pipenum, 1, 1);
    
    if (dir)
        reg_bclr(USB_REG_BIT_DIR, USB_OFFSET_PIPECFG);
    else
        reg_bset(USB_REG_BIT_DIR, USB_OFFSET_PIPECFG);
        
    reg_bset(TransferType, USB_OFFSET_PIPECFG);
    reg_mdfy(endpoint, USB_REG_BIT_EPNUM, USB_OFFSET_PIPECFG);

    bufnum = 0x26 + index * 16;

    reg_bset(USB_REG_BIT_CNTMD | USB_REG_BIT_SHTNAK, USB_OFFSET_PIPECFG);
    reg_write((0x1F << 10) | (bufnum), USB_OFFSET_PIPEBUF);
    reg_write((g_SH7264_USB_Info.device_address << 12) | g_SH7264_USB_Info.disk.endpoints[index].wMaxPacketSize, USB_OFFSET_PIPEMAXP);
    reg_write(0, USB_OFFSET_PIPEPERI);
    reg_write(USB_REG_BIT_BCLR, get_fifoctr_addr(pipenum)); 
}
