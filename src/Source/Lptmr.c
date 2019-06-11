/******************** LQ_K60_������ v1.0 ********************
 * �ļ���           ��Lptmr.c
 * ����             ���͹��ļ���������ʱ����
 * ��ע             ���ٷ��������޸�
 * ����             ��2016-09-01
 * ʵ��ƽ̨         ������ K60VG���İ� 
 * ����             �������� 006
 * �Ա���           ��https://longqiu.taobao.com
 * �������ܳ�����Ⱥ ��202949437
*************************************************************/

#include "include.h"
#include "Lptmr.h"

void time_delay_ms(u16 ms)
{
  LPTMR_delay_ms(ms);
}

void time_delay_us(u16 us)
{
  LPTMR_delay_us(us);
}

//-------------------------------------------------------------------------*
//������: LPTMR_pulse_init                                                        
//��  ��: LPTMR���������ʼ��                                                        
//��  ��: LPT0_ALTn:LPTMR��������ܽ�
//        count   :LPTMR����Ƚ�ֵ
//        LPT_CFG :LPTMR���������ʽ�������ؼ������½��ؼ���
//��  ��: ��                                                              
//��  ��:  LPTMR_pulse_init(LPT0_ALT1,32768,LPT_Rising);                                
//-------------------------------------------------------------------------*
void LPTMR_Pulse_Init(LPT0_ALTn altn, uint16 count, LPT_CFG cfg)
{



    // ����ģ��ʱ��
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;                        //ʹ��LPTģ��ʱ��

    //��������ܽ�
    if(altn == LPT0_ALT1)
    {
         PORTA_PCR19= PORT_PCR_MUX(6);
    }
    else if(altn == LPT0_ALT2)
    {
         PORTC_PCR5= PORT_PCR_MUX(4);
    }
    else                                    //�����ܷ����¼�
    {
       ;  
    }

    // ��״̬�Ĵ���
    LPTMR0_CSR = 0x00;                                          //�ȹ���LPT��������������ʱ�ӷ�Ƶ,��ռ���ֵ��


    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      //| LPTMR_PSR_PRESCALE(1)           //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );


    // �����ۼӼ���ֵ
    LPTMR0_CMR  =   LPTMR_CMR_COMPARE(count);                   //���ñȽ�ֵ

    // �ܽ����á�ʹ���ж�
    LPTMR0_CSR  =  (0
                    | LPTMR_CSR_TPS(altn)       // ѡ������ܽ� ѡ��
                    | LPTMR_CSR_TMS_MASK        // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    | ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    | LPTMR_CSR_TIE_MASK        //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );
}


//-------------------------------------------------------------------------*
//������: LPTMR_pulse_get                                                        
//��  ��: ��ȡLPTMR�������ֵ                                                        
//��  ��: �������ֵ                                                              
//��  ��: ��                                
//-------------------------------------------------------------------------*
uint16 LPTMR_Pulse_Get(void)
{
    uint16 data;
    if(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)     //�Ѿ������
    {

        data = ~0;                          //���� 0xffffffff ��ʾ����
    }
    else
    {

        data = LPTMR0_CNR;
    }
    return data;
}


//-------------------------------------------------------------------------*
//������: LPTMR_pulse_clean                                                        
//��  ��: ���LPTMR�������                                                        
//��  ��: ��
//��  ��: ��                                                              
//��  ��: ��                                
//-------------------------------------------------------------------------*
void LPTMR_Pulse_Clean(void)
{
    LPTMR0_CSR  &= ~LPTMR_CSR_TEN_MASK;     //����LPT��ʱ��ͻ��Զ����������ֵ
    LPTMR0_CSR  |= LPTMR_CSR_TEN_MASK;
}

//-------------------------------------------------------------------------*
//������: LPTMR_delay_ms                                                        
//��  ��: LPTMR��ʱ������ms��                                                        
//��  ��: ms    ����
//��  ��: ��                                                              
//��  ��: LPTMR_delay_ms(32);     // LPTMR ��ʱ32ms                                
//-------------------------------------------------------------------------*
void LPTMR_delay_ms(uint16 ms)
{
    if(ms == 0)
    {
        return;
    }

    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;    //ʹ��LPTģ��ʱ��

    LPTMR0_CSR = 0x00;                      //�ȹ���LPT���Զ����������ֵ

    LPTMR0_CMR = ms;                        //���ñȽ�ֵ������ʱʱ��

    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      //| LPTMR_PSR_PRESCALE(1)           //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

    while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)); //�ȴ��Ƚ�ֵ�����ֵ��ȣ���ʱ�䵽��

    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;          //����Ƚϱ�־λ

    return;
}

