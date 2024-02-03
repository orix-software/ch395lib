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
#include "CH395FTPINC.H"
extern FTP ftp;               
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
/***********************************************************************************/
/* ���ñ������� */
#define MybufLen                     536                             /* ����ch395�������ݵĻ�������󳤶� */
#define CH395_DEBG                    1
UINT8  MyBuffer[2][MybufLen];                                        /* ���ݻ����� */
struct _SOCK_INF  SockInf[2];                                        /* ����Socket��Ϣ */
struct _CH395_SYS CH395Inf;                                          /* ����CH395��Ϣ */
UINT8  SerPort; 
UINT32 TMP_LEN;
/*******************************************************************************
* Function Name  : mStopIfError
* Description    : ����ʹ�ã���ʾ������룬��ͣ��
* Input          : iError
* Output         : None
* Return         : None
*******************************************************************************/
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

/********************************************************************************
* Function Name  : InitCH395InfParam
* Description    : ��ʼ��CH395Inf����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void InitCH395InfParam(void)
{
    memset(&CH395Inf,0,sizeof(CH395Inf));                            /* ��CH395Infȫ������*/
    memcpy(CH395Inf.IPAddr,CH395IPAddr,sizeof(CH395IPAddr));         /* ��IP��ַд��CH395Inf�� */
    memcpy(CH395Inf.GWIPAddr,CH395GWIPAddr,sizeof(CH395GWIPAddr));   /* ������IP��ַд��CH395Inf�� */
    memcpy(CH395Inf.MASKAddr,CH395IPMask,sizeof(CH395IPMask));       /* ����������д��CH395Inf�� */
//  memcpy(CH395Inf.MacAddr,CH395MACAddr,sizeof(CH395MACAddr));
}

/*******************************************************************************
* Function Name  : CH395ServerSocketInitOpen
* Description    : ����CH395 socket ��������ʼ������socket
* Input          : index
* Output         : None
* Return         : None
*******************************************************************************/
void CH395ServerSocketInitOpen(UINT8 index)
{
    UINT8 i;

    CH395SetSocketProtType(index,SockInf[index].ProtoType);          /* ����socket Э������ */
    CH395SetSocketSourPort(index,SockInf[index].SourPort);           /* ����socket Դ�˿� */
    i = CH395OpenSocket(index);                                      /* ��socket  */
    mStopIfError(i);                                                 /* ����Ƿ�ɹ� */
    i = CH395TCPListen(index);                                       /* TCP���� */
    mStopIfError(i);                                                 /* ����Ƿ�ɹ� */
}

/*******************************************************************************
* Function Name  : CH395ClientSocketInitOpen
* Description    : ����CH395 socket ��������ʼ������socket
* Input          : index
* Output         : None
* Return         : None
*******************************************************************************/
void CH395ClientSocketInitOpen(UINT8 index)
{
    UINT8 i;

    CH395SetSocketDesIP(index,SockInf[index].IPAddr);                  /* ����socket Ŀ��IP��ַ */         
    CH395SetSocketProtType(index,SockInf[index].ProtoType);            /* ����socket Э������ */
    CH395SetSocketDesPort(index,SockInf[index].DesPort);               /* ����socket Ŀ�Ķ˿� */
    CH395SetSocketSourPort(index,SockInf[index].SourPort);             /* ����socket Դ�˿� */
    i = CH395OpenSocket(index);                                        /* ��socket  */
    mStopIfError(i);                                                   /* ����Ƿ�ɹ� */
    i = CH395TCPConnect(index);                                        /* TCP���� */
    mStopIfError(i);                                                   /* ����Ƿ�ɹ� */
}

