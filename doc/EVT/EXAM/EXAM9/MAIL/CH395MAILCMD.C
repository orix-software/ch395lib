/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395MAILCMD.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : �����
**********************************************************************************/
const 	UINT8 *g_strBoundary ="18ac0781-9ae4-4a2a-b5f7-5479635efb6b";                                   /* �߽磬���޸�*/
const 	UINT8 *g_strEncode = "base64";                                                                  /* ���뷽ʽ*/
const 	UINT8 *g_strUINT8set ="gb2312";                                                                 /* windows��ʽ (linux��ʽ"utf-8")*/
const   UINT8 *g_xMailer ="X-Mailer: X-WCH-Mail Client Sender\r\n";                                     /* X-Mailer����*/
const   UINT8 *g_Encoding ="Content-Transfer-Encoding: quoted-printable\r\nReply-To: ";                 /* Encoding ����*/
const   UINT8 *g_Custom ="X-Program: CSMTPMessageTester";                                               /* X-Program��ͬ�����޸�*/
const   UINT8 *g_FormatMail = "This is a multi-part message in MIME format.";                           /* �ʼ��ж������ݣ�һ�����и���*/
const   UINT8 *g_AttachHead = "\r\nContent-Transfer-Encoding: quoted-printable\r\n";                    /* ���뷽ʽ*/
#ifdef send_mail
const	  UINT8 *base64_map ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
#endif
#ifdef receive_mail
const UINT8 code base64_decode_map[256] = 
{
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 62, 255, 255, 255, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 255, 255,
     255, 0, 255, 255, 255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
     15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 255, 255, 255, 255, 255, 255, 26, 27, 28,
     29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
     49, 50, 51, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};
#endif
POP	    m_pop3;
POP	    *p_pop3;
SMTP	m_smtp;
SMTP	*p_smtp;
UINT8 	EncodeName[32];                                                                                /* �������󷢼�������*/
UINT8 	send_buff[512];                                                                                /* �������ݻ�����*/
UINT8   MailBodyData[128]= "Demonstration test ch395 mail function wch.cn";                            /* �ʼ��������ݣ�������ʾ���ظ�ʱ�����ݴ������������ݣ�*/
UINT8   AttachmentData[attach_max_len]= "0123456789abcdefghijklmnopqrstuvwxyz";                        /* �������ݣ�������ʾ���ظ�ʱ�����ݴ�����ĸ������ݣ�*/
/****************************************************************************
* Function Name  : Base64Encode
* Description    : base64����
* Input          : src��Ҫ������ַ���
                   src_len��Ҫ�����ַ����ĳ���
                   dst�������ַ���
* Output         : None
* Return         : None
*****************************************************************************/
#ifdef send_mail
void Base64Encode(UINT8 *src, UINT16 src_len, UINT8 *dst)
{
        UINT16 i = 0;
        UINT16 j = 0;
        
        for (; i < src_len - src_len % 3; i += 3) {
                dst[j++] = base64_map[(src[i] >> 2) & 0x3f];
                dst[j++] = base64_map[((src[i] << 4) | (src[i + 1] >> 4)) & 0x3f];
                dst[j++] = base64_map[((src[i + 1] << 2) | (src[i + 2] >> 6 )) & 0x3f];
                dst[j++] = base64_map[src[i + 2] & 0x3f];
        }
        
        if (src_len % 3 == 1) {
                 dst[j++] = base64_map[(src[i] >> 2) & 0x3f];
                 dst[j++] = base64_map[(src[i] << 4) & 0x3f];
                 dst[j++] = '=';
                 dst[j++] = '=';
        }
        else if (src_len % 3 == 2) {
                dst[j++] = base64_map[(src[i] >> 2) & 0x3f];
                dst[j++] = base64_map[((src[i] << 4) | (src[i + 1] >> 4)) & 0x3f];
                dst[j++] = base64_map[(src[i + 1] << 2) & 0x3f];
                dst[j++] = '=';
        }
        
        dst[j] = '\0';
}
#endif	
/*****************************************************************************
* Function Name  : Base64Decode
* Description    : base64����
* Input          : src��Ҫ������ַ���
                   src_len��Ҫ�����ַ����ĳ���
                   dst�������ַ���
* Output         : None
* Return         : None
*****************************************************************************/
#ifdef receive_mail
void Base64Decode(UINT8 *src, UINT16 src_len, UINT8 *dst)
{
      int i = 0, j = 0;
        
    for (; i < src_len; i += 4) {
    if(strncmp( &src[i], "\r\n", 2 ) == 0) i += 2;
    	dst[j++] = base64_decode_map[src[i]] << 2 |
              base64_decode_map[src[i + 1]] >> 4;
    	dst[j++] = base64_decode_map[src[i + 1]] << 4 |
              base64_decode_map[src[i + 2]] >> 2;
    	dst[j++] = base64_decode_map[src[i + 2]] << 6 |
              base64_decode_map[src[i + 3]];
        }
  	if(src_len%4 == 3) {
    	dst[strlen(dst)-1] = '\0'; 
    }
  	else if(src_len%4 == 2) {
    	dst[strlen(dst)-1] = '\0'; 
    	dst[strlen(dst)-1] = '\0'; 
    }
  	else dst[j] = '\0';
}
#endif	

