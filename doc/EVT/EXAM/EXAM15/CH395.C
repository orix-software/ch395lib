/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395������ʾ
**********************************************************************************/

/* ͷ�ļ�����*/
#include "stdio.h"
#include "string.h"
#include "../PUB/CH395INC.H"
#include "CH395.H"


/* ���������ļ� */
#include "../PUB/CH395CMD.H"

/* ���ñ������� */
struct _SOCK_INF idata SockInf;                                      /* ����Socket��Ϣ */
struct _CH395_SYS idata CH395Inf;                                    /* ����CH395��Ϣ */

/* CH395��ض��� */
 UINT8 CH395IPAddr[4];                                               /* CH395IP��ַ */
 UINT8 CH395GWIPAddr[4];                                             /* CH395���� */
 UINT8 CH395IPMask[4];                                               /* CH395�������� */

/* socket ��ض���*/
const UINT8  BroadcastIP[4] = {255,255,255,255};                      /* UDP �㲥��ַ */
const UINT16 Socket0DesPort = 60000;                                  /* Socket 0Ŀ�Ķ˿� */
const UINT16 Socket0SourPort = 50000;                                 /* Socket 0Դ�˿� */

 extern  UINT8 RecvFlag ;
 extern  UINT8 Recvbuf[300] ;	

/********************************************************************************
* Function Name  : DelayuS
* Description    : ��ʱָ��΢��ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
* Input          : us---��ʱʱ��ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void DelayuS(UINT8 us)
{
    while(us --);                                                    /* MCS51@24MHz */
}

/*******************************************************************************
* Function Name  : DelaymS
* Description    : ��ʱָ������ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
* Input          : ms---��ʱʱ��ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void DelaymS(UINT8 ms)
{
    while(ms --) 
    {
        DelayuS(250);
        DelayuS(250);
        DelayuS(250);
        DelayuS(250);
    }
}

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
    printf("Error: %02X\n", (UINT16)iError);                         /* ��ʾ���� */
    while ( 1 ) 
    {
        DelaymS(200);
        DelaymS(200);
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
    /* socket 0ΪUDPģʽ */
    memset(&SockInf,0,sizeof(SockInf));                              /* ��SockInf[0]ȫ������*/
    memcpy(SockInf.IPAddr,BroadcastIP,sizeof(BroadcastIP));          /* �������UDP SERVER���ܣ��轫Ŀ��IP��Ϊ�㲥��ַ255.255.255.255 */
    SockInf.DesPort = Socket0DesPort;                                /* Ŀ�Ķ˿� */
    SockInf.SourPort = Socket0SourPort;                              /* Դ�˿� */
    SockInf.ProtoType = PROTO_TYPE_UDP;                              /* UDPģʽ */

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
   /* socket 0ΪUDPģʽ */
    CH395SetSocketDesIP(0,SockInf.IPAddr);                           /* ����socket 0Ŀ��IP��ַ */         
    CH395SetSocketProtType(0,SockInf.ProtoType);                     /* ����socket 0Э������ */
    CH395SetSocketDesPort(0,SockInf.DesPort);                        /* ����socket 0Ŀ�Ķ˿� */
    CH395SetSocketSourPort(0,SockInf.SourPort);                      /* ����socket 0Դ�˿� */
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
   UINT8  sock_int_socket;
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
       len = CH395GetRecvLength(sockindex);                          /* ��ȡ��ǰ�����������ݳ��� */
       printf("receive len = %d\n",len);
       if(len == 0)return;
	   if(len >300){
       CH395GetRecvData(sockindex,len,NULL);                         /* ��ȡ���� */	   
	   return ;
	   }
       CH395GetRecvData(sockindex,len,Recvbuf);                      /* ��ȡ���� */
	   RecvFlag = 1;
   }
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
 
    init_status = CH395CMDGetGlobIntStatus_ALL();
    if(init_status & GINT_STAT_UNREACH)                              /* ���ɴ��жϣ���ȡ���ɴ���Ϣ */
    {
    }
    if(init_status & GINT_STAT_IP_CONFLI)                            /* ����IP��ͻ�жϣ����������޸�CH395�� IP������ʼ��CH395*/
    {
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                           /* ����PHY�ı��ж�*/
    {
        printf("Init status : GINT_STAT_PHY_CHANGE\n");
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
    CH395CMDSetIPAddr(CH395Inf.IPAddr);                              /* ����CH395��IP��ַ */
    CH395CMDSetGWIPAddr(CH395Inf.GWIPAddr);                          /* �������ص�ַ */
    CH395CMDSetMASKAddr(CH395Inf.MASKAddr);                          /* �����������룬Ĭ��Ϊ255.255.255.0*/ 
    DelaymS(200);  
    i = CH395CMDInitCH395();                                         /* ��ʼ��CH395оƬ */
    return i;
}


