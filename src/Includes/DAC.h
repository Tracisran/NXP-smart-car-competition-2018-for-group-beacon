/******************** LQ_K60_������ v1.0 ********************
 * �ļ���           ��DAC.H
 * ����             ������ADC����ģʽ
 * ��ע             ���ٷ��������޸�
 * ����             ��2016-09-01
 * ʵ��ƽ̨         ������ K60VG���İ� 
 * ����             �������� 006
 * �Ա���           ��https://longqiu.taobao.com
 * �������ܳ�����Ⱥ ��202949437
*************************************************************/

#ifndef     _LQ_DAC_H_
#define     _LQ_DAC_H_ 

typedef enum DACn       //DACģ��
{
    DAC_0,
    DAC_1
} DACn_e;

extern void DAC_Init(DACn_e);               //DACһ��ת����ʼ��
extern void DAC_Out(DACn_e, uint16 val);    //DACһ��ת������

#endif  //_MK60_DAC_H_

