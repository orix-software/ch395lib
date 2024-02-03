 /********************************** (C) COPYRIGHT *********************************
* File Name          : MAILINC.H
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : mail.cͷ�ļ�����
**********************************************************************************/
#ifndef __mailinc_H__
#define __mailinc_H__
extern  UINT8 OrderType;                                                                      /* ��������*/
extern  UINT8 CheckType;                                                                      /* �����źź˶����� */
extern  UINT8 ReceDatFlag;                                                                    /* �յ����ݱ�־λ*/
extern  UINT8 SendDatFalg;     
extern  UINT16 ReceLen;                                                                       /* ���ջ��������յ������ݳ���*/

#define DEBUG                  1                                                              /* ����	������22.1184Mhz��������115200��*/
#define	attach_max_len         512                                                            /* ���͸�����󳤶� */ 
#define SendSocket             0                                                              /* �����ʼ�ʹ�õ�socket*/
#define ReceSocket             1                                                              /* �����ʼ�ʹ�õ�socket*/
#define POP3_SERVER_PORT       110                                                            /* ���ʼ�Ŀ�Ķ˵��*/
#define SMTP_SERVER_PORT       25                                                             /* ���ʼ�Ŀ�Ķ˵��*/
/**************************************************************************************************************************
/* �������� */
/*
/* 1  ֻ�����ʼ�--������ɺ��˳���½���ر�socket���ӣ�
/* 2  ֻ�����ʼ�--������ɡ���ȡ�ʼ��б������˳���½���ر�socket���ӣ�
/* 3  ֻ�����ʼ�--������ɡ���ȡ�ʼ���ɾ�������˳���½���ر�socket���ӣ�
/* 4  �����ʼ��ҽ����ʼ�--������ɺ�ʼ�����ʼ���������ɺ��˳���½���ر�socket����; 
/* 5  �����ʼ��ҽ����ʼ�--������ɺ�ظ��ʼ���������ɺ����˳���½��ɾ���ʼ����˳����ʼ���½�����ر�socket���ӡ�  
/* ˵�����˴�������뷢���õ�����������socket���ӣ�
/*     ���������������Ĳ�����eg����������գ�������������Ͳ���
                   ֻ����ch395mail.c�����е�ch395mail_CheckResponse����
                   �ӳ����н�����ɺ󣬽��������Ϊ SMTP_SEND_START��
*************************************************************************************************************************/
#ifndef	mail_work_mode
#define  mail_work_mode         5
#endif
#if    (mail_work_mode == 1)                                 
#define send_mail                1                                                            /* �����ʼ�*/  
#define	send_over_quit           1                                                            /* ��������˳�*/
#elif (mail_work_mode == 2)                                 
#define receive_mail             1                                                            /* �����ʼ�*/
#define	receive_over_quit        1                                                            /* ��ȡ�ʼ����˳�*/
#elif (mail_work_mode == 3)                                 
#define receive_mail             1                                                            /* �����ʼ�*/
#define	receive_dele_quit        1                                                            /* �����ʼ���ɾ�����˳�*/
#elif (mail_work_mode == 4)                                 
#define send_mail                1                                                            /* �����ʼ�*/
#define send_over_receive        1                                                            /* ���������*/
#define receive_mail             0                                                            /* �����ʼ�*/
#define receive_over_quit        1                                                            /* ��ȡ�ʼ����˳�*/
#elif (mail_work_mode == 5)                                
#define receive_mail             1                                                            /* �����ʼ�*/
#define receive_over_reply       1                                                            /* ��ȡ�ʼ���ظ��ʼ�*/
#define send_mail                0                                                            /* �����ʼ�*/
#define	send_over_quit           1                                                            /* �������˳�*/
#endif
/* ********************************************************************************************************************* */
/* POP��ѡ����
/*
/*********************************************************************************************************************** */
#ifdef receive_mail 
#define POP_RTER    1                                                                         /* 1 ����server�ʼ���ȫ���ı�*/
#define POP_DELE    1                                                                         /* 2 ���ɾ��*/
#define POP_RSET    1                                                                         /* 3 �����������е�DELE����*/
#define POP_TOP     1                                                                         /* 4 ����n���ʼ���ǰm������*/
#define POP_UIDL    1                                                                         /* 5 ����server�������ڸ�ָ���ʼ���Ψһ��ʶ�����û��ָ�����������еġ�*/
#define POP_REFRESH 1                                                                         /* �����ʼ�������û�յ��ʼ�������ѯ��־*/ 
#endif

