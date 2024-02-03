/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395������ʾ
**********************************************************************************/

/**********************************************************************************
CH395 TCP/IP Э����ӿ�
MSC51 ��ʾ����������ʾSocket0������TCP Serverģʽ�£���Ƭ���յ����ݺ󣬰�λȡ����
�ϴ����˹��ܽ�0x44�汾����֧�֣�������������ϵ����֧�����䣺tech@wch.cn��
MCS51@24MHZ,KEIL 3.51
**********************************************************************************/
/* ͷ�ļ�����*/
#include <reg52.h>
#include "stdio.h"
#include "string.h"
#include "../PUB/CH395INC.H"
#include "CH395.H"

/***********************************************************************************
/*
CH395_OP_INTERFACE_MODE����Ϊ1-5
1��Ӳ�����߲������ӷ�ʽ
2������ģ�Ⲣ�����ӷ�ʽ
3: Ӳ��SPI���ӷ�ʽ
4: ����ģ��SPI��ʽ
5: Ӳ���첽�������ӷ�ʽ
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
/* ���������ļ� */
#include "../PUB/CH395CMD.C"

#define  CH395_DEBUG                1

#define  SocketNum                  8                                /*TCP SERVER ֧�ֵ�Socket������(SocketNum -1)*/
#define  TcpMss                     536                              /*����TCPMSS*/
/* ���ñ������� */
UINT8 xdata MyBuffer[SocketNum-1][TcpMss*2];                         /* ���ݻ����� */
struct _SOCK_INF xdata SockInf;                                      /* ����Socket��Ϣ */
struct _CH395_SYS xdata CH395Inf;                                    /* ����CH395��Ϣ */

/* CH395��ض��� */
const UINT8 CH395IPAddr[4] = {192,168,1,10};                         /* CH395IP��ַ */
const UINT8 CH395GWIPAddr[4] = {192,168,1,1};                        /* CH395���� */
const UINT8 CH395IPMask[4] = {255,255,255,0};                        /* CH395�������� */

/* socket ��ض���*/
const UINT16 Socket0SourPort = 5000;                                 /* Socket 0Դ�˿� */

/**********************************************************************************
* Function Name  : mStopIfError
* Description    : ����ʹ�ã���ʾ������룬��ͣ��
* Input          : iError
* Output         : None
* Return         : None
**********************************************************************************/
void mStopIfError(UINT8 iError)
{
    if (iError == CMD_ERR_SUCCESS) return;                           /* �����ɹ� */
#if CH395_DEBUG
    printf("Error: %02X\n", (UINT16)iError);                         /* ��ʾ���� */
#endif
    while ( 1 ) 
    {
        mDelaymS(200);
        mDelaymS(200);
    }
}

/**********************************************************************************
* Function Name  : InitCH395InfParam
* Description    : ��ʼ��CH395Inf����
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitCH395InfParam(void)
{
    memset(&CH395Inf,0,sizeof(CH395Inf));                            /* ��CH395Infȫ������*/
    memcpy(CH395Inf.IPAddr,CH395IPAddr,sizeof(CH395IPAddr));         /* ��IP��ַд��CH395Inf�� */
    memcpy(CH395Inf.GWIPAddr,CH395GWIPAddr,sizeof(CH395GWIPAddr));   /* ������IP��ַд��CH395Inf�� */
    memcpy(CH395Inf.MASKAddr,CH395IPMask,sizeof(CH395IPMask));       /* ����������д��CH395Inf�� */
}

/**********************************************************************************
* Function Name  : InitSocketParam
* Description    : ��ʼ��socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitSocketParam(void)
{
    memset(&SockInf,0,sizeof(SockInf));                              /* ��SockInf[0]ȫ������*/
    SockInf.SourPort = Socket0SourPort;                              /* Դ�˿� */
    SockInf.ProtoType = PROTO_TYPE_TCP;                              /* TCPģʽ */
    SockInf.TcpMode = TCP_SERVER_MODE;
}