/*****************************************************************************
* Function Name  : QuotedPrintableEncode
* Description    : quotedprintable����
* Input          : pSrc��Ҫ������ַ���
                   pDst�������ַ���
                   nSrcLen��Ҫ�����ַ����ĳ���
                   nMaxLineLen�������
* Output         : None
* Return         : None
*****************************************************************************/
#ifdef send_mail
void QuotedPrintableEncode( UINT8 *pSrc, UINT8 *pDst, UINT16 nSrcLen, UINT16 nMaxLineLen )
{
	UINT16 nDstLen  = 0;
	UINT16 nLineLen = 0;
	UINT16 i = 0;

	for(i = 0; i < nSrcLen; i++, pSrc++ ){            
  	if( (*pSrc >= '!') && (*pSrc <= '~') && (*pSrc != '=') ){
      *pDst++ = (char)*pSrc;
    	nDstLen++;
    	nLineLen++;
    }
  	else{
    	sprintf(pDst, "=%02x", *pSrc);
    	pDst += 3; 
    	nDstLen += 3;
    	nLineLen += 3;
    }
  	if( nLineLen >= nMaxLineLen - 3 ){    
     	sprintf(pDst, "=\r\n");
    	pDst += 3;
    	nDstLen += 3;
    	nLineLen = 0;
    }
  }
  *pDst = '\0';
}
#endif	

/*****************************************************************************
* Function Name  : QuotedPrintableEncode
* Description    : quotedprintable����
* Input          : pSrc��Ҫ������ַ���
                   nSrcLen��Ҫ�����ַ����ĳ���
                   pDst�������ַ���
* Output         : None
* Return         : None
*****************************************************************************/
#ifdef receive_mail
void QuotedPrintableDecode( UINT8 *pSrc, UINT8 *pDst, UINT16 nSrcLen )
{
	UINT16 i = 0;
	UINT16 nDstLen = 0;

	while( i < nSrcLen ){
  	if( strncmp( pSrc, "=\r\n", 3 ) == 0 ){
    	pSrc += 3;
    	i += 3;
    }
  	else{
    	if( *pSrc == '=' ){
      	sscanf( pSrc, "=%02x",*pDst);
      	pDst++;
      	pSrc += 3;
      	i += 3;
      }
    	else{
        *pDst++ = *pSrc++;
      	i++;
      }
    	nDstLen++;
    }
  }
  *pDst = '\0';
}
#endif	

