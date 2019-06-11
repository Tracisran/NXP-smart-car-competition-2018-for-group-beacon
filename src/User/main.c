#include "include.h"

//定义PWM高电平宽度
  int duty_dianji;//点击高电平宽度
  int duty_st;//高电平宽度 （920us左 1520中 2120右）15250/19000/22750
  int Speed_set; //设置基础速度
  int duty_dianji_expect; //信标实时位置对应的舵机位置
  float RAG;           //角度中间变量
  float E[3];          //ERROR
  float F[3];
  int flag_all[10];      //所有连续监测到的信标
  int ADJ[2]={0,0};
  int sADJ[2]={0,0};
  float T=30.0;        //有效采样周期（错）
  float Kp=82;//8    比例系数                         160
  float Ti=100000;//5       积分系数                99999999999
  float Td=0;//2       微分系数
  float Kp2=8;
  float Ti2=5000;
  float Td2=1;
  int speed_adj;
  int flag_adj=0;      //连续两次读取的flag 取近似后的值
  int temp;             //      
  int xbsize;            //信标宽度
  int out;               //电机pid输出增量
  int out2;
  int b;                 //判断是否突变
  int speed;
void State_Reset()//改变高电平宽度
{
   //频率：40M/(2^4)/50000=100HZ,每个脉冲为0.2us 
   FTM_PWM_Duty(FTM0,FTM_CH0,0);//C1 低电平 Left
   FTM_PWM_Duty(FTM0,FTM_CH1,duty_dianji);//C2 低电平 Right
//   FTM_PWM_Duty(FTM0,FTM_CH2,10000);//C3 0-1 P3 Left
//   FTM_PWM_Duty(FTM0,FTM_CH3,10000);//C4 0-1 P4 Right 以上为改变电机高电平宽度
   
   FTM_PWM_Duty(FTM2,FTM_CH1,duty_st);//改变舵机高电平宽度
}
  
//ov7620 采集
int ROW_Count_80=0;   //采集列数计数

//OV7620摄像头图像采集中断处理函数，（图像有偏移，待解决）
void PORTC_Interrupt(void)
{
  int n;
 // time_delay_us(1);
  n=6;   //行中断
  if((PORTC_ISFR & (1<<n)))
  {
      PORTC_ISFR |= (1<<n);
      
      /* 用户自行添加中断内程序 */
      for(int yanshi=0;yanshi<140;yanshi++);  //行中断前延时，消除图像偏移
      if(Is_SendPhoto == 0)  //如果现在还没采集完成就继续踩，采集完成就退出
      {
          return;
      }
      //time_delay_us(1);
      //if(ROW_Count%2==0)    //每3行采集一行
      {
          //DMA采集，在这里修改采集接口PTD_BYTE0_IN是D0--D7。   PLCK接的是PTA19。
          DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTD_BYTE0_IN, Image_data[ROW_Count_80], PTA19, DMA_BYTE1, LIN, DMA_rising);
          DMA_EN(DMA_CH4);
          ROW_Count_80++;
      }
      ROW_Count++;
      if(ROW_Count==ROW)//图像采集够了以后
      {
          ROW_Count    = 0; //行计数清零
          ROW_Count_80 =0;
          Is_SendPhoto = 0; //行采集标志位关闭
          DMA_DIS(DMA_CH4);
          disable_irq(89);  //关闭PORTC的中断
      }
  } 
  n=7;  //场中断
  //time_delay_us(1);
  if((PORTC_ISFR & (1<<n)))
  {
      PORTC_ISFR |= (1<<n); 
      /* 用户自行添加中断内程序 */
      ROW_Count    = 0;    //行计数清零
      Is_SendPhoto = 1;    //行采集标志位打开
  } 
}