/**********************************************************************************
* Function Name  : CH395SocketInitOpen
* Description    : ����CH395 socket ��������ʼ������socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInitOpen(void)
{
    UINT8 i;
    /* socket 0ΪTCP ������ģʽ */
    CH395SetSocketProtType(0,SockInf.ProtoType);                       /* ����socket 0Э������ */
    CH395SetSocketSourPort(0,SockInf.SourPort);                        /* ����socket 0Դ�˿� */
    i = CH395OpenSocket(0);                                            /* ��socket 0 */
    mStopIfError(i);                                                   /* ����Ƿ�ɹ� */
    i = CH395TCPListen(0);                                             /* TCP���� */
    mStopIfError(i); 
  
  for(i=1;i<SocketNum;i++)
  {
      CH395SetSocketProtType(i,SockInf.ProtoType);                     /* ����socket 0Э������ */
      CH395SetSocketSourPort(i,SockInf.SourPort);                      /* ����socket 0Դ�˿� */
  }                                                
}

/**********************************************************************************
* Function Name  : CH395SocketInterrupt
* Description    : CH395 socket �ж�,��ȫ���ж��б�����
* Input          : sockindex
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInterrupt(UINT8 sockindex)
{
   UINT8  sock_int_socket,i;
   UINT16 len;
   UINT16 tmp;
   UINT8 idata buf[10];

   sock_int_socket = CH395GetSocketInt(sockindex);                   /* ��ȡsocket ���ж�״̬ */
   if(sock_int_socket & SINT_STAT_SENBUF_FREE)                       /* ���ͻ��������У����Լ���д��Ҫ���͵����� */
   {
   }
   if(sock_int_socket & SINT_STAT_SEND_OK)                           /* ��������ж� */
   {
   }
   if(sock_int_socket & SINT_STAT_RECV)                              /* �����ж� */
   {
       len = CH395GetRecvLength(sockindex);                          /* ��ȡ��ǰ�����������ݳ��� */
#if CH395_DEBUG
       printf("receive len = %d\n",len);
#endif
       if(len == 0)return;
       if(len > 100)len = 100;                                       /* MyBuffer����������Ϊ512*/
       CH395GetRecvData(sockindex,len,MyBuffer[sockindex]);          /* ��ȡ���� */
       CH395SendData(sockindex,MyBuffer[sockindex],len);
   }
   if(sock_int_socket & SINT_STAT_CONNECT)                           /* �����жϣ�����TCPģʽ����Ч*/
   {
#if CH395_DEBUG
       printf("Tcp Connect\n");
#endif
       if(SockInf.TcpMode == TCP_SERVER_MODE)                       /* ���socket Ϊ������ģʽ���û����Ի�ȡԶ�˵�IP�Ͷ˿�*/
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
   **�����Ͽ������жϺͳ�ʱ�ж�ʱ��CH395Ĭ���������ڲ������رգ��û�����Ҫ�Լ��رո�Socket����������óɲ������ر�Socket��Ҫ����
   **SOCK_CTRL_FLAG_SOCKET_CLOSE��־λ��Ĭ��Ϊ0��������ñ�־Ϊ1��CH395�ڲ�����Socket���йرմ������û��������жϺͳ�ʱ�ж�ʱ����
   **CH395CloseSocket������Socket���йرգ�������ر����SocketһֱΪ���ӵ�״̬����ʵ���Ѿ��Ͽ������Ͳ�����ȥ�����ˡ�
   */
   /*SOCK_CTRL_FLAG_SOCKET_CLOSE����λ��������ж���Ҫ�ر�Socket���ٽ�����������������λ����ҪCH395CloseSocket��CH395�ڲ��Ѿ��رա�*/
   if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* �Ͽ��жϣ�����TCPģʽ����Ч */
   {
	   
	   i = CH395CloseSocket(sockindex);                             
		mStopIfError(i);
	   mDelaymS(2);
	   i = CH395OpenSocket(sockindex);
	   mStopIfError(i);
   }
	if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* ��ʱ�жϣ�����TCPģʽ����Ч */
	{
		i = CH395CloseSocket(sockindex);                             
		mStopIfError(i);
		mDelaymS(2);
		i = CH395OpenSocket(sockindex);
		mStopIfError(i);
	}
}

