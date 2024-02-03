/********************************** (C) COPYRIGHT *********************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬFTP�ͻ���Ӧ��-������
**********************************************************************************/
#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include "../PUB/CH395INC.H"
#include "../PUB/CH395CMD.H"
#include "CH395FTPINC.H"
#include "ch395.h"
extern FTP ftp;			
/**********************************************************************************/
#define CH395_INT_WIRE       INT1                                   /* CH395��INT#����  */
/* CH395��ض��� */
const  UINT8 CH395MACAddr[6]  = {0x02,0x03,0x04,0x05,0x06,0x07};    /* CH395MAC��ַ */
const  UINT8 CH395IPAddr[4]   = {192,168,1,10};                   /* CH395IP��ַ */
const  UINT8 CH395GWIPAddr[4] = {192,168,1,1};                  /* CH395���� */
const  UINT8 CH395IPMask[4]   = {255,255,255,0};                    /* CH395�������� */
const  UINT8 DestIPAddr[4]    = {192,168,1,100};                   /* Ŀ��IP */
/* FTP��ض��� */
const  UINT8 *pUserName = "anonymous";                              /* ������½ */
const  UINT8 *pPassword = "123@";									
char   ListName[24];                                                /* ���ڱ���Ŀ¼�� */
char   ListMdk[24];                                                 /*���ڱ����´�����Ŀ¼�� */
char   FileName[24];                                                /*���ڱ����Ѳ��ļ����ļ���	*/
char   SourIP[17];                                                  /*���ڱ���ת�����ַ���IP��ַ */

/***********************************************************************************
* Function Name  : main
* Description    : ������
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void main( void )
{
  CH395_FTPConnect( );                                              /*����TCP FTP��������*/
	while(1)
	{
		if(CH395_INT_WIRE == 0) CH395GlobalInterrupt();
    CH395_FTPClientCmd( );                                          /*��ѯ״ִ̬����Ӧ����*/
	}
}