/*****************************************************************************
* Function Name  : ch395mail_xtochar
* Description    : 16����ת�ַ���
* Input          : dat-Ҫת����ʮ����������
                   p-ת�����Ӧ���ַ���
                   len-Ҫת���ĳ���
* Output         : None
* Return         : None
*****************************************************************************/
#ifdef receive_over_reply
void ch395mail_xtochar( UINT8 data *dat,UINT8 data *p,UINT8 len)
{
	UINT8 k;
	for(k=0;k<len;k++){
    *p = (((dat[k]&0xf0)>>4)/10)?(((dat[k]&0xf0)>>4)+'A'-10):(((dat[k]&0xf0)>>4)+'0');
  	p++;
    *p = ((dat[k]&0x0f)/10)?((dat[k]&0x0f)+'A'-10):((dat[k]&0x0f)+'0');
  	p++;
  	if(k<len-1){
      *p = '.';
    	p++;    
    }
  }
}
#endif 
/******************************************************************************/
#ifdef send_mail
/*****************************************************************************
* Function Name  : ch395mail_IsMIME
* Description    : �ж����޸���
* Input          : None
* Output         : None
* Return         : 0-�޸���
                   1-�и���
*****************************************************************************/
void ch395mail_IsMIME( )
{
	if( strlen(p_smtp->m_strFile) <= 0 ) p_smtp->g_MIME = 0;
	else p_smtp->g_MIME = 1;
}

/*****************************************************************************
* Function Name  : ch395mail_GetAttachHeader
* Description    : �����齨�����ŷ� 
* Input          : pFileName-��������
                   pAttachHeader-�齨�õ��ŷ�����
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_GetAttachHeader(  UINT8 *pFileName, UINT8 *pAttachHeader )
{
	const UINT8 *strContentType = "application/octet-stream";
	sprintf(pAttachHeader, "\r\n\r\n--%s\r\nContent-Type: %s;\r\n name=\"%s\"%sContent-Disposition: \
	attachment;\r\n filename=\"%s\"\r\n\r\n", g_strBoundary, strContentType, pFileName,g_AttachHead, pFileName ); 
}  

/*****************************************************************************
* Function Name  : ch395mail_GetAttachEnd
* Description    : �齨������������ 
* Input          : EndSize,pAttachEnd
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_GetAttachEnd( UINT16 *EndSize, UINT8 *pAttachEnd )
{
	strcat( pAttachEnd, "\r\n--" );
	strcat( pAttachEnd, g_strBoundary );
	strcat( pAttachEnd, "--" );
  *EndSize = strlen(pAttachEnd);
}

/*****************************************************************************
* Function Name  : ch395mail_SendHeader
* Description    : �ʼ��ŷ�  
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_SendHeader(  )
{
	UINT8   s;
	const 	UINT8 *strContentType = "multipart/mixed";
  /* "FROM: "*/
	memset( send_buff, '\0', sizeof(send_buff) );
	strcat( send_buff, "From: \"" );
	strcat( send_buff, p_smtp->m_strSenderName );
	strcat( send_buff, "\" <" );
	strcat( send_buff, p_smtp->m_strSendFrom );
	strcat( send_buff, ">\r\n" );
  /* "TO: " */
	strcat( send_buff, "To: <" );
	strcat( send_buff, p_smtp->m_strSendTo );
	strcat( send_buff, ">\r\n" );
  /* "Subject: " */
	strcat( send_buff, "Subject: ");
#ifdef receive_over_reply
	strcat( send_buff, "Re: ");
#endif
	strcat( send_buff, p_smtp->m_strSubject );
	strcat( send_buff, "\r\n" );
  /*"Date: "*/ 
	strcat( send_buff, "Date: ");
	strcat( send_buff, '\0' );                                                                          /* ʱ��*/
 	strcat( send_buff, "\r\n" );
  /* "X-Mailer: " */
	strcat( send_buff, g_xMailer );
                                                                                                      
	if( p_smtp->g_MIME == 1 ){																			                                    /* �и���*/
  	strcat( send_buff, "MIME-Version: 1.0\r\nContent-Type: " );
  	strcat( send_buff, strContentType );
  	strcat( send_buff, ";\r\n\tboundary=\"" );
  	strcat( send_buff, g_strBoundary );
  	strcat( send_buff, "\"\r\n" );
  }
  /* Encoding information*/
	strcat( send_buff, g_Encoding );
	strcat( send_buff, p_smtp->m_strSenderName );
	strcat( send_buff, " <" );
	strcat( send_buff, p_smtp->m_strSendFrom );
	strcat( send_buff, ">\r\n" );
  /*add custom-tailor*/
	strcat( send_buff, g_Custom );
  /*end of mail header*/
	strcat( send_buff, "\r\n\r\n" );
