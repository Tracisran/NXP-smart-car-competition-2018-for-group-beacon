/******************** LQ_K60_������ v1.0 ********************
 * �ļ���           ��ADC.c
 * ����             ������ADC����ģʽ
 * ��ע             ���ٷ��������޸�
 * ����             ��2016-09-01
 * ʵ��ƽ̨         ������ K60VG���İ� 
 * ��������         ��IAR 7.3
 * ����             �������� 006
 * �Ա���           ��https://longqiu.taobao.com
 * �����������ܿƼ�������Ⱥ ��202949437
*************************************************************/
#include "include.h"
#include "ADC.h"

//-------------------------------------------------------------------------*
//������: ADC_init                                                        *
//��  ��: ��ʼ��ADC                                                       * 
//��  ��: adc_n:ģ����ADC0��ADC1                                          *
//��  ��: ��                                                              *
//��  ��: ADC_init��ADC0����ʼ��ADC0ģ��                                   *
//-------------------------------------------------------------------------*
void ADC_Init(ADCn_e adc_n)
{      
    if(adc_n==ADC0)
    {
        SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK );        //����ADC0ʱ��
        SIM_SOPT7 &= ~(SIM_SOPT7_ADC0ALTTRGEN_MASK  | SIM_SOPT7_ADC0PRETRGSEL_MASK);
        SIM_SOPT7 |= SIM_SOPT7_ADC0TRGSEL(0);
    }
    else 
    {
        SIM_SCGC3 |= (SIM_SCGC3_ADC1_MASK );
        SIM_SOPT7 &= ~(SIM_SOPT7_ADC1ALTTRGEN_MASK  | SIM_SOPT7_ADC1PRETRGSEL_MASK) ;
        SIM_SOPT7 |= SIM_SOPT7_ADC1TRGSEL(0);
    }
        

 }

//-------------------------------------------------------------------------*
//������: adc_start                                                        *
//��  ��: ����ADCת��                                                      * 
//��  ��: adc_n:ģ����ADC0��ADC1                                           *
//        adc_ch:ͨ�����                                                  *
//        bit:����ѡ��ADC_8bit��ADC_12bit��ADC_10bit��ADC_16bit            * 
//��  ��: ��                                                              *
//��  ��: һ����adc_once�������ã�������ʹ��                                *
//-------------------------------------------------------------------------*
void ADC_Start(ADCn_e adc_n,ADCn_Ch_e adc_ch,ADC_nbit bit)
{
   if(adc_n==ADC0)
   {
     ADC0_CFG1 = (0  | ADC_CFG1_ADIV(2)              //ʱ�ӷ�Ƶѡ��,��Ƶϵ��Ϊ 2^n,2bit
                     | ADC_CFG1_ADLSMP_MASK          //����ʱ�����ã�0Ϊ�̲���ʱ�䣬1 Ϊ������ʱ��
                     | ADC_CFG1_MODE(bit)             //��ȷ��ѡ��
                     | ADC_CFG1_ADICLK(0)            //0Ϊ����ʱ��,1Ϊ����ʱ��/2,2Ϊ����ʱ�ӣ�ALTCLK����3Ϊ �첽ʱ�ӣ�ADACK����
                 );


    ADC0_CFG2  = (0  | ADC_CFG2_ADHSC_MASK          //��������,0Ϊ����ת�����У�1Ϊ����ת������
                     | ADC_CFG2_ADLSTS(0)           //������ʱ��ѡ��ADCKΪ4+n������ѭ��������ѭ����0Ϊ20��1Ϊ12��2Ϊ6��3Ϊ2
                  );

    //д�� SC1A ����ת��
    ADC0_SC1(0) = (0 | ADC_SC1_AIEN_MASK          // ת������ж�,0Ϊ��ֹ��1Ϊʹ��
                     //| ADC_SC1_ADCH( 0x0c )       //ADC0 ͨ��12
                     | ADC_SC1_ADCH( adc_ch )     //ADC0 ͨ��13
                  );
   }
   else 
   {
     ADC1_CFG1 = (0  | ADC_CFG1_ADIV(2)              //ʱ�ӷ�Ƶѡ��,��Ƶϵ��Ϊ 2^n,2bit
                     | ADC_CFG1_ADLSMP_MASK          //����ʱ�����ã�0Ϊ�̲���ʱ�䣬1 Ϊ������ʱ��
                     | ADC_CFG1_MODE(bit)             //��ȷ��ѡ��
                     | ADC_CFG1_ADICLK(0)            //0Ϊ����ʱ��,1Ϊ����ʱ��/2,2Ϊ����ʱ�ӣ�ALTCLK����3Ϊ �첽ʱ�ӣ�ADACK����
                 );


    ADC1_CFG2  = (0  | ADC_CFG2_ADHSC_MASK          //��������,0Ϊ����ת�����У�1Ϊ����ת������
                     | ADC_CFG2_ADLSTS(0)           //������ʱ��ѡ��ADCKΪ4+n������ѭ��������ѭ����0Ϊ20��1Ϊ12��2Ϊ6��3Ϊ2
                  );

    //д�� SC1A ����ת��
    ADC1_SC1(0) = (0 | ADC_SC1_AIEN_MASK          // ת������ж�,0Ϊ��ֹ��1Ϊʹ��
                     //| ADC_SC1_ADCH( 0x0c )       //ADC0 ͨ��12
                     | ADC_SC1_ADCH( adc_ch )     //ADC0 ͨ��13
                  );
   }
}

