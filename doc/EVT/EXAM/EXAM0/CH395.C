/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395功能演示程序
**********************************************************************************/

/**********************************************************************************
CH395 TCP/IP 协议族接口
MSC51 演示程序，演示4个socket 分别使用UDP,TCP Client,TCP Server以及IP RAW ,单片机
收到数据后，按位取反后上传。MCS51@24MHZ,KEIL 4
**********************************************************************************/
/* 头文件包含*/
#include <reg52.h>
#include "stdio.h"
#include "string.h"
#include "../PUB/CH395INC.H"
#include "CH395.H"

/**********************************************************************************/
/*
CH395_OP_INTERFACE_MODE可以为1-5
1：硬件总线并口连接方式
2：软件模拟并口连接方式
3: 硬件SPI连接方式
4: 软件模拟SPI方式
5: 硬件异步串口连接方式
*/
#define   CH395_OP_INTERFACE_MODE             3                     
#if   (CH395_OP_INTERFACE_MODE == 1)                                 /* SEL = 0, TX = 1*/
#include "../PUB/CH395PARA_HW.C"                                           
#elif (CH395_OP_INTERFACE_MODE == 2)                                 /* SEL = 0, TX = 1*/
#include "../PUB/CH395PARA_SW.C"                                            
#elif (CH395_OP_INTERFACE_MODE == 3)                                 /* SEL = 1, TX = 0*/
#include "../PUB/CH395SPI_HW.C"
#elif (CH395_OP_INTERFACE_MODE == 4)                                 /* SEL = 1, TX = 0*/
#include "../PUB/CH395SPI_SW.C"
#elif (CH395_OP_INTERFACE_MODE == 5)                                 /* SEL = 1, TX = 1*/
#include "../PUB/CH395UART.C"
#else
#error "Please Select Correct Communication Interface "
#endif

/**********************************************************************************/
/* 包含命令文件 */
#include "../PUB/CH395CMD.C"

#define     CH395_DEBUG                0
/* 常用变量定义 */
UINT8 xdata MyBuffer[4][100];                                        /* 数据缓冲区 ,此数据缓冲区长度可根据单片机资源适当调大,以提高通讯速度*/
struct _SOCK_INF xdata SockInf[4];                                   /* 保存Socket信息 */
struct _CH395_SYS xdata CH395Inf;                                    /* 保存CH395信息 */

/* CH395相关定义 */
const UINT8 CH395IPAddr[4] = {192,168,1,10};                         /* CH395IP地址 */
const UINT8 CH395GWIPAddr[4] = {192,168,1,1};                        /* CH395网关 */
const UINT8 CH395IPMask[4] = {255,255,255,0};                        /* CH395子网掩码 */

/* socket 相关定义,为方便演示，socket0-2定义了源端口和目的端口,socket3 */
/* 为IP RAW，需要定义IP 协议字段的协议类型 */
const UINT8  Socket0DesIP[4] = {192,168,1,100};                      /* Socket 0目的IP地址 */
const UINT16 Socket0DesPort = 1000;                                  /* Socket 0目的端口 */
const UINT16 Socket0SourPort = 5000;                                 /* Socket 0源端口 */

const UINT8  Socket1DesIP[4] = {192,168,1,100};                      /* Socket 1目的IP地址 */
const UINT16 Socket1SourPort = 6000;                                 /* Socket 1源端口 */

const UINT8  Socket2DesIP[4] = {192,168,1,100};                      /* Socket 2目的IP地址 */
const UINT16 Socket2DesPort = 3000;                                  /* Socket 2目的端口 */
const UINT16 Socket2SourPort = 7000;                                 /* Socket 2源端口 */

const UINT8  Socket3DesIP[4] = {192,168,1,100};                      /* Socket 3目的IP地址 */
const UINT8  IPRawProto = 0xE2; 
                                                                     /* IP包协议类型 */

/**********************************************************************************
* Function Name  : mStopIfError
* Description    : 调试使用，显示错误代码，并停机
* Input          : iError   错误代码
* Output         : None
* Return         : None
**********************************************************************************/
void mStopIfError(UINT8 iError)
{
    if (iError == CMD_ERR_SUCCESS) return;                           /* 操作成功 */
#if CH395_DEBUG
    printf("Error: %02X\n", (UINT16)iError);                         /* 显示错误 */
#endif
    while ( 1 ) 
    {
        mDelaymS(200);
        mDelaymS(200);
    }
}

