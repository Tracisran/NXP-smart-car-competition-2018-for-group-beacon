/******************** LQ_K60_������ v1.0 ********************
 * �ļ���           ��LQLED.c
 * ����             ����ʼ������
 * ��ע             ���ٷ��������޸�
 * ����             ��2016-09-01
 * ʵ��ƽ̨         ������ K60VG���İ� 
 * ��������         ��IAR 7.3
 * ����             �������� 006
 * �Ա���           ��https://longqiu.taobao.com
 * �������ܳ�����Ⱥ ��202949437
*************************************************************/

#include "include.h"
//-------------------------------------------------------------------------*
//������: LED_Init����                                                      *
//��  ��: ��ʼ��IO��                                                       * 
//��  ��:                                                                *
//��  ��:                                                              *
//��  ��: LED_Init( )                                                    *
//-------------------------------------------------------------------------*
void LED_Init(void)
{
   GPIO_Init(PORTC,4,GPO,1);
   GPIO_Init(PORTC,18,GPO,1);
   GPIO_Init(PORTA,17,GPO,1);
   GPIO_Init(PORTE,26,GPO,1);
   GPIO_Init(PORTD,15,GPO,1);
   GPIO_Init(PORTC,0,GPO,1);
}

//-------------------------------------------------------------------------*
//������: LED_Ctrl����                                                      *
//��  ��: ʵ��LED�Ƶ�����                                                  * 
//��  ��:                                                                *
//��  ��:                                                              *
//��  ��:  LED_Ctrl(LED3, RVS)                                           *
//-------------------------------------------------------------------------*
void LED_Ctrl(LEDn_e ledno, LEDs_e sta)
{
    switch(ledno) 
    {
    case LED0:
      if(sta==ON)        GPIO_Ctrl(PORTD,15,0);
      else if(sta==OFF) GPIO_Ctrl(PORTD,15, 1);
      else if(sta==RVS) GPIO_Reverse (PORTD,15);
    break;
    
    case LED1:
      if(sta==ON)        GPIO_Ctrl(PORTA,17,0);
      else if(sta==OFF) GPIO_Ctrl(PORTA,17, 1);
      else if(sta==RVS) GPIO_Reverse (PORTA,17);
    break;
    
    case LED2:
      if(sta==ON)        GPIO_Ctrl(PORTC,0,0);
      else if(sta==OFF) GPIO_Ctrl(PORTC,0, 1);
      else if(sta==RVS) GPIO_Reverse (PORTC,0);
    break; 
    
    case LED3:
      if(sta==ON)        GPIO_Ctrl(PORTE,26,0);
      else if(sta==OFF) GPIO_Ctrl(PORTE,26, 1);
      else if(sta==RVS) GPIO_Reverse (PORTE,26);
    break;
    case LEDALL:
      if(sta==ON) 
      {       
          GPIO_Ctrl(PORTD,15,0);
          GPIO_Ctrl(PORTC,0, 0);
          GPIO_Ctrl(PORTA,17,0);
          GPIO_Ctrl(PORTE,26,0);
      }
      else if(sta==OFF)
      { 
          GPIO_Ctrl(PORTD,15,1);
          GPIO_Ctrl(PORTC,0, 1);
          GPIO_Ctrl(PORTA,17,1);
          GPIO_Ctrl(PORTE,26,1);
      }
      else if(sta==RVS)
      {       
          GPIO_Reverse (PORTD,15);     
          GPIO_Reverse (PORTC, 0);      
          GPIO_Reverse (PORTA,17);      
          GPIO_Reverse (PORTE,26);      
      }
    break;
    default:
    break;    
    }   
}