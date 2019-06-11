/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：FTM.H
 * 功能             ：设置FTM工作模式
 * 备注             ：官方例程上修改
 * 日期             ：2016-09-01
 * 实验平台         ：龙丘 K60VG核心板 
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/

#ifndef _FTM_H_
#define _FTM_H_

/**********************************  FTM(引脚复用) ***************************************/

//      模块通道    端口          可选范围              建议
#define FTM0_CH0    PTC1        //PTC1、PTA3            PTA3不要用（与JLINK冲突）
#define FTM0_CH1    PTC2        //PTC2、PTA4
#define FTM0_CH2    PTC3        //PTC3、PTA5
#define FTM0_CH3    PTC4        //PTC4、PTA6
#define FTM0_CH4    PTA7        //PTD4、PTA7
#define FTM0_CH5    PTD5        //PTD5、PTA0            PTA0不要用（与JLINK冲突）
#define FTM0_CH6    PTD6        //PTD6、PTA1            PTA1不要用（与JLINK冲突）
#define FTM0_CH7    PTD7        //PTD7、PTA2            PTA2不要用（与JLINK冲突）

//      模块通道    端口          可选范围              建议
#define FTM1_CH0    PTA12       //PTA8、PTA12、PTB0
#define FTM1_CH1    PTA13       //PTA9、PTA13、PTB1

//      模块通道    端口          可选范围              建议
#define FTM2_CH0    PTA10       //PTA10、PTB18
#define FTM2_CH1    PTB19       //PTA11、PTB19

//正交解码模块通道  端口          可选范围              建议
#define FTM1_QDPHA  PTA12       //PTA8、PTA12、PTB0
#define FTM1_QDPHB  PTA13       //PTA9、PTA13、PTB1

#define FTM2_QDPHA  PTB18       //PTA10、PTB18
#define FTM2_QDPHB  PTA11      //PTA11、PTB19

/**********************************  FTM(引脚复用) ***************************************/


//定义FTM模块号
typedef enum
{
    FTM0,
    FTM1,
    FTM2,

    FTM_MAX,
} FTMn_e;

//定义FTM 通道号
typedef enum
{

    FTM_CH0,
    FTM_CH1,
    FTM_CH2,
    FTM_CH3,
    FTM_CH4,
    FTM_CH5,
    FTM_CH6,
    FTM_CH7,

} FTM_CHn_e;

typedef enum
{
    FTM_Rising,               //上升沿捕捉
    FTM_Falling,              //下降沿捕捉
    FTM_Rising_or_Falling     //跳变沿捕捉
} FTM_Input_cfg;

#define FTM_IRQ_EN(FTMn,CHn)        FTM_CnSC_REG(FTMN[FTMn],CHn) |= FTM_CnSC_CHIE_MASK       //开启 FTMn_CHn 中断
#define FTM_IRQ_DIS(FTMn,CHn)       FTM_CnSC_REG(FTMN[FTMn],CHn) &= ~FTM_CnSC_CHIE_MASK      //关闭 FTMn_CHn 中断
/*********************** PWM功能函数 **************************/
void FTM_PWM_Init(FTMn_e ftmn, FTM_CHn_e ch, u16 mod, u16 cv);
void FTM_PWM_Duty(FTMn_e ftmn, FTM_CHn_e ch, u16 duty);

/*********************** 输入捕捉及中断功能函数 **************************/
void FTM_Input_init(FTMn_e ftmn, FTM_CHn_e ch, FTM_Input_cfg cfg);
void FTM0_interrupt(void);
void FTM1_interrupt(void);
void FTM2_interrupt(void);
/*********************** 正交解码功能函数 **************************/
void     FTM_AB_Init(FTMn_e ftmn);         //初始化FTM 的正交解码 功能
extern int16  FTM_AB_Get(FTMn_e ftmn);          //获取FTM 正交解码 的脉冲数(负数表示反方向)


#endif  