//-------------------------------------------------------------------------*
//������: LPTMR_timing_ms                                                        
//��  ��: LPTMR��ʱ������ms��                                                        
//��  ��: ms          LPTMR��ʱʱ��(0~65535)
//��  ��: ��                                                              
//��  ��: LPTMR_timing_ms(32);     // LPTMR ��ʱ32ms                                
//-------------------------------------------------------------------------*
void LPTMR_timing_ms(uint16 ms)
{
    if(ms == 0)
    {
        return;
    }

    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;    //ʹ��LPTģ��ʱ��

    LPTMR0_CSR = 0x00;                      //�ȹ���LPT���Զ����������ֵ

    LPTMR0_CMR = ms;                        //���ñȽ�ֵ������ʱʱ��

    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      //| LPTMR_PSR_PRESCALE(1)           //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    | LPTMR_CSR_TIE_MASK        //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

   enable_irq(85);                           // ʹ��LPTMR�ж�
}


//-------------------------------------------------------------------------*
//������: LPTMR_delay_us                                                        
//��  ��: LPTMR��ʱ������us��                                                        
//��  ��: us          LPTMR��ʱʱ��(0~41942)
//��  ��: ��                                                              
//��  ��:  LPTMR_delay_us(32);     // LPTMR ��ʱ32us                              
//-------------------------------------------------------------------------*
void LPTMR_delay_us(uint16 us)
{


    if(us == 0)
    {
        return;
    }

    OSC_CR |= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK


    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;                        //ʹ��LPTģ��ʱ��

    LPTMR0_CSR = 0x00;                                          //�ȹ���LPT���Զ����������ֵ

    LPTMR0_CMR = (us * 50 + 16) / 32;                 //���ñȽ�ֵ������ʱʱ��

    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(3)          //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      | LPTMR_PSR_PRESCALE(4)     //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

    while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)); //�ȴ��Ƚ�ֵ�����ֵ��ȣ���ʱ�䵽��

    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;          //����Ƚϱ�־λ

    
}

//-------------------------------------------------------------------------*
//������: LPTMR_timing_us                                                        
//��  ��: LPTMR��ʱ������us��                                                        
//��  ��: us        LPTMR��ʱʱ��(0~41942)
//��  ��: ��                                                              
//��  ��: LPTMR_timing_us(32);     // LPTMR ��ʱ32us                           
//-------------------------------------------------------------------------*
void LPTMR_timing_us(uint16 us)
{

    if(us == 0)
    {
        return;
    }


    OSC_CR |= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK


    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;                        //ʹ��LPTģ��ʱ��

    LPTMR0_CSR = 0x00;                                          //�ȹ���LPT���Զ����������ֵ

    LPTMR0_CMR = (us * 50 + 16) / 32;                 //���ñȽ�ֵ������ʱʱ��

    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(3)          //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      | LPTMR_PSR_PRESCALE(4)     //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    | LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

    enable_irq(85);                           // ʹ��LPTMR�ж�
}

//-------------------------------------------------------------------------*
//������: LPTMR��ʱ������ms,��ʱʱ�����Ϊ65534ms��                                                      
//��  ��: LPTMR��ʱ����                                                        
//��  ��: ��
//��  ��: ��                                                              
//��  ��:          
/*                 void my_delay(uint32 time)
                    {
                        volatile uint32 i = time;
                        while(i--);

                    }


                    LPTMR_time_start_ms();

                    my_delay(600000);
                    i = LPTMR_time_get_ms();
                    if(i == ~0)
                    {
                        UART_Put_Str("\n��ʱʱ�䳬ʱ");
                    }
                    else
                    {
                        UART_Put_Str("\n��ʱʱ��Ϊ��%dms",i);
                    }
 */
                        