/**********************************************************************************
* Function Name  : CH395GlobalInterrupt
* Description    : CH395ȫ���жϺ���
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395GlobalInterrupt(void)
{
   UINT16  init_status;
   UINT8  buf[10]; 
 
    init_status = CH395CMDGetGlobIntStatus_ALL();
    if(init_status & GINT_STAT_UNREACH)                              /* ���ɴ��жϣ���ȡ���ɴ���Ϣ */
    {
        CH395CMDGetUnreachIPPT(buf);                                
    }
    if(init_status & GINT_STAT_IP_CONFLI)                            /* ����IP��ͻ�жϣ����������޸�CH395�� IP������ʼ��CH395*/
    {
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                           /* ����PHY�ı��ж�*/
    {
#if CH395_DEBUG
        printf("Init status : GINT_STAT_PHY_CHANGE\n");
#endif
    }
    if(init_status & GINT_STAT_SOCK0)
    {
        CH395SocketInterrupt(0);                                     /* ����socket 0�ж�*/
    }
    if(init_status & GINT_STAT_SOCK1)                                
    {
        CH395SocketInterrupt(1);                                     /* ����socket 1�ж�*/
    }
    if(init_status & GINT_STAT_SOCK2)                                
    {
        CH395SocketInterrupt(2);                                     /* ����socket 2�ж�*/
    }
    if(init_status & GINT_STAT_SOCK3)                                
    {
        CH395SocketInterrupt(3);                                     /* ����socket 3�ж�*/
    }
    if(init_status & GINT_STAT_SOCK4)
    {
        CH395SocketInterrupt(4);                                     /* ����socket 4�ж�*/
    }
    if(init_status & GINT_STAT_SOCK5)                                
    {
        CH395SocketInterrupt(5);                                     /* ����socket 5�ж�*/
    }
    if(init_status & GINT_STAT_SOCK6)                               
    {
        CH395SocketInterrupt(6);                                     /* ����socket 6�ж�*/
    }
    if(init_status & GINT_STAT_SOCK7)                               
    {
        CH395SocketInterrupt(7);                                     /* ����socket 7�ж�*/
    }
}

/**********************************************************************************
* Function Name  : CH395Init
* Description    : ����CH395��IP,GWIP,MAC�Ȳ���������ʼ��
* Input          : None
* Output         : None
* Return         : ����ִ�н��
**********************************************************************************/
UINT8  CH395Init(void)
{
    UINT8 i;
    
    i = CH395CMDCheckExist(0x65);                      
    if(i != 0x9a)return CH395_ERR_UNKNOW;                            /* �����������޷�ͨ������0XFA */
#if (CH395_OP_INTERFACE_MODE == 5)                                   
#ifdef UART_WORK_BAUDRATE
    CH395CMDSetUartBaudRate(UART_WORK_BAUDRATE);                     /* ���ò����� */   
    mDelaymS(1);
    SetMCUBaudRate();
#endif
#endif
    CH395CMDSetIPAddr(CH395Inf.IPAddr);                              /* ����CH395��IP��ַ */
    CH395CMDSetGWIPAddr(CH395Inf.GWIPAddr);                          /* �������ص�ַ */
    CH395CMDSetMASKAddr(CH395Inf.MASKAddr);                          /* �����������룬Ĭ��Ϊ255.255.255.0*/   
	/*
	**SOCK_CTRL_FLAG_SOCKET_CLOSE�ñ�־λ��1��Ĭ��Ϊ0����CH395�ڲ��������ر�Socket��һЩ�쳣�ж����磺��ʱ���Ͽ�����ʱ����Ҫ�����йر�Socket
	**������رգ���Socketһֱ��ռ�ã������ͷš�
	*/
    CH395SetStartPara(FUN_PARA_FLAG_TCP_SERVER|SOCK_CTRL_FLAG_SOCKET_CLOSE);                     /*TCP SERVER֧�ֶ�����ʱ�����ʼ������������*/
    mDelaymS(100);
    i = CH395CMDInitCH395();                                         /* ��ʼ��CH395оƬ */
    return i;
}

