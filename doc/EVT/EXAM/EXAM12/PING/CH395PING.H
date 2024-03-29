/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395PINC.H
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        :  CH563FTP.c ���ܺ���˵��
**********************************************************************************/
#ifndef __CH395PING_H__
#define __CH395PING_H__

#define DEBUG	                	1	        


#define ICMP_SOKE_CON	    	0
#define ICMP_SEND_ERR	    	1
#define ICMP_SEND_SUC	    	2
#define ICMP_RECV_ERR	    	3
#define ICMP_RECV_SUC	    	4
#define ICMP_UNRECH	            5
#define ICMP_REPLY	            6
#define ICMP_REPLY_SUC	        7
#define ICMP_KEEP_NO	       10

#define ICMP_HEAD_TYPE	    8
#define ICMP_HEAD_REPLY     0
#define ICMP_HEAD_CODE	    0
#define ICMP_HEAD_ID	    512
#define ICMP_HEAD_SEQ	    100
#define ICMP_DATA_BYTES	    32

/*ICMP��ͷ�ֶ����ݽṹ*/
typedef struct _icmphdr 
{
	  UINT8   i_type;                                                           /*ICMP��������*/
    UINT8   i_code;                                                           /*�������еĴ����*/
    UINT16  i_cksum;                                                          /*У���*/ 
    UINT16  i_id;                                                             /*Ωһ�ı�ʶ��*/
    UINT16  i_seq;                                                            /*���к�*/
	  UINT8   i_data[32];
}IcmpHeader,*IcmpHead;

extern UINT8  IcmpResp;
extern UINT8  IcmpSuc;
extern UINT8  IcmpCont;

void InitParameter( void );

void mInitSTDIO( );

void CH395_PINGCmd( void );                                                      /* ��ѯ״̬��ִ�������� */

void CH395PINGInit( void );                                                      /* 563��ʼ�� */

void InitPing( void );

void Intervalometer_4ms( void );

void CH395IcmpRecvData( UINT32 len, UINT8 *pDat );
#endif	