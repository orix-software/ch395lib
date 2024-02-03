/********************************** (C) COPYRIGHT *********************************
* File Name          : MAIN
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395 mail ������ʾ
**********************************************************************************/
#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include "../PUB/CH395INC.H"
#include "../PUB/CH395CMD.H"
#include "ch395.h"
/**************************************************************************************************************************
* �������� (Ĭ��Ϊ������)*
* 1  ֻ�����ʼ�--������ɺ��˳���½���ر�socket���ӣ�
* 2  ֻ�����ʼ�--������ɡ���ȡ�ʼ��б������˳���½���ر�socket���ӣ�
* 3  ֻ�����ʼ�--������ɡ���ȡ�ʼ���ɾ�������˳���½���ر�socket���ӣ�
* 4  �����ʼ��ҽ����ʼ�--������ɺ�ʼ�����ʼ���������ɺ��˳���½���ر�socket����; 
* 5  �����ʼ��ҽ����ʼ�--������ɺ�ظ��ʼ���������ɺ����˳���½��ɾ���ʼ����˳����ʼ���½�����ر�socket���ӡ�  
* ˵�����˴�������뷢���õ�����������socket���ӣ�
*       ���������������Ĳ�����eg����������գ�������������Ͳ���ֻ����ch395mail.c�����е�
        ch395mail_CheckResponse�����ӳ����н�����ɺ󣬽��������Ϊ SMTP_SEND_START��
*************************************************************************************************************************/

#define  mail_work_mode               5           
#define CH395_INT_WIRE              INT1                                                          /* CH395��INT#���� */
#include "mailinc.h"
/**********************************************************************************/

/* CH395��ض��� */
const UINT8 CH395IPAddr[4] =   {192,168,1,100};                                                    /* CH395IP��ַ */
const UINT8 CH395GWIPAddr[4] = {192,168,1,1};                                                      /* CH395���� */
const UINT8 CH395IPMask[4] =   {255,255,255,0};                                                     /* CH395�������� */
/* socket0 ��ض���,SMTP�����ʼ�  */                      
const UINT8  Socket0DesIP[4] = {58,213,45,186};                                                     /* Socket 0Ŀ��IP��ַ */
const UINT16 Socket0DesPort =  SMTP_SERVER_PORT;                                                    /* Socket 0Ŀ�Ķ˿�*/
const UINT16 Socket0SourPort = 6212;                                                                /* Socket 0Դ�˿� */
/*  {123,58,178,201}   "smtp.126.com"    */
/*  {123,58,178,204}  "smtp.163.com"    */
/*  {113,108,16,44}    "smtp.qq.com"    */
/*  {74,125,129,108}  "smtp.gmail.com" */
/* socket1 ��ض���, POP���ʼ�*/                        
const UINT8  Socket1DesIP[4] =  {58,213,45,186};                                                     /* Socket 1Ŀ��IP��ַ*/
const UINT16 Socket1DesPort =  POP3_SERVER_PORT;                                                     /* Socket 1Ŀ�Ķ˿� */
const UINT16 Socket1SourPort = 8152;                                                                 /* Socket 1Դ�˿�   */
/*  {220,181,15,128}  "pop.126.com", */
/*  {220,181,12,101}  "pop.163.com", */
/*  {113,108,16,116}  "pop.qq.com",  */
/*  {74,125,141,108}  "pop.gmail.com"*/
/* smtp�����ʼ���ز��� */
const UINT8  *m_Server =   "0000000";                                                                 /* ����������  */
const UINT8  *m_UserName = "000";                                                                     /* �û���    */
const UINT8  *m_PassWord = "00000000";                                                                /* ����      */
const UINT8  *m_SendFrom = "000000000";                                                               /* �����˵�ַ  */
const UINT8  *m_SendName = "one";                                                                     /* ����������  */
const UINT8  *m_SendTo =   "000000";                                                                  /* �ռ��˵�ַ  */
const UINT8  *m_Subject =  "text";                                                                    /* ����       */
const UINT8  *m_FileName = "m_file.txt";                                                              /* ��������(��������͸���,����Ϊ"\0") */
/* pop�����ʼ���� */
const UINT8 *p_Server =   "00000000";                                                                 /* POP������       */
const UINT8 *p_UserName = "0000000";                                                                  /* POP��½�û���    */
const UINT8 *p_PassWord = "0000000";                                                                  /* POP��½����    */

/***********************************************************************************/
UINT8   ReceDatFlag = 0;                                                                              /* �յ����ݱ�־λ*/
UINT8   SendDatFalg = 0;     
UINT8   CheckType;                                                                                    /* �����źź˶�����*/ 
UINT8   OrderType;                                                                                    /* ��������    */
UINT16   ReceLen;                                                                                     /* ���ջ��������յ������ݳ��� */
/***********************************************************************************
* Function Name  : main
* Description    : ������
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void main(  )
{
  ch395mail( );
#ifdef send_mail 
  ch395mail_smtpinit( );
  if( send_mail )   CH395SocketInitOpen( SendSocket );
#endif
#ifdef  receive_mail
  ch395mail_pop3init( );
  if( receive_mail )   CH395SocketInitOpen( ReceSocket );
#endif
  while(1){
    if(CH395_INT_WIRE == 0) CH395GlobalInterrupt();
    if(ReceDatFlag){
      ReceDatFlag = 0;
      ch395mail_MailCommand( OrderType );    
#ifdef  POP_RTER 
      if(OrderType == POP_RECEIVE_RTER ) ch395mail_pop3Retr( '1' );                                   /* ����server�ʼ���ȫ���ı�����  �������ʼ��ţ�*/
#endif
#ifdef  POP_DELE 
      if(OrderType == POP_RECEIVE_DELE ) ch395mail_pop3Dele( '1' );                                   /* ����server���ɾ����QUIT����ִ��ʱ������ɾ���������ʼ��ţ�*/
#endif
#ifdef  POP_RSET 
      if(OrderType == POP_RECEIVE_RSET ) ch395mail_pop3Rset(  );                                      /* �����������е�DELE����  */
#endif
#ifdef  POP_TOP 
      if(OrderType == POP_RECEIVE_TOP ) ch395mail_pop3Top( '1','3' );                                 /* ����n���ʼ���ǰm�����ݣ������ʼ��ţ��кţ�����Ϊ��Ȼ������*/
#endif
#ifdef  POP_UIDL 
      if(OrderType == POP_RECEIVE_UIDL ) ch395mail_pop3Uidl( '1' );                                   /* ����server�������ڸ�ָ���ʼ���Ψһ��ʶ�����û��ָ�����������еġ��������ʼ��ţ�*/
#endif
    }
  }
}