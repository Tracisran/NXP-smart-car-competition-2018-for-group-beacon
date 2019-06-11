/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：GPIO.H
 * 功能             ：设置GPIO口工作模式
 * 备注             ：官方例程上修改
 * 日期             ：2016-09-01
 * 实验平台         ：龙丘 K60VG核心板 
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/

#ifndef __GPIO_H__
#define __GPIO_H__


typedef enum PORTx
{
    PTA,
    PTB,
    PTC,
    PTD,
    PTE
} PORTx;

//定义管脚方向
typedef enum GPIO_CFG
{
    //这里的值不能改！！！
    GPI         = 0,                          //定义管脚输入方向      GPIOx_PDDRn里，0表示输入，1表示输出
    GPO         = 1,                          //定义管脚输出方向

    GPI_DOWN    = 0x02,                       //输入下拉              PORTx_PCRn需要PE=1，PS=0
    GPI_UP      = 0x03,                       //输入上拉              PORTx_PCRn需要PE=1，PS=1
    GPI_PF      = 0x10,                       //输入，带无源滤波器,滤波范围：10 MHz ~ 30 MHz 。不支持高速接口（>=2MHz）  0b10000           Passive Filter Enable
    GPI_DOWN_PF = GPI_DOWN | GPI_PF ,         //输入下拉，带无源滤波器
    GPI_UP_PF   = GPI_UP   | GPI_PF ,         //输入上拉，带无源滤波器

    GPO_HDS     = 0x41,                        //输出高驱动能力   0b100 0001    High drive strength
    GPO_SSR     = 0x05,                        //输出慢变化率          0b101     Slow slew rate
    GPO_HDS_SSR = GPO_HDS | GPO_SSR,           //输出高驱动能力、慢变化率
} GPIO_CFG;  //最低位为0，肯定是输入；GPI_UP 和 GPI_UP_PF的最低位为1，其他为输出






typedef enum exti_cfg
{
    rising_DMA        = 0x01,     //上升沿触发DMA中断
    falling_DMA       = 0x02,     //下降沿触发DMA中断
    either_down_DMA   = 0x03,     //跳变沿触发DMA中断
    
    //用最高位标志上拉和下拉
    zero          = 0x08,     //低电平触发
    rising        = 0x09,     //上升沿触发
    falling       = 0x0A,     //下降沿触发
    either        = 0x0B,     //跳变沿触发
    one           = 0x0C,      //高电平触发
    
    zero_down     = 0x08u,     //低电平触发，内部下拉
    rising_down   = 0x09u,     //上升沿触发，内部下拉
    falling_down  = 0x0Au,     //下降沿触发，内部下拉
    either_down   = 0x0Bu,     //跳变沿触发，内部下拉
    one_down      = 0x0Cu,     //高电平触发，内部下拉

    //用最高位标志上拉和下拉
    zero_up       = 0x88u,     //低电平触发，内部上拉
    rising_up     = 0x89u,     //上升沿触发，内部上拉
    falling_up    = 0x8Au,     //下降沿触发，内部上拉
    either_up     = 0x8Bu,     //跳变沿触发，内部上拉
    one_up        = 0x8Cu      //高电平触发，内部上拉
} exti_cfg;
/***********************链接内部地址*************************/
#define PORTA    PTA_BASE_PTR
#define PORTB    PTB_BASE_PTR
#define PORTC    PTC_BASE_PTR
#define PORTD    PTD_BASE_PTR
#define PORTE    PTE_BASE_PTR


#define GPIOX_BASE(PTxn)    GPIOX[PTX(PTxn)]       //GPIO模块的地址
#define PTX(PTxn)           ((PTxn)>>5)
#define PTn(PTxn)           ((PTxn)&0x1f)
#define PORTX_BASE(PTxn)     PORTX[PTX(PTxn)]       //PORT模块的地址
/*********************** GPIO功能函数 **************************/
void GPIO_Init (GPIO_MemMapPtr port, int index, GPIO_CFG dir,int data);
void GPIO_Ctrl (GPIO_MemMapPtr port, int index, int data);
void GPIO_Reverse (GPIO_MemMapPtr port, int index);
u8 GPIO_Get(PTXn_e ptxn);
void EXTI_Init(PORTx portx, u8 n, exti_cfg cfg);
void PORTA_Interrupt();
void PORTB_Interrupt();
void PORTC_Interrupt();
void PORTD_Interrupt();
void PORTE_Interrupt();
#endif 
