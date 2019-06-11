/******************** LQ_K60_������ v1.0 ********************
 * �ļ���           ��FTM.H
 * ����             ������FTM����ģʽ
 * ��ע             ���ٷ��������޸�
 * ����             ��2016-09-01
 * ʵ��ƽ̨         ������ K60VG���İ� 
 * ����             �������� 006
 * �Ա���           ��https://longqiu.taobao.com
 * �������ܳ�����Ⱥ ��202949437
*************************************************************/

#ifndef _FTM_H_
#define _FTM_H_

/**********************************  FTM(���Ÿ���) ***************************************/

//      ģ��ͨ��    �˿�          ��ѡ��Χ              ����
#define FTM0_CH0    PTC1        //PTC1��PTA3            PTA3��Ҫ�ã���JLINK��ͻ��
#define FTM0_CH1    PTC2        //PTC2��PTA4
#define FTM0_CH2    PTC3        //PTC3��PTA5
#define FTM0_CH3    PTC4        //PTC4��PTA6
#define FTM0_CH4    PTA7        //PTD4��PTA7
#define FTM0_CH5    PTD5        //PTD5��PTA0            PTA0��Ҫ�ã���JLINK��ͻ��
#define FTM0_CH6    PTD6        //PTD6��PTA1            PTA1��Ҫ�ã���JLINK��ͻ��
#define FTM0_CH7    PTD7        //PTD7��PTA2            PTA2��Ҫ�ã���JLINK��ͻ��

//      ģ��ͨ��    �˿�          ��ѡ��Χ              ����
#define FTM1_CH0    PTA12       //PTA8��PTA12��PTB0
#define FTM1_CH1    PTA13       //PTA9��PTA13��PTB1

//      ģ��ͨ��    �˿�          ��ѡ��Χ              ����
#define FTM2_CH0    PTA10       //PTA10��PTB18
#define FTM2_CH1    PTB19       //PTA11��PTB19

//��������ģ��ͨ��  �˿�          ��ѡ��Χ              ����
#define FTM1_QDPHA  PTA12       //PTA8��PTA12��PTB0
#define FTM1_QDPHB  PTA13       //PTA9��PTA13��PTB1

#define FTM2_QDPHA  PTB18       //PTA10��PTB18
#define FTM2_QDPHB  PTA11      //PTA11��PTB19

/**********************************  FTM(���Ÿ���) ***************************************/


//����FTMģ���
typedef enum
{
    FTM0,
    FTM1,
    FTM2,

    FTM_MAX,
} FTMn_e;

//����FTM ͨ����
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
    FTM_Rising,               //�����ز�׽
    FTM_Falling,              //�½��ز�׽
    FTM_Rising_or_Falling     //�����ز�׽
} FTM_Input_cfg;

#define FTM_IRQ_EN(FTMn,CHn)        FTM_CnSC_REG(FTMN[FTMn],CHn) |= FTM_CnSC_CHIE_MASK       //���� FTMn_CHn �ж�
#define FTM_IRQ_DIS(FTMn,CHn)       FTM_CnSC_REG(FTMN[FTMn],CHn) &= ~FTM_CnSC_CHIE_MASK      //�ر� FTMn_CHn �ж�
/*********************** PWM���ܺ��� **************************/
void FTM_PWM_Init(FTMn_e ftmn, FTM_CHn_e ch, u16 mod, u16 cv);
void FTM_PWM_Duty(FTMn_e ftmn, FTM_CHn_e ch, u16 duty);

/*********************** ���벶׽���жϹ��ܺ��� **************************/
void FTM_Input_init(FTMn_e ftmn, FTM_CHn_e ch, FTM_Input_cfg cfg);
void FTM0_interrupt(void);
void FTM1_interrupt(void);
void FTM2_interrupt(void);
/*********************** �������빦�ܺ��� **************************/
void     FTM_AB_Init(FTMn_e ftmn);         //��ʼ��FTM ���������� ����
extern int16  FTM_AB_Get(FTMn_e ftmn);          //��ȡFTM �������� ��������(������ʾ������)


#endif  