//主函数

        void main(void)
{    
   DisableInterrupts;           //关闭中断
   PLL_Init(PLL180);            //初始化PLL
   LCD_Init();               //OLED模块初始化，默认使用PTB16-19
   Draw_LQLogo();            //OLED显示LOGO
   OV7620_Init();               //摄像头初始化
   UART_Init(UART4,115200);     //串口4初始化
   KEY_Init();                  //按键及输入口初始化
   time_delay_ms(500);
   LCD_CLS();                   //LCD清屏   
   Draw_Frame();   
   LED_Init();                  //LED初始化
   EnableInterrupts;            //开启中断
   
   //频率：40M/(2^4)/50000=100HZ,每个脉冲为0.2us
   Speed_set=0;                 //初始速度为0
   duty_dianji_expect=Speed_set;        //初始速度为0
   duty_dianji=duty_dianji_expect;
   duty_st=17000;//高电平初始值为0
   FTM_PWM_Init(FTM0,FTM_CH0,0,0);//C1 低电平 Left
   FTM_PWM_Init(FTM0,FTM_CH1,0,duty_dianji);//C2 低电平 Right
//   FTM_PWM_Init(FTM0,FTM_CH2,50000,50000);//C3 0-1 P3 Left
//   FTM_PWM_Init(FTM0,FTM_CH3,50000,50000);//C4 0-1 P4 Right 
   FTM_PWM_Init(FTM2,FTM_CH1,50000,duty_st);
       FTM_AB_Init(FTM1);
   
   while(1)
   { 
     enable_irq(89);            //打开PORTC中断
     LED_Ctrl(LED1, RVS);       //LED指示程序运行状态 
     LCD_CLS();                 //LCD清屏
     Draw_Frame();              //画边框 
     Calc_Offset();             //采样、计算信标位置一步到位
     int flag =  Calc_Offset();  //信标位置（0-160）255无效
     xbsize = Calc_Size();
     
     if(flag==255)                      //开始未检测到信标
      {
        duty_dianji=0;                 //速度为0
      State_Reset();
      time_delay_ms(40);  
     char  flagtxt[16];                 //将flag 转化为txt
     char  dutytxt[16];
     char  sizetxt[16];
     char  outtxt[16];
     char  btxt[16];
     char  duty_dianji_expecttxt[16];
     char  speedtxt[16];
     
     sprintf(flagtxt,"%u",flag_adj);         //屏幕显示  当前信标位置
     LCD_P6x8Str(0,0,(uint8*)flagtxt);          //
     sprintf(dutytxt,"%u",duty_st);             //
     LCD_P6x8Str(0,1,(uint8*)dutytxt);          //舵机电机输出电平
     sprintf(sizetxt,"%u",xbsize);              //
     LCD_P6x8Str(0,2,(uint8*)sizetxt);          //信标宽度
     sprintf(outtxt,"%u",out);                  //
     LCD_P6x8Str(0,3,(uint8*)outtxt);           //舵机增量
     //sprintf(btxt,"%u",b);
     //LCD_P6x8Str(0,4,(uint8*)btxt);             //判断有没有突变
     sprintf(duty_dianji_expecttxt,"%u",duty_dianji_expect);            //
     LCD_P6x8Str(0,4,(uint8*)duty_dianji_expecttxt);//电机电平输出量预期值
     sprintf(speedtxt,"%u",speed);
     LCD_P6x8Str(0,5,(uint8*)speedtxt);
      }
     else
     {
     duty_dianji=50000;                           //状态改变，一检测到信标，开始加速

     //time_delay_ms(40);
     //duty_dianji=30000;
     //State_Reset();
     //time_delay_ms(40);
     //duty_dianji=40000;
     //State_Reset();
     //time_delay_ms(40);
     //duty_dianji=50000;
     //State_Reset();
     //time_delay_ms(20);
     
     }
     if(duty_dianji>0)                           //有了速度以后
     {
       while(1)                                 //进入循环
       {
       enable_irq(89);            //打开PORTC中断
     LED_Ctrl(LED1, RVS);       //LED指示程序运行状态 
     LCD_CLS();                 //LCD清屏
     Draw_Frame();              //画边框 
     Calc_Offset();             //采样、计算信标位置一步到位
     flag =  Calc_Offset();  //信标位置（0-160）255无效
     xbsize =  Calc_Size();
     Speed_set=50000;                                   //设置最高速度
     flag_all[0]=flag_all[1];                           //连续n次检测到的信标位置
     flag_all[1]=flag_all[2];
     flag_all[2]=flag_all[3];
     flag_all[3]=flag_all[4];
     flag_all[4]=flag_all[5];
     flag_all[5]=flag_all[6];
     
//     flag_all[6]=flag_all[7];
//     flag_all[7]=flag_all[8];
//     flag_all[8]=flag_all[9];
     flag_all[6]=flag;
              
     speed= ((-1)* FTM_AB_Get(FTM1))*10;
     
     if((flag_all[0]!=255)&&(flag_all[1]==255)&&(flag_all[2]==255)&&(flag_all[3]==255)&&(flag_all[4]==255)&&(flag_all[5]==255)&&(flag_all[6]==255)||xbsize>10)
     {                                  //如果检测到，后面连续检测到的值都没有，说明信标已经灭了，进入一个冲刺和转圈状态
       if(flag_all[5]>80)               //最后一次检测到的值小于80则右转，大于80则左环
       {
      duty_st=14000; 
       }
       else
       {
       duty_st=20000;
       }
    // State_Reset();

     duty_dianji_expect=20000;                  //突然加速离开
     duty_dianji=duty_dianji_expect;
     State_Reset();
     time_delay_ms(30);
     temp=0;
     
     }
     else if((flag_all[0]==255)&&(flag_all[1]==255)&&(flag_all[2]==255)&&(flag_all[3]==255)&&(flag_all[4]==255)&&(flag_all[5]==255)&&(flag_all[6]==255))
     {                                                  //一直没检测到，转圈
       
     duty_dianji_expect=20000;                                 //转圈速度
     
     if(duty_st>17000)               //最后一次检测到的值小于80则右转，大于80则左环
       {
      duty_st=22000; 
       }
       else
       {
       duty_st=12000;
       }
   //  duty_dianji=duty_dianji_expect;
     //duty_st=11000;
     //State_Reset();
    // time_delay_ms(20);                    
     temp=1;
//     char  flagtxt[16];
//     char  dutytxt[16];
//     char  sizetxt[16];
//     char  outtxt[16];
//     char  btxt[16];
//     char  duty_dianji_expecttxt[16];
//     char  speedtxt[16];
//     
//     sprintf(flagtxt,"%u",flag_adj);
//     LCD_P6x8Str(0,0,(uint8*)flagtxt);
//     sprintf(dutytxt,"%u",duty_st);    
//     LCD_P6x8Str(0,1,(uint8*)dutytxt);
//     sprintf(sizetxt,"%u",xbsize);
//     LCD_P6x8Str(0,2,(uint8*)sizetxt);
//     sprintf(outtxt,"%u",out);
//     LCD_P6x8Str(0,3,(uint8*)outtxt);
//    // sprintf(btxt,"%u",b);
//    // LCD_P6x8Str(0,4,(uint8*)btxt);
//     sprintf(duty_dianji_expecttxt,"%u",duty_dianji_expect);
//     LCD_P6x8Str(0,4,(uint8*)duty_dianji_expecttxt);
//     sprintf(speedtxt,"%u",speed);
//     LCD_P6x8Str(0,5,(uint8*)speedtxt);
     

     }
     else           //检测到的状态
     {  
       if(((flag<10)||(flag>150))&&(flag_all[0]==255)&&(flag_all[1]==255)&&(flag_all[2]==255)&&(flag_all[3]==255)&&(flag_all[4]==255)&&(flag_all[5]==255)&&(flag_all[6]!=255))
     {  
                              //连续检测不到，一检测到冲刺
      if(flag<80)
      {
       duty_st=20000;
      }
      else
      {
        duty_st=14000;
      }
      //目标方向
   //    time_delay_ms(50);
      duty_dianji_expect=0;                                //朝目标方向冲刺
      duty_dianji=duty_dianji_expect;
       State_Reset();
     time_delay_ms(50);
     b=1;
//     char  flagtxt[16];
//     char  dutytxt[16];
//     char  sizetxt[16];
//     char  outtxt[16];
//     char  btxt[16];
//     char  duty_dianji_expecttxt[16];
//     char  speedtxt[16];
//     sprintf(flagtxt,"%u",flag_adj);
//     LCD_P6x8Str(0,0,(uint8*)flagtxt);
//     sprintf(dutytxt,"%u",duty_st);    
//     LCD_P6x8Str(0,1,(uint8*)dutytxt);
//     sprintf(sizetxt,"%u",xbsize);
//     LCD_P6xStr(0,2,(uint8*)sizetxt);
//     sprintf(outtxt,"%u",out);
//     LCD_P6x8Str(0,3,(uint8*)outtxt);
//  //   sprintf(btxt,"%u",b);
//   //  LCD_P6x8Str(0,4,(uint8*)btxt);
//     sprintf(duty_dianji_expecttxt,"%u",duty_dianji_expect);
//     LCD_P6x8Str(0,4,(uint8*)duty_dianji_expecttxt);
//     sprintf(speedtxt,"%u",speed);
//     LCD_P6x8Str(0,5,(uint8*)speedtxt);
     
     }
     if(flag<250)              //信标有效
     {   
       ADJ[0]=ADJ[1];
       ADJ[1]=flag;
       if ((ADJ[1]<(ADJ[0]+3)&&ADJ[1]>(ADJ[0]-3))/*||((ADJ[1]>(ADJ[0]+30))||(ADJ[1]<(ADJ[0]-30)))*/)
         ADJ[1]=ADJ[0];
       flag_adj=ADJ[1];                         //去除微小抖动
       
//       RAG=(flag-80)*(1.732)/80/4.0;    //就算信标角度
//       duty_dianji_expect=(RAG-(RAG*RAG*RAG)/3.0+(RAG*RAG*RAG*RAG*RAG)/5.0)*180/3.1415926*2000/30.0+17000;//化为舵机角度
//       E[0]=E[1];                   
//       E[1]=E[2];
//       E[2]=duty_dianji_expect;           //前三次误差
     }
//     char  flagtxt[16];                    //将flag 转化为txt
//     char  dutytxt[16];
//     char  sizetxt[16];
//     char  out2txt[16];
//     char  duty_dianjitxt[16];
//     char  duty_dianji_expecttxt[16];
//     char  speedtxt[16];
//     
//     sprintf(flagtxt,"%u",flag_adj);              //显示
//     LCD_P6x8Str(0,0,(uint8*)flagtxt);
//     sprintf(dutytxt,"%u",duty_st);    
//     LCD_P6x8Str(0,1,(uint8*)dutytxt);
//     sprintf(sizetxt,"%u",xbsize);
//     LCD_P6x8Str(0,2,(uint8*)sizetxt);
//     sprintf(out2txt,"%u",(4294967296-out2));
//     LCD_P6x8Str(0,3,(uint8*)out2txt);
//     sprintf(duty_dianjitxt,"%u",duty_dianji);
//     LCD_P6x8Str(0,4,(uint8*)duty_dianjitxt);
//     sprintf(duty_dianji_expecttxt,"%u",duty_dianji_expect);
//     LCD_P6x8Str(0,5,(uint8*)duty_dianji_expecttxt);
//     sprintf(speedtxt,"%u",speed);
//     LCD_P6x8Str(0,6,(uint8*)speedtxt);
     
     if((0<xbsize)&&(xbsize<3))                         //信标距离
     {
     duty_dianji_expect=30000+20000/xbsize  ;            //设置速度
     }
     else if((xbsize>2)&&(xbsize<20))
     {
     duty_dianji_expect=5000+(80000/xbsize);
     }
     if (flag<250)                 //如果检测到有效
     {
       E[0]=E[1];
       E[1]=E[2];
       E[2]=flag_adj-80;                //有效偏差
       
//      if(xbsize<7)
  //    {
       out=(E[2])*(Kp*(1+T/Ti+Td/T))-Kp*(1+2*Td/T)*(E[1])+(E[0])*Kp*Td/T;                       //计算舵机输出增量
       //duty_st=15000;
       duty_st=duty_st+out;
       if(duty_st<11000)                                //限幅
       {
       duty_st=11000;
       }
       if(duty_st>23000)
       {
       duty_st=23000;
       }
       
//    duty_st=17000-(Kp*(1+T/Ti+Td/T)*(E[2]-17000)-Kp*(1+2*Td/T)*(E[1]-17000)+Kp*Td/T*(E[0]-17000)); //pid输出舵机计算


     if(flag<160&&flag>=0)
    {
        duty_dianji_expect=duty_dianji_expect*(1-abs(80-flag_adj)/120.0);
    }

 //    }
//     else
  //   {
    // duty_dianji_expect=15000;
     
//     if(flag>80)
  //   {
    // duty_st=15000;
//     }
  //  else
    //{
//    duty_st=19000;
    
  //  }
     
    // }


//     if(Is_SendPhoto) 
//      SendPicture();          //串口发送数据到上位机   
     }
       }
     
     
     
     sADJ[0]=sADJ[1];
       sADJ[1]=speed;
       if ((sADJ[1]<(sADJ[0]+10) && sADJ[1]>(sADJ[0]-10))/*||((ADJ[1]>(ADJ[0]+30))||(ADJ[1]<(ADJ[0]-30)))*/)
         sADJ[1]=sADJ[0];
       speed_adj=sADJ[1];                         //去除微小抖动  
     F[0]=F[1];
     F[1]=F[2];
     F[2]=duty_dianji_expect-speed_adj;
     
     
     char  flagtxt[16];                    //将flag 转化为txt
     char  dutytxt[16];
     char  sizetxt[16];
     char  out2txt[16];
     char  duty_dianjitxt[16];
     char  duty_dianji_expecttxt[16];
     char  speedtxt[16];
     
     sprintf(flagtxt,"%u",flag_adj);              //显示
     LCD_P6x8Str(0,0,(uint8*)flagtxt);
     sprintf(dutytxt,"%u",duty_st);    
     LCD_P6x8Str(0,1,(uint8*)dutytxt);
     sprintf(sizetxt,"%u",xbsize);
     LCD_P6x8Str(0,2,(uint8*)sizetxt);
     sprintf(out2txt,"%u",F[2]);
     LCD_P6x8Str(0,3,(uint8*)out2txt);
     sprintf(duty_dianjitxt,"%u",duty_dianji);
     LCD_P6x8Str(0,4,(uint8*)duty_dianjitxt);
     sprintf(duty_dianji_expecttxt,"%u",duty_dianji_expect);
     LCD_P6x8Str(0,5,(uint8*)duty_dianji_expecttxt);
     sprintf(speedtxt,"%u",speed_adj );
     LCD_P6x8Str(0,6,(uint8*)speedtxt);
     
     
     out2=(F[2])*(Kp2*(1+T/Ti2+Td2/T))-Kp2*(1+2*Td2/T)*(F[1])+(F[0])*Kp2*Td2/T;
       duty_dianji=duty_dianji+out2;
     if(duty_dianji<0)                                //限幅
       {
       duty_dianji=0;
       }
       if(duty_dianji>50000)
       {
       duty_dianji=50000;
       }
                                          //duty_dianji=50000;
                                          //duty_dianji_expect=50000;
       State_Reset();
       time_delay_ms(30);
                                //状态输出
               //采样频率
       
       } 
   }
   }
}
