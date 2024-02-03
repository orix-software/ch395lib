/********************************** (C) COPYRIGHT *********************************
* File Name          : MAIL.H
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : ch395mail.c���ݽṹ����
**********************************************************************************/
#ifndef __mail_H__
#define __mail_H__

typedef struct
{
  UINT8 g_MIME;                                                                  /* ���޸�����־λ*/
  UINT8  m_strFile[48];                                                           /* ��������*/
  UINT8  m_strSendFrom[48];                                                       /* �����˵�ַ*/
  UINT8  m_strSendTo[48];                                                         /* �ռ��˵�ַ*/
  UINT8  m_strSMTPServer[32];                                                     /* ����������*/
  UINT8  m_strUSERID[32];                                                         /* �û���*/
  UINT8  m_strPASSWD[32];                                                         /* ����*/
  UINT8  m_strSubject[32];                                                        /* ����*/
  UINT8  m_strSenderName[32];                                                     /* ����������*/
}SMTP;
typedef struct
{
  UINT8  analysemailflag;                                                         /* �����ʼ�������־λ*/
  UINT8  identitycheck;                                                           /* ��֤�������ݱ�־λ*/
  UINT8  RefreshTime;                                                             /* û�Ѳ鵽�ʼ�����������־λ*/
  UINT8  sBufTime[40];                                                            /* ��������ʼ���ʱ��*/
  UINT8  Ccname[48];                                                              /* ���͵�����*/
  UINT8  DecodeRName[32];                                                         /* ������յ��ʼ������ķ���������*/
  UINT8 pPop3Server[32];                                                          /* POP������*/
  UINT8 pPop3UserName[32];                                                        /* POP��½�û���*/
  UINT8 pPop3PassWd[32];                                                          /* POP��½����*/
}POP;
const  UINT8 code SMTP_CLIENT_CMD[5][11] =                                       /*SMTP CMD Codes*/
{
  "EHLO",                                                                        /* 0 �˳�*/
  "AUTH LOGIN",                                                                  /* 1 ��½*/
  "MAIL FROM:",                                                                  /* 2 �����˵�ַ*/
  "RCPT TO:",                                                                    /* 3 �ռ��˵�ַ*/
  "DATA",                                                                        /* 4 ��ʼ������������*/
};
const UINT8 code POP3_CLIENT_CMD[12][5] =                                        /*POP3 CMD Codes*/
{
  /* basic POP3 commands*/
  "QUIT",                                                                        /* 0 �˳�*/
  "USER",                                                                        /* 1 �û���*/
  "PASS",                                                                        /* 2 ����*/
  "STAT",                                                                        /* 3 ����ͳ������*/
  "LIST",                                                                        /* 4 ����ָ���ʼ��Ĵ�С*/
  "RETR",                                                                        /* 5 �ʼ���ȫ���ı�*/
  "DELE",                                                                        /* 6 ���ɾ��*/
  "RSET",                                                                        /* 7 �������е�DELE����*/
  "NOOP",                                                                        /* 8 ����һ���϶�����Ӧ*/
  /* alternative POP3 commands*/
  "APOP",                                                                        /* 9  ��֤һ�ְ�ȫ�������İ취��ִ�гɹ�����״̬ת��*/
  "TOP" ,                                                                        /* 10 ��������n���ʼ���ǰm�����ݣ�m��������Ȼ��*/
  "UIDL"                                                                         /* 11 �������ڸ�ָ���ʼ���Ψһ��ʶ*/
};
/* smtp�����ʼ���ز��� */
extern const UINT8 *m_Server;                                                    /* ����������*/
extern const UINT8  *m_UserName;                                                 /* �û���*/
extern const UINT8  *m_PassWord;                                                 /* ����*/
extern const UINT8  *m_SendFrom;                                                 /* �����˵�ַ*/
extern const UINT8  *m_SendName;                                                 /* ����������*/
extern const UINT8  *m_SendTo;                                                   /* �ռ��˵�ַ*/
extern const UINT8  *m_Subject;                                                  /* ����*/
extern const UINT8  *m_FileName;                                                 /* ��������(��������͸���,����Ϊ"\0")*/
/* pop�����ʼ���� */
extern const UINT8  *p_Server;                                                   /* POP������*/
extern const UINT8  *p_UserName;                                                 /* POP��½�û���*/
extern const UINT8  *p_PassWord;                                                 /* POP��½����*/
#endif