//-------------------------------------------------------------------------*
//������: adc_once                                                        
//��  ��: ����ADC����ת��                                                   
//��  ��: adc_n:ģ����ADC0��ADC1                                           
//        adc_ch:ͨ�����                                                  
//        bit:����ѡ��ADC_8bit��ADC_12bit��ADC_10bit��ADC_16bit             
//��  ��: result                                                              
//��  ��: adc_once(ADC0,ADC1_SE8,ADC_12bit) ͨ�����Ϊ ADC1_SE8�����ţ�����
//          һ��ADCת����ADC1_SE8��Ӧ������鿴ADC.H�ļ�                    
//-------------------------------------------------------------------------*

u16 ADC_Once(ADCn_e adc_n,ADCn_Ch_e adc_ch,ADC_nbit bit) //�ɼ�ĳ·ģ������ADֵ
{
   
    int result = 0;

    ADC_Start(adc_n,adc_ch,bit);      //����ADCת��
   if(adc_n==ADC0)
   {
      while ((ADC0_SC1(0) & ADC_SC1_COCO_MASK )!= ADC_SC1_COCO_MASK);   //ֻ֧�� Aͨ��
      result = ADC0_R(0);
      ADC0_SC1(0) &= ~ADC_SC1_COCO_MASK;
      return result;
   }
   else 
   {
      while ((ADC1_SC1(0) & ADC_SC1_COCO_MASK )!= ADC_SC1_COCO_MASK);   //ֻ֧�� Aͨ��
      result = ADC1_R(0);
      ADC1_SC1(0) &= ~ADC_SC1_COCO_MASK;
      return result;
   }
}

//-------------------------------------------------------------------------*
//������: adc_mid                                                        
//��  ��: ����ADC3��ת��ȡ�м�ֵ                                                   
//��  ��: adc_n:ģ����ADC0��ADC1                                           
//        adc_ch:ͨ�����                                                  
//        bit:����ѡ��ADC_8bit��ADC_12bit��ADC_10bit��ADC_16bit             
//��  ��: tmp                                                              
//��  ��: adc_mid(ADC0,ADC1_SE8,ADC_12bit) ͨ�����Ϊ ADC1_SE8�����ţ�����
//          3��ADCת����ѡȡ�м�ֵ���ء�                   
//-------------------------------------------------------------------------*
u16 ADC_Mid(ADCn_e adc_n,ADCn_Ch_e adc_ch,ADC_nbit bit) //��ֵ�˲�
{
    u16 i,j,k,tmp;
    //1.ȡ3��A/Dת�����
    i = ADC_Once(adc_n,adc_ch,bit); 
    j = ADC_Once(adc_n,adc_ch,bit); 
    k = ADC_Once(adc_n,adc_ch,bit); 
        //2.ȡ��ֵ
        if (i > j)
        {
          tmp = i; i = j; j = tmp;
         }
         if (k > j) 
           tmp = j;
         else if(k > i) 
           tmp = k; 
         else 
          tmp = i;
         
     return tmp;
}
//-------------------------------------------------------------------------*
//������: adc_ave                                                        
//��  ��: ����ADC���ת����ȡƽ��ֵ                                                   
//��  ��: adc_n:ģ����ADC0��ADC1                                           
//        adc_ch:ͨ�����                                                  
//        bit:����ѡ��ADC_8bit��ADC_12bit��ADC_10bit��ADC_16bit 
//        N  :��ֵ�˲�������1~65535��
//��  ��: tmp                                                              
//��  ��: adc_ave(ADC0,ADC1_SE8,ADC_12bit,100) ͨ�����Ϊ ADC1_SE8�����ţ�����
//          100��ADCת���������ƽ��ֵ���ء�                   
//-------------------------------------------------------------------------*

u16 ADC_Ave(ADCn_e adc_n,ADCn_Ch_e adc_ch,ADC_nbit bit,u16 N) //��ֵ�˲�
{
    u32 tmp = 0;
    u8  i;
    
    for(i = 0; i < N; i++)
       tmp += ADC_Mid(adc_n,adc_ch,bit); 
    
    tmp = tmp / N; 
    
    return (u16)tmp;
}
