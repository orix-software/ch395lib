/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395FTPCMD.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬFTP������Ӧ��-FTP�������
**********************************************************************************/
#include "Ch395.h"
const char code FTP_SERVICE_CMD[10][60] = 
{
   "220 Microsoft FTP Service\r\n",                                             /* 0 ��½  */
   "226 Transfer complete\r\n",                                                 /* 1 ������� */
   "331 access allowed send identity as password.\r\n",                         /* 2 �û�����ȷ   */
   "230 user logged in.\r\n",                                                   /* 3 ��½�ɹ� */
   "530 user cannot log in.\r\n",                                               /* 4 ��½ʧ�� */
   "200 PORT command successful.\r\n",                                          /* 5 PORT����ɹ�*/
   "125 Data connection already open;Transfer starting\r\n",                    /* 6 ��ȡ�б�����  */
   "150 Opening ASCII mode data connection for",                                /* 7 ����/�ϴ��ļ�����ɹ�*/
   "550 The ch563 cannot find the file specified",                              /* 8 �ļ�������*/
   "221 Goodbye\r\n",                                                           /* 9 �˳�      */
};
const char  code FTP_SERVICE_CMD1[10][40] = 
{
   "200 Type set to",                                                           /* 0 TYPE�����ļ���������  */
   "215 Windows_NT\r\n",                                                        /* 1 systϵͳ����     */
   "257 \"/\" is current directory.\r\n",                                       /* 2 PWD��ӡĿ¼    */
   "250 command successful.\r\n",                                               /* 3 ����ɹ�       */
   "227 Entering Passive Mode(",                                                /* 4 PASV����ɹ�    */
   "213 ",                                                                      /* 5 size����ɹ�    */
   "250 DELE command success\r\n",                                              /* 6              */
   "550 Access is denied.\r\n",                                                 /* 7              */
   "200 noop command success.\r\n",                                             /* 8              */
   "500 command not understood\r\n",                                            /* 9 ��֧�ֵ�����     */
};
const char  code FTP_SERVICE_CMD2[2][50] = 
{
   "350 File exists,ready for destination name\r\n",                            /* 0               */
   "250 RNTO command successful\r\n",                                           /* 1              */
};                                     
#define SendBufLen            500                                               /* ���ͻ���������     */
FTP   ftp;
char  SendBuf[SendBufLen];                                                      /* ���ڱ���Ŀ�Ķ˴��͵�IP��ַ*/
UINT8 CmdPortIP[4];                                                             /* ���ڱ���ת�����ַ���IP��ַ */
UINT16 TemAddr;                                                                 /* ���ڷ���IP��ַ���˿ڵ���ʱ���� */
UINT16 NumPort;                                                                 /* �˿ں�  */
UINT32 gLEN;                                                                    /* �ļ�����*/
/* ���ڱ����ļ��� */
char  gFileName[16];                                                             /* ���ڱ����������ص��ļ���*/
char  pFileName[16];                                                             /* ���ڱ��������ϴ����ļ���*/
char  ListName[16]; 
                                                             /* ���ڱ��浱ǰĿ¼   */
/********************************************************************************
* Function Name  : CH395_FTPGetPortNum
* Description    : ��ȡ�˿ںż�IP��ַ
* Input          : recv_buff- ���յ�������
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPGetPortNum( char *recv_buff )
{
   UINT8 k,i;
      
   i = 0;
   memset((void *)CmdPortIP,0,4);
   for(k=0;k<strlen(recv_buff);k++){
      if(recv_buff[k]>='0'&&recv_buff[k]<='9'){
         if(i<4){                                                               /* ��ȡĿ��IP��ַ*/ 
            TemAddr = 0;
            while(recv_buff[k] != ','){
               TemAddr += recv_buff[k] - '0';
               k++;
               TemAddr *= 10;
            }
             TemAddr /= 10;
             CmdPortIP[i] = TemAddr; 
             i++;
         }
         else{                                                                   /* ��ȡ�˿ں� */
            NumPort = 0;
            while(recv_buff[k] != ','){
               NumPort += (recv_buff[k]-'0')*256;
               k++;
               NumPort *= 10;
            }
            NumPort /= 10;
            k++;
            TemAddr = 0;
            while(recv_buff[k]>='0'&&recv_buff[k]<='9'){
               TemAddr +=  (recv_buff[k]-'0');
               k++;
               TemAddr *= 10;
            }
            TemAddr /= 10;
            NumPort += TemAddr;
            break;
         }
      }
   }
