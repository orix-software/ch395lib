/********************************** (C) COPYRIGHT *********************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395WEB������ʾ
**********************************************************************************/

/*********************************************************************************
CH395 TCP/IP Э����ӿ�
CH563 ��ʾ����������ʾCH395WEB����������
1��ͨ��WEBSEVER����ҳ��ɷ���Ľ���LED���ƣ�����ʱ��ʾLED״̬��
2����ͨ��Ĭ��IP��192.168.111.64��Ĭ�϶˿�80������WebServer�������ڿ���ҳ�����IP��Port���ã����change��ť��
   ��������������µ�IP�Ͷ˿ڣ�ͨ���µ�IP��Port����WebServer��
3���ڼ�ⴰ�ڣ���ͨ�����watch��ťʵʱ�鿴�����Ϣ��
**********************************************************************************/

/* ͷ�ļ�����*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "..\SRC\CH563SFR.H"
#include "..\SRC\SYSFREQ.H"
#include "..\..\PUB\CH395INC.H"
#include "CH395.H"
#include "HTTPS.H"

/**********************************************************************************/
/*
CH395_OP_INTERFACE_MODE����Ϊ1-5
1��Ӳ�����߲������ӷ�ʽ
2������ģ�Ⲣ�����ӷ�ʽ
3: Ӳ��SPI���ӷ�ʽ
4: ����ģ��SPI��ʽ
5: Ӳ���첽�������ӷ�ʽ
*/
#define   CH395_OP_INTERFACE_MODE             3                    
#if   (CH395_OP_INTERFACE_MODE == 1)                                                  /* SEL = 0, TX = 1*/
#include "CH395PARA_HW.C"                                           
#elif (CH395_OP_INTERFACE_MODE == 2)                                                  /* SEL = 0, TX = 1*/
#include "CH395PARA_SW.C"                                                             /*ģ�Ⲣ�ڣ����޽ӿڳ���*/
#elif (CH395_OP_INTERFACE_MODE == 3)                                                  /* SEL = 1, TX = 0*/
#include "CH395SPI_HW.C"
#elif (CH395_OP_INTERFACE_MODE == 4)                                                  /* SEL = 1, TX = 0*/
#include "CH395SPI_SW.C"                                                              /*ģ��spi���޽ӿڳ���*/
#elif (CH395_OP_INTERFACE_MODE == 5)                                                  /* SEL = 1, TX = 1*/
#include "CH395UART.C"
#else
#error "Please Select Correct Communication Interface "
#endif

/***********************************************************************************/
/* ���������ļ� */
#include "..\..\PUB\CH395CMD.C"

/* ���ñ������� */
 st_http_request *http_request;  
 UINT8  flag;		   		  									                                              /*CH395���³�ʼ����־*/
 UINT8  RecvBuffer[4096];               

/* CH395��ض��� */
 UINT8 CH395MACAddr[6] = {0x02,0x03,0x04,0x05,0x06,0x07};                                /* CH395MAC��ַ */
 UINT8 CH395IPAddr[4] = {192,168,1,10};                                                /* CH395IP��ַ */
 UINT8 CH395GWIPAddr[4] = {192,168,10,1};                                               /* CH395���� */
 UINT8 CH395IPMask[4] = {255,255,255,0};                                                 /* CH395�������� */

/* socket ��ض���*/
/* socket ��ض���*/
 UINT16 Socket0SourPort = 80;                                                            /* Socket 0Դ�˿� */

__irq void FIQ_Handler( void )
{
    while(1);
}
__irq void IRQ_Handler( void )
{
    while(1);
}