#if DEBUG
	printf("Mail Header:%s\n", send_buff);
#endif
	s = ch395mail_SendData( send_buff, strlen(send_buff),uncheck ,SendSocket);  
#if DEBUG
	if( s != send_data_success ) printf("ERROR: %02x\n",(UINT16)s);
#endif
}
/*****************************************************************************
* Function Name  : ch395mail_SendAttach
* Description    : ���͸���  
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_SendAttach(  )
{
	UINT16  EndSize;
	UINT8	s;

	memset( send_buff, '\0', sizeof(send_buff) );
	ch395mail_GetAttachHeader( p_smtp->m_strFile, send_buff);
#if DEBUG
	printf("Attach Header:%s\n", send_buff);
#endif
	s = ch395mail_SendData( send_buff, strlen(send_buff),uncheck ,SendSocket);   
#if DEBUG
	if( s != send_data_success ) printf("ERROR: %02x\n",(UINT16)s);
#endif


/*���͸������� */
/*	GetAttachedFileBody( &FileSize, m_smtp->m_strFile, pAttachedFileBody );*/
	memset( send_buff, '\0', sizeof(send_buff) );
	QuotedPrintableEncode( AttachmentData, send_buff, strlen(AttachmentData),200 );
#if DEBUG
	printf("Attach Data send_buff:\n%s\n", send_buff);
#endif
	s = ch395mail_SendData( send_buff, strlen(send_buff),uncheck ,SendSocket);
#if DEBUG
	if( s != send_data_success ) printf("ERROR: %02x\n",(UINT16)s);
#endif
	memset( send_buff, '\0', sizeof(send_buff) );
	ch395mail_GetAttachEnd( &EndSize, send_buff );
#if DEBUG
	printf("Attach End :%s\n", send_buff);
#endif
	s = ch395mail_SendData( send_buff, strlen(send_buff),uncheck ,SendSocket);                          /*Send attached file end*/
#if DEBUG
	if( s != send_data_success ) printf("ERROR: %02x\n",(UINT16)s);
#endif
} 

/*****************************************************************************
* Function Name  : ch395mail_SendAttachHeader
* Description    : ���͸����ŷ�  
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_SendAttachHeader( )
{
	UINT8 s;
	const UINT8 *strContentType = "text/plain";

	s = ch395mail_SendData( g_FormatMail, strlen(g_FormatMail),uncheck,SendSocket );
#if DEBUG
	if( s != send_data_success ) printf("ERROR: %02x\n",(UINT16)s);
#endif
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "\r\n--%s\r\nContent-Type: %s;\r\n\tcharset=\"%s\"%s\r\n", g_strBoundary, strContentType, g_strUINT8set,g_AttachHead );
#if DEBUG
	printf("MIME Header:\n%s\n", send_buff);
#endif
	s = ch395mail_SendData( send_buff, strlen(send_buff),uncheck,SendSocket);
#if DEBUG
	if( s != send_data_success ) printf("ERROR: %02x\n",(UINT16)s);
#endif
}

/*****************************************************************************
* Function Name  : ch395mail_smtpEhlo
* Description    : ���뷢���ʼ�״̬  
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_smtpEhlo(  )
{
	memset( EncodeName, '\0', sizeof(EncodeName) );
	QuotedPrintableEncode( (UINT8 *)p_smtp->m_strSenderName, EncodeName, strlen(p_smtp->m_strSenderName),76 );
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "%s %s\r\n", SMTP_CLIENT_CMD[0],EncodeName );
#if DEBUG
	printf("EHLO :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff),SMTP_CHECK_HELO ,SendSocket);
}

/*****************************************************************************
* Function Name  : ch395mail_smtpLoginAuth
* Description    : ���뷢���ʼ�״̬  
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_smtpLoginAuth(  )
{
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "%s\r\n",SMTP_CLIENT_CMD[1]);
#if DEBUG
	printf("AUTH :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff),SMTP_CHECK_AUTH ,SendSocket);                      /*send "AUTH LOGIN" command*/
}