//-------------------------------------------------------------------------*
void LPTMR_time_start_ms(void)
{
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;    //ʹ��LPTģ��ʱ��

    LPTMR0_CSR = 0x00;                      //�ȹ���LPT���Զ����������ֵ

    LPTMR0_CMR = ~0;                        //���ñȽ�ֵ������ʱʱ��

    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      //| LPTMR_PSR_PRESCALE(1)           //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );
    
}


//-------------------------------------------------------------------------*
//������: LPTMR_time_get_ms                                                        
//��  ��: ��ȡLPTMR��ʱʱ�䣨ms��                                                        
//��  ��: ��
//��  ��: ��ʱʱ�䣨����ֵΪ ~0 ��ʾ��ʱ��ʱ������ֵ�� 0~ 65534ms ������ ��                                                              
//��  ��: �ο� LPTMR_time_start_ms �ĵ�������                          
//-------------------------------------------------------------------------*

uint32 LPTMR_time_get_ms(void)
{
    uint32 data;

    if(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)     //�Ѿ������
    {

        data = ~0;                          //���� 0xffffffff ��ʾ����
    }
    else
    {
        data = LPTMR0_CNR;                  //���ؼ�ʱʱ��(��ֵ���Ϊ 0xffff)
    }

    return data;
}

/*!
 *  @brief      �ر� LPTMR��ʱ
 *  @since      v5.0
 */

//-------------------------------------------------------------------------*
//������: LPTMR_time_close                                                        
//��  ��: �ر� LPTMR��ʱ                                                        
//��  ��: ��
//��  ��: ��                                                              
//��  ��: ��                        
//-------------------------------------------------------------------------*
void LPTMR_time_close()
{
    LPTMR0_CSR = 0x00;                                          //�ȹ���LPT���Զ����������ֵ�����������
}



//-------------------------------------------------------------------------*
//������: LPTMR��ʱ������us,��ʱʱ�����Ϊ41942us��                                                      
//��  ��: LPTMR��ʱ����                                                        
//��  ��: ��
//��  ��: ��                                                              
//��  ��:          
/*                 void my_delay(uint32 time)
                    {
                        volatile uint32 i = time;
                        while(i--);

                    }

                    uint32 i;
                    LPTMR_time_start_us();

                    my_delay(5894);

                    i = LPTMR_time_get_us();
                    if(i == ~0)
                    {
                        UART_Put_Str("\n��ʱʱ�䳬ʱ");
                    }
                    else
                    {
                        UART_Put_Str("\n��ʱʱ��Ϊ��%dus",i);
                    }
 */
                        
//-------------------------------------------------------------------------*
void LPTMR_time_start_us(void)
{


    OSC_CR |= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK


    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;                        //ʹ��LPTģ��ʱ��

    LPTMR0_CSR = 0x00;                                          //�ȹ���LPT���Զ����������ֵ

    LPTMR0_CMR = ~0;                                            //���ñȽ�ֵΪ���ֵ

    //ѡ��ʱ��Դ
    LPTMR0_PSR  =   ( 0
                      | LPTMR_PSR_PCS(3)          //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      | LPTMR_PSR_PRESCALE(4)     //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    LPTMR0_CSR  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

   
}


//-------------------------------------------------------------------------*
//������: LPTMR_time_get_us                                                        
//��  ��: ��ȡLPTMR��ʱʱ�䣨us��                                                        
//��  ��: ��
//��  ��: ��ʱʱ�䣨����ֵΪ ~0 ��ʾ��ʱ��ʱ������ֵ�� 0~ 41942us ������ ��                                                              
//��  ��: �ο� LPTMR_time_start_us �ĵ�������                          
//-------------------------------------------------------------------------*
uint32 LPTMR_time_get_us(void)
{
    uint32 data;

    if(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)     //�Ѿ������
    {

        data = ~0;                          //���� 0xffffffff ��ʾ����
    }
    else
    {

        data = (LPTMR0_CNR * 32) / 50; //���е�λ����
    }

    return data;
}



//-------------------------------------------------------------------------*
//������: LPTMR_interrupt                                                        
//��  ��: LPTMR��ʱ�жϺ���                                                        
//��  ��: ��
//��  ��: ��                                                              
//��  ��: ��                               
//-------------------------------------------------------------------------*
void LPTMR_Interrupt(void)
{
    LPTMR_Flag_Clear();

    //�������û����ʵ�ִ���
}


