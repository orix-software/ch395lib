
/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395SPI_HW
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395 Ӳ��spi�ӿں���
*             
**********************************************************************************/

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
		for( i =20; i != 0; -- i );
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
* Function Name  : xEndCH395Cmd
* Description    : ȡ��Ƭѡ
* Input          : none
* Output         : None
* Return         : None
*********************************************************************************/
void xEndCH395Cmd( void )
{ 
R32_PB_OUT |= CAT0 ;
}

 /**********************************************************************************
* Function Name  : xCH395CmdStart
* Description    : Ƭѡ
* Input          : none
* Output         : None
* Return         : None
*********************************************************************************/
void xCH395CmdStart(void )
{
	R32_PB_CLR |= CAT0 ;
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
  R8_SPI0_CTRL_MOD = RB_SPI_MOSI_OE|RB_SPI_SCK_OE;
  R8_SPI0_CLOCK_DIV = 0x0a;         	                                /* 10��Ƶ��100/10=10M */
  R32_PB_DIR |= (MOSI | SCK0 | SCS |CAT0  );                            /* MISO(PB15)Ϊ���룬MOSI(PB14)��SCS(PB13)Ϊ����� */
  R32_PB_PU  |=  CAT0  ; 
  R8_SPI0_CTRL_DMA = 0;				  	                                /* ������DMA��ʽ */
}

 /**********************************************************************************
* Function Name  : SPI0_MASTER_Trans
* Description    : spi��������
* Input          : data������������
* Output         : None
* Return         : None
*********************************************************************************/
void SPI0_MASTER_Trans( UINT8 data )
{
	R32_SPI0_FIFO = data;
	R16_SPI0_TOTAL_CNT = 0x01;
	while( R8_SPI0_FIFO_COUNT != 0 );	                             	/*�ȴ����ݷ������*/
}

 /**********************************************************************************
* Function Name  : SPI0_MASTER_Recv
* Description    : spi��������
* Input          : NOne
* Output         : ���յ�������
* Return         : None
*********************************************************************************/
UINT8 SPI0_MASTER_Recv( void )
{

	R32_SPI0_FIFO = 0xff;
	R16_SPI0_TOTAL_CNT = 0x01;
	while( R8_SPI0_FIFO_COUNT != 0 );		                              /*�ȴ����ݻ���*/
  return 	R8_SPI0_BUFFER;
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
  R32_PB_OUT |= CAT0 ;				 
  R32_PB_CLR |= CAT0 ;								
	SPI0_MASTER_Trans( mCmd );  									
	mDelayuS(1 );  										
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
	SPI0_MASTER_Trans( mData );
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
	return( SPI0_MASTER_Recv( ) );

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