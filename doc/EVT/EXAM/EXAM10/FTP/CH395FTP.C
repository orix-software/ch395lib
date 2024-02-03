/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395FTP.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬFTP������Ӧ��-FTP�������
**********************************************************************************/
#include <reg52.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../PUB/CH395INC.H"
#include "../PUB/CH395CMD.H"
#include "CH395FTPINC.H"
#include "CH395FTPCMD.C"
#include "CH395FTPFILE.C"

/*******************************************************************************
* Function Name  : CH395_FTPSendData
* Description    : ׼����Ҫ���͵�����
* Input          : pName - �ļ�������Ϊ��Ҫ��ȡ�б���Ϣ����Ϊ�ַ�0��
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPDataReady( char *pName )
{
     ftp.CmdDataS = FTP_MACH_SENDDATA;
     CH395_FTPFileRead( pName );                                                  /* ��Ҫ���͵�����д�뷢�ͻ�����*/
}

/******************************************************************************
* Function Name  : CH395_FTPProcessReceDat
* Description    : �������յ�������
* Input          : recv_buff- ������Ϣ
                   sockeid  - socket����
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPProcessReceDat( char *recv_buff,UINT8 sockeid )
{
     if( sockeid == ftp.SocketDat ){
          if(ftp.CmdDataS == FTP_MACH_RECEDATA){                                  /* �����ļ�����*/
               CH395_FTPFileWrite(recv_buff,(UINT16)strlen(recv_buff));
          }
     }
     if( sockeid == ftp.SocketCtl ){                                              /* ��������     */          
          CH395_FTPCmdRespond(recv_buff);
     }     
}

/******************************************************************************
* Function Name  : CH395_FTPServerCmd
* Description    : ��ѯ״̬��ִ����Ӧ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPServerCmd( )
{
     UINT8 i;

     if(ftp.TcpCtlSta == FTP_MACH_CONNECT){                                      /* ����TCP FTP��������*/
          ftp.TcpCtlSta = FTP_MACH_KEPCONT; 
          CH395_FTPSendData( (char *)FTP_SERVICE_CMD[0],strlen(FTP_SERVICE_CMD[0]),ftp.SocketCtl );
     }
     if(ftp.TcpDatSta == FTP_MACH_CONNECT){                                      /* ch395������������*/
          if( ftp.CmdPortS == 1 ){
               ftp.CmdPortS = 0;
               CH395_FTPSendData( (char *)FTP_SERVICE_CMD[5],strlen(FTP_SERVICE_CMD[5]),ftp.SocketCtl );
          }
          else if( ftp.CmdPasvS == 1 ){
               ftp.CmdPasvS = 0;
          }
     }                                                                                                  
     if(ftp.TcpDatSta == FTP_MACH_DISCONT){                                       /* �����������*/
          if(ftp.CmdDataS == FTP_MACH_RECEDATA){
               ftp.CmdDataS = FTP_MACH_DATAOVER;
               CH395_FTPListRenew( ftp.ListFlag );                                /* ����Ŀ¼       */
          }
     } 
     if(ftp.CmdDataS == FTP_MACH_DATAOVER){                                       /* ���ݴ������*/
          ftp.CmdDataS = FTP_MACH_CLOSECTL;     
          CH395_FTPSendData( (char *)FTP_SERVICE_CMD[1],strlen(FTP_SERVICE_CMD[1]),ftp.SocketCtl );
     }
     if(ftp.CmdDataS == FTP_MACH_SENDDATA){                                       /* ��������     */
          if(ftp.TcpDatSta >= FTP_MACH_CONNECT){ 
               ftp.TcpDatSta = FTP_MACH_KEPCONT;
               CH395_FTPSendData( SendBuf,strlen(SendBuf),ftp.SocketDat );
               if(ftp.DataOver ){                                                  /* ��⵽�������*/
                    ftp.CmdDataS = FTP_MACH_DATAOVER;                              /* �÷�����ɱ�־*/
                    i = CH395CloseSocket( ftp.SocketDat );                         /* �ر���������      */
                    mStopIfError(i); 
               }
          }
     }
}