/*****************************************************************************
* Function Name  : ch395mail_smtpLoginUser
* Description    : ��֤�û���   
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_smtpLoginUser()
{
	memset( send_buff, '\0', sizeof(send_buff) );
	Base64Encode( (UINT8 *)p_smtp->m_strUSERID, strlen(p_smtp->m_strUSERID), send_buff );
	sprintf( send_buff, "%s\r\n", send_buff);
#if DEBUG
	printf("USER :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff),SMTP_CHECK_USER,SendSocket );
}

/*****************************************************************************
* Function Name  : ch395mail_smtpLoginPass
* Description    : ��½����    
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_smtpLoginPass( )
{
	memset( send_buff, '\0', sizeof(send_buff) );
	Base64Encode( (UINT8 *)p_smtp->m_strPASSWD, strlen(p_smtp->m_strPASSWD), send_buff);
	sprintf( send_buff, "%s\r\n", send_buff);
#if DEBUG
	printf("PASS :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff),SMTP_CHECK_PASS,SendSocket );
}

/*****************************************************************************
* Function Name  : ch395mail_smtpCommandMail
* Description    : ���ͷ���������  
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_smtpCommandMail( )
{
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "%s <%s>\r\n", SMTP_CLIENT_CMD[2],p_smtp->m_strSendFrom );
#if DEBUG
	printf("MAIL :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff),SMTP_CHECK_MAIL,SendSocket );
}

/*****************************************************************************
* Function Name  : ch395mail_smtpCommandRcpt
* Description    : �ռ��˵�ַ 
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_smtpCommandRcpt( )
{
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "%s <%s>\r\n", SMTP_CLIENT_CMD[3],p_smtp->m_strSendTo );  
#if DEBUG
	printf("RCPT :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff),SMTP_CHECK_RCPT,SendSocket );
}

/*****************************************************************************
* Function Name  : ch395mail_smtpCommandData
* Description    : ����data���� 
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_smtpCommandData(  )
{
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "%s\r\n",SMTP_CLIENT_CMD[4] );
#if DEBUG
	printf("DATA :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff),SMTP_CHECK_DATA,SendSocket );
}

/*****************************************************************************
* Function Name  : ch395mail_smtpSendData
* Description    : �����ʼ�����  
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_smtpSendData( )
{
	UINT8	s;

	ch395mail_IsMIME( );                                                                                /* �ж����޸���*/
	ch395mail_SendHeader(  );                                                                           /* �����ʼ��ŷ�*/
	if( p_smtp->g_MIME ==  1 ){
    ch395mail_SendAttachHeader(  );                                                                     /* ���͸����ŷ�*/
  }
	else {
  	ch395mail_SendData("\r\n", strlen("\r\n"),uncheck,SendSocket);
#if DEBUG
  	if( s != send_data_success ) printf("ERROR: %02x\n",(UINT16)s);
#endif
  }
	memset( send_buff, '\0', sizeof(send_buff) );
#ifdef	receive_over_reply
	ch395mail_replybody( );
#else   /* receive_over_reply */
 	QuotedPrintableEncode( (UINT8 *)MailBodyData, send_buff, strlen(MailBodyData),76 );
#endif	/* receive_over_reply */
#if DEBUG
	printf("text data %s\n",send_buff);
#endif
	s = ch395mail_SendData( send_buff, strlen(send_buff),uncheck ,SendSocket);
#if DEBUG
	if( s != send_data_success ) printf("ERROR: %02x\n",(UINT16)s);
#endif
	if( 1 == p_smtp->g_MIME ) ch395mail_SendAttach( );                                                   /*Send Attached file*/
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "\r\n.\r\n" );
#if DEBUG
	printf("OVER :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff),SMTP_CHECK_DATA_END ,SendSocket);                   /*Send end flag of Mail*/

}