/**********************************************************************************
* Function Name  : mInitSTDIO
* Description    : ���ڳ�ʼ��,������ʹ��
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void  mInitSTDIO(void)
{
  UINT32  x, x2;

  /*����0��ʼ�������ڴ�ӡ������Ϣ*/
  x = 10 * FREQ_SYS * 2 / 16 /115200;                                                   /* ���ڲ�����Ϊ115200bps*/
  x2 = x % 10;
  x /= 10;
  if ( x2 >= 5 ) x ++;                                                                   /* �������� */
  R8_UART0_LCR = 0x80;                                                                   /* DLABλ��1   */
  R8_UART0_DIV = 1;                                                                      /* Ԥ��Ƶ   */
  R8_UART0_DLM = x>>8;
  R8_UART0_DLL = x&0xff;
  R8_UART0_LCR = RB_LCR_WORD_SZ ;                                                         /*�����ֽڳ���Ϊ8 */
  R8_UART0_FCR = RB_FCR_FIFO_TRIG|RB_FCR_TX_FIFO_CLR|RB_FCR_RX_FIFO_CLR |  RB_FCR_FIFO_EN ;/*����FIFO����Ϊ28���巢�ͺͽ���FIFO��FIFOʹ��  */
  R8_UART0_IER = RB_IER_TXD_EN | RB_IER_LINE_STAT |RB_IER_THR_EMPTY | RB_IER_RECV_RDY  ;  /* TXD enable */
  R8_UART0_MCR = RB_MCR_OUT2;                                                             /*�����ж����ʹ�� */
  R8_INT_EN_IRQ_0 |= RB_IE_IRQ_UART0;  
  R32_PB_SMT |= RXD0|TXD0;                                                                /* RXD0 schmitt input, TXD1 slow rate     */
  R32_PB_PD &= ~ RXD0;                                                                    /* disable pulldown for RXD1, keep pullup */
  R32_PB_DIR |= TXD0;                                                                     /* TXD1 output enable */

  /*����1��ʼ��������CH395ͨ��*/
  x = 10 * FREQ_SYS * 2 / 16 /9600;                                                       /* 9600bps */
  x2 = x % 10;
  x /= 10;
  if ( x2 >= 5 ) x ++; 
  R8_UART1_LCR = 0x80;  
  R8_UART1_DIV = 1;  
  R8_UART1_DLM = x>>8;
  R8_UART1_DLL = x&0xff;
  R8_UART1_LCR = RB_LCR_WORD_SZ ;
  R8_UART1_FCR = RB_FCR_FIFO_TRIG|RB_FCR_TX_FIFO_CLR|RB_FCR_RX_FIFO_CLR |  RB_FCR_FIFO_EN ;
  R8_UART1_IER = RB_IER_TXD_EN | RB_IER_LINE_STAT |RB_IER_THR_EMPTY | RB_IER_RECV_RDY  ;  
  R8_UART1_MCR = RB_MCR_OUT2; 
  R8_INT_EN_IRQ_0 |= RB_IE_IRQ_UART1;  
  R32_PB_SMT |= RXD1|TXD1;  
  R32_PB_PD &= ~ RXD1; 
  R32_PB_DIR |= TXD1;  
}

int  fputc( int c, FILE *f )
{
  R8_UART0_THR = c;                          
  while( ( R8_UART0_LSR & RB_LSR_TX_FIFO_EMP ) == 0 );      
  return( c );
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
   UINT16 i;
   R32_PA_DIR = 0xffffffff;                                                            /*����PA�������*/
   R32_PA_PU  = 0xffffffff;
   R32_PA_DIR |= (TACK|TDI);                                                           /*GPIO��ʼ��������LED�ƿ���*/
   R32_PA_PU  |= (TACK|TDI);
   R32_PA_OUT |= (TACK|TDI);
   mDelaymS(100);
   mInitSTDIO();
   http_request = (st_http_request*)RecvBuffer;
   printf("CH395EVT Test Demo\n");
   CH395_PROT_INIT();
 restart:                                                                               /* ��ʱ100���� */ 
   InitCH395InfParam();                                                                /* ��ʼ��CH395��ر��� */
   i = CH395Init();                                                                    /* ��ʼ��CH395оƬ */
   mStopIfError(i);
   while(1)
   {                                                                                   /* �ȴ���̫�����ӳɹ�*/
       if(CH395CMDGetPHYStatus() == PHY_DISCONN)                                       /* ��ѯCH395�Ƿ����� */
       {
           mDelaymS(200);                                                              /* δ������ȴ�200MS���ٴβ�ѯ */
       }
       else 
       {
           printf("CH395 Connect Ethernet\n");                                         /* CH395оƬ���ӵ���̫������ʱ������ж� */
           break;
       }
   }
   InitSocketParam();                                                                  /* ��ʼ��socket��ر��� */
   CH395SocketInitOpen();
   while(1)
   {
     if(Query395Interrupt())CH395GlobalInterrupt();                                    /*��ѯ���ж�*/
     WebServer();
     if((flag & IPCHANGE )|( flag & PORTCHANGE) )                                      /*IP����Port�ı���λCH395�����³�ʼ��*/
     {
       flag  &=~(IPCHANGE|PORTCHANGE) ;
       printf("reset all!\n");
       CH395CMDReset();
       for(i=0;i<15;i++) { mDelaymS(200);}                                             /*��Լ��ʱ3S*/
       printf("restart\n");
       goto restart;
     }
   }
}
