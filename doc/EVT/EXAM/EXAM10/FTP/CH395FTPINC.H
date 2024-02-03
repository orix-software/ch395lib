/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395FTPINC.H
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395FTP.cͷ�ļ�˵��
**********************************************************************************/
#ifndef __FTPINC_H__
#define __FTPINC_H__
/**********************************************************************************/
#define DEBUG            1                                                 /* ����  ������22.1184Mhz��������115200��*/
#ifndef Access_Authflag
#define Access_Authflag        0                                           /* ����ָ���û������ʣ������û����޷����ʷ�������*/
#endif
/**********************************************************************************/
typedef struct _FTP
{
  UINT8 TimeCount;                                                         /* ��ʱ������*/
  UINT8 FileFlag;                                                          /* �ж��ļ����ȴ���64�ֽڱ�־λ��������֧������ϴ��ļ�����64��*/
  UINT8 BufStatus;                                                         /* ��������־λ��0-3λ�ֱ��Ӧ�ĸ�������״̬��0Ϊ��1Ϊ��ʹ�ã�*/
  UINT8 AuthFlag;                                                          /* �û�Ȩ�ޱ�־λ*/
  UINT8 ListFlag;          
  UINT8 CmdDataS;                                                          /* ���ݴ���״̬*/
  UINT8 CmdReceDatS;                                                       /* ��������״̬*/
  UINT8 DataOver;                                                          /* ���ݶ�ȡ���	*/
  UINT8 CmdPortS;
  UINT8 CmdPasvS;
  UINT8 ListState;                                                         /* 0Ϊ��ǰĿ¼ 1Ϊ��һ��Ŀ¼*/
  UINT8 TcpCtlSta;                                                         /* TCP��������״̬*/
  UINT8 TcpDatSta;                                                         /* TCP��������״̬ */
  UINT8 SocketCtl;                                                         /* ftp��������*/
  UINT8 SocketDat;                                                         /* ftp��������*/
}FTP;
extern   char  UserName[16];                                               /* ���ڱ����û���*/
extern   char   SourIP[17];                                                /* ���ڱ���ת�����ַ���IP��ַ*/
#if Access_Authflag
extern   char  *pUserName;                                                 /* ���Է��ʷ�����û����������û����޷����ʷ�������*/
#endif
extern   char  *pPassWord;                                                 /* ��Ȩ�޵����루��������ֻ֧�ֶ���*/
/* TCP����״̬ */
#define FTP_MACH_DISCONT           0x01                                    /* TCPδ����*/
#define FTP_MACH_CONNECT           0x02                                    /* TCP������*/
#define FTP_MACH_KEPCONT           0x04                                    /* TCP��������*/
/*  ���ݴ���״̬ */
#define FTP_MACH_SENDDATA           0x01                                   /* ��Ҫ��������*/
#define FTP_MACH_RECEDATA           0x02                                   /* ��Ҫ��������*/
#define FTP_MACH_DATAOVER           0x04                                   /* ���ݴ������*/
#define FTP_MACH_CLOSECTL           0x08                                   /* ��������*/
#define FTP_MACH_LIST               0x00                                   /* ��ȡ�б�*/
#define FTP_MACH_FILE               0x01                                   /* �����ļ�*/
#define FTP_CMD_SUCCESS              0x00                                  /* ��ȡ�б�*/
#define FTP_CMD_ERROR               0x01                                   /* �����ļ�*/

/**********************************************************************************/
void CH395_FTPSendData( char *PSend, UINT16 Len,UINT8 index  );            /* ��������*/
   
void CH395_FTPConnect( void );                                             /* 395��ʼ��*/

void CH395_FTPInitVari( );                             

void CH563NET_FTPGetSockeID( UINT8 socketid );                   

void  mInitFtpList( void );                            

void CH395_FTPListRenew( UINT8 index );                      
                  
void CH395_FTPProcessReceDat( char *recv_buff,UINT8 index );               /* �������յ�������*/

void CH395_FTPCmdRespond( char *recv_buff );                               /* �����յ�������������Ӧ�Ļظ�*/ 

void CH395_FTPDataReady( char *pSendDat );                                 /* ׼��Ҫ���͵�����*/

void CH395_FTPGetPortNum( char *recv_buff );                               /* ��ȡ�������ӵĶ˿�*/

void CH395_FTPGetFileName( char *recv_buff,char *pBuf );      

UINT8 CH395_FTPFileOpen(  char *pName,UINT8 index  );                      /* ���ļ�*/

UINT32 CH395_FTPFileSize ( char *pName );                                  /* ��ȡ�ļ�����*/
          
void CH395_FTPFileRead( char *pName );                                     /* ���ļ�*/

void CH395_FTPFileWrite( char *recv_buff, UINT16 LEN );                    /* д�ļ�*/

void CH395_FTPCtlServer( UINT8 socketid );                                 /* ����FTP��������*/

void CH395_FTPDatClient( UINT8 socketid,UINT16 port,UINT8 *pAddr );        /* ����FTP�ͻ�����������*/
                            
void CH395_FTPDatServer( UINT8 socketid,UINT16 *port );                    /* ����FTP��������������*/

void CH395_FTPGetSockeID( UINT8 socketid  );                               /* ��ȡ��������socket����*/

void CH395_FTPServerCmd( void );                                           /* ��ѯ״̬��ִ����Ӧ���ӳ���*/
#endif  
 /**********************************************************************************/