/********************************** (C) COPYRIGHT *********************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395 DNS������ʾ
**********************************************************************************

 CH395 DNS��������Ӧ�ã����������ɲ�ѯ����������Ӧʵ��IP�� 
 1�������̰���һ��CH395DNS�������̣�ͨ������DnsQuery()��������ʵ��������IP��ת����
 2��DNS.c�ļ���DNS_SERVER_IP��Ҫ����ʵ����������޸ġ�
**********************************************************************************/
/* ͷ�ļ�����*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "..\SRC\CH563SFR.H"
#include "..\SRC\SYSFREQ.H"
#include "..\..\PUB\CH395INC.H"
#include "CH395.H"
#include "dns.h"
/**********************************************************************************
CH395_OP_INTERFACE_MODE����Ϊ1-5
1��Ӳ�����߲������ӷ�ʽ
2������ģ�Ⲣ�����ӷ�ʽ
3: Ӳ��SPI���ӷ�ʽ
4: ����ģ��SPI��ʽ
5: Ӳ���첽�������ӷ�ʽ
**********************************************************************************/
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

#define MAX_URL_SIZE                            128

UINT8  url_dn1[MAX_URL_SIZE] = "www.baidu.com";    
UINT8  url_dn2[MAX_URL_SIZE] = "www.google.com";    
UINT8  url_dn3[MAX_URL_SIZE] = "www.wch.cn";    

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
/* Ϊprintf��getkey���������ʼ������ */
void mInitSTDIO( void )
{
    UINT32    x, x2;

    x = 10 * FREQ_SYS * 2 / 16 / 115200;                                        /* 115200bps */
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;                                                        /* �������� */
    R8_UART0_LCR = 0x80;                                                        /* DLABλ��1 */
    R8_UART0_DIV = 1;                                                           /* Ԥ��Ƶ */
    R8_UART0_DLM = x>>8;
    R8_UART0_DLL = x&0xff;

    R8_UART0_LCR = RB_LCR_WORD_SZ ;                                             /* �����ֽڳ���Ϊ8 */
    R8_UART0_FCR = RB_FCR_FIFO_TRIG|RB_FCR_TX_FIFO_CLR|RB_FCR_RX_FIFO_CLR |    
                   RB_FCR_FIFO_EN ;                                             /* ����FIFO������Ϊ14���巢�ͺͽ���FIFO��FIFOʹ�� */
    R8_UART0_IER = RB_IER_TXD_EN;                                               /* TXD enable */
    R32_PB_SMT |= RXD0|TXD0;                                                    /* RXD0 schmitt input, TXD0 slow rate */
    R32_PB_PD &= ~ RXD0;                                                        /* disable pulldown for RXD0, keep pullup */
    R32_PB_DIR |= TXD0;                                                         /* TXD0 output enable */
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
   
	UINT8 i,ip[4]={0,0,0,0,};
	Delay_ms(100);                                                                      /* ��ʱ100���� */ 
	mInitSTDIO(); 

	R32_PA_DIR = 0xffffffff;                                                            /*����PA�������*/
	R32_PA_PU  = 0xffffffff;

	printf("CH395EVT  DNS TEST \n");
	CH395_PROT_INIT();
	InitCH395InfParam();                                                                /* ��ʼ��CH395��ر��� */
	i = CH395Init();                                                                    /* ��ʼ��CH395оƬ */
	mStopIfError(i);

	while(1)
	{                                                                                    /* �ȴ���̫�����ӳɹ�*/
		if(CH395CMDGetPHYStatus() == PHY_DISCONN)                                         /* ��ѯCH395�Ƿ����� */
		{                                          
			Delay_ms(10);                                                                 /* δ������ȴ�200MS���ٴβ�ѯ */
		}
		else 
		{
			break;
		}
	}
	Delay_ms(10);

	while(1)
	{
		if(Query395Interrupt())CH395GlobalInterrupt();                                      /*��ѯ���ж�*/
		i = DnsQuery(0,url_dn3,ip);                                                         /*��ѯ"www.baidu.com"*/
		if(i)
		{ 
			printf("Domain name: %s \n",url_dn3);
			printf(" HTTPs_IP= %d.%d.%d.%d\n\n",ip[0],ip[1],ip[2],ip[3]);
			status = 1;
			break;
		}   
	}
	while(1);  
}

