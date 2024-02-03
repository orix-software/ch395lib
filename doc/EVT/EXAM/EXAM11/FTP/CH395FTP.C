/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395FTP.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬFTP�ͻ���Ӧ��-FTP�ӿ�����
**********************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../PUB/CH395INC.H"
#include "../PUB/CH395CMD.H"
#include "CH395.H"
#include "CH395FTPINC.H"
#include "CH395FTPCMD.C"
#include "CH395FTPFILE.C"

/******************************************************************************
* Function Name  : CH395_FTPProcessReceDat
* Description    : �������յ�������
* Input          : recv_buff- ������Ϣ
                   check_type-���˶Ե���������
                   sockeid  - socket����
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPProcessReceDat( char *recv_buff,UINT8 check_type,UINT8 socketid )
{
    UINT8 S;
    if( socketid == ftp.DatTransfer ){
        if(ftp.CmdDataS == FTP_MACH_RECEDATA){                                        /* �����ļ�����    */
            if( ftp.InterCmdS == FTP_MACH_GETFILE ){ 
                S = CH395_FTPFileWrite(recv_buff,strlen(recv_buff));
            }
            else if(ftp.InterCmdS == FTP_MACH_FINDLIST ){
                S = CH395_FTPFindList( recv_buff );                                    /* ��֤�������ݣ����ڲ���ָ����Ŀ¼��*/
                if( S == FTP_CHECK_SUCCESS ) ftp.FindList = 1;                         /* �鵽ָ����Ŀ¼��*/
            }
            else if(ftp.InterCmdS == FTP_MACH_FINDFILE ){
                S = CH395_FTPFindFile( recv_buff );                                    /* �����ļ�    */
                if( S == FTP_CHECK_SUCCESS ) ftp.FindFile = 1;                         /* �ҵ��ļ�    */
            }                                                                                    
        }
    }
    else if( socketid == ftp.SocketCtl ){                                              /* ����Ϊ����Ӧ��*/            
        S = CH395_FTPCmdRespond( recv_buff,check_type );

    }
}

/*******************************************************************************
* Function Name  : CH395_FTPSendFile
* Description    : ��������
* Input          : NONE
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPSendFile( )
{
    UINT8 S;    

    S = CH395_FTPFileOpen( FileName );
    if(S == FTP_CHECK_SUCCESS)    CH395_FTPFileRead( );
    CH395_SocketSendData( send_buff, strlen(send_buff),ftp.DatTransfer );
    if(ftp.CmdDataS == FTP_MACH_DATAOVER){
        CH395CloseSocket( ftp.DatTransfer );
    }    
}

/*******************************************************************************
* Function Name  : CH395_FTPClientCmd
* Description    : ��ѯ״̬��ִ����Ӧ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPClientCmd( )
{
    if(ftp.CmdDataS == FTP_MACH_SENDDATA){
    if(ftp.TcpStatus == FTP_MACH_CONNECT) CH395_FTPSendFile( );                       /* ���������������*/
        return ;     
    }
    if(ftp.FileCmd){
        CH395_FTPInterCmd( );                                                         /* ִ�ж�Ӧ�Ľӿ�����*/
    }
}

/*******************************************************************************
* Function Name  : CH395_FTPInterCmd
* Description    : ִ�ж�Ӧ����������е���ִ��˳��
* Input          : NONE
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_FTPInterCmd( )
{
    switch(ftp.FileCmd){
        case FTP_CMD_LOGIN:                                                            /* ��½      */
            if( CH395_FTPLogin( ) == FTP_COMMAND_SUCCESS ){                            /* ��½�ɹ�,�ɽ�����������*/
                CH395_FTPSearch("USER","FILELIST.TXT" );                               /* ��ʼ��ѯָ����Ŀ¼�µ��ļ�*/
            }
            break;
        case FTP_CMD_SEARCH:                                                           /* �Ѳ��ļ���������Ŀ¼�����ļ�����*/
            if( CH395_FTPSearch("USER","FILELIST.TXT" ) == FTP_COMMAND_SUCCESS ){      /* ��ѯ����ִ����ϣ��ɽ����������� */        
                if( ftp.FindFile )CH395_FTPGetFile("FILELIST.TXT" );                   /* �Ѳ鵽ָ����Ŀ¼�µ��ļ���ʼ�����ļ� */
                else CH395_FTPQuit( );                                                 /* û�Ѳ鵽ָ����Ŀ¼�µ��ļ����˳���Ҳ�ɽ����������������ϴ���*/
            }
            break;
        case FTP_CMD_GETFILE:                                                          /* �����ļ����������ļ�����*/
            if(CH395_FTPGetFile("FILELIST.TXT" ) == FTP_COMMAND_SUCCESS ){             /* �����ļ��ɹ����ɽ�����������    */
                CH395_FTPPutFile("TEXT","abc.txt");                                    /* �ϴ��ļ�      */
            }
            break;
        case FTP_CMD_PUTFILE:                                                          /* �ϴ��ļ���������Ŀ¼�����ļ�����*/
            if( CH395_FTPPutFile("TEXT","abc.txt")== FTP_COMMAND_SUCCESS ){            /* �ϴ��ļ��ɹ����ɽ�����������*/
                CH395_FTPQuit( );                                                      /* �˳�    */
            }
            break;
        default:
            break;
    }
}