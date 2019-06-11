/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：GPIO.c
 * 功能             ：设置GPIO口工作模式
 * 备注             ：官方例程上修改
 * 日期             ：2016-09-01
 * 实验平台         ：龙丘 K60VG核心板 
 * 开发环境         ：IAR 7.3
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/
#include "include.h"
#include "GPIO.h"


GPIO_MemMapPtr GPIOX[5] = {PTA_BASE_PTR, PTB_BASE_PTR, PTC_BASE_PTR, PTD_BASE_PTR, PTE_BASE_PTR}; //定义五个指针数组保存 GPIOX 的地址
PORT_MemMapPtr PORTX[5] = {PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR};
//-------------------------------------------------------------------------*
//函数名: gpio_init                                                        *
//功  能: 初始化gpio                                                       * 
//参  数: port:端口名                                                      *
//        index:指定端口引脚                                               *
//        dir:引脚方向,0=输入,1=输出                                       * 
//        data:初始状态,0=低电平,1=高电平                                  *
//返  回: 无                                                              *
//简  例: gpio_init (PORTA, 17, 1,0);  PTA17引脚初始化为低电平             *
//-------------------------------------------------------------------------*
void GPIO_Init (GPIO_MemMapPtr port, int index, GPIO_CFG dir,int data)
{
  
     SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                 | SIM_SCGC5_PORTB_MASK
                 | SIM_SCGC5_PORTC_MASK
                 | SIM_SCGC5_PORTD_MASK
                 | SIM_SCGC5_PORTE_MASK );
     
     PORT_MemMapPtr p;
     switch((u32)port)
     {
     case 0x400FF000u:
         p = PORTA_BASE_PTR;
         break;
     case 0x400FF040u:
         p = PORTB_BASE_PTR;
         break;
     case 0x400FF080u:
         p = PORTC_BASE_PTR;
         break;
     case 0x400FF0C0u:
         p = PORTD_BASE_PTR;
         break;
     case 0x400FF100u:
         p = PORTE_BASE_PTR;
         break;
     default:
         break;
     }
     PORT_PCR_REG(p,index)=(0|PORT_PCR_MUX(1));

     if(dir == 1)//output
     {
    	 GPIO_PDDR_REG(port) |= (1<<index);
    	 if(data == 1)//output
			  GPIO_PDOR_REG(port) |= (1<<index);
		 else
			  GPIO_PDOR_REG(port) &= ~(1<<index);
     }
         
     else
         GPIO_PDDR_REG(port) &= ~(1<<index);

}

//-------------------------------------------------------------------------* 
//函数名: gpio_ctrl                                                        *
//功  能: 设置引脚状态                                                     *
//参  数: port:端口名                                                      *
//        index:指定端口引脚                                               *
//        data: 状态,0=低电平,1=高电平                                     *
//返  回: 无                                                               *
//简  例: gpio_ctrl (PORTA, 17,0);  PTA17引脚初设置为低电平                 *
//-------------------------------------------------------------------------*
void GPIO_Ctrl (GPIO_MemMapPtr port, int index, int data)
{
    if(data == 1)//output
         GPIO_PDOR_REG(port) |= (1<<index);
    else
         GPIO_PDOR_REG(port) &= ~(1<<index);
}

//-----------------------------------------------------------------------* 
//函数名: gpio_reverse                                                   *
//功  能: 改变引脚状态                                                   *
//参  数: port:端口名;                                                   *
//        index:指定端口引脚                                             *
//返  回: 无                                                             *
//简  例: gpio_reverse (PORTA, 17);  PTA17引脚输出电平反转                *
//-----------------------------------------------------------------------*
void GPIO_Reverse (GPIO_MemMapPtr port, int index)
{
    GPIO_PDOR_REG(port) ^= (1<<index);
}

