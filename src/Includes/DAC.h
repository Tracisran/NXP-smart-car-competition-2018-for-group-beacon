/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：DAC.H
 * 功能             ：设置ADC工作模式
 * 备注             ：官方例程上修改
 * 日期             ：2016-09-01
 * 实验平台         ：龙丘 K60VG核心板 
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/

#ifndef     _LQ_DAC_H_
#define     _LQ_DAC_H_ 

typedef enum DACn       //DAC模块
{
    DAC_0,
    DAC_1
} DACn_e;

extern void DAC_Init(DACn_e);               //DAC一次转换初始化
extern void DAC_Out(DACn_e, uint16 val);    //DAC一次转换操作

#endif  //_MK60_DAC_H_

