/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395��������
**********************************************************************************/
/* ͷ�ļ�����*/
#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include "../PUB/CH395INC.H"
#include "CH395.H"
#include "mailinc.h"
/***********************************************************************************/
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
#endif
/***********************************************************************************/

/* ���������ļ� */
#include "../PUB/CH395CMD.C"
/**********************************************************************************/

/* ���ñ������� */
#define MybufLen  4096                                              /* ����ch395�������ݵĻ�������󳤶� */
UINT8  MyBuffer[2][MybufLen];                                       /* ���ݻ����� */
struct _SOCK_INF  SockInf[2];                                       /* ����Socket��Ϣ */
struct _CH395_SYS CH395Inf;                                         /* ����CH395��Ϣ */
UINT8  tcptimeout_flag;
UINT8  phyDiscont_flag;

/**********************************************************************************
* Function Name  : mStopIfError
* Description    : ����ʹ�ã���ʾ������룬��ͣ��
* Input          : iError
* Output         : None
* Return         : None
**********************************************************************************/
void mStopIfError(UINT8 iError)
{
    if (iError == CMD_ERR_SUCCESS) return;                          /* �����ɹ� */
    printf("Error: %02X\n", (UINT16)iError);                        /* ��ʾ���� */
    while ( 1 ) 
    {
        mDelaymS(200);
        mDelaymS(200);
    }
}

/**********************************************************************************
* Function Name  : CH395CloseSocket
* Description    : �ر�socket��
* Input          : sockindex Socket����
* Output         : None
* Return         : ����ִ�н��
**********************************************************************************/
UINT8  CH395CloseSocketp(UINT8 sockindex)
{
    UINT8 i = 0;
    UINT8 s = 0;
	OrderType = COMMAND_UNUSEFULL;
	CheckType = uncheck;
    xWriteCH395Cmd(CMD1W_CLOSE_SOCKET_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(5);                                                 /* ��ʱ��ѯ������2MS����*/
        s = CH395GetCmdStatus();                                     /* ���ܹ���Ƶ����ѯ*/
        if(s !=CH395_ERR_BUSY)break;                                 /* ���CH395оƬ����æ״̬*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* ��ʱ�˳�*/
    }
    return s;
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

/*********************************************************************************
* Function Name  : InitSocketParam
* Description    : ��ʼ��socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitSocketParam( )
{
  memset(&SockInf[0],0,sizeof(SockInf[0]));                         /* ��SockInf[0]ȫ������*/
  memcpy(SockInf[0].IPAddr,Socket0DesIP,sizeof(Socket0DesIP));      /* ��Ŀ��IP��ַд�� */
  SockInf[0].DesPort = Socket0DesPort;                              /* Ŀ�Ķ˿� */
  SockInf[0].SourPort = Socket0SourPort;                            /* Դ�˿� */
  SockInf[0].ProtoType = PROTO_TYPE_TCP;                            /* UDPģʽ */
  SockInf[0].TcpMode = TCP_CLIENT_MODE;                             /* TCP������ģʽ */

  memset(&SockInf[1],0,sizeof(SockInf[1]));                         /* ��SockInf[1]ȫ������*/
  memcpy(SockInf[1].IPAddr,Socket1DesIP,sizeof(Socket1DesIP));      /* ��Ŀ��IP��ַд�� */
  SockInf[1].DesPort = Socket1DesPort;                              /* Ŀ�Ķ˿� */
  SockInf[1].SourPort = Socket1SourPort;                            /* Դ�˿� */
  SockInf[1].ProtoType = PROTO_TYPE_TCP;                            /* TCPģʽ */
  SockInf[1].TcpMode = TCP_CLIENT_MODE;                             /* TCP������ģʽ */
}

/**********************************************************************************
* Function Name  : CH395SocketInitOpen
* Description    : ����CH395 socket ��������ʼ������socket
* Input          : index
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInitOpen(UINT8 index)
{
    UINT8 i;
    /* socket indexΪTCP �ͻ���ģʽ */
    memset( MyBuffer[index], '\0', sizeof(MyBuffer[index]) );
    if( index == SendSocket ) CheckType = SMTP_CHECK_CNNT;
    if( index == ReceSocket ) CheckType = POP_CHECK_CNNT;
    CH395SetSocketDesIP(index,SockInf[index].IPAddr);                /* ����socket 0Ŀ��IP��ַ */         
    CH395SetSocketProtType(index,SockInf[index].ProtoType);          /* ����socket 0Э������ */
    CH395SetSocketDesPort(index,SockInf[index].DesPort);             /* ����socket 0Ŀ�Ķ˿� */
    CH395SetSocketSourPort(index,SockInf[index].SourPort);           /* ����socket 0Դ�˿� */
    i = CH395OpenSocket(index);                                      /* ��socket   */
    mStopIfError(i);                                                 /* ����Ƿ�ɹ� */
    i = CH395TCPConnect(index);                                      /* TCP����      */
    mStopIfError(i);                                                 /* ����Ƿ�ɹ� */
}

