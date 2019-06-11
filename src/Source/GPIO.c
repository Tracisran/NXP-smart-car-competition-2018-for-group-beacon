/******************** LQ_K60_������ v1.0 ********************
 * �ļ���           ��GPIO.c
 * ����             ������GPIO�ڹ���ģʽ
 * ��ע             ���ٷ��������޸�
 * ����             ��2016-09-01
 * ʵ��ƽ̨         ������ K60VG���İ� 
 * ��������         ��IAR 7.3
 * ����             �������� 006
 * �Ա���           ��https://longqiu.taobao.com
 * �������ܳ�����Ⱥ ��202949437
*************************************************************/
#include "include.h"
#include "GPIO.h"


GPIO_MemMapPtr GPIOX[5] = {PTA_BASE_PTR, PTB_BASE_PTR, PTC_BASE_PTR, PTD_BASE_PTR, PTE_BASE_PTR}; //�������ָ�����鱣�� GPIOX �ĵ�ַ
PORT_MemMapPtr PORTX[5] = {PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR};
//-------------------------------------------------------------------------*
//������: gpio_init                                                        *
//��  ��: ��ʼ��gpio                                                       * 
//��  ��: port:�˿���                                                      *
//        index:ָ���˿�����                                               *
//        dir:���ŷ���,0=����,1=���                                       * 
//        data:��ʼ״̬,0=�͵�ƽ,1=�ߵ�ƽ                                  *
//��  ��: ��                                                              *
//��  ��: gpio_init (PORTA, 17, 1,0);  PTA17���ų�ʼ��Ϊ�͵�ƽ             *
//-------------------------------------------------------------------------*
void GPIO_Init (GPIO_MemMapPtr port, int index, GPIO_CFG dir,int data)
{
  
     SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                 | SIM_SCGC5_PORTB_MASK
                 | SIM_SCGC5_PORTC_MASK
                 | SIM_SCGC5_PORTD_MASK
                 | SIM_SCGC5_PORTE_MASK );
     
     PORT_MemMapPtr p;
     switch((u32)port)
     {
     case 0x400FF000u:
         p = PORTA_BASE_PTR;
         break;
     case 0x400FF040u:
         p = PORTB_BASE_PTR;
         break;
     case 0x400FF080u:
         p = PORTC_BASE_PTR;
         break;
     case 0x400FF0C0u:
         p = PORTD_BASE_PTR;
         break;
     case 0x400FF100u:
         p = PORTE_BASE_PTR;
         break;
     default:
         break;
     }
     PORT_PCR_REG(p,index)=(0|PORT_PCR_MUX(1));

     if(dir == 1)//output
     {
    	 GPIO_PDDR_REG(port) |= (1<<index);
    	 if(data == 1)//output
			  GPIO_PDOR_REG(port) |= (1<<index);
		 else
			  GPIO_PDOR_REG(port) &= ~(1<<index);
     }
         
     else
         GPIO_PDDR_REG(port) &= ~(1<<index);

}

//-------------------------------------------------------------------------* 
//������: gpio_ctrl                                                        *
//��  ��: ��������״̬                                                     *
//��  ��: port:�˿���                                                      *
//        index:ָ���˿�����                                               *
//        data: ״̬,0=�͵�ƽ,1=�ߵ�ƽ                                     *
//��  ��: ��                                                               *
//��  ��: gpio_ctrl (PORTA, 17,0);  PTA17���ų�����Ϊ�͵�ƽ                 *
//-------------------------------------------------------------------------*
void GPIO_Ctrl (GPIO_MemMapPtr port, int index, int data)
{
    if(data == 1)//output
         GPIO_PDOR_REG(port) |= (1<<index);
    else
         GPIO_PDOR_REG(port) &= ~(1<<index);
}

//-----------------------------------------------------------------------* 
//������: gpio_reverse                                                   *
//��  ��: �ı�����״̬                                                   *
//��  ��: port:�˿���;                                                   *
//        index:ָ���˿�����                                             *
//��  ��: ��                                                             *
//��  ��: gpio_reverse (PORTA, 17);  PTA17���������ƽ��ת                *
//-----------------------------------------------------------------------*
void GPIO_Reverse (GPIO_MemMapPtr port, int index)
{
    GPIO_PDOR_REG(port) ^= (1<<index);
}