/*******************************************************************************
* Function Name  : CH395_FTPCtlServer
* Description    : ����TCP��������
* Input          : socketid
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPCtlServer( UINT8 socketid )
{
    memset(&SockInf[socketid],0,sizeof(SockInf[socketid]));             /* ����ӦSockInfȫ������*/
    SockInf[socketid].SourPort = 21;                                    /* Դ�˿� */
    SockInf[socketid].ProtoType = PROTO_TYPE_TCP;                       /* TCPģʽ */
    SockInf[socketid].TcpMode = TCP_SERVER_MODE;                        /* TCP������ģʽ */
    CH395ServerSocketInitOpen( socketid );
    memset((void *)MyBuffer[socketid],'\0',sizeof(MyBuffer[socketid]));                 
}

/*******************************************************************************
* Function Name  : CH395_FTPDatServer
* Description    : ��ʼ��socket
* Input          : socketid,port
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPDatServer( UINT8 socketid,UINT16 *port )
{
    if(SerPort < 100||SerPort > 200) SerPort = 100;
    SerPort++;
    memset(&SockInf[socketid],0,sizeof(SockInf[socketid]));             /* ����ӦSockInfȫ������*/
    SockInf[socketid].SourPort = 256*5 + SerPort;                       /* Դ�˿� */
    *port = SockInf[socketid].SourPort;
    SockInf[socketid].ProtoType = PROTO_TYPE_TCP;                       /* TCPģʽ */
    SockInf[socketid].TcpMode = TCP_SERVER_MODE;                        /* TCP������ģʽ */
    CH395ServerSocketInitOpen( socketid );
    memset((void *)MyBuffer[socketid],'\0',sizeof(MyBuffer[socketid]));
}

/*******************************************************************************
* Function Name  : CH395_FTPDatClient
* Description    : ��ʼ��socket
* Input          : socketid,port,pAddr
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPDatClient( UINT8 socketid,UINT16 port,UINT8 *pAddr )
{
    memset(&SockInf[socketid],0,sizeof(SockInf[socketid]));             /* ����ӦSockInfȫ������*/
    memcpy(SockInf[socketid].IPAddr,pAddr,4);                           /* ��Ŀ��IP��ַд�� */
    SockInf[socketid].DesPort = port;                                   /* Ŀ�Ķ˿� */
    SockInf[socketid].SourPort = 20;                                    /* Դ�˿� */
    SockInf[socketid].ProtoType = PROTO_TYPE_TCP;                       /* TCPģʽ */
    SockInf[socketid].TcpMode = TCP_CLIENT_MODE;                        /* TCP�ͻ���ģʽ */
    CH395ClientSocketInitOpen( socketid );
    memset((void *)MyBuffer[socketid],'\0',sizeof(MyBuffer[socketid]));
}

