/********************************** (C) COPYRIGHT *********************************
* File Name          : ch395FTPCMD.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬFTP�ͻ���Ӧ��-FTP�������
**********************************************************************************/
FTP    ftp;
char   send_buff[536];                                                                    /* �������ݻ�����*/
UINT16 ListNum = 0;
/********************************************************************************
* Function Name  : CH395_FTPLoginUser
* Description    : ��֤�û���   
* Input          : None
* Output         : None
* Return         : None
*********************************************************************************/
void CH395_FTPLoginUser( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "USER %s\r\n", pUserName);
#if DEBUG
  printf("USER :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
} 

/*********************************************************************************
* Function Name  : CH395_FTPLoginPass
* Description    : ��֤����   
* Input          : None
* Output         : None
* Return         : None
*********************************************************************************/
void CH395_FTPLoginPass( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "PASS %s\r\n", pPassword);
#if DEBUG
  printf("PASS :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/*********************************************************************************
* Function Name  : CH395_FTPSyst
* Description    : ��ѯ������ϵͳ����   
* Input          : None
* Output         : None
* Return         : None
*********************************************************************************/
void CH395_FTPSyst( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "SYST\r\n");
#if DEBUG
  printf("SYST :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/*********************************************************************************
* Function Name  : CH395_FTPList
* Description    : ��ȡ�ļ��б����ļ�����ʱ��   
* Input          : None
* Output         : None
* Return         : None
*********************************************************************************/
void CH395_FTPList( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "LIST\r\n");
#if DEBUG
  printf("LIST :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/*********************************************************************************
* Function Name  : CH395_FTPNlst
* Description    : ��ȡ�ļ��б�   
* Input          : None
* Output         : None
* Return         : None
*********************************************************************************/
void CH395_FTPNlst( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "NLST\r\n");
#if DEBUG
  printf("NLST :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/*********************************************************************************
* Function Name  : CH395_FTPType
* Description    : �����ļ���������   
* Input          : None
* Output         : None
* Return         : None
*********************************************************************************/
void CH395_FTPType( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "TYPE A\r\n");
#if DEBUG
  printf("TYPE :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/*********************************************************************************
* Function Name  : CH395_FTPSize
* Description    : ��ȡ�ļ���Ϣ   
* Input          : None
* Output         : None
* Return         : None
*********************************************************************************/
void CH395_FTPSize( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "SIZE %s\r\n",FileName);
#if DEBUG
  printf("SIZE :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/*********************************************************************************
* Function Name  : CH395_FTPRetr
* Description    : �����ļ�   
* Input          : None
* Output         : None            
* Return         : None
*********************************************************************************/
void CH395_FTPRetr( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "RETR %s\r\n",FileName);
#if DEBUG
  printf("RETR :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/*********************************************************************************
* Function Name  : CH395_FTPCdup
* Description    : ������һ��Ŀ¼   
* Input          : None
* Output         : None         
* Return         : None
*********************************************************************************/
void CH395_FTPCdup( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "CDUP\r\n");
#if DEBUG
  printf("CDUP :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/********************************************************************************
* Function Name  : CH395_FTPMkd
* Description    : ����Ŀ¼   
* Input          : ReName
* Output         : None         
* Return         : None
*********************************************************************************/
void CH395_FTPMkd(UINT8 ReName )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  if(ReName){
    ListNum++;
    sprintf( send_buff, "MKD %s%d\r\n",ListName,ListNum);
  }
  else sprintf( send_buff, "MKD %s\r\n",ListName);
  sprintf( ListMdk,&send_buff[4] );                                                       /*����Ŀ¼��*/
#if DEBUG
  printf("MKD :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/*********************************************************************************
* Function Name  : CH395_FTPRmd
* Description    : ɾ��Ŀ¼   
* Input          : None
* Output         : None         
* Return         : None
*********************************************************************************/
void CH395_FTPRmd( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "RMD %s\r\n",ListName); 
#if DEBUG
  printf("RMD :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/*********************************************************************************
* Function Name  : CH395_FTPAppe
* Description    : �ϴ��ļ�   
* Input          : None
* Output         : None         
* Return         : None
*********************************************************************************/
void CH395_FTPAppe( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "APPE %s\r\n",FileName);
#if DEBUG
  printf("STOR :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/********************************************************************************
* Function Name  : CH395_FTPAllo
* Description    : �����ļ�   
* Input          : None
* Output         : None         
* Return         : None
*********************************************************************************/
void CH395_FTPAllo( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "ALLO 500000\r\n");
#if DEBUG
  printf("ALLO :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/********************************************************************************
* Function Name  : CH395_FTPNoop
* Description    : ���Ϳ�ָ��   
* Input          : None
* Output         : None
* Return         : None
*********************************************************************************/
void CH395_FTPNoop( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "NOOP\r\n");
#if DEBUG
  printf("NOOP :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/*********************************************************************************
* Function Name  : CH395_FTPPort
* Description    : �趨���Ӷ˿ں�   
* Input          : None
* Output         : None
* Return         : None
*********************************************************************************/
void CH395_FTPPort( UINT16 port )
{
  memset( send_buff, '\0', sizeof(send_buff));
  sprintf( send_buff, "PORT %s%02d,%02d\r\n",SourIP,port/256,port%256);
#if DEBUG
  printf("PORT :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/*********************************************************************************
* Function Name  : CH395_FTPCwd
* Description    : ��Ŀ¼   
* Input          : index
* Output         : None
* Return         : None
*********************************************************************************/
void CH395_FTPCwd( UINT8 index )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  if(index==1) sprintf( send_buff, "CWD /%s\r\n",ListMdk);
  else sprintf( send_buff, "CWD /%s\r\n",ListName);
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/*********************************************************************************
* Function Name  : CH395_FTPQuit
* Description    : �˳���¼   
* Input          : None
* Output         : None
* Return         : None
*********************************************************************************/
void CH395_FTPQuit( )
{
  memset( send_buff, '\0', sizeof(send_buff) );
  sprintf( send_buff, "QUIT\r\n");
#if DEBUG
  printf("QUIT :%s\n", send_buff);
#endif
  CH395_SocketSendData( send_buff, strlen(send_buff),ftp.SocketCtl );
}

/********************************************************************************
* Function Name  : CH395_FTPCheckLogin
* Description    : ��֤��½�����ź�
* Input          : recv_buff- ������Ϣ
                   check_type-��������
* Output         : None
* Return         : None
*********************************************************************************/
UINT8 CH395_FTPCheckLogin( char *recv_buff )
{
  if( strncmp("220", recv_buff, 3) == 0 ){                                                /* ���ӳɹ�*/
    CH395_FTPLoginUser( );
  }
  else if( strncmp("331", recv_buff, 3) == 0 ){                                            /* �û�����ȷ*/
    CH395_FTPLoginPass( );          
  }
  else if( strncmp("230", recv_buff, 3) == 0 ){                                            /* ��¼�ɹ�  */
    ftp.CmdStatus = FTP_MACH_LOGINSUC;
  }
  else if( strncmp("530", recv_buff, 3) == 0 ){                                           /* ��½ʧ��*/
    CH395CloseSocket( ftp.SocketCtl );
  }
  else if( strncmp("221", recv_buff, 3) == 0 ){                                           /* �˳���½*/
    CH395CloseSocket( ftp.SocketCtl );                                                    /* �ر�����*/
  }
  return FTP_ERR_LOGIN; 
}

/********************************************************************************
* Function Name  : CH395_FTPCheckSearch
* Description    : ��֤�Ѳ��ļ������ź�
* Input          : recv_buff- ������Ϣ
                   check_type-��������
* Output         : None
* Return         : None
*********************************************************************************/
UINT8 CH395_FTPCheckSearch( char *recv_buff )
{
	printf("#####");

  if( strncmp("200 Type", recv_buff, 8) == 0 ){                                           /* ��ʽ����*/ 
    CH395_FTPDatServer( 1,&ftp.SouPort );
    CH395_FTPPort( ftp.SouPort );
  }
  else if( strncmp("200 PORT", recv_buff, 8 ) == 0){       
    CH395_FTPList( );
  }
  else if( strncmp("150", recv_buff, 3) == 0 ){                                           /* ��ȡ�б��ɹ�*/
    ftp.CmdDataS = FTP_MACH_RECEDATA;
  }
  else if( strncmp("226", recv_buff, 3) == 0 ){                                           /* �б���Ϣ����*/
    if(ftp.InterCmdS == FTP_MACH_FINDFILE) ftp.CmdStatus = FTP_MACH_SEARCHSUC;
	 if(ftp.FindList == 1){
         ftp.FindList = 0;
         CH395_FTPCwd( 0 );
	 }
    ftp.InterCmdS = 0;
  }
  else if( strncmp("425", recv_buff, 3) == 0 ){                                           /* �޷�������������*/
    CH395_FTPQuit( );
  }                              
  else if( strncmp("250", recv_buff, 3) == 0 ){                                           /* ��Ŀ¼*/
 	printf("250^^^^^^^^^^^^^^^^^^^^^^^^^^");
   ftp.InterCmdS = FTP_MACH_FINDFILE;
    CH395_FTPType( );
  }
  else if( strncmp("221", recv_buff, 3) == 0 ){                                           /* �˳���½  */
    CH395CloseSocket( ftp.SocketCtl );                                                    /* �ر�����  */
  }
  else return FTP_ERR_SEARCH;
  return FTP_CHECK_SUCCESS;
}

/*********************************************************************************
* Function Name  : CH395_FTPCheckGetfile
* Description    : ��֤�����ļ������ź�
* Input          : recv_buff- ������Ϣ
                   check_type-��������
* Output         : None
* Return         : None
*********************************************************************************/
UINT8 CH395_FTPCheckGetfile( char *recv_buff )
{      
  if( strncmp("200 Type", recv_buff, 8) == 0 ){                                           /* ��ʽ���� */
    CH395_FTPSize( );
  }
  else if( strncmp("213", recv_buff, 3) == 0 ){                                           /* �ļ���Ϣ */
    CH395_FTPDatServer( 1,&ftp.SouPort );
    CH395_FTPPort( ftp.SouPort );
  }
  else if( strncmp("200 PORT", recv_buff, 8) == 0 ){      
    CH395_FTPRetr( );
  }
  else if( strncmp("150", recv_buff, 3) == 0 ){      
    ftp.CmdDataS = FTP_MACH_RECEDATA;
    memset( send_buff, '\0', sizeof(send_buff) );
  }
  else if(strncmp("550", recv_buff, 3) == 0 ){                                            /* û���ҵ��ļ�*/
    CH395_FTPQuit( );
  }
  else if(strncmp("226", recv_buff,3) == 0 ){                                             /* �������  */
    ftp.CmdStatus = FTP_MACH_GETSUC;           
  }
  else if( strncmp("221", recv_buff, 3) == 0 ){                                           /* �˳���½   */
    CH395CloseSocket( ftp.SocketCtl );                                                    /* �ر�����      */
  }
  else return FTP_ERR_GETFILE;
  return (FTP_CHECK_SUCCESS);
}

/*********************************************************************************
* Function Name  : CH395_FTPCheckPutfile
* Description    : ��֤�ϴ��ļ������ź�
* Input          : recv_buff- ������Ϣ
                   check_type-��������
* Output         : None
* Return         : None
*********************************************************************************/
UINT8 CH395_FTPCheckPutfile( char *recv_buff )
{
  if( strncmp("250", recv_buff, 3) == 0 ){                                                /* ������һ��Ŀ¼ */
    if(ftp.InterCmdS == FTP_MACH_MKDLIST){
      CH395_FTPMkd( 0 );
    }
    else if(ftp.InterCmdS == FTP_MACH_PUTFILE){
      CH395_FTPType( );
    }
  }
  else if( strncmp("257", recv_buff, 3) == 0 ){                                           /* �����ɹ�     */
    ftp.InterCmdS = FTP_MACH_PUTFILE;                                                     /* �ϴ��ļ�     */
    CH395_FTPCwd( 1 );                                                                    /* ��Ŀ¼      */
  }
  else if( strncmp("550", recv_buff, 3) == 0 ){                                           /* ������Ŀ¼������*/
#if  1
    CH395_FTPMkd( 1 );                                                                    /* ��Ŀ¼�������򴴽�����һ��Ŀ¼������ԭ��Ŀ¼�����ۼ������磺TXET1��*/
#else
    CH395_FTPCwd( 0 );                                                                    /* ��Ŀ¼��������ֱ�Ӵ� */
#endif                      
  }
  if( strncmp("200 Type", recv_buff, 8) == 0 ){                                           /* ��ʽ����   */
    CH395_FTPDatServer( 1,&ftp.SouPort );
    CH395_FTPPort( ftp.SouPort );
  }
  else if( strncmp("200 PORT", recv_buff, 8 ) == 0){    
    CH395_FTPAppe( );                  
  }
  else if( strncmp("150", recv_buff, 3) == 0 ){                                           /* �����ϴ��ļ�����ɹ�*/
    ftp.CmdDataS = FTP_MACH_SENDDATA;                                                     /* ��Ҫ��������  */
  }
  else if( strncmp("226", recv_buff, 3) == 0 ){                                           /* �ϴ�����    */
    ftp.CmdStatus = FTP_MACH_PUTSUC;       
  }
  else if( strncmp("221", recv_buff, 3) == 0 ){                                           /* �˳���½   */
    CH395CloseSocket( ftp.SocketCtl );                                                    /* �ر�����    */
  }
  else return FTP_ERR_PUTFILE;
  return (FTP_CHECK_SUCCESS);
}

/********************************************************************************
* Function Name  : CH395_FTPCmdRespond
* Description    : ��֤��½�����ź�
* Input          : recv_buff- ������Ϣ
                   check_type-��������
* Output         : None
* Return         : None
*********************************************************************************/
UINT8 CH395_FTPCmdRespond( char *recv_buff,UINT8 check_type )
{
  UINT8 s;
  
  if( strncmp("421", recv_buff, 3) == 0 ){                                                /* ����������Ͽ� */
    return (FTP_CHECK_SUCCESS);
  }
  switch( check_type ){
    case FTP_CMD_LOGIN:                                                                   /* �˶Ե�½��������еķ�����Ϣ*/
      s = CH395_FTPCheckLogin( recv_buff );
      return s;
    case FTP_CMD_SEARCH:                                                                  /* �˶��Ѳ��ļ������еķ�����Ϣ  */
      s = CH395_FTPCheckSearch( recv_buff );
      return s;
    case FTP_CMD_GETFILE:                                                                 /* �˶������ļ������еķ�����Ϣ  */
      s = CH395_FTPCheckGetfile( recv_buff );
      return s;
    case FTP_CMD_PUTFILE:                                                                 /* �˶��ϴ��ļ������еķ�����Ϣ  */
      s = CH395_FTPCheckPutfile( recv_buff );
      return s;
    default:
      return FTP_ERR_UNKW;
  }
}

/********************************************************************************
* Function Name  : CH395_FTPFindList
* Description    : �˶�Ŀ¼����
* Input          : pReceiveData- ��ȡ���б���Ϣ
* Output         : None
* Return         : None
*********************************************************************************/
UINT8 CH395_FTPFindList( char *pReceiveData ) 
{
  UINT32 len,i,j;

  len = ftp.RecDatLen;
  for(i=0;i<len;i++){
    if(strncmp(ListName,&pReceiveData[i],strlen(ListName))==0){
      j = i;
      while(strncmp("<DIR>",&pReceiveData[j],strlen("<DIR>"))){
        j--;
        if( pReceiveData[j]== 'M' ) return FALSE;
      }
#if DEBUG
  printf("*********\nfind list\n*********\n");
#endif
      return FTP_CHECK_SUCCESS;                /* �ҵ���ָ�����ļ��� */
    } 
  }  
  return FTP_ERR_UNLIST;
}

/********************************************************************************
* Function Name  : CH395_FTPFindFile
* Description    : �˶��ļ�����
* Input          : pReceiveData-��ȡ���ļ��б���Ϣ
* Output         : None
* Return         : None
*********************************************************************************/
UINT8 CH395_FTPFindFile( char *pReceiveData ) 
{
  UINT32 len,i;
  len = ftp.RecDatLen;
  for(i=0;i<len;i++){
  if(strncmp(FileName,&pReceiveData[i],strlen(FileName))==0){
#if DEBUG
  printf("*********\nfind file\n*********\n");
#endif
      return FTP_CHECK_SUCCESS;                /* �ҵ���ָ�����ļ���*/
    } 
  }  
  return FTP_ERR_UNFILE;
}

/*********************************************************************************
* Function Name  : ch563ftp_loginstart
* Description    : ��½
* Input          : None
* Output         : None
* Return         : ����ִ�н��
*********************************************************************************/
UINT8 CH395_FTPLogin( )
{
  if(ftp.CmdFirtS == 0){
    ftp.CmdFirtS  = 1;
    ftp.FileCmd = FTP_CMD_LOGIN;                                                          /* �����Ѳ��ļ�״̬   */
    CH395_FTPCtlClient( 0 );
  }
  if( ftp.CmdStatus == FTP_MACH_LOGINSUC ){                                               /* ��½�ɹ�       */
#if DEBUG
  printf("************\nlogin success\n*********\n");
#endif
    ftp.CmdFirtS = 0;
    ftp.CmdStatus = 0;
    ftp.FileCmd = 0;    
    return FTP_COMMAND_SUCCESS;  
  }  
  return FTP_COMMAND_CONTINUE;  
}

/*********************************************************************************
* Function Name  : ch563ftp_searchfile
* Description    : �Ѳ��ļ�
* Input          : pListName - ��Ҫ�Ѳ���ļ�����Ŀ¼��
                   pFileName - ��Ŀ¼�µ��ļ���
* Output         : None
* Return         : None
*********************************************************************************/
UINT8 CH395_FTPSearch(char *pListName,char *pFileName )
{
  if( ftp.CmdFirtS == 0 ){
    ftp.CmdFirtS = 1;
    ftp.FileCmd = FTP_CMD_SEARCH;                                                         /* �����Ѳ��ļ�״̬  */
    ftp.InterCmdS = FTP_MACH_FINDLIST;                                                    /* ����Ŀ¼       */
    memcpy((void *)ListName,'\0',sizeof(ListName));    
    sprintf( ListName,pListName );                                                        /* �����ļ�������  */
    memcpy((void *)FileName,'\0',sizeof(FileName));         
    sprintf( FileName,pFileName );                                                        /* �����Ѳ��ļ�������  */
    CH395_FTPType( );
  }
  if(ftp.TcpStatus == FTP_MACH_DISCONT){
    if(ftp.CmdStatus == FTP_MACH_SEARCHSUC){                                              /* �Ѳ��ļ����     */
#if DEBUG
  printf("**********\nsearch success\n*********\n");
#endif
      ftp.CmdFirtS = 0;
      ftp.CmdStatus = 0;
      ftp.FileCmd = 0;   
      return FTP_COMMAND_SUCCESS;
    }
  }
  return FTP_COMMAND_CONTINUE; 
}

/*********************************************************************************
* Function Name  : ch563ftp_getfile
* Description    : �����ļ�
* Input          : pFileName - ���ص��ļ���
* Output         : None
* Return         : None
*********************************************************************************/
UINT8 CH395_FTPGetFile( char *pFileName )
{
  if(ftp.CmdFirtS == 0){
    ftp.CmdFirtS  = 1;
    ftp.FileCmd = FTP_CMD_GETFILE;                                                        /* ���������ļ�״̬   */
    ftp.InterCmdS = FTP_MACH_GETFILE;                                                     /* �����ļ�       */
    memcpy((void *)FileName,'\0',sizeof(FileName));         
    sprintf( FileName,pFileName );                                                        /* ���������ļ������� */
    CH395_FTPType( );
  }
  if(ftp.TcpStatus == FTP_MACH_DISCONT){
    if(ftp.CmdStatus == FTP_MACH_GETSUC){                                                 /* �����ļ��ɹ�     */
#if DEBUG
  printf("*********\ngetfile success\n*********\n");
#endif
    ftp.CmdFirtS = 0;
    ftp.CmdStatus = 0;
    ftp.FileCmd = 0;     
    return FTP_COMMAND_SUCCESS;
    }
  }  
  return FTP_COMMAND_CONTINUE; 
}

/*********************************************************************************
* Function Name  : CH395_FTPPutFile
* Description    : �ϴ��ļ�
* Input          : pListNAme - �ϴ��ļ����豣���Ŀ¼��
                   pFileName - �ϴ��ļ����豣����ļ���
* Output         : None
* Return         : None
*********************************************************************************/
UINT8 CH395_FTPPutFile(char *pListNAme, char *pFileName)
{
  if(ftp.CmdFirtS ==0){
    ftp.CmdFirtS = 1;
    ftp.FileCmd = FTP_CMD_PUTFILE;                                                        /* �����ϴ��ļ�״̬*/
    ftp.InterCmdS = FTP_MACH_MKDLIST;                                                     /* ����Ŀ¼      */
    memcpy((void *)ListName,'\0',sizeof(ListName));                    
    sprintf( ListName,pListNAme );                                                        /* �����ϴ��ļ�������*/
    memcpy((void *)FileName,'\0',sizeof(FileName));         
    sprintf( FileName,pFileName );                                                        /* �����ϴ��ļ������� */
    CH395_FTPCdup( );
  }
  if(ftp.TcpStatus == FTP_MACH_DISCONT){
    if(ftp.CmdStatus == FTP_MACH_PUTSUC){                                                 /* �ϴ��ļ��ɹ�   */
#if DEBUG
  printf("*********\nputfile success\n*********\n");
#endif
      ftp.CmdFirtS = 0;
      ftp.CmdStatus = 0;
      ftp.FileCmd = 0;
      return FTP_COMMAND_SUCCESS;
    }
  }  
  return FTP_COMMAND_CONTINUE;   
}