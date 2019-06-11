/******************** LQ_K60_������ v1.0 ********************
 * �ļ���           ��PIT.H
 * ����             ������PIT����ģʽ
 * ��ע             ���ٷ��������޸�
 * ����             ��2016-09-01
 * ʵ��ƽ̨         ������ K60VG���İ� 
 * ����             �������� 006
 * �Ա���           ��https://longqiu.taobao.com
 * �������ܳ�����Ⱥ ��202949437
*************************************************************/

#ifndef _PIT_H_
#define _PIT_H_


#define PIT_Flag_Clear(PITn)   PIT_TFLG(PITn)|=PIT_TFLG_TIF_MASK      //���жϱ�־


//ģ�鶨��
typedef enum PITn
{
    PIT0,
    PIT1,
    PIT2,
    PIT3
} PITn;

/*********************** PIT���ܺ��� **************************/
void PIT_Init(PITn, u32 cnt);                                            //��ʼ��PITn�������ö�ʱʱ��(��λΪbusʱ������)
void PIT0_Interrupt();
void PIT1_Interrupt();
void PIT2_Interrupt();
void PIT3_Interrupt();

#endif  