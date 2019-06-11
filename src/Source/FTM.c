/******************** LQ_K60_������ v1.0 ********************
 * �ļ���           ��FTM.c
 * ����             ������FTM����ģʽ
 * ��ע             ���ٷ��������޸�
 * ����             ��2016-09-01
 * ʵ��ƽ̨         ������ K60VG���İ� 
 * ��������         ��IAR 7.3
 * ����             �������� 006
 * �Ա���           ��https://longqiu.taobao.com
 * �������ܳ�����Ⱥ ��202949437
*************************************************************/

#include "include.h"
#include "FTM.h"


FTM_MemMapPtr FTMN[3] = {FTM0_BASE_PTR, FTM1_BASE_PTR, FTM2_BASE_PTR}; //��������ָ�����鱣�� FTMn_e �ĵ�ַ


//-------------------------------------------------------------------------*
//������: FTM_PWM_init                                                        
//��  ��: FTM��PWM����                                                        
//��  ��: ftmn:ģ����FTM0,FTM1��FTM2
//        ch  :ͨ���� ��Ӧ���Ųο�ftm.h�ļ�
//        mod :������  ��Χ��0~65535��
//        cv  :�ߵ�ƽ��� ��Χ��0~mod��
//��  ��: ��                                                              
//��  ��: FTM_PWM_init(FTM0,FTM_CH6,65535,32768); ����ռ�ձ���50%                                 
//-------------------------------------------------------------------------*
void FTM_PWM_Init(FTMn_e ftmn, FTM_CHn_e ch, u16 mod, u16 cv)
{
  
  /******************* ����ʱ�� �� ����IO��*******************/
    switch(ftmn)
    {
    case FTM0:
        SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;       //ʹ��FTM0ʱ��
        switch(ch)
        {
        case FTM_CH0:
            if(FTM0_CH0 == PTC1)
            {
              
                PORTC_PCR1= PORT_PCR_MUX(4); 
            }
            else if(FTM0_CH0 == PTA3)
            {
              
                PORTA_PCR3= PORT_PCR_MUX(3); 
            }
            else
            {
                 break;
            }
            break;

        case FTM_CH1:
            if(FTM0_CH1 == PTC2)
            {
              
                PORTC_PCR2= PORT_PCR_MUX(4); 
            }
            else if(FTM0_CH1 == PTA4)
            {
              
                PORTA_PCR4= PORT_PCR_MUX(3); 
            }
            else
            {
                 break;
            }
            break;

        case FTM_CH2:
            if(FTM0_CH2 == PTC3)
            {
              
                PORTC_PCR3= PORT_PCR_MUX(4); 
            }
            else if(FTM0_CH2 == PTA5)
            {
              
                PORTA_PCR5= PORT_PCR_MUX(3); 
            }
            else
            {
                 break;
            }
            break;

        case FTM_CH3:
            if(FTM0_CH3 == PTC4)
            {
             
                PORTC_PCR4= PORT_PCR_MUX(4); 
            }
            else if(FTM0_CH3 == PTA6)
            {
              
                PORTA_PCR6= PORT_PCR_MUX(3); 
            }
            else
            {
                break;
            }
            break;

        case FTM_CH4:
            if(FTM0_CH4 == PTD4)
            {
              
                PORTD_PCR4= PORT_PCR_MUX(4); 
            }
            else if(FTM0_CH4 == PTA7)
            {
             
                PORTA_PCR7= PORT_PCR_MUX(3); 
            }
            else
            {
               break;
            }
            break;

        case FTM_CH5:
            if(FTM0_CH5 == PTD5)
            {
             
                PORTD_PCR5= PORT_PCR_MUX(4); 
            }
            else if(FTM0_CH5 == PTA0)
            {
              
                PORTA_PCR0= PORT_PCR_MUX(3); 
            }
            else
            {
                 break;
            }
            break;

        case FTM_CH6:
            if(FTM0_CH6 == PTD6)
            {
             
                PORTD_PCR6= PORT_PCR_MUX(4); 
            }
            else if(FTM0_CH6 == PTA1)
            {
             
                PORTA_PCR1= PORT_PCR_MUX(3); 
            }
            else
            {
                break;
            }
            break;

        case FTM_CH7:
            if(FTM0_CH7 == PTD7)
            {
              
                PORTD_PCR7= PORT_PCR_MUX(4); 
            }
            else if(FTM0_CH7 == PTA2)
            {
             
                PORTA_PCR2= PORT_PCR_MUX(3); 
            }
            else
            {
               break;
            }
            break;
        default:
            return;
        }
        break;

    case FTM1:
        SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;       //ʹ��FTM1ʱ��
        switch(ch)
        {
        case FTM_CH0:
            if(FTM1_CH0 == PTA8)
            {
               
                PORTA_PCR8= PORT_PCR_MUX(3); 
            }
            else if(FTM1_CH0 == PTA12)
            {
                PORTA_PCR12= PORT_PCR_MUX(3); 
            }
            else if(FTM1_CH0 == PTB0)
            {
                PORTB_PCR0= PORT_PCR_MUX(3); 
            }
            break;


        case FTM_CH1:
            if(FTM1_CH1 == PTA9) 
            {
               
                PORTA_PCR9= PORT_PCR_MUX(3); 
            }
            else if(FTM1_CH1 == PTA13)
            {
                PORTA_PCR13= PORT_PCR_MUX(3); 
            }
              else if(FTM1_CH1 == PTB1)
            {
                PORTB_PCR1= PORT_PCR_MUX(3); 
            }
            break;

        default:
            return;
        }
        break;

    case FTM2:
        SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //ʹ��FTM2ʱ��
        switch(ch)
        {
        case FTM_CH0:
            if(FTM2_CH0 == PTA10)
            {
              
                PORTA_PCR10= PORT_PCR_MUX(3); 
            }
            else if(FTM2_CH0 == PTB18)
            {
                PORTB_PCR18= PORT_PCR_MUX(3); 
            }
            break;

        case FTM_CH1:
            if(FTM2_CH1 == PTA11) 
            {
             
                PORTA_PCR11= PORT_PCR_MUX(3); 
            }
            else if(FTM2_CH1 == PTB19)
            {
                PORTB_PCR19= PORT_PCR_MUX(3); 
            }
            break;

        default:
            return;
        }
        break;
    default:
        break;
    }


    /******************** ѡ�����ģʽΪ ���ض���PWM *******************/
    //ͨ��״̬���ƣ�����ģʽ��ѡ�� ���ػ��ƽ
    FTM_CnSC_REG(FTMN[ftmn], ch) &= ~FTM_CnSC_ELSA_MASK;
    FTM_CnSC_REG(FTMN[ftmn], ch)  = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;

    /******************** ����ʱ�Ӻͷ�Ƶ ********************/
    FTM_SC_REG(FTMN[ftmn])    = ( 0
                                 
                                  | FTM_SC_PS(3)             //��Ƶ  0   Ƶ��Ϊ  1.37khz
                                  | FTM_SC_CLKS(1)            //ʱ��ѡ��busʱ��
                                 
                                );
   // FTM_MODE_REG(FTMN[ftmn])&=~1;
    FTM_MOD_REG(FTMN[ftmn])   = mod;                        //ģ��, EPWM������Ϊ ��MOD - CNTIN + 0x0001
    FTM_CNTIN_REG(FTMN[ftmn]) = 0;                          //��������ʼ��ֵ�����������ȣ�(CnV - CNTIN).
    FTM_CnV_REG(FTMN[ftmn], ch) = cv;
    FTM_CNT_REG(FTMN[ftmn])   = 0;                          //��������ֻ�е�16λ���ã�д�κ�ֵ���˼Ĵ������������ CNTIN ��ֵ��
}



