/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395UART.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395 uart�ӿں���
*             
**********************************************************************************/

/*���ڲ�����Ĭ��Ϊ9600*/
/*********************************************************************************
* Function Name  : mDelayuS
* Description    : ��ʱָ��΢��ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
* Input          : us---��ʱʱ��ֵ
* Output         : None
* Return         : None
*********************************************************************************/
void mDelayuS( UINT8 us )
{
	int i;
	
	while( us -- ) 
	{
		for( i = 0; i != 20; i++ );
	}
}

/**********************************************************************************
* Function Name  : mDelaymS
* Description    : ��ʱָ������ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
* Input          : ms---��ʱʱ��ֵ
* Output         : None
* Return         : None
*********************************************************************************/
void mDelaymS( UINT8 ms )
{
	int i;
	
	while( ms -- ) 
	{
		for( i = FREQ_SYS / 5000; i != 0; -- i );

	}
}

/**********************************************************************************
* Function Name  : UART1_SendByte
* Description    : ����1���ֽڷ�������
* Input          : dat
* Output         : None
* Return         : None
*********************************************************************************/
void UART1_SendByte( UINT8 dat )   
{		
		R8_UART1_THR  = dat;
    while( ( R8_UART1_LSR & RB_LSR_TX_ALL_EMP ) == 0 );                                 /* �ȴ����ݷ���*/	   
}

/**********************************************************************************
* Function Name  : UART1_RcvByte
* Description    : ����1���ֽڽ�������
* Input          : None
* Output         : None
* Return         : ���յ�������
*********************************************************************************/
UINT8 UART1_RcvByte(void)    
{
     UINT8 Rcvdat; 
     while( ( R8_UART1_LSR & RB_LSR_DATA_RDY  ) == 0 );                                /* �ȴ�����׼���� */
     Rcvdat = R8_UART1_RBR;                                                            /* �ӽ��ջ���Ĵ����������� */
	   return( Rcvdat );
}
 /**********************************************************************************
* Function Name  : xEndCH395Cmd
* Description    : ����Ƭѡ��Ϊ�ռ��ݳ���
* Input          : none
* Output         : None
* Return         : None
*********************************************************************************/
void xEndCH395Cmd( void )
{ 

}

 /**********************************************************************************
* Function Name  : CH395_PROT_INIT
* Description    : �ӿڳ�ʼ��
* Input          : none
* Output         : None
* Return         : None
*********************************************************************************/
void CH395_PROT_INIT ( )
{
}

 /**********************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : д����
* Input          : ��д������
* Output         : None
* Return         : None
*********************************************************************************/
void xWriteCH395Cmd( UINT8 mCmd )  
{
    UART1_SendByte(0x57);
	  UART1_SendByte(0xAB);
    UART1_SendByte(mCmd);
	  Delay_us(2);											
}
 /**********************************************************************************
* Function Name  : xWriteCH395Data
* Description    : д����
* Input          : ��д������
* Output         : None
* Return         : None
*********************************************************************************/
void xWriteCH395Data( UINT8 mData ) 
{
 	UART1_SendByte(mData); 			
}
 /**********************************************************************************
* Function Name  : xReadCH395Data
* Description    : ������
* Input          : None
* Output         : ����������
* Return         : None
*********************************************************************************/
UINT8 xReadCH395Data( void ) 
{
  UINT8 Rcv;
  Rcv = UART1_RcvByte(); 
  return (Rcv);
}

 /**********************************************************************************
* Function Name  : Query395Interrupt
* Description    : ��ѯ�ж�
* Input          : None
* Output         : None
* Return         : �ж�״̬
*********************************************************************************/
UINT8 Query395Interrupt( void )
{
	return( (R32_PB_PIN & PWM0) ? FALSE : TRUE );  
}