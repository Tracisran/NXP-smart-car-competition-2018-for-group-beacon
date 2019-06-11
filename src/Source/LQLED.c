/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：LQLED.c
 * 功能             ：初始化按键
 * 备注             ：官方例程上修改
 * 日期             ：2016-09-01
 * 实验平台         ：龙丘 K60VG核心板 
 * 开发环境         ：IAR 7.3
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/

#include "include.h"
//-------------------------------------------------------------------------*
//函数名: LED_Init（）                                                      *
//功  能: 初始化IO口                                                       * 
//参  数:                                                                *
//返  回:                                                              *
//简  例: LED_Init( )                                                    *
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
//函数名: LED_Ctrl（）                                                      *
//功  能: 实现LED灯的亮灭                                                  * 
//参  数:                                                                *
//返  回:                                                              *
//简  例:  LED_Ctrl(LED3, RVS)                                           *
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