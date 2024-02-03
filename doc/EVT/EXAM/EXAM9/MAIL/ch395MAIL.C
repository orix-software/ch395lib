/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395MAIL.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬ�����շ��ʼ�
**********************************************************************************/
#include <reg52.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../PUB/CH395INC.H"
#include "../PUB/CH395CMD.H"
#include "mailinc.h"
#include "ch395.h"
#include "mail.h"
#include "ch395mailcmd.c"

#ifdef  receive_over_reply
const  UINT8 *g_mailbody =   "welcome.";                                                       /* �ظ��ʼ��������ݣ����޸�*/
const  UINT8 *g_autograph1 = "\r\n\r\nBest Regards!\r\n----- Original Message ----\r\n";       /* �ظ��ʼ�ǩ������������*/
const  UINT8 *g_autograph2 = "\r\nweb:http://www.wch.cn\r\n";                                  /* �ظ��ʼ�ǩ������������*/
UINT8 data  macaddr[6];                                                                        /* ����mac��ַ��16�������ݣ�*/
UINT8 data  macaddrchar[18];                                                                   /* ���ڱ���ת�����mac��ַ�ַ�*/
UINT8   R_argv[3][32];                                                                         /* ���ڱ���������ʼ�����Ϣ*/
#endif  
/**********************************************************************************
* Function Name  : ch395mail_pop3init
* Description    : �����ʼ���ʼ��
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
#ifdef receive_mail
void ch395mail_pop3init( )
{
  p_pop3 = &m_pop3;
  memset( p_pop3, '\0', sizeof(POP) );
  strcpy( p_pop3->pPop3Server,   p_Server );                                                   /* ����������*/
  strcpy( p_pop3->pPop3UserName, p_UserName );                                                 /* �û���*/
  strcpy( p_pop3->pPop3PassWd,   p_PassWord );                                                 /* ���� */ 
}
#endif   
/******************************************************************************/
#ifdef send_mail
/**********************************************************************************
* Function Name  : ch395mail_smtpinit
* Description    : �����ʼ���ʼ��
* Input          : type �����ж����ֶ����÷��ͣ����ǻظ��ʼ�����
* Output         : None
* Return         : None
**********************************************************************************/
void ch395mail_smtpinit(  )
{
  p_smtp = &m_smtp;
  p_smtp->g_MIME = 0;
  memset( p_smtp, '\0', sizeof(SMTP) );
  strcpy( p_smtp->m_strSMTPServer, m_Server );                                                  /* ����������*/
  strcpy( p_smtp->m_strUSERID,m_UserName );                                                     /* �û���*/
  strcpy( p_smtp->m_strPASSWD,m_PassWord );                                                     /* ���� */   
  strcpy( p_smtp->m_strSendFrom,m_SendFrom );                                                   /* �����˵�ַ*/
  strcpy( p_smtp->m_strSenderName, m_SendName );                                                /* ����������*/
#ifdef receive_over_reply
  strcpy( p_smtp->m_strSendTo,R_argv[0] );                                                      /* �ռ��˵�ַ*/
  strcpy( p_smtp->m_strSubject,R_argv[1] );                                                     /* ����*/
  strcpy(  p_smtp->m_strFile,R_argv[2] );                                                       /* ��������(��������͸������˴������ʼ��)*/
#else 
  strcpy( p_smtp->m_strSendTo,m_SendTo );                                                       /* �ռ��˵�ַ*/
  strcpy( p_smtp->m_strSubject,m_Subject );                                                     /* ����*/
  strcpy( p_smtp->m_strFile,m_FileName );                                                       /* ��������(��������͸������˴������ʼ��)*/
#endif 
}