#if DEBUG
   printf("CmdPortIP: %d.%d.%d.%d\n",CmdPortIP[0],CmdPortIP[1],CmdPortIP[2],CmdPortIP[3]);
   printf("NumPort = %08d\n",NumPort);
#endif   
} 

/*******************************************************************************
* Function Name  : CH395_FTPGetFileName
* Description    : ��ȡ����
* Input          : recv_buff-���յ�������
                   pBuf-     �������ֵĻ�����
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPGetFileName( char *recv_buff,char *pBuf )
{
    UINT8 k,i;
   
   k = 0;
   while((recv_buff[k] != ' ')&&k<strlen(recv_buff)) k++;                       /* �ҵ�Ҫ��ȡ�����ݵ�λ��*/
   k++;
   i = 0;
   while((recv_buff[k] != '\r')&&k<strlen(recv_buff)){                          /* �������������*/
      if(i > 14 ) return;
      if(recv_buff[k] != '/'){
         pBuf[i] = recv_buff[k];
         i++;
      }
      k++;
   }
#if DEBUG
   printf("Name: %s\n",pBuf);
#endif   
}

/******************************************************************************
* Function Name  : CH395_FTPCmdRespond
* Description    : ����client���������������Ӧ�Ļظ�
* Input          : recv_buff- ���յ��ͻ��˷��͵��������
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPCmdRespond( char *recv_buff ) 
{
   UINT8 s;
   UINT8 len;
   UINT16 port;
   
   if( strncmp("USER", recv_buff, 4) == 0 ){                                    /* �û��� */
      memset((void *)UserName,'\0',sizeof(UserName));   
      CH395_FTPGetFileName( recv_buff,UserName );   
      CH395_FTPSendData( (char *)FTP_SERVICE_CMD[2],strlen(FTP_SERVICE_CMD[2]),ftp.SocketCtl );
   }
   else if( strncmp("PASS", recv_buff, 4) == 0 ){                               /* ����       */
#if Access_Authflag                                                             /* �����Ҫ����û���*/
      if(strncmp(pUserName, UserName, strlen(pUserName)) == 0){                 /* �û�����ȷ    */
         CH395_FTPSendData( (char *)FTP_SERVICE_CMD[3],strlen(FTP_SERVICE_CMD[3]),ftp.SocketCtl );
      }
      else{                                                                     /* �û�������*/
         CH395_FTPSendData( (char *)FTP_SERVICE_CMD[4],strlen(FTP_SERVICE_CMD[4]),ftp.SocketCtl );
      }
#else
         CH395_FTPSendData( (char *)FTP_SERVICE_CMD[3],strlen(FTP_SERVICE_CMD[3]),ftp.SocketCtl );
#endif
      memset((void *)UserName,'\0',sizeof(UserName));   
      CH395_FTPGetFileName( recv_buff,UserName );
      if(strncmp(pPassWord, UserName, strlen(pPassWord)) == 0){                 /* ��֤�����Ƿ���ȷ��������������д�ļ�Ȩ�ޣ�*/
         ftp.AuthFlag = 1;
         printf("have more authority\n");
      }   
   }
   else if( strncmp("PORT", recv_buff, 4) == 0 ){                               /* �ͻ��˷��Ϳͻ��˴�����TCP SERVER�˿ںż�IP��ַ*/
        CH395_FTPGetPortNum( recv_buff );
        CH395_FTPSendData( (char *)FTP_SERVICE_CMD[5],strlen(FTP_SERVICE_CMD[5]),ftp.SocketCtl );
        CH395_FTPDatClient( 1,NumPort, CmdPortIP);        
   }
   else if( strncmp("LIST", recv_buff, 4) == 0 ){                                /* ��ȡ��ǰ�б���Ϣ*/
        CH395_FTPSendData( (char *)FTP_SERVICE_CMD[6],strlen(FTP_SERVICE_CMD[6]),ftp.SocketCtl );
        CH395_FTPDataReady( "0" );
   }
   else if( strncmp("STOR", recv_buff, 4) == 0){                                 /* �ͻ��������ϴ����ݵ������Ҫ�������ݣ�*/
      if(ftp.AuthFlag == 1){                                                     /* �����Ȩ����֧���ϴ�   */
         ftp.CmdReceDatS = 1;
         ftp.FileFlag = 0;
         memset((void *)pFileName,'\0',sizeof(pFileName));   
         CH395_FTPGetFileName( recv_buff,pFileName );
         memset((void *)SendBuf,'\0',sizeof(SendBuf));
         sprintf(SendBuf,"%s %s.\r\n",FTP_SERVICE_CMD[7],pFileName);
         CH395_FTPSendData( SendBuf,strlen(SendBuf),ftp.SocketCtl );
      }
      else{                                                                     /* ��Ȩ�޲�֧�ֵ�����ر�����*/
         CH395CloseSocket(ftp.SocketDat);
         CH395CloseSocket(ftp.SocketCtl);
         CH395_FTPSendData( (char *)FTP_SERVICE_CMD1[7],strlen(FTP_SERVICE_CMD1[7]),ftp.SocketCtl );
      }
   }
   else if( strncmp("RETR", recv_buff, 4) == 0){                                 /* �ͻ��������������ݵ������Ҫ�������ݣ�*/
      if(recv_buff[5] == '/'){
         memset((void *)SendBuf,'\0',sizeof(SendBuf));   
         CH395_FTPGetFileName( recv_buff,SendBuf );
         len = strlen(ListName);
         sprintf(gFileName,&SendBuf[len]); 
      }
      else{
         memset((void *)gFileName,'\0',sizeof(gFileName));   
         CH395_FTPGetFileName( recv_buff,gFileName );
      } 
      memset((void *)SendBuf,'\0',sizeof(SendBuf));
       s = CH395_FTPFileOpen( gFileName,FTP_MACH_FILE );      
      if( s ){                                                                  /* �ļ����ڣ���ʼ��������*/
         sprintf(SendBuf,"%s %s.\r\n",FTP_SERVICE_CMD[7],gFileName);
         CH395_FTPSendData( SendBuf,strlen(SendBuf),ftp.SocketCtl );
         CH395_FTPDataReady( gFileName );
      }
      else{                                                                     /* �ļ����ڣ����ʹ�����Ӧ*/
         sprintf(SendBuf,"%s��%s).\r\n",FTP_SERVICE_CMD[8],gFileName); 
         CH395_FTPSendData( SendBuf,strlen(SendBuf),ftp.SocketCtl );
         s = CH395CloseSocket( ftp.SocketDat );
         mStopIfError(s); 
         ftp.SocketDat = 255;
      }
   }
   else if( strncmp("QUIT", recv_buff, 4) == 0 ){                               /* �˳���¼   */
      CH395_FTPSendData( (char *)FTP_SERVICE_CMD[9],strlen(FTP_SERVICE_CMD[9]),ftp.SocketCtl );
   }
   else if( strncmp("TYPE", recv_buff, 4) == 0 ){                                /* �������������*/
      memset((void *)SendBuf,'\0',sizeof(SendBuf));
      sprintf(SendBuf,"%s %c.\r\n",FTP_SERVICE_CMD1[0],recv_buff[5]);
      CH395_FTPSendData( SendBuf,strlen(SendBuf),ftp.SocketCtl );
   }
   else if( strncmp("syst", recv_buff, 4) == 0 ){                                 /* ϵͳ����*/
      CH395_FTPSendData( (char *)FTP_SERVICE_CMD1[1],strlen(FTP_SERVICE_CMD1[1]),ftp.SocketCtl );
   }
   else if( strncmp("PWD", recv_buff, 3) == 0 ){                                  /* ��ӡĿ¼   */
      CH395_FTPSendData( (char *)FTP_SERVICE_CMD1[2],strlen(FTP_SERVICE_CMD1[2]),ftp.SocketCtl );
   }
   else if( strncmp("CWD", recv_buff, 3) == 0 ){                                  /* ��Ŀ¼*/
      memset((void *)ListName,'\0',sizeof(ListName));   
      CH395_FTPGetFileName( recv_buff,ListName );
      s = CH395_FTPFileOpen( ListName,FTP_MACH_LIST );
      if(s){
         CH395_FTPSendData( (char *)FTP_SERVICE_CMD1[3],strlen(FTP_SERVICE_CMD1[3]),ftp.SocketCtl ); 
      }
      else{
         memset((void *)SendBuf,'\0',sizeof(SendBuf));
         sprintf(SendBuf,"%s.\r\n",FTP_SERVICE_CMD[8]);
         CH395_FTPSendData( SendBuf,strlen(SendBuf),ftp.SocketCtl ); 
      }
   }
   else if( strncmp("PASV", recv_buff, 4) == 0 ){                                /* �ͻ������ڻ�ȡ������������TCP�˿ںż�IP��ַ*/
      CH395_FTPDatServer( 1,&port );
      memset((void *)SendBuf,'\0',sizeof(SendBuf));
      sprintf(SendBuf,"%s%s%02d.%02d).\r\n",FTP_SERVICE_CMD1[4],SourIP,(port/256),(port%256));
      CH395_FTPSendData( SendBuf,strlen(SendBuf),ftp.SocketCtl );
   }
   else if( strncmp("SIZE", recv_buff, 4) == 0 ){                                /* ��ȡ�ļ���С*/
      if(recv_buff[5] == '/'){
         memset((void *)SendBuf,'\0',sizeof(SendBuf));   
         CH395_FTPGetFileName( recv_buff,SendBuf );
         len = strlen(ListName);
         sprintf(gFileName,&SendBuf[len]); 
      }
      else{
         memset((void *)pFileName,'\0',sizeof(gFileName));   
         CH395_FTPGetFileName( recv_buff,gFileName );
      } 
      memset((void *)SendBuf,'\0',sizeof(SendBuf));
      gLEN = CH395_FTPFileSize( gFileName );
      sprintf(SendBuf,"%s%d\r\n",FTP_SERVICE_CMD1[5],gLEN);
      CH395_FTPSendData( (char *)SendBuf,strlen(SendBuf),ftp.SocketCtl );
   }
   else if( strncmp("DELE", recv_buff, 4) == 0 ){                               /* ɾ���ļ�   */
      if(ftp.AuthFlag == 1){                                                    /* �����Ȩ����֧��ɾ��   */
         if(recv_buff[5] == '/'){
            memset((void *)SendBuf,'\0',sizeof(SendBuf));   
            CH395_FTPGetFileName( recv_buff,SendBuf );
            len = strlen(ListName);
            sprintf(pFileName,&SendBuf[len]); 
         }
         else{
            memset((void *)pFileName,'\0',sizeof(pFileName));   
            CH395_FTPGetFileName( recv_buff,pFileName );
         } 
         ftp.ListFlag = 2;
         CH395_FTPListRenew( ftp.ListFlag );
         CH395_FTPSendData( (char *)FTP_SERVICE_CMD1[6],strlen(FTP_SERVICE_CMD1[6]),ftp.SocketCtl );
      }
      else{
         CH395_FTPSendData( (char *)FTP_SERVICE_CMD1[7],strlen(FTP_SERVICE_CMD1[7]),ftp.SocketCtl );
      }
   }
   else if( strncmp("noop", recv_buff, 4) == 0 ){
      CH395_FTPSendData( (char *)FTP_SERVICE_CMD1[8],strlen(FTP_SERVICE_CMD1[8]),ftp.SocketCtl );
   }
   else if( strncmp("RNFR", recv_buff, 4) == 0 ){
      if((ftp.AuthFlag == 1)&&(ftp.ListState == 0)){                            /* �����Ȩ����֧��������*/
         memset((void *)SendBuf,'\0',sizeof(SendBuf));   
         if(recv_buff[5] == '/'){
            CH395_FTPGetFileName( recv_buff,SendBuf );
            len = strlen(ListName);
            sprintf(pFileName,&SendBuf[len]); 
         }
         else{
            memset((void *)pFileName,'\0',sizeof(pFileName));   
            CH395_FTPGetFileName( recv_buff,pFileName );
         } 
         CH395_FTPSendData( (char *)FTP_SERVICE_CMD2[0],strlen(FTP_SERVICE_CMD2[0]),ftp.SocketCtl );
      }
      else{
         CH395_FTPSendData( (char *)FTP_SERVICE_CMD1[7],strlen(FTP_SERVICE_CMD1[7]),ftp.SocketCtl );
      }
   }
   else if( strncmp("RNTO", recv_buff, 4) == 0 ){
      if((ftp.AuthFlag == 1)&&(ftp.ListState == 0)){                            /* �����Ȩ����֧��������*/
         memset((void *)gFileName,'\0',sizeof(gFileName));   
         if(recv_buff[5] == '/'){
            memset((void *)SendBuf,'\0',sizeof(SendBuf));   
            CH395_FTPGetFileName( recv_buff,SendBuf );
            len = strlen(ListName);
            sprintf(gFileName,&SendBuf[len]); 
         }
         else{
            CH395_FTPGetFileName( recv_buff,gFileName );
         } 
         ftp.ListFlag = 5;
         CH395_FTPListRenew( ftp.ListFlag );
         CH395_FTPSendData( (char *)FTP_SERVICE_CMD2[1],strlen(FTP_SERVICE_CMD2[1]),ftp.SocketCtl );
      }
      else{
         CH395_FTPSendData( (char *)FTP_SERVICE_CMD1[7],strlen(FTP_SERVICE_CMD1[7]),ftp.SocketCtl );
      }
   }
   else{                                                                        /* ���֧�� */
      CH395_FTPSendData( (char *)FTP_SERVICE_CMD1[9],strlen(FTP_SERVICE_CMD1[9]),ftp.SocketCtl );
   }
}