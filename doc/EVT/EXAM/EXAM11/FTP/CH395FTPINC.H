/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395FTPINC
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395FTPINCͷ�ļ�����
**********************************************************************************/
#ifndef __FTPINC_H__
#define __FTPINC_H__
#define DEBUG            1                                   /* ����  ������22.1184Mhz��������115200��*/
typedef struct _FTP
{
  UINT8 TcpStatus;                                           /* TCP����״̬*/
    UINT8 CmdStatus;                                         /* �ӿ�����ִ��״̬*/
  UINT8 InterCmdS;                                           /* �ӿ�����ִ�б�־λ*/
  UINT8 CmdFirtS;                                            /* �����һ��ִ�б�־λ*/
  UINT8 FindList;                                            /* �ҵ�ָ�����ļ���*/
  UINT8 FindFile;                                            /* �ҵ�ָ�����ļ���*/
  UINT8 CmdDataS;                                            /* ��������״̬*/
  UINT8 SocketCtl;                                           /* ftp��������socket����*/
  UINT8 DatMonitor;                                          /* ftp������������socket����*/
  UINT8 DatTransfer;                                         /* ftp�������Ӵ���socket����*/
  UINT8 FileCmd;                                             /* �ӿ���������  */
  UINT16 SouPort;                                            /* �˿ں�*/
  UINT16 RecDatLen;                                          /* ���յ������ݳ���*/
}FTP;
extern   char  ListName[24];                                 /* ���ڱ���Ŀ¼�� */
extern   char  ListMdk[24];                                  /* ���ڱ����´�����Ŀ¼��*/
extern   char  FileName[24];                                 /* ���ڱ����ļ���*/
extern   char  SourIP[17];                                   /* ���ڱ���ת�����ַ���IP��ַ*/
extern  const UINT8 *pUserName;                              /* ������½*/
extern  const UINT8 *pPassword;                              /* e-mail ������Ϊ��½����*/
/* TCP����״̬ */
#define FTP_MACH_DISCONT               0x00                  /* TCP�Ͽ�����*/
#define FTP_MACH_CONNECT               0x01                  /* TCP����*/
/* ����״̬ */
#define FTP_MACH_QUIT                  0x01                  /* ��½ǰ����*/
#define FTP_MACH_CLOSE                 0x02                  /* ��½�����*/
/* ���ݴ���״̬ */
#define FTP_MACH_SENDDATA              0x01                  /* ��Ҫ��������*/
#define FTP_MACH_RECEDATA              0x02                  /* ��Ҫ��������*/
#define FTP_MACH_DATAOVER              0x04                  /* ���ݴ������*/
/* �ӿ���ִ��״̬ */
#define FTP_MACH_LOGINSUC              0x01                  /* ��¼�ɹ�*/
#define FTP_MACH_SEARCHSUC             0x02                  /* Ѱ���ļ����*/
#define FTP_MACH_GETSUC                0x04                  /* �����ļ��ɹ�*/
#define FTP_MACH_PUTSUC                0x08                  /* �ϴ��ļ��ɹ�*/
/*  ����ִ�й��̱�־λ */
#define FTP_MACH_FINDLIST              0x01                  /* Ѱ��Ŀ¼��־λ*/
#define FTP_MACH_FINDFILE              0x02                  /* Ѱ���ļ���־λ*/
#define FTP_MACH_GETFILE               0x04                  /* �����ļ���־λ*/
#define FTP_MACH_MKDLIST               0x08                  /* ����Ŀ¼��־λ*/
#define FTP_MACH_PUTFILE               0x10                  /* �ϴ��ļ���־λ*/
/* ����״̬��Ϣ */ 
#define FTP_CHECK_NULL                 0x00                  /* û�����źŷ����򲻽��з�����Ϣ�˶�*/ 
#define FTP_CHECK_SUCCESS              0x00                  /* ���������Ϣ��ȷ  */
#define FTP_COMMAND_SUCCESS            0x00                  /* �ӿ�����ִ�гɹ�*/
#define FTP_COMMAND_CONTINUE           0x01                  /* �ӿ������ִ�й�����*/
/* ������ӿ��������� */
#define FTP_CMD_LOGIN                  0x01                  /* ��¼*/
#define FTP_CMD_SEARCH                 0x02                  /* Ѱ���ļ�*/
#define FTP_CMD_GETFILE                0x03                  /* �����ļ�*/
#define FTP_CMD_PUTFILE                0x04                  /* �ϴ��ļ�*/
/* *********************************************************************************************************************
* �����źź˶Դ������
************************************************************************************************************************/
#define FTP_ERR_LOGIN               0x20                     /* ��¼*/
#define FTP_ERR_SEARCH              0x21                     /* Ѱ���ļ�*/
#define FTP_ERR_GETFILE             0x22                     /* �����ļ�*/
#define FTP_ERR_PUTFILE             0x23                     /* �ϴ��ļ�*/
#define FTP_ERR_UNLIST        0x2a                           /* δ�ҵ�Ŀ¼*/ 
#define FTP_ERR_UNFILE        0x2b                           /* δ�ҵ��ļ�*/
#define FTP_ERR_UNKW        0x2f                             /* δ֪����*/

void  CH395_FTPClientCmd( void );                            /* ��ѯ״̬��ִ��������*/

void  CH395_FTPInterCmd( void );                             /* ִ�ж�Ӧ����������е���ִ��˳��*/
 
void  CH395_SocketSendData( char *PSend, UINT32 Len,UINT8 index  ); /* ��������*/

void  CH395_FTPConnect( void );                               /* 563��ʼ��*/

void  CH395_FTPInitVari( void );                              /* ������ʼ��*/
                  
void CH395_FTPProcessReceDat( char *recv_buff,UINT8 check_type,UINT8 socketid );/* ���Ӧ����Ϣ*/

UINT8 CH395_FTPCmdRespond( char *recv_buff,UINT8 check_type );  /* ���Ӧ����Ϣ*/

void  CH395_FTPCtlClient( UINT8 socketid);                      /* ����FTP��������*/

void CH395_FTPDatServer( UINT8 socketid,UINT16 *port );         /* ����FTP��������*/

void  CH395_FTPCommand( UINT8 choiceorder);                     /* ������������ִ�ж�Ӧ������*/

UINT8 CH395_FTPLogin( void );                                   /* ��½ */ 
 
UINT8 CH395_FTPSearch(char *pListNAme,char *pFileName );        /* �����ļ�*/

UINT8 CH395_FTPGetFile( char *pFileName );                      /* �����ļ�*/

UINT8 CH395_FTPPutFile(char *pListNAme, char *pFileName);       /* �ϴ��ļ�*/

UINT8 CH395_FTPFindFile( char *pReceiveData );                  /* �˶��ļ�����*/

UINT8 CH395_FTPFindList( char *pReceiveData );                  /* �˶��ļ�����*/

void  CH395_FTPGetSockeID( UINT8 socketid );                    /* ��ȡ��������socket����*/

UINT8 CH395_FTPMkdirCheck( char *rece_buf);                     /* ����Ŀ¼����ִ��״̬*/

UINT8 CH395_FTPGetFileData( char *recv_buff );                  /* ��ȡ���ص�����*/

void  CH395_FTPQuit( void );                                    /* �˳�*/

UINT8 CH395_FTPWriteData ( char *recv_buff ,UINT16 len);

#endif  