/**********************************************************************************
* Function Name  : InitCH395InfParam
* Description    : 初始化CH395Inf参数
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitCH395InfParam(void)
{
    memset(&CH395Inf,0,sizeof(CH395Inf));                            /* 将CH395Inf全部清零*/
    memcpy(CH395Inf.IPAddr,CH395IPAddr,sizeof(CH395IPAddr));         /* 将IP地址写入CH395Inf中 */
    memcpy(CH395Inf.GWIPAddr,CH395GWIPAddr,sizeof(CH395GWIPAddr));   /* 将网关IP地址写入CH395Inf中 */
    memcpy(CH395Inf.MASKAddr,CH395IPMask,sizeof(CH395IPMask));       /* 将子网掩码写入CH395Inf中 */
}

/**********************************************************************************
* Function Name  : InitSocketParam
* Description    : 初始化socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitSocketParam(void)
{
    memset(&SockInf[0],0,sizeof(SockInf[0]));                        /* 将SockInf[0]全部清零*/
    memcpy(SockInf[0].IPAddr,Socket0DesIP,sizeof(Socket0DesIP));     /* 将目的IP地址写入 */
    SockInf[0].DesPort = Socket0DesPort;                             /* 目的端口 */
    SockInf[0].SourPort = Socket0SourPort;                           /* 源端口 */
    SockInf[0].ProtoType = PROTO_TYPE_UDP;                           /* UDP模式 */

    memset(&SockInf[1],0,sizeof(SockInf[1]));                        /* 将SockInf[1]全部清零*/
    SockInf[1].SourPort = Socket1SourPort;                           /* 源端口 */
    SockInf[1].ProtoType = PROTO_TYPE_TCP;                           /* TCP模式 */
    SockInf[1].TcpMode = TCP_SERVER_MODE;                            /* TCP服务器模式 */

    memset(&SockInf[2],0,sizeof(SockInf[2]));                        /* 将SockInf[2]全部清零*/
    memcpy(SockInf[2].IPAddr,Socket2DesIP,sizeof(Socket2DesIP));     /* 将目的IP地址写入 */
    SockInf[2].DesPort = Socket2DesPort;                             /* 目的端口 */
    SockInf[2].SourPort = Socket2SourPort;                           /* 源端口 */
    SockInf[2].ProtoType = PROTO_TYPE_TCP;                           /* TCP模式 */
    SockInf[2].TcpMode = TCP_CLIENT_MODE;                            /* TCP客户端模式 */

    memset(&SockInf[3],0,sizeof(SockInf[3]));                        /* 将SockInf[3]全部清零*/
    memcpy(SockInf[3].IPAddr,Socket3DesIP,sizeof(Socket3DesIP));     /* 将目的IP地址写入 */
    SockInf[3].ProtoType = PROTO_TYPE_IP_RAW;                        /* IPRAW */
}

/**********************************************************************************
* Function Name  : CH395SocketInitOpen
* Description    : 配置CH395 socket 参数，初始化并打开socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInitOpen(void)
{
    UINT8 i;

    /* socket 0为UDP模式 */
    CH395SetSocketDesIP(0,SockInf[0].IPAddr);                        /* 设置socket 0目标IP地址 */         
    CH395SetSocketProtType(0,PROTO_TYPE_UDP);                        /* 设置socket 0协议类型 */
    CH395SetSocketDesPort(0,SockInf[0].DesPort);                     /* 设置socket 0目的端口 */
    CH395SetSocketSourPort(0,SockInf[0].SourPort);                   /* 设置socket 0源端口 */
    i = CH395OpenSocket(0);                                          /* 打开socket 0 */
    mStopIfError(i);

    /* socket 1为TCP Server模式,无需设置目的IP和源端口 */
    CH395SetSocketProtType(1,PROTO_TYPE_TCP);                        /* 设置socket 1协议类型 */
    CH395SetSocketSourPort(1,SockInf[1].SourPort);                   /* 设置socket 1源端口 */
    i = CH395OpenSocket(1);                                          /* 打开socket 1 */
    mStopIfError(i);                                                 /* 检查是否成功 */
    i = CH395TCPListen (1);                                          /* 开始监听 */
    mStopIfError(i);                                                 /* 检查是否成功 */

    /* socket 2为TCP Client模式 */
    CH395SetSocketDesIP(2,SockInf[2].IPAddr);                        /* 设置socket 2目标IP地址 */
    CH395SetSocketProtType(2,PROTO_TYPE_TCP);                        /* 设置socket 2协议类型 */
    CH395SetSocketDesPort(2,SockInf[2].DesPort);                     /* 设置socket 2目的端口 */
    CH395SetSocketSourPort(2,SockInf[2].SourPort);                   /* 设置socket 2源端口 */
    i = CH395OpenSocket(2);                                          /* 打开socket 2 */
    mStopIfError(i);                                                 /* 检查是否成功 */
    i = CH395TCPConnect(2);                                          /* 开始连接 */
    mStopIfError(i);                                                 /* 检查是否成功,如果失败，则socket会关闭 */

    /* socket 3为IP RAW模式 */
    CH395SetSocketDesIP(3,SockInf[3].IPAddr);                        /* 设置socket 3目标IP地址 */
    CH395SetSocketProtType(3,PROTO_TYPE_IP_RAW);                     /* 设置IP RAW模式下的IP包协议字段 */
    CH395SetSocketIPRAWProto(3,IPRawProto);                          /* 设置协议字段 */
    i = CH395OpenSocket(3);                                          /* 打开socket 3 */
    mStopIfError(i);                                                 /* 检查是否成功 */
}