/** ********************************************************************************************************************* */
/* ������� */
/* 0x01-0x0f Ϊ�����ʼ�������
/****************** 0x01-0x07 Ϊ��������
/****************** 0x09-0x0e ΪP��ѡ����
/****************** 0x01-0x02-0x03-0x06 Ϊȷ��״̬
/****************** 0x04-0x05 0x09-0x0e Ϊ����״̬
/* 0x10-0x1f Ϊ�����ʼ�������
/****************** 0x10-0x17 �谴˳��ִ��
/******************       �����Ϊ��ѡ�����
/* 0x00    �����κβ���
/** ********************************************************************************************************************* */
#define COMMAND_UNUSEFULL		0x00
//POP RECEIVE CODE
#define POP_RECEIVE_USER		0x01	                                                        /* ��֤�û�*/
#define POP_RECEIVE_PASS		0x02	                                                        /* ��֤����*/
#define POP_RECEIVE_STAT		0x03	                                                        /* ����ͳ������*/
#define POP_RECEIVE_LIST		0x04	                                                        /* ����ָ���ʼ��Ĵ�С*/
#define POP_RECEIVE_RTER		0x05	                                                        /* ��ȡ�ʼ���ȫ���ı�*/
#define POP_RECEIVE_QUIT		0x06	                                                        /* �˳���½*/
#define POP_CLOSE_SOCKET		0x07	                                                        /* �ر�pop��socket*/
#define POP_RECEIVE_START		0x08	                                                        /* ���������ʼ�*/
#define POP_RECEIVE_DELE		0x09	                                                        /* ���ɾ��*/
#define POP_RECEIVE_RSET    0x0a	                                                        /* �������е�DELE����*/
#define POP_RECEIVE_TOP     0x0b	                                                        /* ��������n���ʼ���ǰm�����ݣ�m��������Ȼ��*/
#define POP_RECEIVE_UIDL		0x0e	                                                        /* �������ڸ�ָ���ʼ���Ψһ��ʶ*/
#define POP_ERR_CHECK       0x0F	                                                        /* POP���ֳ����˳���½���ر�socket*/
//SMTP SEND CODE
#define SMTP_SEND_HELO	  	0x10	                                                        /* ����hello����*/
#define SMTP_SEND_AUTH	  	0x11	                                                        /* ���͵�½����*/
#define SMTP_SEND_USER	  	0x12	                                                        /* �����û���*/
#define SMTP_SEND_PASS	  	0x13	                                                        /* ��������*/
#define SMTP_SEND_MAIL	  	0x14	                                                        /* ���ͷ����ߵ�ַ*/
#define SMTP_SEND_RCPT      0x15                                                          /* ���ͽ����ߵ�ַ*/
#define SMTP_SEND_DATA	  	0x16	                                                        /* ���ͷ�����������*/
#define SMTP_DATA_OVER	  	0x17	                                                        /* �����ʼ�����*/
#define SMTP_SEND_QUIT      0x18                                                          /* �˳���½*/
#define SMTP_CLOSE_SOCKET   0x19	                                                        /* �ر�smtp��socket*/
#define SMTP_SEND_START	  	0x1E	                                                        /* ���������ʼ�*/
#define SMTP_ERR_CHECK	  	0x1F	                                                        /* smtp���ֳ����˳���½���ر�socket*/
/* ********************************************************************************************************************* */
/* �����źź˶Դ������
/* ********************************************************************************************************************* */
//POP ERR CHECK CODE
#define POP_ERR_CNNT	  	0x20	                                                        /* pop���Ӵ���--------------�����Ƕ˿ںţ�������IP�����ô���*/
#define POP_ERR_USER	  	0x21	                                                        /* ��֤�û���������-------�û�����ʽ���ԣ���USER��������������������鿴���͵�������˶��Ƿ���ȷ����ͬ��*/
#define POP_ERR_PASS	  	0x22	                                                        /* ��½ʧ��-----------------�û�����������󣬻�PASS���������*/
#define POP_ERR_STAT	  	0x23	                                                        /* ��ȡ�ʼ���Ϣ����---------STAT��������������ʧ�����µ�½���绹��ʧ�ܼ����������*/
#define POP_ERR_LIST      0x24                                                          /* ��ȡ�ʼ���Ϣ�б�---------LIST���������*/
#define POP_ERR_RETR	  	0x25	                                                        /* ��ȡָ���ʼ�����Ϣ����---��ȡ���ʼ������ڣ���RETR��������󣬻�����Ĳ�������ȷ*/
#define POP_ERR_DELE	  	0x26	                                                        /* ɾ��ĳ���ʼ�ʧ��---------ɾ�����ʼ������ڣ���RETR��������󣬻�����Ĳ�������ȷ*/
#define POP_ERR_QUIT	  	0x27	                                                        /* �˳���½ʧ��-------------QUIT���������*/
#define POP_ERR_RSET	  	0x28	                                                        /* �ָ�ɾ�����ʼ�-----------RSET������������������˳�֮ǰִ�в�����Ч��*/
#define POP_ERR_TOP	    	0x2B	                                                        /* ��ȡĳ���ʼ�ǰm��ʧ��----��ȡ���ʼ������ڣ���RETR��������󣬻�����Ĳ�������ȷ*/
#define POP_ERR_UIDL	  	0x2C	                                                        /* ��ȡĳ���ʼ�IDʧ��-------�ʼ������ڣ���UIDL��������󣬻�����Ĳ�������ȷ*/
#define POP_ERR_UNKW	  	0x2F	                                                        /* pop����δ֪--------------����δ֪*/
//SMTP ERR CHECK CODE
#define SMTP_ERR_CNNT	  	0x31	                                                        /* smtp���Ӵ���-------------�����Ƕ˿ںţ�������IP�����ô���*/
#define SMTP_ERR_HELO	  	0x32	                                                        /* ����helo����ʧ��---------HELO��������󣬻�����������ʽ����*/
#define SMTP_ERR_AUTH	  	0x33	                                                        /* �����½����ʧ��---------AUTH���������*/
#define SMTP_ERR_USER	  	0x34	                                                        /* �����û���δʶ��---------�û�����ʽ���ԣ���USER���������*/
#define SMTP_ERR_PASS	  	0x35	                                                        /* ��½ʧ��-----------------�û�����������󣬻�PASS���������*/
#define SMTP_ERR_MAIL	  	0x36	                                                        /* �����˵�ַ����ʧ��-------�����˵�ַ����ȷ����MAIL���������*/
#define SMTP_ERR_RCPT	  	0x37	                                                        /* ���ͽ��յ�ַ����---------�ռ��˵�ַ����ȷ����RCPT���������*/
#define SMTP_ERR_DATA	  	0x38	                                                        /* ����������-------------DATA���������*/
#define SMTP_ERR_DATA_END	0x39	                                                        /* �������ݹ��̳���*/
#define SMTP_ERR_QUIT	  	0x3A	                                                        /*	�˳���½ʧ��*/
#define	SMTP_ERR_UNKNOW   0x3F	                                                        /* smtp����δ֪*/