//-------------------------------------------------------------------------*
//������: FTM_PWM_Duty                                                        
//��  ��: �ı�PWM��ռ�ձ�                                                        
//��  ��: ftmn:ģ����FTM0,FTM1��FTM2
//        ch  :ͨ���� ��Ӧ���Ųο�ftm.h�ļ�
//        duty:�ߵ�ƽ���  ��Χ��0~mod��mod�ڳ�ʼ����ʱ���趨
//��  ��: ��                                                              
//��  ��: FTM_PWM_Duty(FTM0, FTM_CH6,duty);duty�趨��0~mod֮����κ���                                
//-------------------------------------------------------------------------*
void FTM_PWM_Duty(FTMn_e ftmn, FTM_CHn_e ch, u16 duty)
{
 
    FTM_CnV_REG(FTMN[ftmn], ch) = duty;

}


//////////////////////////////// ����Ϊ�������� //////////////////////////////////////////



//-------------------------------------------------------------------------*
//������: FTM_AB_Init                                                        
//��  ��: ��ʼ��FTM���������빦��                                                      
//��  ��: ftmn:ģ����FTM1��FTM2
//��  ��: ��                                                              
//��  ��:  FTM_AB_Init(FTM2);                               
//-------------------------------------------------------------------------*
void FTM_AB_Init(FTMn_e ftmn)
{
   

    /******************* ����ʱ�� �� ����IO��*******************/
    switch(ftmn)
    {

    case FTM1:
        SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;       //ʹ��FTM1ʱ��
        if(FTM1_QDPHA == PTA8)                  //�ܽŸ���
        {
          
             PORTA_PCR8= PORT_PCR_MUX(6);
        }
        else if(FTM1_QDPHA == PTA12)
        {
           
             PORTA_PCR12= PORT_PCR_MUX(7);
        }
        else if(FTM1_QDPHA == PTB0)
        {
         
             PORTB_PCR0= PORT_PCR_MUX(6);
        }
        else
        {
             break;
        }

        if(FTM1_QDPHB == PTA9)
        {
         
             PORTA_PCR9= PORT_PCR_MUX(6);
        }
        else if(FTM1_QDPHB == PTA13)
        {
        
             PORTA_PCR13= PORT_PCR_MUX(7);
        }
        else if(FTM1_QDPHB == PTB1)
        {
         
             PORTB_PCR1= PORT_PCR_MUX(6);
        }
        else
        {
            break;
        }
        break;
       

    case FTM2:
        SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //ʹ��FTM2ʱ��
        if(FTM2_QDPHA == PTA10)                  //�ܽŸ���
        {
        
             PORTA_PCR10= PORT_PCR_MUX(6);
        }
        else if(FTM2_QDPHA == PTB18)
        {
         
             PORTB_PCR18= PORT_PCR_MUX(6);
        }
        else
        {
            break;
        }

        if(FTM2_QDPHB == PTA11)                  //�ܽŸ���
        {
          
             PORTA_PCR11= PORT_PCR_MUX(6);
        }
        else if(FTM2_QDPHB == PTB19)
        {
        
             PORTB_PCR19= PORT_PCR_MUX(6);
        }
        else
        {
             break;    
        }break;
    }

    FTM_MODE_REG(FTMN[ftmn])  |=    (0
                                     | FTM_MODE_WPDIS_MASK  //д������ֹ
                                     //| FTM_MODE_FTMEN_MASK   //ʹ�� FTM
                                    );
    FTM_QDCTRL_REG(FTMN[ftmn]) |=   (0
                                    | FTM_QDCTRL_QUADMODE_MASK
                                     );
    FTM_CNTIN_REG(FTMN[ftmn])   = 0;
    FTM_MOD_REG(FTMN[ftmn])     = FTM_MOD_MOD_MASK;
    FTM_QDCTRL_REG(FTMN[ftmn]) |=   (0
                                    | FTM_QDCTRL_QUADEN_MASK
                                     );
    FTM_MODE_REG(FTMN[ftmn])  |= FTM_QDCTRL_QUADEN_MASK;;
    FTM_CNT_REG(FTMN[ftmn])     = 0;                    //��������ֻ�е�16λ���ã�д�κ�ֵ���˼Ĵ������������ CNTIN ��ֵ��
    
}