/**********************************************************************************
* Function Name  : ch395mail_replybody
* Description    : �����ظ��ʼ���������,���ݿ��޸�
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
#ifdef receive_over_reply
void ch395mail_replybody( ) 
{
  memset( MailBodyData,'\0',sizeof(MailBodyData));
  QuotedPrintableEncode( (UINT8 *)"Hello" ,MailBodyData, strlen("Hello"),76 );
  strcat( send_buff, MailBodyData );
  strcat( send_buff, "\r\n    " );
  strcat( send_buff, p_pop3->DecodeRName );
  strcat( send_buff, "!" );
  memset( MailBodyData,'\0',sizeof(MailBodyData));
  QuotedPrintableEncode( (UINT8 *)g_mailbody ,MailBodyData, strlen(g_mailbody),76 );
  strcat( send_buff, MailBodyData );
  if(p_pop3->identitycheck == 1){
    memset( macaddr, '\0', sizeof(macaddr) );
    memset( macaddrchar, '\0', sizeof(macaddrchar) );
    CH395CMDGetMACAddr(macaddr);
    ch395mail_xtochar(macaddr,&macaddrchar[0],strlen(macaddr));
#if DEBUG  
  printf("B len= %02X\n",(UINT16)strlen(macaddrchar));
#endif
    if(strlen(macaddrchar)>17) macaddrchar[17] = '\0';
    strcat( send_buff, "\r\n" );
    memset( MailBodyData,'\0',sizeof(MailBodyData));
     QuotedPrintableEncode( (UINT8 *)"I am ", MailBodyData, strlen("I am "),76 );
    strcat( send_buff, MailBodyData );
    memset( MailBodyData,'\0',sizeof(MailBodyData));
     QuotedPrintableEncode( (UINT8 *)macaddrchar, MailBodyData, strlen(macaddrchar),76 );
    strcat( send_buff, MailBodyData );
    strcat( send_buff, "��\r\n" );
  }
  strcat( send_buff, g_autograph1 );
  strcat( send_buff, "From: \"" );
  strcat( send_buff, p_pop3->DecodeRName );
  strcat( send_buff, "\" <" );
  strcat( send_buff, R_argv[0] );
  strcat( send_buff, ">\r\n" );
  strcat( send_buff, "To: ");
  strcat( send_buff, p_smtp->m_strSendFrom );
   strcat( send_buff, "\r\n" );
  if(strlen(p_pop3->Ccname)){
    strcat( send_buff, "Cc: ");
    strcat( send_buff, p_pop3->Ccname);
     strcat( send_buff, "\r\n" );
  }
  strcat( send_buff, "Sent: ");
  strcat( send_buff, p_pop3->sBufTime );
  strcat( send_buff, "\r\nSubject: ");
  strcat( send_buff, p_smtp->m_strSubject );
  strcat( send_buff, g_autograph2 );
}
#endif  
/******************************************************************************/
#endif  
/*********************************************************************************
* Function Name  : ch395mail_analysemaildata
* Description    : �������յ����ʼ�
* Input          : recv_buff-�ʼ�����
* Output         : None
* Return         : None
**********************************************************************************/
#ifdef receive_over_reply
void ch395mail_analysemaildata( char *recv_buff ) 
{
   UINT16  i,j;
  char AttachEncodetype;
  char conreceiveattach;  

  if(conreceiveattach){
    i = 0;
    if(AttachEncodetype==2) goto type2;                                                        /*  ��ת���������� ���뷽ʽ--quoted-printable*/
    if(AttachEncodetype==1) goto type1;                                                        /*  ��ת���������� ���뷽ʽ--base64*/
    if(AttachEncodetype==0) goto type0;                                                        /*  ��ת���������� ���뷽ʽ--others*/
  }
  for(i=0;i<ReceLen;i++){
/* ����������/��ַ */        
    if( strncmp("\nFrom: ", &recv_buff[i], 7) == 0 ){
      i += 7;
#if DEBUG
  printf("1#From \n");
#endif
      if(recv_buff[i] == '"') i++;  
      if(recv_buff[i] == '='){                                                                  /* ���־�������*/
        while(strncmp("?B?", &recv_buff[i], 3)&&(i < ReceLen)) i++;
        i += 3;
        j = 0;
        memset( send_buff, '\0', sizeof(send_buff) );
        while((recv_buff[i] != '=')&&(recv_buff[i] != '?')&&(i < ReceLen)){
          send_buff[j] = recv_buff[i];
          j++;
          i++;      
        }
        memset( p_pop3->DecodeRName, '\0', sizeof(p_pop3->DecodeRName) );
        Base64Decode(send_buff,  strlen(send_buff),p_pop3->DecodeRName);
#if DEBUG
  printf("DecName:%s\n", p_pop3->DecodeRName);
#endif
      }
      else{  /* ����δ����*/
        j = 0;
        while((recv_buff[i] != '"')&&(i < ReceLen)){
          p_pop3->DecodeRName[j] = recv_buff[i];
          j++;
          i++;      
        }
      }             
      while((recv_buff[i] != '<')&&(i < ReceLen)) i++;
      i++;
      j = 0;
      while((recv_buff[i] != '>')&&(i < ReceLen)){
        R_argv[0][j] = recv_buff[i];
        j++;
        i++; 
      }
    }
// ����
    if( strncmp("\nCc: ", &recv_buff[i], 5) == 0 ){
      i += 5;
#if DEBUG
  printf("2#Cc \n");
#endif    
      while(recv_buff[i] != '<')  i++;
      i++;
      j = 0;
      while(recv_buff[i] != '>'&&(i < ReceLen)){                                               /* �����˵ĵ�ַ*/
        p_pop3->Ccname[j] = recv_buff[i];
        j++;
        i++;      
      }
    }
// ����
    if( strncmp("\nDate: ", &recv_buff[i], 7) == 0 ){
      i += 7;
#if DEBUG
  printf("3#date \n");
#endif
      j = 0;
      while((recv_buff[i] != '\r')&&(i < ReceLen)){                                            /* �����ʼ�����*/
        p_pop3->sBufTime[j] = recv_buff[i];
        j++;
        i++;      
      }
    }
// ����
    if( strncmp("\nSubject: ", &recv_buff[i], 10) == 0 ){
      i += 10;
#if DEBUG
  printf("4#Subject \n");
#endif
      if(recv_buff[i] == '[') {
        while(recv_buff[i] == ']'&&(i < ReceLen)) i++;
        i += 2;
      }
      if(recv_buff[i] == '='){
        while(strncmp("?B?", &recv_buff[i], 3)&&(i < ReceLen)) i++;                            /* ���⾭������*/
        i += 3;
        j = 0;
        memset( send_buff, '\0', sizeof(send_buff) );
        while((recv_buff[i] != '=')&&(recv_buff[i] != '?')&&(i < ReceLen)){                    /* �ʼ�������*/
          send_buff[j] = recv_buff[i];
          j++;
          i++;    
        }
        Base64Decode(send_buff,strlen(send_buff),R_argv[1]);
#if DEBUG
  printf("Decodesubject:%s\n", R_argv[1]);
#endif
      }
      else{                                                                                    /* δ����*/
        j = 0;
        while(recv_buff[i] != '\r'&&(i < ReceLen)){                                            /* �ʼ�������*/
          R_argv[1][j] = recv_buff[i];
          j++;
          i++;    
        }
      }
    }
/* �����ı��뷽ʽ�����֡�����*/
    if( strncmp("name=", &recv_buff[i], 5) == 0 ){
#if DEBUG
  printf("5#\n");
#endif
      i += 5;
      while(strncmp("Content-Transfer-Encoding: ", &recv_buff[i], 27)&&strncmp("filename=", &recv_buff[i], 9)&&(i < ReceLen)) i++;
      if(strncmp("Content-Transfer-Encoding: ", &recv_buff[i], 27) == 0){
/* ���뷽ʽ*/
        i += 27;
        if(strncmp("base64", &recv_buff[i], 6) == 0 ){
#if DEBUG
  printf("5#base64 \n");
#endif
          i += 6;
          AttachEncodetype = 1;
        }
        else if(strncmp("quoted-printable", &recv_buff[i], 16) == 0 ) {
#if DEBUG
  printf("5#quoted-printable \n");
#endif
          i += 16;
          AttachEncodetype = 2;
        }
        else AttachEncodetype = 0;  
/* ��������*/
        while(strncmp("filename=", &recv_buff[i], 9)) i++;
        i += 9;
        while(recv_buff[i] != '"'&&i<ReceLen) i++;
        i++;
#if DEBUG
  printf("6#filename \n");
#endif
        if(recv_buff[i] == '='){
          while(strncmp("?B?", &recv_buff[i], 3)&&(i < ReceLen)) i++;
          i += 3;
          j = 0;
          memset(send_buff,'\0', sizeof(send_buff) );
          while((recv_buff[i] != '=')&&(recv_buff[i] != '?')&&(i < ReceLen)){
            send_buff[j] = recv_buff[i];
            j++;
            i++;    
          }
          Base64Decode(send_buff,  strlen(send_buff),R_argv[2]);
        }
        else{
          j = 0;
          while(recv_buff[i] != '"'&&(i < ReceLen)){
            R_argv[2][j] = recv_buff[i];
            j++;
            i++;      
          }
        }
      }
/* ��������*/
      else if( strncmp("filename=", &recv_buff[i], 9) == 0 ){
#if DEBUG
  printf("6#filename \n");
#endif
        i += 9;
        while(recv_buff[i] != '"'&&i<ReceLen) i++;
        i++;
        if(recv_buff[i] == '='){
          while(strncmp("?B?", &recv_buff[i], 3)&&(i < ReceLen)) i++;
          i += 3;
          j = 0;
          memset(send_buff,'\0', sizeof(send_buff) );
          while((recv_buff[i] != '=')&&(recv_buff[i] != '?')&&(i < ReceLen)){
            send_buff[j] = recv_buff[i];
            j++;
            i++;    
          }
          Base64Decode(send_buff,  strlen(send_buff),R_argv[2]);
        }
        else{
          j = 0;
          while(recv_buff[i] != '"'&&(i < ReceLen)){
            R_argv[2][j] = recv_buff[i];
            j++;
            i++;      
          }
        }
/* �������뷽ʽ*/
        while(strncmp("Content-Transfer-Encoding: ", &recv_buff[i], 27)) i++;
        i += 27;
        if(strncmp("base64", &recv_buff[i], 6) == 0 ){
#if DEBUG
  printf("5#base64 \n");
#endif
          i += 6;
          AttachEncodetype = 1;
        }
        else if(strncmp("quoted-printable", &recv_buff[i], 16) == 0 ) {
#if DEBUG
  printf("5#quoted-printable \n");
#endif
          i += 16;
          AttachEncodetype = 2;
        }
        else AttachEncodetype = 0;  
      }
/* ��������*/
      while( strncmp("\n\r\n", &recv_buff[i], 3) != 0 &&(i < ReceLen)) i++;
      i += 3;
      if(AttachEncodetype==1){  
        j = 0;
        memset(send_buff,'\0', sizeof(send_buff) );
type1:      while((recv_buff[i] != '=')&&strncmp("\r\n--", &recv_buff[i], 4)&&strncmp("\r\n\r\n", &recv_buff[i], 4)&&(i < ReceLen)&&(j<attach_max_len)){
          send_buff[j] = recv_buff[i];
          j++;
          i++;      
        }
        if(i>=ReceLen-1) conreceiveattach = 1;
        else {
          conreceiveattach = 0;
          Base64Decode(send_buff,  strlen(send_buff),AttachmentData);
        }
      }
      else if(AttachEncodetype==2){   
        j = 0;
        memset(send_buff,'\0', sizeof(send_buff));
type2:      while(strncmp("\r\n.\r\n", &recv_buff[i], 5)&&strncmp("\r\n--", &recv_buff[i], 4)&&(i < ReceLen)&&(j<attach_max_len)){
          send_buff[j] = recv_buff[i];
          j++;
          i++;      
        }
        if(i>=ReceLen-1) conreceiveattach = 1;
        else {
          conreceiveattach = 0;
          QuotedPrintableDecode(send_buff,(char *)AttachmentData, strlen(send_buff));
        }
      }
      else{  
#if DEBUG
  printf("7# \n");
#endif
        j = 0;
type0:      while(recv_buff[i] != '\r'&&(i < ReceLen)&&(j<attach_max_len)){
          AttachmentData[j] = recv_buff[i];
          j++;
          i++;      
        }
        if(i>=ReceLen-1) conreceiveattach = 1;
        else conreceiveattach = 1; 
      }
    }
  }
#if DEBUG
  printf("addr:\n %s\n",R_argv[0]);                                                            /* �����˵ĵ�ַ*/
  printf("send name:\n %s\n",p_pop3->DecodeRName);                                             /* ����������*/
  printf("subject:\n %s\n",R_argv[1]);                                                         /* �ʼ�����*/
  printf("attach name:\n%s\n",R_argv[2]);                                                      /* ��������*/
  printf("send time:\n %s\n",p_pop3->sBufTime);                                                /* ����ʱ��*/
  printf("attach text:\n%s\n",AttachmentData);                                                 /* ��������*/
#endif
  if(p_pop3->analysemailflag){
    if(strncmp("wch", AttachmentData, 3) == 0) p_pop3->identitycheck = 1;
    else p_pop3->identitycheck = 0;
#if DEBUG
  printf("11# \n");
#endif
    ch395mail_smtpinit(  );
  }
}
#endif  
/**********************************************************************************
* Function Name  : ch395mail_CheckResponse
* Description    : ��֤�����ź�
* Input          : recv_buff-������Ϣ
                   check_type-��������
* Output         : None
* Return         : None
**********************************************************************************/
UINT8 ch395mail_CheckResponse( UINT8 *recv_buff,UINT8 check_type )
{
  switch(check_type){
#ifdef send_mail
    case SMTP_CHECK_CNNT:   
      if( strncmp("220", recv_buff, 3) == 0 ){
        OrderType = SMTP_SEND_HELO;
        return (CHECK_SUCCESS);
      }
      return SMTP_ERR_CNNT;
    case SMTP_CHECK_HELO:   
      if( strncmp("250", recv_buff, 3) == 0 ){
        OrderType = SMTP_SEND_AUTH;
        return (CHECK_SUCCESS);
      }
      return SMTP_ERR_HELO;
    case SMTP_CHECK_AUTH:                                                                      /* ��½����*/
      if(strncmp("250", recv_buff, 3) == 0){
        OrderType = COMMAND_UNUSEFULL;
        return (CHECK_SUCCESS);
      }
      if(strncmp("334", recv_buff, 3) == 0){
        OrderType = SMTP_SEND_USER;
        return (CHECK_SUCCESS);
      }
      return SMTP_ERR_AUTH;
    case SMTP_CHECK_USER:                                                                      /* �û���*/
      if(strncmp("334", recv_buff, 3) == 0){
        OrderType = SMTP_SEND_PASS;
        return (CHECK_SUCCESS);
      }
      return SMTP_ERR_USER;
    case SMTP_CHECK_PASS:                                                                      /* ����*/
      if(strncmp("235", recv_buff, 3) == 0){
        OrderType = SMTP_SEND_MAIL;
        return (CHECK_SUCCESS);
      }
      return SMTP_ERR_PASS;
    case SMTP_CHECK_MAIL:                                                                      /* ������*/
      if(strncmp("250", recv_buff, 3) == 0){
        OrderType = SMTP_SEND_RCPT;
        return (CHECK_SUCCESS);  
      } 
      return SMTP_ERR_MAIL;
    case SMTP_CHECK_RCPT:                                                                      /* ����*/
      if(strncmp("250", recv_buff, 3) == 0){
        OrderType = SMTP_SEND_DATA;
        return (CHECK_SUCCESS);      
      }
      return SMTP_ERR_RCPT;
    case SMTP_CHECK_DATA:                                                                      /* data ����*/
      if(strncmp("354", recv_buff, 3) == 0){
        OrderType = SMTP_DATA_OVER;
        return (CHECK_SUCCESS);
      } 
      return SMTP_ERR_DATA;
    case SMTP_CHECK_DATA_END:                                                                  /* ���ݷ������*/
      if(strncmp("250", recv_buff, 3) == 0){
#ifdef  send_over_receive
        OrderType = POP_RECEIVE_START;
#endif
#ifdef  send_over_quit
        OrderType = SMTP_SEND_QUIT;
#endif
        return (CHECK_SUCCESS);
      }
      return SMTP_ERR_DATA_END;
    case SMTP_CHECK_QUIT:                                                                      /* �˳���½*/
      if(strncmp("220", recv_buff, 3) == 0||strncmp("221", recv_buff, 3) == 0){
        OrderType = SMTP_CLOSE_SOCKET;
        return (CHECK_SUCCESS);
      }
      return SMTP_ERR_QUIT;
#endif 
#ifdef receive_mail
    case POP_CHECK_CNNT:                                                                       /* �û���*/
      if(strncmp("+OK", recv_buff, 3) == 0){
        OrderType = POP_RECEIVE_USER;
        return (CHECK_SUCCESS);
      }
      return POP_ERR_CNNT;
    case POP_CHECK_USER:                                                                       /* �û���*/
      if(strncmp("+OK", recv_buff, 3) == 0){
        OrderType = POP_RECEIVE_PASS;
        return (CHECK_SUCCESS);
      }
      return POP_ERR_USER;
    case POP_CHECK_PASS:                                                                       /* ����*/
      if(strncmp("+OK", recv_buff, 3) == 0){
        OrderType = POP_RECEIVE_STAT;
        return (CHECK_SUCCESS);
      }
      return POP_ERR_PASS;
    case POP_CHECK_STAT:                                                                       /* ���ʼ���Ϣ*/
      if(strncmp("+OK", recv_buff, 3) == 0){
        OrderType = POP_RECEIVE_LIST;
        return (CHECK_SUCCESS);  
      } 
      return POP_ERR_STAT;
    case POP_CHECK_LIST:                                                                       /* �ʼ��б�*/
      if((strncmp("+OK", recv_buff, 3) == 0)){
        OrderType = COMMAND_UNUSEFULL;
        return (CHECK_SUCCESS);
      }
      if((strncmp(".", recv_buff, 1) == 0)){
        p_pop3->RefreshTime = 1;
        OrderType = POP_RECEIVE_QUIT;
        return (CHECK_SUCCESS);
      }
      if((strncmp("1", recv_buff, 1) == 0)){
        p_pop3->RefreshTime = 0;
#ifdef  receive_over_reply
        OrderType = POP_RECEIVE_RTER;
#endif
#ifdef  receive_over_quit  
        OrderType = POP_RECEIVE_QUIT;
#endif
#ifdef  receive_dele_quit
        OrderType = POP_RECEIVE_RTER;
#endif
        return (CHECK_SUCCESS);
      }
      return POP_ERR_LIST;
    case POP_CHECK_QUIT:                                                                       /* �˳���½*/
      if(strncmp("+OK", recv_buff, 3) == 0){
#ifdef  POP_REFRESH
        if(p_pop3->RefreshTime){
          mDelaymS(200);
          mDelaymS(200);
          OrderType = POP_RECEIVE_START;
        } 
        else OrderType = POP_CLOSE_SOCKET; 
#else
        OrderType = POP_CLOSE_SOCKET;
#endif   
        return (CHECK_SUCCESS);
      }
      return POP_ERR_QUIT;
    case POP_CHECK_RETR:                                                                       /* ��ȡ�ʼ�����*/ 
      if(strncmp("+OK", recv_buff, 3) == 0){
        OrderType = COMMAND_UNUSEFULL;  
#ifdef  receive_over_reply
        memset(AttachmentData,'\0',sizeof(AttachmentData));
        memset(R_argv,'\0',sizeof(R_argv));
        memset( p_pop3, '\0', sizeof(POP) );
#endif
        return (CHECK_SUCCESS);
      } 
      else if(strncmp("-ERROR", recv_buff, 6) != 0){
        if( strncmp("\r\n.\r\n", &recv_buff[ReceLen-5], 5) == 0 ){
#ifdef  receive_dele_quit
        OrderType = POP_RECEIVE_DELE;
#endif  
#ifdef  receive_over_quit  
        OrderType = POP_RECEIVE_QUIT;  
#endif

#ifdef  receive_over_reply
          p_pop3->analysemailflag = 1;                                                         /* �����ʼ���ɱ�־*/
          ch395mail_analysemaildata( recv_buff );                                              /* �����ʼ�����*/
          OrderType = SMTP_SEND_START;
        }
        else{
          p_pop3->analysemailflag = 0;
          ch395mail_analysemaildata( recv_buff );
#endif   
        }
        return (CHECK_SUCCESS);
      } 
      return POP_ERR_RETR;
    case POP_CHECK_DELE:                                                                       /* ɾ��*/
      if(strncmp("+OK", recv_buff, 3) == 0){
        OrderType = POP_RECEIVE_QUIT;                                                          /* ִ�����ִ������������������ѡ����һ������*/
        return (CHECK_SUCCESS);
      }
      return POP_ERR_DELE;
    case POP_CHECK_RSET:                                                                       /* ����ɾ��*/ 
      if(strncmp("+OK", recv_buff, 3) == 0){
        OrderType = POP_RECEIVE_QUIT;                                                          /* ִ�����ִ������������������ѡ����һ������*/
        return (CHECK_SUCCESS);
      }
      return POP_ERR_RSET;
    case POP_CHECK_TOP:                                                                        /* ��ȡ�ʼ�ǰn��*/ 
      if(strncmp("+OK", recv_buff, 3) == 0){
        OrderType = POP_RECEIVE_QUIT;                                                          /* ִ�����ִ������������������ѡ����һ������*/
        return (CHECK_SUCCESS);
      }
      if(strncmp("Return", recv_buff, 6) == 0){
        OrderType = POP_RECEIVE_QUIT;                                                          /* ִ�����ִ������������������ѡ����һ������*/
        return (CHECK_SUCCESS);
      } 
      return POP_ERR_TOP;
    case POP_CHECK_UIDL:                                                                       /* ��ȡ�ʼ�Ψһ��ʶ��*/
      if(strncmp("+OK", recv_buff, 3) == 0){
        OrderType = POP_RECEIVE_QUIT;                                                          /* ִ�����ִ������������������ѡ����һ������*/
        return (CHECK_SUCCESS);
      }
      return POP_ERR_UIDL;
#endif  
    default:
      return SMTP_ERR_UNKNOW;
  }
}