/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395��������
**********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "..\SRC\CH563SFR.H"
#include "..\SRC\SYSFREQ.H"
#include "..\..\PUB\CH395INC.H"
#include "CH395.H"
#include "..\..\PUB\ch395cmd.h"
#include "dns.h"

/*��ر�����ʼ��*/
 struct  _SOCK_INF   SockInf[2];
 struct  _CH395_SYS  CH395Inf;     
 UINT8  MyBuffer[2][4096];  

/* CH395��ض��� */
 UINT8 CH395IPAddr[4] = {192,168,0,10};                            /* CH395IP��ַ */
 UINT8 CH395GWIPAddr[4] = {192,168,0,1};                           /* CH395���� */
 UINT8 CH395IPMask[4] = {255,255,255,0};                           /* CH395�������� */
 
 UINT16 Socket0SourPort = 2000;                                    /* Socket 0Դ�˿� */
 UINT16 Socket1SourPort = 4000;                                    /* Socket 1Դ�˿� */

/*********************************************************************************
* Function Name  : InitCH395InfParam
* Description    : ��ʼ��CH395Inf����
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitCH395InfParam(void)
{
    memset(&CH395Inf,0,sizeof(CH395Inf));                           /* ��CH395Infȫ������*/
    memcpy(CH395Inf.IPAddr,CH395IPAddr,sizeof(CH395IPAddr));        /* ��IP��ַд��CH395Inf�� */
    memcpy(CH395Inf.GWIPAddr,CH395GWIPAddr,sizeof(CH395GWIPAddr));  /* ������IP��ַд��CH395Inf�� */
    memcpy(CH395Inf.MASKAddr,CH395IPMask,sizeof(CH395IPMask));      /* ����������д��CH395Inf�� */
}

/*********************************************************************************
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
        Delay_ms(200);
        Delay_ms(200);
    }
}

/*********************************************************************************
* Function Name  : UDPSocketParamInit
* Description    : ��ʼ��socket
* Input          : sockindex��addr��SourPort��DesPort
* Output         : None
* Return         : None
**********************************************************************************/
void UDPSocketParamInit(UINT8 sockindex,UINT8 *addr,UINT16 SourPort,UINT16 DesPort)
{
    UINT8 i;
    memset(&SockInf[sockindex],0,sizeof(SockInf[sockindex]));        /* ��SockInf[0]ȫ������*/
    memcpy(SockInf[sockindex].IPAddr,addr,sizeof(addr));             /* ��Ŀ��IP��ַд�� */
    SockInf[sockindex].DesPort = DesPort;                            /* Ŀ�Ķ˿� */
    SockInf[sockindex].SourPort = SourPort;                          /* Դ�˿� */
    SockInf[sockindex].ProtoType = PROTO_TYPE_UDP; 

    CH395SetSocketDesIP(sockindex,SockInf[sockindex].IPAddr);        /* ����socket 0Ŀ��IP��ַ */         
    CH395SetSocketProtType(sockindex,PROTO_TYPE_UDP);                /* ����socket 0Э������ */
    CH395SetSocketDesPort(sockindex,SockInf[sockindex].DesPort);     /* ����socket 0Ŀ�Ķ˿� */
    CH395SetSocketSourPort(sockindex,SockInf[sockindex].SourPort);   /* ����socket 0Դ�˿� */
    i = CH395OpenSocket(sockindex);                                  /* ��socket 0 */
    mStopIfError(i);  
}

/*********************************************************************************
* Function Name  : UDPSendData
* Description    : UDP��ʽ��������
* Input          : sockindex��databuf��len
* Output         : None
* Return         : None
**********************************************************************************/
UINT16 UDPSendData(UINT8 sockindex,UINT8 *databuf,UINT16 len)
{
  UINT16 ret;
  if(len>512){
  ret = 512;
  }
  else ret =len;
  CH395SendData(sockindex,databuf,len); 
  return ret;
}