/**********************************************************************************
* Function Name  : CH395SocketInterrupt
* Description    : CH395 socket 中断,在全局中断中被调用
* Input          : sockindex  Socket索引
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInterrupt(UINT8 sockindex)
{
    UINT8  sock_int_socket;
    UINT8  i;
    UINT16 len;
    UINT16 tmp;
    UINT8 idata buf[10];

    sock_int_socket = CH395GetSocketInt(sockindex);                  /* 获取socket 的中断状态 */
    if(sock_int_socket & SINT_STAT_SENBUF_FREE)                      /* 发送缓冲区空闲，可以继续写入要发送的数据 */
    {

    }
    if(sock_int_socket & SINT_STAT_SEND_OK)                          /* 发送完成中断 */
    {
    }
    if(sock_int_socket & SINT_STAT_RECV)                             /* 接收中断 */
    {
        len = CH395GetRecvLength(sockindex);                         /* 获取当前缓冲区内数据长度 */
#if CH395_DEBUG
        printf("receive len = %d\n"	,len);
#endif
        if(len == 0)return;
        if(len > 100)len = 100;                                      /* 发送缓冲区最大为2048 */
        CH395GetRecvData(sockindex,len,MyBuffer[sockindex]);         /* 读取数据 */
        for(tmp =0; tmp < len; tmp++)                                /* 将所有数据按位取反 */
        {
            MyBuffer[sockindex][tmp] = ~MyBuffer[sockindex][tmp];
        }
        CH395SendData(sockindex,MyBuffer[sockindex],len);

   }
   if(sock_int_socket & SINT_STAT_CONNECT)                            /* 连接中断，仅在TCP模式下有效*/
   {
#if CH395_DEBUG
       printf("Tcp Connect\n");
#endif
       if(SockInf[sockindex].TcpMode == TCP_SERVER_MODE)              /* 如果socket 为服务器模式，用户可以获取远端的IP和端口*/
       {
           CH395CMDGetRemoteIPP(sockindex,buf);
           tmp = (UINT16)(buf[5]<<8) + buf[4];
#if CH395_DEBUG
	       printf("IP address = %d.%d.%d.%d\n",(UINT16)buf[0],(UINT16)buf[1],(UINT16)buf[2],(UINT16)buf[3]);                    
	       printf("Port = %d\n",tmp);    
#endif

       }
   }
   /*
   **产生断开连接中断和超时中断时，CH395默认配置是内部主动关闭，用户不需要自己关闭该Socket，如果想配置成不主动关闭Socket需要配置
   **SOCK_CTRL_FLAG_SOCKET_CLOSE标志位（默认为0），如果该标志为1，CH395内部不对Socket进行关闭处理，用户在连接中断和超时中断时调用
   **CH395CloseSocket函数对Socket进行关闭，如果不关闭则该Socket一直为连接的状态（事实上已经断开），就不能再去连接了。
   */
   if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* 断开中断，仅在TCP模式下有效 */
   {
		//i = CH395CloseSocket(sockindex);                             
		//mStopIfError(i);
   }
   if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* 超时中断，仅在TCP模式下有效 */
   {
		//i = CH395CloseSocket(sockindex);
		//mStopIfError(i);
       if(SockInf[sockindex].TcpMode == TCP_CLIENT_MODE)             
       {
           mDelaymS(200);                                            /* 延时200MS后再次重试，没有必要过于频繁连接 */
           i = CH395OpenSocket(sockindex);
           mStopIfError(i);
           CH395TCPConnect(sockindex);                               /* 开始连接 */
           mStopIfError(i);
       }
    }
}

