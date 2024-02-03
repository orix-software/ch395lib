/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395SPI_SW.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬ ģ��SPI�������ӵ�Ӳ������� V1.0
*                      
*******************************************************************************/

/* Ӳ����غ궨�� */
/* �����е�Ӳ�����ӷ�ʽ����(ʵ��Ӧ�õ�·���Բ����޸��������弰�ӳ���) */
/* ��Ƭ��������               CH395оƬ������
        P1.4                       SCS
        P1.5                       SDI
        P1.6                       SDO
        P1.7                       SCK                                */
sbit    P14          =             P1^4;
sbit    P15          =             P1^5;
sbit    P16          =             P1^6;
sbit    P17          =             P1^7;
#define CH395_SPI_SCS              P14                               /* CH395��SCS���� */
#define CH395_SPI_SDI              P15                               /* CH395��SDI���� */
#define CH395_SPI_SDO              P16                               /* CH395��SDO���� */
#define CH395_SPI_SCK              P17                               /* CH395��SCK���� */

#define CH395_INT_WIRE             INT1                              /* CH395��INT#���� */

#define xCH395CmdStart( )         {CH395_SPI_SCS = 0;}
#define xEndCH395Cmd()            {CH395_SPI_SCS = 1;}


/********************************************************************************
* Function Name  : mDelayuS
* Description    : ��ʱָ��΢��ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
* Input          : us---��ʱʱ��ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void mDelayuS(UINT8 us)
{
    while(us --);                                                    /* MCS51@24MHz */
}

/*******************************************************************************
* Function Name  : mDelaymS
* Description    : ��ʱָ������ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
* Input          : ms---��ʱʱ��ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void mDelaymS(UINT8 ms)
{
    while(ms --) 
    {
        mDelayuS(250);
        mDelayuS(250);
        mDelayuS(250);
        mDelayuS(250);
    }
}

/*******************************************************************************
* Function Name  : CH395_PORT_INIT
* Description    : Ӳ����ʼ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_PORT_INIT(void)
{
    CH395_SPI_SCS = 1;
    CH395_SPI_SCK = 1;                                               /*  SIP_CLKΪ�ߵ�ƽ��������ʹ��SPIģʽ3��Ҳ����ʹ��ģʽ0*/
}

/*******************************************************************************
* Function Name  : Spi395OutByte
* Description    : ��CH395����8λ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Spi395OutByte(UINT8 mdata)                                      /* SPI���8λ���� */
{                                                                    /* �����Ӳ��SPI����ֻ��Ҫ������д��SPI�����ݼĴ����� */
    UINT8   i;                                                       /* Ȼ��ȴ�������ɼ��� */

    for ( i = 0; i < 8; i ++ ) 
    {
        CH395_SPI_SCK = 0;                                           /* CLK���� */
        if( mdata & 0x80 )CH395_SPI_SDI = 1; 
        else CH395_SPI_SDI = 0;
        mdata <<= 1;                                                 /* ����λ�Ǹ�λ��ǰ */
        CH395_SPI_SCK = 1;                                           /* CH395��ʱ�������ز������� */
    }
}

/*******************************************************************************
* Function Name  : Spi395InByte
* Description    : ��CH395��8λ����
* Input          : None
* Output         : None
* Return         : 8λ����
*******************************************************************************/
UINT8 Spi395InByte(void)                                             /* SPI����8λ���� */
{                                                                    /* �����Ӳ��SPI�ӿ�,Ӧ�����Ȳ�ѯSPI״̬�Ĵ����Եȴ�SPI�ֽ�*/
    UINT8   i, d;                                                    /* �������,Ȼ���SPI���ݼĴ����������� */
    d = 0;

    for ( i = 0; i < 8; i ++ )  
    {
        CH395_SPI_SCK = 0;                                           /* CH395��ʱ���½������ */
        d <<= 1;  /* ����λ�Ǹ�λ��ǰ */
        if ( CH395_SPI_SDO) d ++;
        CH395_SPI_SCK = 1;
    }
    return( d );
}

/*******************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : ��CH395����8λ������
* Input          : 8λ������
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Cmd(UINT8 cmd)                                       /* ��CH395д���� */
{
    xEndCH395Cmd();                                                  /* ��ֹCSԭ��Ϊ�ͣ��Ƚ�CD�ø� */
    xCH395CmdStart( );                                               /* ���ʼ������CS���� */
    Spi395OutByte(cmd);                                              /* Ӳ��SPI����8λ������ */
    mDelayuS(2);                                                     /* ��Ҫ��ʱ,��ʱ1.5uSȷ����д���ڲ�С��1.5uS */
}

/********************************************************************************
* Function Name  : xWriteCH395Data
* Description    : ��CH395����8λ����
* Input          : 8λ����
* Output         : None
* Return         : None
*******************************************************************************/
void  xWriteCH395Data(UINT8 mdata)                                  /* ��CH395д���� */
{   
    Spi395OutByte(mdata);                                                      
}

/********************************************************************************
* Function Name  : xReadCH395Data
* Description    : ��CH395��������
* Input          : None 
* Output         : None
* Return         : 8λ����
*******************************************************************************/
UINT8   xReadCH395Data( void )                                       /* ��CH395������ */
{
    UINT8 i;
    i = Spi395InByte( );
    return i;
}

/**************************** endfile *************************************/