u8 GPIO_Get(PTXn_e ptxn)
{

    return ((GPIO_PDIR_REG(GPIOX_BASE(ptxn)) >> PTn(ptxn )) & 0x01);        // ��ȡ GPIO PDIR ptxn ״̬������ȡ�ܽ������ƽ
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************************************
                                                    gpio�ⲿ�жϺ���
**************************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------* 
//������: Exti_init                                                   
//��  ��: �ⲿGPIO�жϳ�ʼ��                                                   
//��  ��: portx:�˿�����PTA,PTB,PTC,PTD,PTE��                                                  
//          n  :ָ���˿����źţ�0~31��
//     exti_cfg:�жϴ�����ʽ  �磺 rising_down //�����ش������ڲ�����
//��  ��: ��                                                             
//��  ��: exti_init(PTC, 16, rising_down);            
//-----------------------------------------------------------------------*
void EXTI_Init(PORTx portx, u8 n, exti_cfg cfg)
{
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << portx);    //����PORTx�˿�

    PORT_PCR_REG(PORTX[portx], n) = PORT_PCR_MUX(1) | PORT_PCR_IRQC(cfg & 0x7f ) | PORT_PCR_PE_MASK | ((cfg & 0x80 ) >> 7); // ����GPIO , ȷ������ģʽ ,������������������
    GPIO_PDDR_REG(GPIOX[portx]) &= ~(1 << n);       //����ģʽ
    enable_irq(portx + 87);                         //ʹ��PORT�жϣ�PORTA��ISR�жϺ�Ϊ87
}


/******************************************************************
                      gpio�ⲿ�жϺ���
                      �����������
*****************************************************************/
void PORTA_Interrupt()
{
  int n;
  n=0;
  if((PORTA_ISFR & (1<<n)))
  {
      PORTA_ISFR |= (1<<n); 
      /* �û���������ж��ڳ��� */
  } 
  n=1;
  if((PORTA_ISFR & (1<<n)))
  {
      PORTA_ISFR |= (1<<n); 
      /* �û���������ж��ڳ��� */
  } 
}

void PORTB_Interrupt()
{
  int n;
  n=20;
  if((PORTB_ISFR & (1<<n)))
  {
      PORTB_ISFR |= (1<<n);
      // �û���������ж��ڳ��� 
      LED_Ctrl(LED0,ON);          //����һ�δ���һ���ⲿ�ж�,��ӦLED��һ��
  }
  n=21;
  if((PORTB_ISFR & (1<<n)))
  {
      PORTB_ISFR |= (1<<n);
      // �û���������ж��ڳ��� 
      LED_Ctrl(LED1,ON);          //����һ�δ���һ���ⲿ�ж�,��ӦLED��һ��
  }
  n=22;
  if((PORTB_ISFR & (1<<n)))
  {
      PORTB_ISFR |= (1<<n);
      // �û���������ж��ڳ��� 
      LED_Ctrl(LED2,ON);          //����һ�δ���һ���ⲿ�ж�,��ӦLED��һ��
  }
}
/*
void PORTC_Interrupt()
{
  int n;
  n=0;
  if((PORTC_ISFR & (1<<n)))
  {
      PORTC_ISFR |= (1<<n); 
      //�û���������ж��ڳ��� 
  } 
  n=1;
  if((PORTC_ISFR & (1<<n)))
  {
      PORTC_ISFR |= (1<<n); 
      //�û���������ж��ڳ��� 
  } 
}
*/
void PORTD_Interrupt()
{
  int n;
  n=0;
  if((PORTD_ISFR & (1<<n)))
  {
      PORTD_ISFR |= (1<<n); 
      /* �û���������ж��ڳ��� */
  } 
  n=1;
  if((PORTD_ISFR & (1<<n)))
  {
      PORTD_ISFR |= (1<<n); 
      /* �û���������ж��ڳ��� */
  } 
}

void PORTE_Interrupt()
{
  int n;
  n=0;
  if((PORTE_ISFR & (1<<n)))
  {
      PORTE_ISFR |= (1<<n); 
      /* �û���������ж��ڳ��� */
  } 
  n=1;
  if((PORTE_ISFR & (1<<n)))
  {
      PORTE_ISFR |= (1<<n); 
      /* �û���������ж��ڳ��� */
  } 
}