/********************************************************************************
* Function Name  : CH395GlobalInterrupt
* Description    : CH395全局中断函数
* Input          : None
* Output         : None
* Return         : None
*********************************************************************************/
void CH395GlobalInterrupt(void)
{
   UINT16  init_status;
   UINT8  buf[10]; 
 
    init_status = CH395CMDGetGlobIntStatus_ALL();
    if(init_status & GINT_STAT_UNREACH)                              /* 不可达中断，读取不可达信息 */
    {
        CH395CMDGetUnreachIPPT(buf);           
    }
    if(init_status & GINT_STAT_IP_CONFLI)                            /* 产生IP冲突中断，建议重新修改CH395的 IP，并初始化CH395*/
    {
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                           /* 产生PHY改变中断*/
    {
#if CH395_DEBUG
        printf("Init status : GINT_STAT_PHY_CHANGE\n");
#endif
	}
    if(init_status & GINT_STAT_SOCK0)
    {
        CH395SocketInterrupt(0);                                     /* 处理socket 0中断*/
    }
    if(init_status & GINT_STAT_SOCK1)                               
    {
        CH395SocketInterrupt(1);                                     /* 处理socket 1中断*/
    }
    if(init_status & GINT_STAT_SOCK2)                                
    {
        CH395SocketInterrupt(2);                                     /* 处理socket 2中断*/
    }
    if(init_status & GINT_STAT_SOCK3)                                
    {
        CH395SocketInterrupt(3);                                     /* 处理socket 3中断*/
    }
    if(init_status & GINT_STAT_SOCK4)
    {
        CH395SocketInterrupt(4);                                     /* 处理socket 4中断*/
    }
    if(init_status & GINT_STAT_SOCK5)                                
    {
        CH395SocketInterrupt(5);                                     /* 处理socket 5中断*/
    }
    if(init_status & GINT_STAT_SOCK6)                                
    {
        CH395SocketInterrupt(6);                                     /* 处理socket 6中断*/
    }
    if(init_status & GINT_STAT_SOCK7)                                
    {
        CH395SocketInterrupt(7);                                     /* 处理socket 7中断*/
    }
}

/**********************************************************************************
* Function Name  : CH395Init
* Description    : 配置CH395的IP,GWIP,MAC等参数，并初始化
* Input          : None
* Output         : None
* Return         : 函数执行结果
**********************************************************************************/
UINT8  CH395Init(void)
{
    UINT8 i;
    
    i = CH395CMDCheckExist(0x65);   
    if(i != 0x9a)return CH395_ERR_UNKNOW;                            /* 测试命令，如果无法通过返回0XFA */
                                                                     /* 返回0XFA一般为硬件错误或者读写时序不对 */
#if (CH395_OP_INTERFACE_MODE == 5)                                   
#ifdef UART_WORK_BAUDRATE
    CH395CMDSetUartBaudRate(UART_WORK_BAUDRATE);                     /* 设置波特率 */   
    mDelaymS(1);
    SetMCUBaudRate();
#endif
#endif
    CH395CMDSetIPAddr(CH395Inf.IPAddr);                              /* 设置CH395的IP地址 */
    CH395CMDSetGWIPAddr(CH395Inf.GWIPAddr);                          /* 设置网关地址 */
    CH395CMDSetMASKAddr(CH395Inf.MASKAddr);                          /* 设置子网掩码，默认为255.255.255.0*/   
    mDelaymS(10);
    i = CH395CMDInitCH395();                                         /* 初始化CH395芯片 */
    return i;
}

/**********************************************************************************
* Function Name  : mInitSTDIO
* Description    : 串口初始化,仅调试使用
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void mInitSTDIO( void )
{
    SCON = 0x50;
    PCON = 0x80;
    TMOD = 0x21;
    TH1 = 0xf3;                                                      /* 24MHz晶振, 9600bps */
    TR1 = 1;
    TI = 1;
}

/**********************************************************************************
* Function Name  : main
* Description    : main主函数
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
int main(void)
{
    UINT8 i;
    mDelaymS(100);
    mInitSTDIO();                                                    /* 延时100毫秒 */ 
#if CH395_DEBUG
    printf("CH395EVT Test Demo\n");
#endif
    CH395_PORT_INIT();
    InitCH395InfParam();                                             /* 初始化CH395相关变量 */
    i = CH395Init();                                                 /* 初始化CH395芯片 */
    mStopIfError(i);                                                                     
  
    while(1)
    {                                                                /* 等待以太网连接成功*/
       if(CH395CMDGetPHYStatus() == PHY_DISCONN)                     /* 查询CH395是否连接 */
       {
           mDelaymS(200);                                            /* 未连接则等待200MS后再次查询 */
       }
       else 
       {
#if CH395_DEBUG
           printf("CH395 Connect Ethernet\n");                       /* CH395芯片连接到以太网，此时会产生中断 */
#endif
           break;
       }
    }
    InitSocketParam();                                                /* 初始化socket相关变量 */
    CH395SocketInitOpen();
    while(1)
    {
       if(CH395_INT_WIRE == 0)CH395GlobalInterrupt();
    }
}