/*********************************************************************************
* Function Name  : CH395SocketInterrupt
* Description    : CH395 socket �ж�,��ȫ���ж��б�����
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInterrupt(UINT8 sockindex)
{
   char    check=0;  
   UINT8  sock_int_socket;
   UINT8  i;
   UINT16 len;
   sock_int_socket = CH395GetSocketInt(sockindex);                   /* ��ȡsocket ���ж�״̬ */
   if(sock_int_socket & SINT_STAT_SENBUF_FREE)                       /* ���ͻ��������У����Լ���д��Ҫ���͵����� */
   {
   }
   if(sock_int_socket & SINT_STAT_SEND_OK)                           /* ��������ж� */
   {
     if(sockindex == SendSocket) SendDatFalg = 1;
   }
   if(sock_int_socket & SINT_STAT_RECV)                              /* �����ж� */
   {
       ReceDatFlag = 1;
       len = CH395GetRecvLength(sockindex);                          /* ��ȡ��ǰ�����������ݳ��� */
       if(len == 0)return;
       if(len>MybufLen) len = MybufLen;
       CH395GetRecvData(sockindex,len,MyBuffer[sockindex]);          /* ��ȡ���� */
       SockInf[sockindex].RemLen = len;                              /* ���泤�� */
       ReceLen = len; 
#if  DEBUG
     printf("ReceLen= %04x\n",ReceLen);
     printf("CheckType= %02x\n",(UINT16)CheckType);
     printf("MyBuffer = %s\n",MyBuffer[sockindex]);
#endif
     if(CheckType != uncheck){
       i = ch395mail_CheckResponse(MyBuffer[sockindex],CheckType);
       if(i != CHECK_SUCCESS){
            if(sockindex == SendSocket) OrderType = SMTP_ERR_CHECK;
            if(sockindex == ReceSocket) OrderType = POP_ERR_CHECK;
#if  DEBUG
  printf("ERROR: %02x\n",(UINT16)i);
#endif
          }
     }     
  }
   if(sock_int_socket & SINT_STAT_CONNECT)                          /* �����жϣ�����TCPģʽ����Ч*/
   {
#if  DEBUG
     printf("Tcp Connect\n");
#endif
   }
   if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* �Ͽ��жϣ�����TCPģʽ����Ч */
   {
   }
   if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* ��ʱ�жϣ�����TCPģʽ����Ч */
   {
#if  DEBUG
      printf("Tcp Time out\n");
#endif
       tcptimeout_flag = 1;
       /* ������ʱ�жϱ�ʾ����/����/�������ݳ�ʱ����ʧ�ܣ�������ʱʱCH395оƬ�ڲ����Ὣ��    */
       /* socket�رգ���ĳЩ�����CH395�������������ӣ�����Զ�˶˿�δ�򿪣����CH395���ӣ��� */
       /* Զ���豸���ܻᷢ��RSTǿ�ƽ������Ӹ�λ����ʱCH395��Ȼ�������ʱ�жϡ������������ʾ */
       /* ʵ��Ӧ���в��Ƽ�����������ʱ�жϺ��������ӣ����Լ��һ��ʱ�������´�socket������ */
       /* ���ɡ�*/         
       if(SockInf[sockindex].TcpMode  == TCP_CLIENT_MODE)                       
       {
           mDelaymS(200);                                            /* ��ʱ200MS���ٴ����ԣ�û�б�Ҫ����Ƶ������ */
           i = CH395OpenSocket(sockindex);
           mStopIfError(i);
           CH395TCPConnect(sockindex);                               /* ��ʼ���� */
           mStopIfError(i);
      }
   }
}