/*******************************************************************************
* Function Name  : CH395SocketInterrupt
* Description    : CH395 socket �ж�,��ȫ���ж��б�����
* Input          : sockindex
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SocketInterrupt(UINT8 sockindex)
{
   UINT8  sock_int_socket;
   UINT16 len;

#if CH395_DEBG
     printf("sockindex =%d\n",(UINT16)sockindex);
#endif     
  sock_int_socket = CH395GetSocketInt(sockindex);                        /* ��ȡsocket ���ж�״̬ */
  if(sock_int_socket & SINT_STAT_SENBUF_FREE)                            /* ���ͻ��������У����Լ���д��Ҫ���͵����� */
   {
   }
   if(sock_int_socket & SINT_STAT_SEND_OK)                               /* ��������ж� */
   {
#if CH395_DEBG
     printf("send data success\n");
#endif
          if(ftp.CmdDataS == FTP_MACH_CLOSECTL){
               if(sockindex == ftp.SocketCtl){
                    ftp.CmdDataS = 0;
                    CH395CloseSocket( ftp.SocketCtl );
               }
          }
   }
   if(sock_int_socket & SINT_STAT_CONNECT)                                /* �����жϣ�����TCPģʽ����Ч*/
   {
#if CH395_DEBG
     printf("TCP Connect\n");
#endif     
          CH395_FTPGetSockeID( sockindex );
          if(ftp.CmdReceDatS == 1){                                       /* ��Ҫ��������     */
               if(ftp.TcpDatSta == FTP_MACH_CONNECT){
                    ftp.CmdReceDatS = 0;
                    ftp.CmdDataS = FTP_MACH_RECEDATA;
               }
          }
   }
   if(sock_int_socket & SINT_STAT_RECV)                                   /* �����ж� */
   {
 
       len = CH395GetRecvLength(sockindex);                               /* ��ȡ��ǰ�����������ݳ��� */
        if(len){
              if(len>MybufLen) len = MybufLen;
              CH395GetRecvData(sockindex,len,MyBuffer[sockindex]);         /* ��ȡ���� */
              SockInf[sockindex].RemLen = len;                             /* ���泤�� */
#if CH395_DEBG
     printf("len:%d\n",len);
     printf("MyBuffer:\n%s\n",MyBuffer[sockindex]);
#endif     

             CH395_FTPProcessReceDat((char *)MyBuffer[sockindex],sockindex);
             memset((void *)MyBuffer[sockindex],'\0',sizeof(MyBuffer[sockindex]));
          }
   }
   if(sock_int_socket & SINT_STAT_DISCONNECT)                               /* �Ͽ��жϣ�����TCPģʽ����Ч */
   {
#if CH395_DEBG                                                              /* �������жϣ�CH563NET_���ڲ��Ὣ��socket�������Ϊ�ر�*/
    printf("TCP Disconnect\n");                                             /* Ӧ������������´������� */
#endif
          CH395CloseSocket(sockindex);
          if( sockindex == ftp.SocketCtl ){
               ftp.TcpCtlSta = FTP_MACH_DISCONT; 
               CH395_FTPCtlServer( 0 );
          }
          else if( sockindex == ftp.SocketDat ){
               ftp.TcpDatSta = FTP_MACH_DISCONT;
          }  
   }
   if(sock_int_socket & SINT_STAT_TIM_OUT)                                  /* ��ʱ�жϣ�����TCPģʽ����Ч */
   {
#if CH395_DEBG                                                              /* �������жϣ�CH563NET_���ڲ��Ὣ��socket�������Ϊ�ر�*/
    printf("TCP Timout\n");                                                 /* Ӧ������������´������� */
#endif
        CH395CloseSocket(sockindex);
        if( sockindex == ftp.SocketCtl ){
             ftp.TcpCtlSta = FTP_MACH_DISCONT; 
             CH395_FTPCtlServer( 0 );
        }
        else if( sockindex == ftp.SocketDat ){
            ftp.TcpDatSta = FTP_MACH_DISCONT;
        }  
       /* ������ʱ�жϱ�ʾ����/����/�������ݳ�ʱ����ʧ�ܣ�������ʱʱCH395оƬ�ڲ����Ὣ��    */
       /* socket�رգ���ĳЩ�����CH395�������������ӣ�����Զ�˶˿�δ�򿪣����CH395���ӣ��� */
       /* Զ���豸���ܻᷢ��RSTǿ�ƽ������Ӹ�λ����ʱCH395��Ȼ�������ʱ�жϡ������������ʾ */
       /* ʵ��Ӧ���в��Ƽ�����������ʱ�жϺ��������ӣ����Լ��һ��ʱ�������´�socket������ */
       /* ���ɡ�*/         
   }
}

