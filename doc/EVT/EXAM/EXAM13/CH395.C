/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395������ʾ
**********************************************************************************/

/**********************************************************************************
CH395 TCP/IP Э����ӿ�
MSC51 ��ʾ����������ʾCH395�ĵ��ܺ�ģʽ��GPIO��������EEPROM(4K)������MCS51@24MHZ,KEIL 3.51
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
UINT8 xdata MyBuffer[512];                                           /* ���ݻ����� */
struct _SOCK_INF xdata SockInf;                                      /* ����Socket��Ϣ */
struct _CH395_SYS xdata CH395Inf;                                    /* ����CH395��Ϣ */

//UINT8 MyBuffer_temp[512];                                           /* ���ݻ����� */
/* CH395��ض��� */
const UINT8 CH395IPAddr[4] = {192,168,1,10};                         /* CH395IP��ַ */
const UINT8 CH395GWIPAddr[4] = {192,168,1,1};                        /* CH395���� */
const UINT8 CH395IPMask[4] = {255,255,255,0};                        /* CH395�������� */

/* socket ��ض���*/
const UINT8  Socket0DesIP[4] = {192,168,1,100};                      /* Socket 0Ŀ��IP��ַ */
const UINT16 Socket0DesPort = 1000;                                  /* Socket 0Ŀ�Ķ˿� */
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
    printf("Error: %02X\n", (UINT16)iError);                         /* ��ʾ���� */
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
    memcpy(SockInf.IPAddr,Socket0DesIP,sizeof(Socket0DesIP));        /* ��Ŀ��IP��ַд�� */
    SockInf.DesPort = Socket0DesPort;                                /* Ŀ�Ķ˿� */
    SockInf.SourPort = Socket0SourPort;                              /* Դ�˿� */
    SockInf.ProtoType = PROTO_TYPE_TCP;                              /* TCPģʽ */
    SockInf.TcpMode = TCP_CLIENT_MODE;
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

    /* socket 0ΪTCP �ͻ���ģʽ */
    CH395SetSocketDesIP(0,SockInf.IPAddr);                           /* ����socket 0Ŀ��IP��ַ */         
    CH395SetSocketProtType(0,SockInf.ProtoType);                     /* ����socket 0Э������ */
    CH395SetSocketDesPort(0,SockInf.DesPort);                        /* ����socket 0Ŀ�Ķ˿� */
    CH395SetSocketSourPort(0,SockInf.SourPort);                      /* ����socket 0Դ�˿� */
    i = CH395OpenSocket(0);                                          /* ��socket 0 */
    mStopIfError(i);                                                 /* ����Ƿ�ɹ� */
    i = CH395TCPConnect(0);                                          /* TCP���� */
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
   UINT8  i;
   UINT16 len;
   UINT16 tmp;

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
       if(len > 512)len = 512;                                       /* MyBuffer����������Ϊ512 */
       CH395GetRecvData(sockindex,len,MyBuffer);                     /* ��ȡ���� */
       for(tmp =0; tmp < len; tmp++)                                 /* ���������ݰ�λȡ�� */
       {
          MyBuffer[tmp] = ~MyBuffer[tmp];
       }
       CH395SendData(sockindex,MyBuffer,len);

   }
   if(sock_int_socket & SINT_STAT_CONNECT)                          /* �����жϣ�����TCPģʽ����Ч*/
   {
    }
   /*
   **��TCPӦ��ʱ�����Ͽ��жϺͳ�ʱ�ж�ʱ����Ҫ�ڳ�����ʹ��CH395CloseSocket()�����رմ�Socket���ٴ�ʹ��
   **�ʹ򿪣���Ҫ���Ӿ����ӣ����������û�йرմ�Socket�ᵼ�´�Socket���ᱻ�ͷţ�������ʹ�á�
   **
   */
   if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* �Ͽ��жϣ�����TCPģʽ����Ч */
   {
	   i = CH395CloseSocket(sockindex);                             
		mStopIfError(i);
   }
   if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* ��ʱ�жϣ�����TCPģʽ����Ч */
   {
        i = CH395CloseSocket(sockindex);                             
		mStopIfError(i);
       if(SockInf.TcpMode == TCP_CLIENT_MODE)                        /* ������ʵ��ֻ����һ��socket */
       {
           mDelaymS(200);                                            /* ��ʱ200MS���ٴ����ԣ�û�б�Ҫ����Ƶ������ */
           i = CH395OpenSocket(sockindex);
           mStopIfError(i);
           CH395TCPConnect(sockindex);                               /* ��ʼ���� */
           mStopIfError(i);
      }
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
   UINT8   buf[10]; 
 
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
    if(i != 0x9a)return CH395_ERR_UNKNOW;                            /* �����������޷�ͨ������0XFA */
                                                                     /* ����0XFAһ��ΪӲ��������߶�дʱ�򲻶� */
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
	/* CH395������ܺ�ģʽ,����SINT_STAT_SEND_OK�ж�*/
    CH395SetStartPara(FUN_PARA_FLAG_LOW_PWR|SOCK_DISABLE_SEND_OK_INT);                         
    mDelaymS(100);

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
* Function Name  : CH395EEPROMTEST
* Description    : EEPROM������ʾ
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395EEPROMTEST(void)
{
   UINT8 i,s,Buf[64],BufT[64],n = 0;
   UINT16 add = 0;
   UINT16 mdata = 0;
   UINT8 k;
   printf("Test EEPROM\n");
   i = CH395EEPROMErase();                                           /*����EEPROM����*/
   printf("EEPROM Erase = %02x\n",(UINT16)i);

   while(1)
   {
     for(i = 0; i < 64;i++)
     {
      Buf[i] = mdata++;
      BufT[i] = Buf[i];
     }
     if((n==0 || n==31 || n== 63))
     {
      printf("|-----EepromNum: %d ---------|\n",(UINT16)n);
      printf("Write EEProm:.......  ");
     }
     k = CH395EEPROMWrite(add,Buf,64);                 
     if((n==0 || n==31 || n== 63))                                   /*дEEPROM����*/
     {       
      printf("WS = %02x      ",(UINT16)k);        
      printf("Read EEProm:.......\n");                   
      for(i = 0; i < 64;i++)Buf[i] = 0;
     }

     CH395EEPROMRead(add,Buf,64);
     if((n==0 || n==31 || n== 63))                                   /*��EEPROM����*/
     {            
      for(i = 0; i < 32;i++)printf("%02x ",(UINT16)Buf[i]);
      printf("\n");
      for(i = 0; i < 32;i++)printf("%02x ",(UINT16)BufT[i]);
      printf("\n");
     }          
     s = memcmp(Buf,BufT,64);  
     if(s) printf("********************EEPROM ERROR*****************\n");        
     add += 64;
     n++;
     if(mdata >255) mdata = 0;
     if(add >= 4096)break;
    }
}

/**********************************************************************************
* Function Name  : CH395GpioTest
* Description    : GPIO������ʾ
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395GpioTest(void)
{
   UINT8 i,j;
   printf("|*******************GPIO TesT*******************************|\r\n");

   printf("|************GPIO OutPutTest ***********************|\r\n");
   CH395WriteGPIOAddr(GPIO_DIR_REG,0xff);                            /*�����*/
   CH395WriteGPIOAddr(GPIO_PU_REG, 0xff);       					 /*����*/
   CH395WriteGPIOAddr(GPIO_OUT_REG,0x00);                            /*����͵�ƽ*/
   
   printf("|*************Twinkle 5 times*************************|\r\n");
   for(i=0;i<5;i++)
   {                                                                 /*��ʾGPIO��������ܣ�����LED����˸5��*/
     CH395WriteGPIOAddr(GPIO_OUT_REG,0x00);
     mDelaymS(200);
     CH395WriteGPIOAddr(GPIO_OUT_REG,0x55);
     mDelaymS(200);
     CH395WriteGPIOAddr(GPIO_OUT_REG,0xaa);
     mDelaymS(200);
     CH395WriteGPIOAddr(GPIO_OUT_REG,0xff);
     mDelaymS(200);
   }
   printf("|************GPIO InPutTest ***********************|\r\n");  
   CH395WriteGPIOAddr(GPIO_DIR_REG,0x00);     
   CH395WriteGPIOAddr(GPIO_PU_REG, 0xff);       
   mDelaymS(500);
   for( j=0;j<50;j++)
   {
     i = CH395ReadGPIOAddr(GPIO_IN_REG);                             /*��ȡ��ǰGPIO״̬*/
     printf("CurrentStatus : %2x \r\n",(UINT16)i);
     mDelaymS(1000);
   }
    
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
    printf("CH395EVT Test Demo\n");
    CH395_PORT_INIT();
    InitCH395InfParam();                                             /* ��ʼ��CH395��ر��� */
    i = CH395Init();                                                 /* ��ʼ��CH395оƬ ���˺�������ʾ������ܺ�ģʽ*/
    mStopIfError(i);
    
	printf("����PHY���ӷ�ʽ��PHY_10M_FLL \n");
	CH395CMDSetPHY(PHY_10M_FLL);	                                  /*����PHY���ӷ�ʽ  */
	mDelaymS(200);   
	mDelaymS(200);   
	
    CH395EEPROMTEST();                                               /*EEPROM������ʾ*/
    CH395GpioTest();                                                 /*GPIO������ʾ*/                  
 
    while(1)
    {                                                                 /* �ȴ���̫�����ӳɹ�*/
       if(CH395CMDGetPHYStatus() == PHY_DISCONN)                     /* ��ѯCH395�Ƿ����� */
       {
           mDelaymS(200);                                            /* δ������ȴ�200MS���ٴβ�ѯ */
       }
       else 
       {
           printf("CH395 Connect Ethernet\n");                       /* CH395оƬ���ӵ���̫������ʱ������ж� */
           break;
       }
    }
    while(1)
    {
       if(CH395_INT_WIRE == 0)CH395GlobalInterrupt();
    }
}