/*********************************************************************************
* Function Name  : CH395GlobalInterrupt
* Description    : CH395ȫ���жϺ���
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395GlobalInterrupt(void) 
{
   UINT16  init_status;
   UINT16 i;
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
#if  DEBUG
        printf("Init status : GINT_STAT_PHY_CHANGE\n");
#endif
        i = CH395CMDGetPHYStatus();                                  /* ��ȡPHY״̬ */
        if(i == PHY_DISCONN){
        phyDiscont_flag = 1;
#if  DEBUG
      printf("Ethernet Disconnect\n");                               /* �����PHY_DISCONN��CH395�ڲ����Զ��ر����е�socket*/
#endif
    }
    }
    if(init_status & GINT_STAT_SOCK0)
    {
    CH395SocketInterrupt(0);                                         /* ����socket 0�ж�*/
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
         CH395SocketInterrupt(4);                                    /* ����socket 4�ж�*/
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
    
    i = CH395CMDCheckExist(0x65);   
	printf("check ======%2x\n",(UINT16)i);                   
    if(i != 0x9a)return CH395_ERR_UNKNOW;                            /* �����������޷�ͨ������0XFA */
                                                                     /* ����0XFAһ��ΪӲ��������߶�дʱ�򲻶� */
#if (CH395_OP_INTERFACE_MODE == 5)                                   
#ifdef UART_WORK_BAUDRATE
    CH395CMDSetUartBaudRate(UART_WORK_BAUDRATE);                     /* ���ò����� */   
    mDelaymS(1);
    SetMCUBaudRate();
    i = xReadCH395Data();                                            /* ������óɹ���CH395����CMD_ERR_SUCCESS */
    if(i == CMD_ERR_SUCCESS)printf("Set Success\n");
#endif
#endif
    CH395CMDSetIPAddr(CH395Inf.IPAddr);                              /* ����CH395��IP��ַ */
    CH395CMDSetGWIPAddr(CH395Inf.GWIPAddr);                          /* �������ص�ַ */
    CH395CMDSetMASKAddr(CH395Inf.MASKAddr);                          /* �����������룬Ĭ��Ϊ255.255.255.0*/   
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
/*********************************************************************************
* Function Name  : SendData
* Description    : ��������
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
UINT8 ch395mail_SendData( UINT8 *PSend, UINT16 Len,UINT8 type,UINT8 index  )
{
  UINT16  count;
  CheckType = type;
  memset( MyBuffer[index], '\0', sizeof(MyBuffer[index]) );
  CH395SendData(index,PSend,Len);
  SendDatFalg = 0;
  phyDiscont_flag = 0;
  tcptimeout_flag = 0;
  count = 0;
  if(CheckType == uncheck){
    while(SendDatFalg==0){
      if(CH395_INT_WIRE == 0)  CH395GlobalInterrupt();
      if( phyDiscont_flag ){
        OrderType = COMMAND_UNUSEFULL; 
        return 0;
      } 
      if( tcptimeout_flag ){
        OrderType = COMMAND_UNUSEFULL; 
        return 0;
      }
      mDelaymS(1);
      count++;
      if(count>10000){
#if DEBUG
        printf("wait send Mail Header timeout\n");
#endif
        OrderType = SMTP_ERR_CHECK;
        return send_data_timeout;
      }
    }
    return send_data_success; 
  }
    return send_data_success; 
}

/*********************************************************************************
* Function Name  : main
* Description    : main������
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void ch395mail( )
{
  UINT8 i;
  
  ReceDatFlag = 0;
  mDelaymS(100);
  mInitSTDIO();                                                      /* ��ʱ100���� */
#if  DEBUG
  printf("start:\n");
#endif
  CH395_PORT_INIT();
  InitCH395InfParam();                                               /* ��ʼ��CH395��ر��� */
  i = CH395Init();                                                   /* ��ʼ��CH395оƬ */
  mStopIfError(i);
  while(1)
  {                                                                  /* �ȴ���̫�����ӳɹ�*/
     if(CH395CMDGetPHYStatus() == PHY_DISCONN)                       /* ��ѯCH395�Ƿ����� */
     {
         mDelaymS(200);                                              /* δ������ȴ�200MS���ٴβ�ѯ */
     }
     else 
     {
#if  DEBUG
         printf("Connect Ethernet\n");                               /* CH395оƬ���ӵ���̫������ʱ������ж� */
#endif
         break;
     }
  }
  InitSocketParam( );                                                /* ��ʼ��socket��ر��� */
}
