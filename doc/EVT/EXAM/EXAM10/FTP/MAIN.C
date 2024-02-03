/********************************** (C) COPYRIGHT *********************************
* File Name          : MAIN.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬFTP������Ӧ��
**********************************************************************************/
#define Access_Authflag          0                                    /* ����ָ���û������ʣ������û����޷����ʷ�������*/
                                                                      /* ��1�����˹��ܣ�Ĭ��Ϊ0�κ��û����ɵ�¼     */
#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include "../PUB/CH395INC.H"
#include "../PUB/CH395CMD.H"
#include "CH395FTPINC.H"
#include "CH395.h"

/**********************************************************************************/
/* CH395��ض��� ,����ʹ�����������Ӧ���޸�*/
const UINT8 CH395MACAddr[6]  = {0x02,0x03,0x04,0x05,0x06,0x07};       /* CH395MAC��ַ */
const UINT8 CH395IPAddr[4]   = {192,168,1,100};                      /* CH395IP��ַ */
const UINT8 CH395GWIPAddr[4] = {192,168,1,1};                     /* CH395���� */
const UINT8 CH395IPMask[4]   = {255,255,255,0};                       /* CH395�������� */

#define CH395_INT_WIRE       INT1                                     /*  CH395��INT#����  */
extern FTP ftp;      
char   UserName[16];                                                  /* ���ڱ����û��� */
char   SourIP[17];                                                    /* ���ڱ���ת�����ַ���IP��ַ */
#if Access_Authflag
char  *pUserName = "anonymous";                                       /* ���Է��ʷ�����û����������û����޷����ʷ�������*/
#endif
char  *pPassWord = "IEUser@";                                         /* ��Ȩ�޵����루��������ֻ֧�ֶ�������*/

/*********************************************************************************
* Function Name  : main
* Description    : ������
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void main( void )
{
  CH395_FTPConnect( );                                                /* ����TCP FTP�������Ӽ���ʼ��*/
  while(1)
  {
    if(CH395_INT_WIRE == 0) CH395GlobalInterrupt();                   /* ��ѯ�ж�״̬      */
    CH395_FTPServerCmd( );                                            /* ��ѯ״ִ̬����Ӧ����  */
  }
}