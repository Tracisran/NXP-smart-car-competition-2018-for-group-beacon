/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：LQKEY.c
 * 功能             ：初始化按键
 * 备注             ：官方例程上修改
 * 日期             ：2016-09-01
 * 实验平台         ：龙丘 K60VG核心板 
 * 开发环境         ：IAR 7.3
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/
#ifndef __KEY_H__
#define __KEY_H__

//定义模块号
typedef enum
{
    KEY0=0,
    KEY1=1,
    KEY2=2,   
} KEYn_e;
typedef enum
{
    LOW=0,  //亮
    HIGH=1, //灭   
    FAIL=0xff,
}KEYs_e;


/*********************** UART功能函数 **************************/
//初始化
extern void KEY_Init(void);
extern u8 KEY_Read(KEYn_e keyno);
/********************************************************************/

#endif 