/*********************************************************************************
* Function Name  : mInitSTDIO
* Description    : ���ڳ�ʼ��,������ʹ��
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void mInitSTDIO( void )
{
    SCON = 0x50;
    PCON = 0x80;
    TMOD = 0x21;
    TH1 = 0xf3;                                                      /* 24MHz����, 9600bps */
    TR1 = 1;
    TI = 1;
}

/*********************************************************************************
* Function Name  : Socket_R_S_Buf_Modify
* Description    : Socket�շ�����������
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void Socket_R_S_Buf_Modify(void )
{

   CH395SetSocketRecvBuf(0,0,4);                                     /* Socket 0 �����ջ�����4*512 = 2K�����ͻ�����2*512 = 1K*/
   CH395SetSocketSendBuf(0,4,2);  
  
   CH395SetSocketRecvBuf(1,6,4);                                     /* Socket 1 */
   CH395SetSocketSendBuf(1,10,2);  
  
   CH395SetSocketRecvBuf(2,12,4);                                    /* Socket 2 */
   CH395SetSocketSendBuf(2,16,2);  
  
   CH395SetSocketRecvBuf(3,18,4);                                    /* Socket 3 */
   CH395SetSocketSendBuf(3,22,2);  
  
   CH395SetSocketRecvBuf(4,24,4);                                    /* Socket 4 */
   CH395SetSocketSendBuf(4,28,2);  
  
   CH395SetSocketRecvBuf(5,30,4);                                    /* Socket 5 */
   CH395SetSocketSendBuf(5,34,2);
  
   CH395SetSocketRecvBuf(6,36,4);                                    /* Socket 6 */
   CH395SetSocketSendBuf(6,40,2);  
  
   CH395SetSocketRecvBuf(7,42,4);                                    /* Socket 7 */
   CH395SetSocketSendBuf(7,46,2);  

}
/**********************************************************************************
* Function Name  : main
* Description    : main������
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
int main(void)
{
    UINT8 i;
 
    mDelaymS(100);
    mInitSTDIO();                                                    /* ��ʱ100���� */
#if CH395_DEBUG
    printf("CH395EVT Test Demo\n");
#endif
    CH395_PORT_INIT();
 
    i =   CH395CMDGetVer();
    if(i < 0x44)                                                     /* TCP SERVER������ģʽ��0X44�����ϰ汾֧�֣�0x44���°汾TCP SERVERֻ֧��һ������ */
    {
#if CH395_DEBUG
     printf("Error: Not Support!\n");                                /* ��ʾ���� */
#endif
     while ( 1 ) 
     {
        mDelaymS(200);
        mDelaymS(200);
     }   
    }
    InitCH395InfParam();                                             /* ��ʼ��CH395��ر��� */
    i = CH395Init();                                                 /* ��ʼ��CH395оƬ */
    mStopIfError(i);
 
    Socket_R_S_Buf_Modify();                                         /* ����Socket���ͺͽ��ջ�����*/
    CH395SetTCPMss(TcpMss);                                          /* ����TCPMSS*/
                                                                    
    while(1)
    {                                                                /* �ȴ���̫�����ӳɹ�*/
       if(CH395CMDGetPHYStatus() == PHY_DISCONN)                     /* ��ѯCH395�Ƿ����� */
       {
           mDelaymS(200);                                            /* δ������ȴ�200MS���ٴβ�ѯ */
       }
       else 
       {
#if CH395_DEBUG
           printf("CH395 Connect Ethernet\n");                       /* CH395оƬ���ӵ���̫������ʱ������ж� */
#endif
           break;
       }
    }
    InitSocketParam();                                                /* ��ʼ��socket��ر��� */
    CH395SocketInitOpen();
    while(1)
    {
       if(CH395_INT_WIRE == 0)CH395GlobalInterrupt();
    }
}