u8 GPIO_Get(PTXn_e ptxn)
{

    return ((GPIO_PDIR_REG(GPIOX_BASE(ptxn)) >> PTn(ptxn )) & 0x01);        // 获取 GPIO PDIR ptxn 状态，即读取管脚输入电平
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************************************
                                                    gpio外部中断函数
**************************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------* 
//函数名: Exti_init                                                   
//功  能: 外部GPIO中断初始化                                                   
//参  数: portx:端口名（PTA,PTB,PTC,PTD,PTE）                                                  
//          n  :指定端口引脚号（0~31）
//     exti_cfg:中断触发方式  如： rising_down //上升沿触发，内部下拉
//返  回: 无                                                             
//简  例: exti_init(PTC, 16, rising_down);            
//-----------------------------------------------------------------------*
void EXTI_Init(PORTx portx, u8 n, exti_cfg cfg)
{
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << portx);    //开启PORTx端口

    PORT_PCR_REG(PORTX[portx], n) = PORT_PCR_MUX(1) | PORT_PCR_IRQC(cfg & 0x7f ) | PORT_PCR_PE_MASK | ((cfg & 0x80 ) >> 7); // 复用GPIO , 确定触发模式 ,开启上拉或下拉电阻
    GPIO_PDDR_REG(GPIOX[portx]) &= ~(1 << n);       //输入模式
    enable_irq(portx + 87);                         //使能PORT中断，PORTA的ISR中断号为87
}


/******************************************************************
                      gpio外部中断函数
                      内容自行添加
*****************************************************************/
void PORTA_Interrupt()
{
  int n;
  n=0;
  if((PORTA_ISFR & (1<<n)))
  {
      PORTA_ISFR |= (1<<n); 
      /* 用户自行添加中断内程序 */
  } 
  n=1;
  if((PORTA_ISFR & (1<<n)))
  {
      PORTA_ISFR |= (1<<n); 
      /* 用户自行添加中断内程序 */
  } 
}

void PORTB_Interrupt()
{
  int n;
  n=20;
  if((PORTB_ISFR & (1<<n)))
  {
      PORTB_ISFR |= (1<<n);
      // 用户自行添加中断内程序 
      LED_Ctrl(LED0,ON);          //按下一次触发一次外部中断,对应LED亮一次
  }
  n=21;
  if((PORTB_ISFR & (1<<n)))
  {
      PORTB_ISFR |= (1<<n);
      // 用户自行添加中断内程序 
      LED_Ctrl(LED1,ON);          //按下一次触发一次外部中断,对应LED亮一次
  }
  n=22;
  if((PORTB_ISFR & (1<<n)))
  {
      PORTB_ISFR |= (1<<n);
      // 用户自行添加中断内程序 
      LED_Ctrl(LED2,ON);          //按下一次触发一次外部中断,对应LED亮一次
  }
}
/*
void PORTC_Interrupt()
{
  int n;
  n=0;
  if((PORTC_ISFR & (1<<n)))
  {
      PORTC_ISFR |= (1<<n); 
      //用户自行添加中断内程序 
  } 
  n=1;
  if((PORTC_ISFR & (1<<n)))
  {
      PORTC_ISFR |= (1<<n); 
      //用户自行添加中断内程序 
  } 
}
*/
void PORTD_Interrupt()
{
  int n;
  n=0;
  if((PORTD_ISFR & (1<<n)))
  {
      PORTD_ISFR |= (1<<n); 
      /* 用户自行添加中断内程序 */
  } 
  n=1;
  if((PORTD_ISFR & (1<<n)))
  {
      PORTD_ISFR |= (1<<n); 
      /* 用户自行添加中断内程序 */
  } 
}

void PORTE_Interrupt()
{
  int n;
  n=0;
  if((PORTE_ISFR & (1<<n)))
  {
      PORTE_ISFR |= (1<<n); 
      /* 用户自行添加中断内程序 */
  } 
  n=1;
  if((PORTE_ISFR & (1<<n)))
  {
      PORTE_ISFR |= (1<<n); 
      /* 用户自行添加中断内程序 */
  } 
}