/******************************************************************************/
#endif	
/******************************************************************************/
#ifdef receive_mail
/*****************************************************************************
* Function Name  : ch395mail_pop3LoginUser
* Description    : ��֤�û���  
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_pop3LoginUser(  )              
{
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf(send_buff, "%s %s\r\n",POP3_CLIENT_CMD[1], p_pop3->pPop3UserName);
#if DEBUG
	printf("USER :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff), POP_CHECK_USER ,ReceSocket); 
}

/*****************************************************************************
* Function Name  : ch395mail_pop3LoginPass
* Description    : ����  
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_pop3LoginPass(  )              
{
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "%s %s\r\n", POP3_CLIENT_CMD[2], p_pop3->pPop3PassWd );
#if DEBUG
	printf("PASS :%s\n", send_buff);
#endif
	ch395mail_SendData(  send_buff, strlen(send_buff), POP_CHECK_PASS,ReceSocket );  
}

/*****************************************************************************
* Function Name  : ch395mail_pop3Stat
* Description    : ��������ͳ������  
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_pop3Stat( )
{
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff,"%s\r\n", POP3_CLIENT_CMD[3] );
#if DEBUG
	printf("STAT :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff), POP_CHECK_STAT,ReceSocket );
}

/*****************************************************************************
* Function Name  : ch395mail_pop3List
* Description    : ����server����ָ���ʼ��Ĵ�С 
* Input          : None
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_pop3List( )
{
#if	0	    /* �����ָ��ĳ���ʼ�����1��*/
	UINT8 num;
	num = '1';  /* ������Ҫ�޸��ʼ��ţ�*/
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "%s %c\r\n", POP3_CLIENT_CMD[4],num );
#else
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "%s\r\n", POP3_CLIENT_CMD[4] );
#endif	
#if DEBUG
	printf("LIST :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff), POP_CHECK_LIST,ReceSocket);  
}

/*****************************************************************************
* Function Name  : ch395mail_pop3Retr
* Description    : ����server�ʼ���ȫ���ı� 
* Input          : num-�ʼ���
* Output         : None
* Return         : None
*****************************************************************************/
#ifdef	POP_RTER 
void ch395mail_pop3Retr( UINT8 num )
{
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "%s %c\r\n", POP3_CLIENT_CMD[5], num );
#if DEBUG
	printf("RTER :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff), POP_CHECK_RETR,ReceSocket );
}
#endif	
 
/*****************************************************************************
* Function Name  : ch395mail_pop3Dele
* Description    : ����server���ɾ�� 
* Input          : num-�ʼ���
* Output         : None
* Return         : None
*****************************************************************************/
#ifdef	POP_DELE
void ch395mail_pop3Dele( UINT8 num )
{
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "%s %c\r\n", POP3_CLIENT_CMD[6], num );
#if DEBUG
	printf("DELE :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff), POP_CHECK_DELE,ReceSocket );
}
#endif	
 
/*****************************************************************************
* Function Name  : ch395mail_pop3Dele
* Description    : ����server����ɾ��
* Input          : num-�ʼ���
* Output         : None
* Return         : None
*****************************************************************************/
#ifdef	POP_RSET
void ch395mail_pop3Rset( )
{
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "%s \r\n", POP3_CLIENT_CMD[7]);
#if DEBUG
	printf("RSET :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff), POP_CHECK_RSET,ReceSocket );
}
#endif	
 
/*****************************************************************************
* Function Name  : ch395mail_pop3Top
* Description    : ����n���ʼ���ǰm������
* Input          : num-�ʼ���
                   m-����
* Output         : None
* Return         : None
*****************************************************************************/
#ifdef	POP_TOP
void ch395mail_pop3Top( UINT8 num ,UINT8 m  )
{
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "%s %c %c\r\n", POP3_CLIENT_CMD[10],num,m);
#if DEBUG
	printf("TOP :%s\n", send_buff);
#endif
	ch395mail_SendData( send_buff, strlen(send_buff), POP_CHECK_TOP,ReceSocket );
}
#endif	

/*****************************************************************************
* Function Name  : ch395mail_pop3Top
* Description    : ����server�������ڸ�ָ���ʼ���Ψһ��ʶ
* Input          : num-�ʼ���
* Output         : None
* Return         : None
*****************************************************************************/
#ifdef	POP_UIDL
void ch395mail_pop3Uidl( UINT8 num )
{
     memset( send_buff, '\0', sizeof(send_buff) );
     sprintf( send_buff, "%s %c\r\n", POP3_CLIENT_CMD[11], num);
#if DEBUG
     printf("UIDL :%s\n", send_buff);
#endif
     ch395mail_SendData( send_buff, sizeof(send_buff), POP_CHECK_UIDL,ReceSocket );
}
#endif	