#define send_data_timeout		0x10	                                                      /* �������ݳ�ʱ*/
#define send_data_success		0x14	                                                      /* �������ݳ�ʱ*/

/* ********************************************************************************************************************* */
/* ����������� 
/* 
/* ********************************************************************************************************************* */
//POP3  CODE
#define OPEN_SOCKET_RECEIVE     0xA0                                                      /* �򿪽���socket*/
#define LOGIN_MAILBOX_RECEIVE   0xA1	                                                    /* ��½��������*/
#define READ_MAIL_STAT          0xA2	                                                    /* ��ȡ�ʼ����б�*/
#define READ_MAIL_LIST	        0xA3	                                                    /* ��ȡ�ʼ��б�*/
#define READ_MAIL_MAILBODYDATA	0xA4	                                                    /* ��ȡ�ʼ���������*/
#define READ_MAIL_ATTACHMENT    0xA5	                                                    /* ��ȡ�ʼ���������*/
#define READ_MAIL_ADDRESS       0xA6	                                                    /* ��ȡ�����˵�ַ*/
#define READ_MAIL_TIME          0xA7	                                                    /* ��ȡ����ʱ��*/
#define READ_MAIL_SUBJECT       0xA8	                                                    /* ��ȡ�ʼ�����*/
#define QUIT_RECEIVE_MAIL       0xA9	                                                    /* �˳���½���ر�socket*/
//SMTP  CODE
#define OPEN_SOCKET_SEND        0xB0	                                                    /* �򿪷���socket*/
#define LOGIN_MAILBOX_SEND	    0xB1	                                                    /* ��½��������*/
#define SEND_MAIL_UNATTACH	    0xB2	                                                    /* �����ʼ�����������*/
#define SEND_MAIL_CONTATTACH    0xB3	                                                    /* �����ʼ���������*/
#define QUIT_SEND_MAIL	        0xB4	                                                    /* �˳���½���ر�socket*/
/* ********************************************************************************************************************* */
/* �˶�������Ϣ�Ĵ������� 
/* �ɹ�����CHECK_SUCCESS���������������Ϊ��һ�������������
/* ���󷵻ض�Ӧ�Ĵ������ 
/* ���������������źţ�����Ҫ�˶�
/* ********************************************************************************************************************* */
#define CHECK_SUCCESS        0x00
#define uncheck	             0x00
//POP CHECK CODE
// ���Ӧ����Ϣ����ȷ���ء�+0k�� �����󷵻ء�-ERR��
#define POP_CHECK_CNNT	  	0x50	
#define POP_CHECK_USER	  	0x51    
#define POP_CHECK_PASS	  	0x52	
#define POP_CHECK_STAT	  	0x53
#define POP_CHECK_LIST	  	0x54	
#define POP_CHECK_RETR	  	0x55
#define POP_CHECK_DELE	  	0x56
#define POP_CHECK_SEAR	  	0x57
#define POP_CHECK_QUIT	  	0x58
#define POP_CHECK_RSET	  	0x59
#define POP_CHECK_APOP	  	0x5A
#define POP_CHECK_TOP       0x5B
#define POP_CHECK_UIDL	  	0x5C
//SMTP CHECK CODE
#define SMTP_CHECK_CNNT	  	0x60
#define SMTP_CHECK_HELO	  	0x61
#define SMTP_CHECK_AUTH	  	0x62
#define SMTP_CHECK_USER	  	0x63
#define SMTP_CHECK_PASS	  	0x64
#define SMTP_CHECK_MAIL	  	0x65
#define SMTP_CHECK_RCPT	  	0x66
#define SMTP_CHECK_DATA	  	0x67
#define SMTP_CHECK_DATA_END	0x68
#define SMTP_CHECK_QUIT	  	0x69
/*************************************************************************/
UINT8 ch395mail_SendData( UINT8 *PSend, UINT16 Len,UINT8 type,UINT8 index  );             /* ��������*/

void ch395mail_Quit(UINT8 index );                                                        /*	�˳������½*/

void ch395mail_MailCommand( UINT8 choiceorder);                                           /*	������������ִ�ж�Ӧ������Զ���������շ�����*/

void ch395mail_pop3init(  );                                                              /*	���ʼ���ʼ��*/

void ch395mail_pop3Retr( UINT8 num );                                                     /*	��ȡĳ���ʼ�*/

void ch395mail_pop3Dele( UINT8 num );                                                     /*	ɾ��ĳ���ʼ�*/

void ch395mail_pop3Rset( );                                                               /*	�������е�ɾ������*/

void ch395mail_pop3Top( UINT8 num ,UINT8 m  );                                            /*	��ȡĳ���ʼ�ǰm��*/

void ch395mail_pop3Uidl( UINT8 num );                                                     /*	��ȡĳ���ʼ���ID*/

void ch395mail_smtpinit( );                                                               /*  ���ʼ���ʼ��*/

UINT8 ch395mail_CheckResponse( UINT8 *recv_buff,UINT8 check_type );                       /*  ���Ӧ����Ϣ*/

void ch395mail_replybody();

#endif