/*********************************************************************************
* Function Name  : CH395SocketInterrupt
* Description    : CH395 socket �ж�,��ȫ���ж��б�����
* Input          : sockindex  Socket����
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInterrupt(UINT8 sockindex)
{
    UINT8  sock_int_socket;
    UINT8  i;
    UINT16 len;
    UINT16 tmp;
    UINT8  buf[10];

    sock_int_socket = CH395GetSocketInt(sockindex);                  /* ��ȡsocket ���ж�״̬ */
    if(sock_int_socket & SINT_STAT_SENBUF_FREE)                      /* ���ͻ��������У����Լ���д��Ҫ���͵����� */
    {
        if(SockInf[sockindex].SendLen >= SockInf[sockindex].RemLen)
        SockInf[sockindex].RemLen = 0;                               /* �����Ѿ�������� */
        else
        {
            SockInf[sockindex].pSend += SockInf[sockindex].SendLen;  /* �Է���ָ�����ƫ�� */
            SockInf[sockindex].RemLen -= SockInf[sockindex].SendLen; /* ����ʣ�೤�� */
            if(SockInf[sockindex].RemLen > 2048)
            SockInf[sockindex].SendLen = 2048;                       /* ���㱾�ο��Է��͵����ݳ��� */
            else SockInf[sockindex].SendLen = 
            SockInf[sockindex].RemLen;     
            CH395SendData(sockindex,SockInf[sockindex].pSend,
            SockInf[sockindex].SendLen);                             /* �������� */
        }
    }
    if(sock_int_socket & SINT_STAT_SEND_OK)                          /* ��������ж� */
    {
    }
    if(sock_int_socket & SINT_STAT_RECV)                             /* �����ж� */
    {
        len = CH395GetRecvLength(sockindex);                         /* ��ȡ��ǰ�����������ݳ��� */
        if(len == 0)return;
    if (len > 0)
    {  
      if(SockInf[sockindex].RemLen != 0)return;                       /* �������δ������ϣ����ٽ��� */
      if(sockindex==0)
      {
         CH395GetRecvData(sockindex,len,dns_buf);
         status = 4;
         i = CH395CloseSocket(sockindex);
         mStopIfError(i);
      }
      else
      {
          if(SockInf[sockindex].RemLen != 0)return;                    /* �������δ������ϣ����ٽ��� */
          CH395GetRecvData(sockindex,len,MyBuffer[sockindex]);         /* ��ȡ���� */
          SockInf[sockindex].RemLen = len;                             /* ���泤�� */
          for(tmp =0; tmp < len; tmp++)                                /* ���������ݰ�λȡ�� */
          {
              MyBuffer[sockindex][tmp] = ~MyBuffer[sockindex][tmp];
          }
          if(len > 2048)len = 2048;                                     /* ���ͻ��������Ϊ2048 */
          CH395SendData(sockindex,MyBuffer[sockindex],len);
          SockInf[sockindex].SendLen = len;                             /* ���淢�͵ĳ��� */
          SockInf[sockindex].pSend = MyBuffer[sockindex];               /* ���ͻ�ȡ��ָ�� */
      }
    }
   }
   if(sock_int_socket & SINT_STAT_CONNECT)                              /* �����жϣ�����TCPģʽ����Ч*/
   {
       printf("Tcp Connect\n");
       if(SockInf[sockindex].TcpMode == TCP_SERVER_MODE)                /* ���socket Ϊ������ģʽ���û����Ի�ȡԶ�˵�IP�Ͷ˿�*/
       {
         CH395CMDGetRemoteIPP(sockindex,buf);
         printf("IP address = %d.%d.%d.%d\n",(UINT16)buf[0],\
         (UINT16)buf[1],(UINT16)buf[2],(UINT16)buf[3]);    
         tmp = (UINT16)(buf[5]<<8) + buf[4];
         printf("Port = %d\n",tmp);    
       }
   }
   if(sock_int_socket & SINT_STAT_DISCONNECT)                          /* �Ͽ��жϣ�����TCPģʽ����Ч */
   {
   }
   if(sock_int_socket & SINT_STAT_TIM_OUT)                             /* ��ʱ�жϣ�����TCPģʽ����Ч */
   {
       printf("Tcp Time out\n");
       status = 2;
    }
}