/******************************************************************************
* Function Name  : CH395GlobalInterrupt
* Description    : CH395ȫ���жϺ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395GlobalInterrupt(void) 
{
   UINT16  init_status;
   UINT16 i;
   UINT8  buf[10]; 
 
    init_status = CH395CMDGetGlobIntStatus_ALL();
    if(init_status & GINT_STAT_UNREACH)                                     /* ���ɴ��жϣ���ȡ���ɴ���Ϣ */
    {
        CH395CMDGetUnreachIPPT(buf);                                
    }
    if(init_status & GINT_STAT_IP_CONFLI)                                    /* ����IP��ͻ�жϣ����������޸�CH395�� IP������ʼ��CH395*/
    {
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                                   /* ����PHY�ı��ж�*/
    {
#if  CH395_DEBG
        printf("Init status : GINT_STAT_PHY_CHANGE\n");
#endif
        i = CH395CMDGetPHYStatus();                                          /* ��ȡPHY״̬ */
        if(i == PHY_DISCONN){
#if  CH395_DEBG
        printf("Ethernet Disconnect\n");                                     /* �����PHY_DISCONN��CH395�ڲ����Զ��ر����е�socket*/
#endif
          }
    }
    if(init_status & GINT_STAT_SOCK0)
    {
         CH395SocketInterrupt(0);                                            /* ����socket 0�ж�*/
    }
    if(init_status & GINT_STAT_SOCK1)                                        
    {
        CH395SocketInterrupt(1);                                             /* ����socket 1�ж�*/
    }
    if(init_status & GINT_STAT_SOCK2)
    {
         CH395SocketInterrupt(2);                                            /* ����socket 2�ж�*/
    }
    if(init_status & GINT_STAT_SOCK3)                                        
    {
        CH395SocketInterrupt(3);                                             /* ����socket 3�ж�*/
    }    
	if(init_status & GINT_STAT_SOCK4)
    {
         CH395SocketInterrupt(4);                                            /* ����socket 4�ж�*/
    }
    if(init_status & GINT_STAT_SOCK5)                                        
    {
        CH395SocketInterrupt(5);                                             /* ����socket 5�ж�*/
    }    
	if(init_status & GINT_STAT_SOCK6)
    {
         CH395SocketInterrupt(6);                                            /* ����socket 6�ж�*/
    }
    if(init_status & GINT_STAT_SOCK7)                                        
    {
        CH395SocketInterrupt(7);                                             /* ����socket 7�ж�*/
    }
}

/*******************************************************************************
* Function Name  : CH395Init
* Description    : ����CH395��IP,GWIP,MAC�Ȳ���������ʼ��
* Input          : None
* Output         : None
* Return         : ����ִ�н��
*******************************************************************************/
UINT8  CH395Init(void)
{
    UINT8 i;
    
    i = CH395CMDCheckExist(0x65);                      
    if(i != 0x9a)return CH395_ERR_UNKNOW;                                   /* �����������޷�ͨ������0XFA */
                                                                            /* ����0XFAһ��ΪӲ��������߶�дʱ�򲻶� */
#if (CH395_OP_INTERFACE_MODE == 5)                                   
#ifdef UART_WORK_BAUDRATE
    CH395CMDSetUartBaudRate(UART_WORK_BAUDRATE);                            /* ���ò����� */   
    mDelaymS(1);
    SetMCUBaudRate();
    i = xReadCH395Data();                                                   /* ������óɹ���CH395����CMD_ERR_SUCCESS */
    if(i == CMD_ERR_SUCCESS)printf("Set Success\n");
#endif
#endif
    CH395CMDSetIPAddr(CH395Inf.IPAddr);                                     /* ����CH395��IP��ַ */
    CH395CMDSetGWIPAddr(CH395Inf.GWIPAddr);                                 /* �������ص�ַ */
    CH395CMDSetMASKAddr(CH395Inf.MASKAddr);                                 /* �����������룬Ĭ��Ϊ255.255.255.0*/   
    i = CH395CMDInitCH395();                                                /* ��ʼ��CH395оƬ */
    return i;
}

/*******************************************************************************
* Function Name  : mInitSTDIO
* Description    : ���ڳ�ʼ��,������ʹ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void mInitSTDIO( void )
{
     SCON = 0x50;                                                            
     PCON = 0x80;                                                            
     TL2 = RCAP2L = 0 - 6;     
     TH2 = RCAP2H = 0xFF;
     T2CON = 0x34;                                                              
     TI = 1;
}

/*******************************************************************************
* Function Name  : CH395_FTPSendData
* Description    : ��������
* Input          : PSend-���͵���������
                   Len-  ����
                   index-socket����
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPSendData( UINT8 *PSend, UINT16 Len,UINT8 index  )
{
     CH395SendData(index,PSend,Len);
}

/*******************************************************************************
* Function Name  : CH563NET_FTPGetSockeID
* Description    : ��ȡ��������socketid
* Input          : socketid-socket����
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPGetSockeID( UINT8 socketid )
{
     UINT8 i;

#if DEBUG
     printf("get socketid\n");
#endif     
     if(SockInf[socketid].SourPort == 21){
          if(ftp.TcpCtlSta <= FTP_MACH_DISCONT){
               ftp.TcpCtlSta = FTP_MACH_CONNECT;
               ftp.SocketCtl = socketid;
#if DEBUG
     printf("ftp.SocketCtl = %d\n",(UINT16)ftp.SocketCtl);
#endif     
          }
          else{
               i = CH395CloseSocket(socketid);                               /* ֻ����һ���ͻ�������      */
               mStopIfError( i ); 