/******************************************************************************/
#endif	

/*****************************************************************************
* Function Name  : Quit
* Description    : �˳���½
* Input          : index
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_Quit(UINT8 index )
{
	memset( send_buff, '\0', sizeof(send_buff) );
	sprintf( send_buff, "%s\r\n", POP3_CLIENT_CMD[0]);
	if(index==SendSocket){
#if  DEBUG
	printf("smtp QUIT\n");
#endif
  	ch395mail_SendData( send_buff, strlen(send_buff),SMTP_CHECK_QUIT,index );
  }
	if(index==ReceSocket){
#if  DEBUG
	printf("pop3 QUIT\n");
#endif
  	ch395mail_SendData( send_buff, strlen(send_buff),POP_CHECK_QUIT,index );
  }

}

/*****************************************************************************
* Function Name  : ch395mail_MailCommand
* Description    : �ж���������Ӧ�ӳ���
* Input          : choiceorder-��������
* Output         : None
* Return         : None
*****************************************************************************/
void ch395mail_MailCommand( UINT8 choiceorder )
{
	UINT8 i;
  
	switch( choiceorder ){
#ifdef send_mail
  	case SMTP_SEND_HELO: 
    	ch395mail_smtpEhlo( );
    	break;
  	case SMTP_SEND_AUTH: 
    	ch395mail_smtpLoginAuth( );
    	break;
  	case SMTP_SEND_USER: 
    	ch395mail_smtpLoginUser( );
    	break;
  	case SMTP_SEND_PASS: 
    	ch395mail_smtpLoginPass( );
    	break;
  	case SMTP_SEND_MAIL: 
    	ch395mail_smtpCommandMail( );
    	break;
  	case SMTP_SEND_RCPT: 
    	ch395mail_smtpCommandRcpt( );
    	break;
  	case SMTP_SEND_DATA: 
    	ch395mail_smtpCommandData( );
    	break;
  	case SMTP_DATA_OVER:
    	ch395mail_smtpSendData( );
    	break;
  	case SMTP_ERR_CHECK:
    	ch395mail_Quit( SendSocket );
    	i = CH395CloseSocketp( SendSocket );
    	break;
  	case SMTP_SEND_QUIT:
    	ch395mail_Quit( SendSocket );
    	break;
  	case SMTP_CLOSE_SOCKET:
    	CheckType = uncheck;
#if DEBUG
    	printf("clost smtp socket\n");
#endif
    	i = CH395CloseSocketp( SendSocket );
#ifdef	receive_over_reply
    	OrderType = POP_RECEIVE_DELE;
#endif
    	break;
  	case SMTP_SEND_START:
    	CH395SocketInitOpen( SendSocket );
    	break;
#endif	
#ifdef receive_mail
  	case POP_RECEIVE_USER: 
    	ch395mail_pop3LoginUser(  );
    	break;
  	case POP_RECEIVE_PASS: 
    	ch395mail_pop3LoginPass(  );
    	break;
  	case POP_RECEIVE_STAT: 
    	ch395mail_pop3Stat(  );
    	break;
  	case POP_RECEIVE_LIST: 
    	ch395mail_pop3List(  );
    	break;
  	case POP_ERR_CHECK:
    	ch395mail_Quit( ReceSocket );
    	i = CH395CloseSocketp( ReceSocket );
    	break;
  	case POP_RECEIVE_QUIT:
    	ch395mail_Quit( ReceSocket );
    	break;
  	case POP_CLOSE_SOCKET:
    	CheckType = uncheck;
#if DEBUG
    	printf("close pop3 socket\n");
#endif
    	i = CH395CloseSocketp( ReceSocket );
#ifdef	send_over_receive
    	OrderType = SMTP_SEND_QUIT;
#endif
    	break;
  	case POP_RECEIVE_START:
    	CH395SocketInitOpen( ReceSocket );
    	break;
#endif	
  	default: 
#if DEBUG
    	printf("COMMAND UNUSEFULL\n");
#endif
    	break;
  }
}