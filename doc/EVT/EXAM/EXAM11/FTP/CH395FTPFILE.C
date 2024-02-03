/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395FTPFILE.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬFTP�ͻ���Ӧ��-�ļ���д����
*                     
**********************************************************************************/
const  UINT8 *pTest = "abcdefghijklmnopqrstuvwxyz0123456789";     /* �ϴ��ĵ������� */
/**********************************************************************************
* Function Name  : CH395_FTPFileOpen
* Description    : ���ļ�   
* Input          : pFileName-�ļ���
* Output         : None                  
* Return         : None
*********************************************************************************/
UINT8 CH395_FTPFileOpen ( char *pFileName)
{
     return FTP_CHECK_SUCCESS;
}

/*********************************************************************************
* Function Name  : CH395_FTPFileWrite
* Description    : ��ȡ��������   
* Input          : recv_buff-����
                   len -     ����
* Output         : None                  
* Return         : None
*********************************************************************************/
UINT8 CH395_FTPFileWrite ( char *recv_buff ,UINT16 len)
{
     memcpy(send_buff,recv_buff,len);          
     return (FTP_CHECK_SUCCESS);
}

/*********************************************************************************
* Function Name  : CH395_FTPFileRead
* Description    : ��������   
* Input          : None
* Output         : None                  
* Return         : None
*********************************************************************************/
void CH395_FTPFileRead( )
{
     memset( send_buff, '\0', sizeof(send_buff));
     if(strlen(pTest)<536) {
          sprintf( send_buff, "%s\r\n",pTest);                  /*������д�뷢�ͻ������������ݽϴ󣬷ֶ��д�� */
          ftp.CmdDataS = FTP_MACH_DATAOVER;                     /*������ݶ�������Ϊ���ݶ����־λ */
     }
}