//-------------------------------------------------------------------------*
//������: FTM_AB_get                                                        
//��  ��: ��ȡFTM���������������                                                      
//��  ��: ftmn:ģ����FTM1��FTM2
//��  ��: val                                                             
//��  ��: speed=FTM_AB_get(FTM2);                              
//-------------------------------------------------------------------------*
int16 FTM_AB_Get(FTMn_e ftmn)
{
    int16 val;
   
    val = FTM_CNT_REG(FTMN[ftmn]);
    
    FTM_CNT_REG(FTMN[ftmn]) = 0;             

    return val;
}



//-------------------------------------------------------------------------*
//������: FTM_Input_init                                                        
//��  ��: FTM�����벶׽����                                                        
//��  ��: ftmn:ģ����FTM0,FTM1��FTM2
//        ch  :ͨ���� ��Ӧ���Ųο�ftm.h�ļ�
//        cfg :������ʽ  :
//                        FTM_Rising,               //�����ز�׽
//                        FTM_Falling,              //�½��ز�׽
//                        FTM_Rising_or_Falling     //�����ز�׽
//��  ��: ��                                                              
//��  ��: FTM_Input_init(FTM0,FTM_CH6,FTM_Rising);                                
//-------------------------------------------------------------------------*
void FTM_Input_init(FTMn_e ftmn, FTM_CHn_e ch, FTM_Input_cfg cfg)
{
   
    /******************* ����ʱ�� �� ����IO��*******************/
    switch(ftmn)
    {
    case FTM0:
        SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;       //ʹ��FTM0ʱ��
        switch(ch)
        {
        case FTM_CH0:
            if(FTM0_CH0 == PTC1)
            {
                PORTC_PCR1= PORT_PCR_MUX(4);
            }
            else if(FTM0_CH0 == PTA3)
            {
                PORTA_PCR3= PORT_PCR_MUX(3);
            }
            else
            {
               ;                      
            }
            break;

        case FTM_CH1:
            if(FTM0_CH1 == PTC2)
            {
                PORTC_PCR2= PORT_PCR_MUX(4);
            }
            else if(FTM0_CH1 == PTA4)
            {
                PORTA_PCR4= PORT_PCR_MUX(3);
            }
            else
            {
                ;                      //���ùܽ�����
            }
            break;

        case FTM_CH2:
            if(FTM0_CH2 == PTC3)
            {
                PORTC_PCR3= PORT_PCR_MUX(4);
            }
            else if(FTM0_CH2 == PTA5)
            {
                PORTA_PCR5= PORT_PCR_MUX(3);
            }
            else
            {
                ;                      //���ùܽ�����
            }
            break;

        case FTM_CH3:
            if(FTM0_CH3 == PTC4)
            {
                PORTC_PCR4= PORT_PCR_MUX(4);
            }
            else if(FTM0_CH3 == PTA6)
            {
                PORTA_PCR6= PORT_PCR_MUX(3);
            }
            else
            {
                ;                      //���ùܽ�����
            }
            break;

        case FTM_CH4:
            if(FTM0_CH4 == PTD4)
            {
                PORTD_PCR4= PORT_PCR_MUX(4);
            }
            else if(FTM0_CH4 == PTA7)
            {
                PORTA_PCR7= PORT_PCR_MUX(3);
            }
            else
            {
               ;                      //���ùܽ�����
            }
            break;

        case FTM_CH5:
            if(FTM0_CH5 == PTD5)
            {
                PORTD_PCR5= PORT_PCR_MUX(4);
            }
            else if(FTM0_CH5 == PTA0)
            {
                PORTA_PCR0= PORT_PCR_MUX(3);
            }
            else
            {
                ;                      //���ùܽ�����
            }
            break;

        case FTM_CH6:
            if(FTM0_CH6 == PTD6)
            {
                PORTD_PCR6= PORT_PCR_MUX(4);
            }
            else if(FTM0_CH6 == PTA1)
            {
                PORTA_PCR1= PORT_PCR_MUX(3);
            }
            else
            {
               ;                      //���ùܽ�����
            }
            break;

        case FTM_CH7:
            if(FTM0_CH7 == PTD7)
            {
                PORTD_PCR7= PORT_PCR_MUX(4);
            }
            else if(FTM0_CH7 == PTA2)
            {
                PORTA_PCR2= PORT_PCR_MUX(3);
            }
            else
            {
               ;                      //���ùܽ�����
            }
            break;
        default:
            return;
        }
        break;

    case FTM1:
        SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;       //ʹ��FTM1ʱ��
        switch(ch)
        {
        case FTM_CH0:
            if(FTM1_CH0 == PTA8)
            {
                PORTA_PCR8= PORT_PCR_MUX(3);
            }
            else if(FTM1_CH0 == PTA12) 
            {
               PORTA_PCR12= PORT_PCR_MUX(3);
            }
            else if(FTM1_CH0 == PTB0)
            {
               PORTB_PCR0= PORT_PCR_MUX(3);
            }
            break;


        case FTM_CH1:
            if(FTM1_CH1 == PTA9 )
            {
                PORTA_PCR9= PORT_PCR_MUX(3);
            }
            else if  (FTM1_CH1 == PTA13) 
            {
                PORTA_PCR13= PORT_PCR_MUX(3);
            }
            else if(FTM1_CH1 == PTB1)
            {           
                PORTB_PCR1= PORT_PCR_MUX(3);
            }
            break;

        default:
            return;
        }
        break;

    case FTM2:
        SIM_SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //ʹ��FTM2ʱ��
        switch(ch)
        {
        case FTM_CH0:
            if(FTM2_CH0 == PTA10)
            {   
                PORTA_PCR10= PORT_PCR_MUX(3);
            }
            else if (FTM2_CH0 == PTB18) 
            {
               PORTB_PCR18= PORT_PCR_MUX(3);        
            }
            break;

        case FTM_CH1:
            if(FTM2_CH1 == PTA11)
            {
                PORTA_PCR11= PORT_PCR_MUX(3);
            }
            else if(FTM2_CH1 == PTB19)
            {
               PORTB_PCR19= PORT_PCR_MUX(3);                    
            }
            break;

        default:
            return;
        }
        break;
    default:
        break;
    }

    /******************* ����Ϊ���벶׽���� *******************/
    switch(cfg)
    {
        //���벶׽ģʽ�£�DECAPEN = 0 �� DECAPEN = 0 ��CPWMS = 0�� MSnB:MSnA = 0

        // ELSnB:ELSnA         1          10          11
        // ����             ������      �½���      ������

    case FTM_Rising:    //�����ش���
        FTM_CnSC_REG(FTMN[ftmn], ch) |=  ( FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK );                   //��1
        FTM_CnSC_REG(FTMN[ftmn], ch) &= ~( FTM_CnSC_ELSB_MASK  | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //��0
        break;

    case FTM_Falling:   //�½��ش���
        FTM_CnSC_REG(FTMN[ftmn], ch) |= (FTM_CnSC_ELSB_MASK  | FTM_CnSC_CHIE_MASK );                    //��1
        FTM_CnSC_REG(FTMN[ftmn], ch) &= ~( FTM_CnSC_ELSA_MASK | FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK); //��0
        break;

    case FTM_Rising_or_Falling: //�����ء��½��ض�����
        FTM_CnSC_REG(FTMN[ftmn], ch) |=  ( FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK  | FTM_CnSC_CHIE_MASK ); //��1
        FTM_CnSC_REG(FTMN[ftmn], ch) &= ~( FTM_CnSC_MSB_MASK  | FTM_CnSC_MSA_MASK); //��0
        break;
    }

    FTM_SC_REG(FTMN[ftmn]) = FTM_SC_CLKS(0x1);       //ѡ�� bus ʱ��

    FTM_MODE_REG(FTMN[ftmn])    |= FTM_MODE_WPDIS_MASK; //��ֹд����
    FTM_COMBINE_REG(FTMN[ftmn]) = 0;
    FTM_MODE_REG(FTMN[ftmn])    &= ~FTM_MODE_FTMEN_MASK;    //ʹ��FTM
    FTM_CNTIN_REG(FTMN[ftmn])   = 0;

    FTM_STATUS_REG(FTMN[ftmn])  = 0x00;               //���жϱ�־λ

    //�������벶׽�ж�
    //enable_irq(62 + ftmn);
}


//-------------------------------------------------------------------------*
//������: FTM_interrupt                                                        
//��  ��: FTM���жϹ���                                                        
//��  ��: ��                                                              
//��  ��: ��                               
//-------------------------------------------------------------------------*
void FTM0_interrupt(void)
{
    uint8 s = FTM0_STATUS;             //��ȡ��׽�ͱȽ�״̬ 
    uint8 CHn;

    FTM0_STATUS = 0x00;             //���жϱ�־λ

    CHn = 0;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_DIS(FTM0, CHn);     //��ֹ���벶׽�ж�
        /*     �û�����       */

        /*********************/
        //FTM_IRQ_EN(FTM0, CHn); //�������벶׽�ж�

    }

    /* ������� n=1 ��ģ�棬����ģ������� */
    CHn = 1;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_EN(FTM0, CHn); //�������벶׽�ж�
        /*     �û�����       */


        /*********************/
        //�����������￪�����벶׽�ж�
        //FTM_IRQ_EN(FTM0, CHn); //�������벶׽�ж�
    }
}

