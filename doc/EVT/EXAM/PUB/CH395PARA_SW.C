/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395PARA_SW.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395芯片 模拟并口连接的硬件抽象层 V1.0
*                    
*******************************************************************************/

/* 硬件相关宏定义 */
/* 本例中的硬件连接方式如下(实际应用电路可以参照修改下述定义及子程序) */
/* 单片机的引脚    CH395芯片的引脚
      P2.0                 A0
      P2.6                 CS#    如果模拟出的并口上只有CH395,那么CS#可以直接接低电平,强制片选
      P3.6                 WR#
      P3.7                 RD#
      P0(8位端口)         D7-D0 */	/*51单片机注意P0口需要加上拉*/

sbit    CH395_A0        =          P2^0;
sbit    CH395_CS        =          P2^6;
sbit    CH395_WR        =          P3^6;
sbit    CH395_RD        =          P3^7;
#define CH395_DATA_DAT_OUT( d )    { P0 = d; }                   /* 向并口输出数据 */
#define CH395_DATA_DAT_IN( )       ( P0 )                        /* 从并口输入数据 */
#define CH395_DATA_DIR_OUT( )                                    /* 设置并口方向为输出 */
#define CH395_DATA_DIR_IN( )       { P0 = 0xFF; }                /* 设置并口方向为输入 */

#define CH395_INT_WIRE             INT1                          /* 假定CH395的INT#引脚,如果未连接那么也可以通过查询状态端口实现 */

/*******************************************************************************
* Function Name  : mDelayuS
* Description    : 延时指定微秒时间,根据单片机主频调整,不精确
* Input          : us---延时时间值
* Output         : None
* Return         : None
*******************************************************************************/
void mDelayuS( UINT8 us )
{
    while( us -- );                                              /* MCS51@24MHz */
}

/*******************************************************************************
* Function Name  : mDelaymS
* Description    : 延时指定毫秒时间,根据单片机主频调整,不精确
* Input          : ms---延时时间值
* Output         : None
* Return         : None
*******************************************************************************/
void mDelaymS( UINT8 ms )
{
    while( ms -- ) 
    {
        mDelayuS( 250 );
        mDelayuS( 250 );
        mDelayuS( 250 );
        mDelayuS( 250 );
    }
}

/********************************************************************************
* Function Name  : CH395_PORT_INIT
* Description    : 硬件初始化部分
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_PORT_INIT(void)
{
    CH395_WR = 1;                                                    /* WR默认为高 */
    CH395_RD = 1;                                                    /* RD默认为高 */
    CH395_A0 = 0;                                                    /* A0默认为低 */
    CH395_CS = 1;                                                    /* CS默认为高 */
    CH395_DATA_DIR_IN();                                             /* 端口设置为输入 */
}

/********************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : 向CH395写命令
* Input          : cmd 8位命令码
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Cmd(UINT8 cmd)                                       /* 向CH395写命令 */
{
    CH395_DATA_DAT_OUT(cmd);                                         /* 将8位命令码写到数据端口 */
    CH395_DATA_DIR_OUT();                                            /* 设置输出 */
    CH395_A0 = 1;                                                    /* 将A0置为高 */
    CH395_WR = 0;                                                    /* 将WR拉低 */
    CH395_CS = 0;                                                    /* 将CS拉低 */
    CH395_CS = 0;                                                    /* 无效操作仅作延时使用 */
    CH395_CS = 0;                                                    /* 无效操作仅作延时使用 */
    CH395_WR = 1;                                                    /* 将WR置为高 */
    CH395_CS = 1;                                                    /* 将CS置为高 */
    CH395_A0 = 0;                                                    /* 将A0拉低 */
    CH395_DATA_DIR_IN();                                             /* 数据端口设置为输入 */
    mDelayuS(2);                                                     /* 必要延时,延时1.5uS确保读写周期不小于1.5uS */
}

/********************************************************************************
* Function Name  : xWriteCH395Data
* Description    : 向CH395写8位数据
* Input          : mdata 8位数据
* Output         : None
* Return         : None
*******************************************************************************/
void  xWriteCH395Data(UINT8 mdata)
{                                                                    /* 向CH395写数据 */
    CH395_DATA_DAT_OUT(mdata);                                       /* 将8位命令码写到数据端口 */
    CH395_DATA_DIR_OUT();                                            /* 设置输出 */
    CH395_WR = 0;                                                    /* 将WR拉低 */
    CH395_CS = 0;                                                    /* 将CS拉低 */
    CH395_CS = 0;                                                    /* 无效操作仅作延时使用,必须保证读写脉冲宽度大于40ns */
    CH395_WR = 1;                                                    /* 将WR置为高 */
    CH395_CS = 1;                                                    /* 将CS置为高 */
    CH395_DATA_DIR_IN();                                             /* 数据端口设置为输入 */
   // mDelayuS(1);                                                   /* 确保读写周期大于0.6uS */
}

/********************************************************************************
* Function Name  : CH395_PORT_INIT
* Description    : 硬件初始化部分
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8   xReadCH395Data( void )                                       /* 从CH395读数据 */
{
    UINT8 i;
    
    CH395_DATA_DIR_IN();                                             /* 数据端口设置为输入 */
    CH395_RD = 0;                                                    /* 将RD拉低 */
    CH395_CS = 0;                                                    /* 将CS拉低 */
    CH395_CS = 0;                                                    /* 无效操作仅作延时使用 */
    CH395_CS = 0;                                                    /* 无效操作仅作延时使用,必须保证读写脉冲宽度大于40ns */
    i = CH395_DATA_DAT_IN();                                         /* 从端口读数据 */
    CH395_RD = 1;                                                    /* 将RD置为高 */
    CH395_CS = 1;                                                    /* 将CS置为高 */
    CH395_DATA_DIR_IN();                                             /* 数据端口设置为输入 */
    return i;
}
#define xEndCH395Cmd()     {}                                        /* 命令结束，仅在SPI模式下有效 */

/**************************** endfile *************************************/
