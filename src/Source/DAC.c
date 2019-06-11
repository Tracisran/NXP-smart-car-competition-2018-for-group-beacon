/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：DAC.C
 * 功能             ：
 * 备注             ：官方例程上修改
 * 日期             ：2016-09-01
 * 实验平台         ：龙丘 K60VG核心板 
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/

#include "include.h" 

DAC_MemMapPtr DACN[2] = {DAC0_BASE_PTR, DAC1_BASE_PTR}; //定义两个指针数组保存 DACN 的地址

//-------------------------------------------------------------------------*
//函数名: DAC_init                                                        *
//功  能: 初始化ADC                                                       * 
//参  数: DAC_n:模块名DAC0或DAC1                                          *
//返  回: 无                                                              *
//简  例: DAC_init（DAC_1）初始化ADC0模块                                   *
//-------------------------------------------------------------------------*
void DAC_Init(DACn_e dacn)
{
    /* 使能时钟 */
    SIM_SCGC2 |= (SIM_SCGC2_DAC0_MASK << dacn) ;    //使能DAC模块

    /*  配置DAC寄存器  */

    //配置DAC_C0寄存器
    DAC_C0_REG(DACN[dacn])  = ( 0
                                |  DAC_C0_DACTRGSEL_MASK                //选择软件触发
                                |  DAC_C0_DACRFS_MASK                   //选择参考VDD电压(3.3V)
                                |  DAC_C0_DACEN_MASK                    //使能DAC模块
                              );

    //配置DAC_C1寄存器
    DAC_C1_REG(DACN[dacn]) = ( 0
                             ) ;
    //配置DAC_C2寄存器
    DAC_C2_REG(DACN[dacn]) = ( 0
                               | DAC_C2_DACBFRP(0)             //设置缓冲区读指针指向0
                             );

    DAC_DATH_REG(DACN[dacn], 0) = 0;   //默认输出最低电压
    DAC_DATL_REG(DACN[dacn], 0) = 0;
}
//-------------------------------------------------------------------------*
//函数名: DAC_OUT                                                       *
//功  能: 输出波形                                                      * 
//参  数:                                         *
//返  回: 无                                                              *
//简  例:  DAC_Out(DAC_1, result);                                   *
//-------------------------------------------------------------------------*
void DAC_Out(DACn_e dacn, uint16 val)
{
    ASSERT(val < 0x1000);               //val 为 12bit

    DAC_DATH_REG(DACN[dacn], 0) = (val >> 8);   //输出电压
    DAC_DATL_REG(DACN[dacn], 0) = (val & 0xFF);
}
