/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395������ʾ
**********************************************************************************/

/**********************************************************************************
CH395 TCP/IP Э����ӿ�
MSC51 ��ʾ����������ʾSocket0������MAC RAWģʽ�£���Ƭ���յ����ݺ�ֱ��
�ϴ���MCS51@24MHZ,KEIL 4
**********************************************************************************/
/* ͷ�ļ�����*/
#include <reg52.h>
#include "stdio.h"
#include "string.h"
#include "../PUB/CH395INC.H"
#include "CH395.H"

/**********************************************************************************
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
/* ���ñ������� */
UINT8 xdata MyBuffer[512];
struct _SOCK_INF  xdata  SockInf;
struct _CH395_SYS xdata  CH395Inf;
/* CH395��ض��� */
const UINT8 CH395MACAddr[6] = {0x02,0x03,0x04,0x05,0x06,0x07};       /* CH395MAC��ַ */
/*ע��CH395����ʱ����¼MAC��ַ���˴�����MAC��ַ��ҪΪ��ʾ��������������ʹ��ʱ��ֱ�ӵ��û�ȡMAC��ַ���������������MAC��ַ*/

#define  CH395_DEBUG                     0
/**********************************************************************************
* Function Name  : mStopIfError
* Description    : ����ʹ�ã���ʾ������룬��ͣ��
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void mStopIfError(UINT8 iError)
{
    if (iError == CMD_ERR_SUCCESS) return;                           /* �����ɹ� */
#if  CH395_DEBUG
    printf("Error: %02X\n", (UINT16)iError);                         /* ��ʾ���� */
#endif
    while ( 1 ) 
    {
        mDelaymS(200);
        mDelaymS(200);
    }
}

/*********************************************************************************
* Function Name  : InitCH395InfParam
* Description    : ��ʼ��CH395Inf����
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitCH395InfParam(void)
{
    memset(&CH395Inf,0,sizeof(CH395Inf));                            /* ��CH395Infȫ������*/
    memcpy(CH395Inf.MacAddr,CH395MACAddr,sizeof(CH395MACAddr));      /* MAC��ַ */
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
    SockInf.ProtoType = PROTO_TYPE_MAC_RAW;                          /* MAC RAWģʽ */
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
     /* socket 0ΪMAC RAWģʽ */
    CH395SetSocketProtType(0,SockInf.ProtoType);                     /* ����socket 0Э������ */
    i = CH395OpenSocket(0);                                          /* ��socket 0 */
    mStopIfError(i);                                                 /* ����Ƿ�ɹ� */
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
   UINT8  sock_int_socket,tmp1[6],tmp2[6],i;
   UINT16 len;

   sock_int_socket = CH395GetSocketInt(sockindex);                   /* ��ȡsocket ���ж�״̬ */
   if(sock_int_socket & SINT_STAT_SENBUF_FREE)                       /* ���ͻ��������У����Լ���д��Ҫ���͵����� */
   {
   }
   if(sock_int_socket & SINT_STAT_SEND_OK)                           /* ��������ж� */
   {
   }
   if(sock_int_socket & SINT_STAT_RECV)                              /* �����ж� */
   {
    len = CH395GetRecvLength(sockindex);                             /* ��ȡ��ǰ�����������ݳ��� */
#if CH395_DEBUG
    printf("receive len = %d\n",len);
#endif
    if(len == 0)return;
    if(len > 512) len = 512;                                         /*MyBuffer����������Ϊ512��*/
    CH395GetRecvData(sockindex,len,MyBuffer);                        /* ��ȡ���� */
	   
	
    for(i=0;i<6;i++)                                                 /*���յ���ǰ12�ֽ�����λĿ��Mac��ַ��ԴMac��ַ������λ�ý���ת��*/
    {
      tmp1[i] =  MyBuffer[i];
    }
    for(i=0;i<6;i++)
    {
      tmp2[i] =  MyBuffer[i+6];
    }
    for(i=0;i<6;i++)
    {
      MyBuffer[i] =  tmp2[i];
    }
    for(i=0;i<6;i++)
    {
      MyBuffer[i+6] =  tmp1[i];
    }

    CH395SendData(sockindex,MyBuffer,len);
   }
   /*
   **�����Ͽ������жϺͳ�ʱ�ж�ʱ��CH395Ĭ���������ڲ������رգ��û�����Ҫ�Լ��رո�Socket����������óɲ������ر�Socket��Ҫ����
   **SOCK_CTRL_FLAG_SOCKET_CLOSE��־λ��Ĭ��Ϊ0��������ñ�־Ϊ1��CH395�ڲ�����Socket���йرմ������û��������жϺͳ�ʱ�ж�ʱ����
   **CH395CloseSocket������Socket���йرգ�������ر����SocketһֱΪ���ӵ�״̬����ʵ���Ѿ��Ͽ������Ͳ�����ȥ�����ˡ�
   */
   if(sock_int_socket & SINT_STAT_CONNECT)                           /* �����жϣ�����TCPģʽ����Ч*/
   {
       
   }
   if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* �Ͽ��жϣ�����TCPģʽ����Ч */
   {
   }
   if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* ��ʱ�жϣ�����TCPģʽ����Ч */
   {
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
   UINT8 xdata buf[64]; 
 
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
                                                                     /* ����0XFAһ��ΪӲ��������߶�дʱ�򲻶� */
#if (CH395_OP_INTERFACE_MODE == 5)                                   
#ifdef UART_WORK_BAUDRATE
    CH395CMDSetUartBaudRate(UART_WORK_BAUDRATE);                     /* ���ò����� */   
    mDelaymS(1);
    SetMCUBaudRate();
#endif
#endif
/*ע��CH395����ʱ����¼MAC��ַ���˴�����MAC��ַ��ҪΪ��ʾ��������������ʹ��ʱ��ֱ�ӵ��û�ȡMAC��ַ���������������MAC��ַ*/
    //CH395CMDSetMACAddr(CH395Inf.MacAddr);                            /* ����CH395��MAC��ַ��MAC RAW����Ҫ����MAC��ַ */
    mDelaymS(200);                                                   /*��Ҫ����ʱ����*/
    mDelaymS(200);    
    mDelaymS(200);    
    mDelaymS(200);    
    mDelaymS(200);    
    mDelaymS(200);    
    mDelaymS(200);    
    mDelaymS(200);    
    mDelaymS(200);    
    i = CH395CMDInitCH395();                                         /* ��ʼ��CH395оƬ */
    return i;
}

/**********************************************************************************
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
 
    mDelaymS(100);                                                   /* ��ʱ100���� */
    mInitSTDIO();
#if CH395_DEBUG
    printf("CH395EVT Test Demo\n");
#endif
    CH395_PORT_INIT();
    InitCH395InfParam();                                             /* ��ʼ��CH395��ر��� */
    i = CH395Init();                                                 /* ��ʼ��CH395оƬ */
    mStopIfError(i);
                                                                     
   while(1)
   {                                                                 /* �ȴ���̫�����ӳɹ�*/
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
       if(CH395_INT_WIRE == 0)CH395GlobalInterrupt();                /* �жϴ������� */
   }
}