void FTM1_interrupt(void)
{
    uint8 s = FTM1_STATUS;             //��ȡ��׽�ͱȽ�״̬ 
    uint8 CHn;

    FTM1_STATUS = 0x00;             //���жϱ�־λ

    CHn = 0;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_DIS(FTM1, CHn);     //��ֹ���벶׽�ж�
        /*     �û�����       */

        /*********************/
        //FTM_IRQ_EN(FTM1, CHn); //�������벶׽�ж�

    }

    /* ������� n=1 ��ģ�棬����ģ������� */
    CHn = 1;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_EN(FTM1, CHn); //�������벶׽�ж�
        /*     �û�����       */


        /*********************/
        //�����������￪�����벶׽�ж�
        //FTM_IRQ_EN(FTM1, CHn); //�������벶׽�ж�
    }
}


void FTM2_interrupt(void)
{
    uint8 s = FTM2_STATUS;             //��ȡ��׽�ͱȽ�״̬ 
    uint8 CHn;

    FTM2_STATUS = 0x00;             //���жϱ�־λ

    CHn = 0;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_DIS(FTM2, CHn);     //��ֹ���벶׽�ж�
        /*     �û�����       */

        /*********************/
        //FTM_IRQ_EN(FTM2, CHn); //�������벶׽�ж�

    }

    /* ������� n=1 ��ģ�棬����ģ������� */
    CHn = 1;
    if( s & (1 << CHn) )
    {
        //FTM_IRQ_EN(FTM2, CHn); //�������벶׽�ж�
        /*     �û�����       */


        /*********************/
        //�����������￪�����벶׽�ж�
        //FTM_IRQ_EN(FTM2, CHn); //�������벶׽�ж�
    }
}