#if DEBUG
     printf("ERROR: only support a control socket connected\n");
#endif     
          }
     }
     if(SockInf[socketid].SourPort == 20||SockInf[socketid].SourPort == (256*5 + SerPort)){
          if(ftp.TcpDatSta <= FTP_MACH_DISCONT){
               ftp.TcpDatSta = FTP_MACH_CONNECT;
               ftp.SocketDat = socketid;
#if DEBUG
     printf("ftp.SocketDat = %d\n",(UINT16)ftp.SocketDat);
#endif     
          }
          else{
               i = CH395CloseSocket(socketid);                              /* ֻ����һ���ͻ�������      */
               mStopIfError( i ); 
#if DEBUG
     printf("ERROR: only support a data socket connected\n");
#endif     
          }
     }     
}

/*******************************************************************************
* Function Name  : CH563NET_FTPInitVari
* Description    : ������ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPInitVari( )
{
     FTP *pFTP;
     UINT8 i,j;

     pFTP = &ftp;
     memset((void *)pFTP,0,sizeof(ftp));
     ftp.SocketDat = 255;
     ftp.SocketCtl = 255;
     mInitFtpList( );
     memset((void *)SourIP,'\0',sizeof(SourIP));                       
     j = 0;
     for(i=0;i<4;i++){                                                      /* ��ʮ���Ƶ�IP��ַת������Ҫ���ַ���ʽ*/
          if( CH395IPAddr[i]/100 ){
               SourIP[j++] = CH395IPAddr[i]/100 + '0';
               SourIP[j++] = (CH395IPAddr[i]%100)/10 + '0' ;
               SourIP[j++] = CH395IPAddr[i]%10 + '0';
          } 
          else if( CH395IPAddr[i]/10 ){
               SourIP[j++] = CH395IPAddr[i]/10 + '0';
               SourIP[j++] = CH395IPAddr[i]%10 + '0';
          } 
          else SourIP[j++] = CH395IPAddr[i]%10 + '0';
           SourIP[j++] = '.';
     }
}

/*******************************************************************************
* Function Name  : CH395_FTPConnect
* Description    : CH395��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPConnect( )
{
    UINT8 i;
 
    mDelaymS(100);                                                          /* ��ʱ100���� */
    mInitSTDIO();  
     CH395_FTPInitVari( );                                                  
#if  CH395_DEBG
    printf("CH395FTP Demo\n");
#endif     
    CH395_PORT_INIT();

    InitCH395InfParam();                                                     /* ��ʼ��CH395��ر��� */
    i = CH395Init();                                                         /* ��ʼ��CH395оƬ */
    mStopIfError(i);
                                                                     
   while(1)
   {                                                                         /* �ȴ���̫�����ӳɹ�*/
       if(CH395CMDGetPHYStatus() == PHY_DISCONN)                             /* ��ѯCH395�Ƿ����� */
       {
           mDelaymS(200);                                                    /* δ������ȴ�200MS���ٴβ�ѯ */
       }
       else 
       {
#if  CH395_DEBG
           printf("CH395 Connect Ethernet\n");                               /* CH395оƬ���ӵ���̫������ʱ������ж� */
 #endif     
          break;
       }
   }
   CH395_FTPCtlServer( 0 );
}

