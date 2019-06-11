/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：PIT.C
 * 功能             ：设置PIT工作模式
 * 备注             ：官方例程上修改
 * 日期             ：2016-09-01
 * 实验平台         ：龙丘 K60VG核心板 
 * 开发环境         ：IAR 7.3
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/

#include "include.h"
#include "PIT.h"

//-------------------------------------------------------------------------*
//函数名: pit_init                                                        
//功  能: 初始化PIT                                                       
//参  数: pitn:模块名PIT0或PIT1或PIT2或PIT3
//        cnt 中断时间，单位1ms
//返  回: 无                                                              
//简  例: pit_init(PIT0,1000); PIT0中断，1000ms，即1s进入PIT0_interrupt()一次                                  
//-------------------------------------------------------------------------*
void PIT_Init(PITn pitn, u32 cnt)
{
    //PIT 用的是 Bus Clock 总线频率

    /* 开启时钟*/
    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;                            //使能PIT时钟

    /* PIT模块控制 PIT Module Control Register (PIT_MCR) */
    PIT_MCR         &= ~(PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );      //使能PIT定时器时钟 ，调试模式下继续运行

    /* 定时器加载值设置 Timer Load Value Register (PIT_LDVALn) */
    PIT_LDVAL(pitn)  = cnt*bus_clk*1000;                                          //设置溢出中断时间

    //定时时间到了后，TIF 置 1 。写1的时候就会清0
    PIT_Flag_Clear(pitn);                                             //清中断标志位

    /* 定时器控制寄存器 Timer Control Register (PIT_TCTRL0) */
    PIT_TCTRL(pitn) |= ( PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK );   //使能 PITn定时器,并开PITn中断

    enable_irq(pitn + 68);			                                //开接收引脚的IRQ中断
}

//-------------------------------------------------------------------------*
//函数名: PIT0_interrupt                                                        
//功  能: PIT中断函数                                                       
//参  数: 无
//返  回: 无                                                              
//简  例: 由初始化决定，多长时间进入一次                                  
//-------------------------------------------------------------------------*

void PIT0_Interrupt()
{
  PIT_Flag_Clear(PIT0);       //清中断标志位
 /*用户添加所需代码*/  
}

void PIT1_Interrupt()
{
  PIT_Flag_Clear(PIT1);       //清中断标志位
 /*用户添加所需代码*/
}

void PIT2_Interrupt()
{
  PIT_Flag_Clear(PIT2);       //清中断标志位
  /*用户添加所需代码*/
}

void PIT3_Interrupt()
{
  PIT_Flag_Clear(PIT3);       //清中断标志位
  /*用户添加所需代码*/
}