/*********************************************************************************
* Function Name  : CH395GlobalInterrupt
* Description    : CH395ȫ���жϺ���
* Input          : None
* Output         : None
* Return         : None
*********************************************************************************/
void CH395GlobalInterrupt(void)
{
   UINT16  init_status;
   UINT16 i;
   UINT8  buf[10]; 
 
    init_status = CH395CMDGetGlobIntStatus_ALL();
    if(init_status & GINT_STAT_UNREACH)                               /* ���ɴ��жϣ���ȡ���ɴ���Ϣ */
    {
        printf("Init status : GINT_STAT_UNREACH\n");                  /* UDPģʽ�¿��ܻ�������ɴ��ж� */
        CH395CMDGetUnreachIPPT(buf);                                
    }
    if(init_status & GINT_STAT_IP_CONFLI)                             /* ����IP��ͻ�жϣ����������޸�CH395�� IP������ʼ��CH395*/
    {
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                            /* ����PHY�ı��ж�*/
    {
        printf("Init status : GINT_STAT_PHY_CHANGE\n");
        i = CH395CMDGetPHYStatus();       /* ��ȡPHY״̬ */
        if(i==0x08) status = 1;    
        printf("status = 1\n");                           
        if(i == PHY_DISCONN)printf("Ethernet Disconnect\n");          /* �����PHY_DISCONN��CH395�ڲ����Զ��ر����е�socket*/
    }
    if(init_status & GINT_STAT_SOCK0)
    {
        CH395SocketInterrupt(0);                                      /* ����socket 0�ж�*/
    }
    if(init_status & GINT_STAT_SOCK1)                                 
    {
        CH395SocketInterrupt(1);                                      /* ����socket 1�ж�*/
    }
    if(init_status & GINT_STAT_SOCK2)                                 
    {
        CH395SocketInterrupt(2);                                      /* ����socket 2�ж�*/
    }
    if(init_status & GINT_STAT_SOCK3)                                 
    {
        CH395SocketInterrupt(3);                                      /* ����socket 3�ж�*/
    }
    if(init_status & GINT_STAT_SOCK4)
    {
        CH395SocketInterrupt(4);                                      /* ����socket 4�ж�*/
    }
    if(init_status & GINT_STAT_SOCK5)                                 
    {
        CH395SocketInterrupt(5);                                      /* ����socket 5�ж�*/
    }
    if(init_status & GINT_STAT_SOCK6)                                 
    {
        CH395SocketInterrupt(6);                                      /* ����socket 6�ж�*/
    }
    if(init_status & GINT_STAT_SOCK7)                                 
    {
        CH395SocketInterrupt(7);                                      /* ����socket 7�ж�*/
    }
}

/*********************************************************************************
* Function Name  : CH395Init
* Description    : ����CH395��IP,GWIP,MAC�Ȳ���������ʼ��
* Input          : None
* Output         : None
* Return         : ����ִ�н��
**********************************************************************************/
UINT8  CH395Init(void)
{
    UINT8 i;    
    i = CH395CMDCheckExist(0x55); 
    printf("CH395CMDCheckExist = %2x\n",(UINT16)i);
    if(i != 0xaa)return CH395_ERR_UNKNOW;                            /* �����������޷�ͨ������0XFA */
    CH395CMDSetIPAddr(CH395Inf.IPAddr);                              /* ����CH395��IP��ַ */
    CH395CMDSetGWIPAddr(CH395Inf.GWIPAddr);                          /* �������ص�ַ */
    CH395CMDSetMASKAddr(CH395Inf.MASKAddr);                          /* �����������룬Ĭ��Ϊ255.255.255.0*/  
    i = CH395CMDInitCH395();                                         /* ��ʼ��CH395оƬ */
    return i;
}