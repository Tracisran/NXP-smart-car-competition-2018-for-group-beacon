#include "include.h"

//����PWM�ߵ�ƽ���
  int duty_dianji;//����ߵ�ƽ���
  int duty_st;//�ߵ�ƽ��� ��920us�� 1520�� 2120�ң�15250/19000/22750
  int Speed_set; //���û����ٶ�
  int duty_dianji_expect; //�ű�ʵʱλ�ö�Ӧ�Ķ��λ��
  float RAG;           //�Ƕ��м����
  float E[3];          //ERROR
  float F[3];
  int flag_all[10];      //����������⵽���ű�
  int ADJ[2]={0,0};
  int sADJ[2]={0,0};
  float T=30.0;        //��Ч�������ڣ���
  float Kp=82;//8    ����ϵ��                         160
  float Ti=100000;//5       ����ϵ��                99999999999
  float Td=0;//2       ΢��ϵ��
  float Kp2=8;
  float Ti2=5000;
  float Td2=1;
  int speed_adj;
  int flag_adj=0;      //�������ζ�ȡ��flag ȡ���ƺ��ֵ
  int temp;             //      
  int xbsize;            //�ű���
  int out;               //���pid�������
  int out2;
  int b;                 //�ж��Ƿ�ͻ��
  int speed;
void State_Reset()//�ı�ߵ�ƽ���
{
   //Ƶ�ʣ�40M/(2^4)/50000=100HZ,ÿ������Ϊ0.2us 
   FTM_PWM_Duty(FTM0,FTM_CH0,0);//C1 �͵�ƽ Left
   FTM_PWM_Duty(FTM0,FTM_CH1,duty_dianji);//C2 �͵�ƽ Right
//   FTM_PWM_Duty(FTM0,FTM_CH2,10000);//C3 0-1 P3 Left
//   FTM_PWM_Duty(FTM0,FTM_CH3,10000);//C4 0-1 P4 Right ����Ϊ�ı����ߵ�ƽ���
   
   FTM_PWM_Duty(FTM2,FTM_CH1,duty_st);//�ı����ߵ�ƽ���
}
  
//ov7620 �ɼ�
int ROW_Count_80=0;   //�ɼ���������

//OV7620����ͷͼ��ɼ��жϴ���������ͼ����ƫ�ƣ��������
void PORTC_Interrupt(void)
{
  int n;
 // time_delay_us(1);
  n=6;   //���ж�
  if((PORTC_ISFR & (1<<n)))
  {
      PORTC_ISFR |= (1<<n);
      
      /* �û���������ж��ڳ��� */
      for(int yanshi=0;yanshi<140;yanshi++);  //���ж�ǰ��ʱ������ͼ��ƫ��
      if(Is_SendPhoto == 0)  //������ڻ�û�ɼ���ɾͼ����ȣ��ɼ���ɾ��˳�
      {
          return;
      }
      //time_delay_us(1);
      //if(ROW_Count%2==0)    //ÿ3�вɼ�һ��
      {
          //DMA�ɼ����������޸Ĳɼ��ӿ�PTD_BYTE0_IN��D0--D7��   PLCK�ӵ���PTA19��
          DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTD_BYTE0_IN, Image_data[ROW_Count_80], PTA19, DMA_BYTE1, LIN, DMA_rising);
          DMA_EN(DMA_CH4);
          ROW_Count_80++;
      }
      ROW_Count++;
      if(ROW_Count==ROW)//ͼ��ɼ������Ժ�
      {
          ROW_Count    = 0; //�м�������
          ROW_Count_80 =0;
          Is_SendPhoto = 0; //�вɼ���־λ�ر�
          DMA_DIS(DMA_CH4);
          disable_irq(89);  //�ر�PORTC���ж�
      }
  } 
  n=7;  //���ж�
  //time_delay_us(1);
  if((PORTC_ISFR & (1<<n)))
  {
      PORTC_ISFR |= (1<<n); 
      /* �û���������ж��ڳ��� */
      ROW_Count    = 0;    //�м�������
      Is_SendPhoto = 1;    //�вɼ���־λ��
  } 
}

//������

        void main(void)
{    
   DisableInterrupts;           //�ر��ж�
   PLL_Init(PLL180);            //��ʼ��PLL
   LCD_Init();               //OLEDģ���ʼ����Ĭ��ʹ��PTB16-19
   Draw_LQLogo();            //OLED��ʾLOGO
   OV7620_Init();               //����ͷ��ʼ��
   UART_Init(UART4,115200);     //����4��ʼ��
   KEY_Init();                  //����������ڳ�ʼ��
   time_delay_ms(500);
   LCD_CLS();                   //LCD����   
   Draw_Frame();   
   LED_Init();                  //LED��ʼ��
   EnableInterrupts;            //�����ж�
   
   //Ƶ�ʣ�40M/(2^4)/50000=100HZ,ÿ������Ϊ0.2us
   Speed_set=0;                 //��ʼ�ٶ�Ϊ0
   duty_dianji_expect=Speed_set;        //��ʼ�ٶ�Ϊ0
   duty_dianji=duty_dianji_expect;
   duty_st=17000;//�ߵ�ƽ��ʼֵΪ0
   FTM_PWM_Init(FTM0,FTM_CH0,0,0);//C1 �͵�ƽ Left
   FTM_PWM_Init(FTM0,FTM_CH1,0,duty_dianji);//C2 �͵�ƽ Right
//   FTM_PWM_Init(FTM0,FTM_CH2,50000,50000);//C3 0-1 P3 Left
//   FTM_PWM_Init(FTM0,FTM_CH3,50000,50000);//C4 0-1 P4 Right 
   FTM_PWM_Init(FTM2,FTM_CH1,50000,duty_st);
       FTM_AB_Init(FTM1);
   
   while(1)
   { 
     enable_irq(89);            //��PORTC�ж�
     LED_Ctrl(LED1, RVS);       //LEDָʾ��������״̬ 
     LCD_CLS();                 //LCD����
     Draw_Frame();              //���߿� 
     Calc_Offset();             //�����������ű�λ��һ����λ
     int flag =  Calc_Offset();  //�ű�λ�ã�0-160��255��Ч
     xbsize = Calc_Size();
     
     if(flag==255)                      //��ʼδ��⵽�ű�
      {
        duty_dianji=0;                 //�ٶ�Ϊ0
      State_Reset();
      time_delay_ms(40);  
     char  flagtxt[16];                 //��flag ת��Ϊtxt
     char  dutytxt[16];
     char  sizetxt[16];
     char  outtxt[16];
     char  btxt[16];
     char  duty_dianji_expecttxt[16];
     char  speedtxt[16];
     
     sprintf(flagtxt,"%u",flag_adj);         //��Ļ��ʾ  ��ǰ�ű�λ��
     LCD_P6x8Str(0,0,(uint8*)flagtxt);          //
     sprintf(dutytxt,"%u",duty_st);             //
     LCD_P6x8Str(0,1,(uint8*)dutytxt);          //�����������ƽ
     sprintf(sizetxt,"%u",xbsize);              //
     LCD_P6x8Str(0,2,(uint8*)sizetxt);          //�ű���
     sprintf(outtxt,"%u",out);                  //
     LCD_P6x8Str(0,3,(uint8*)outtxt);           //�������
     //sprintf(btxt,"%u",b);
     //LCD_P6x8Str(0,4,(uint8*)btxt);             //�ж���û��ͻ��
     sprintf(duty_dianji_expecttxt,"%u",duty_dianji_expect);            //
     LCD_P6x8Str(0,4,(uint8*)duty_dianji_expecttxt);//�����ƽ�����Ԥ��ֵ
     sprintf(speedtxt,"%u",speed);
     LCD_P6x8Str(0,5,(uint8*)speedtxt);
      }
     else
     {
     duty_dianji=50000;                           //״̬�ı䣬һ��⵽�ű꣬��ʼ����

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
     if(duty_dianji>0)                           //�����ٶ��Ժ�
     {
       while(1)                                 //����ѭ��
       {
       enable_irq(89);            //��PORTC�ж�
     LED_Ctrl(LED1, RVS);       //LEDָʾ��������״̬ 
     LCD_CLS();                 //LCD����
     Draw_Frame();              //���߿� 
     Calc_Offset();             //�����������ű�λ��һ����λ
     flag =  Calc_Offset();  //�ű�λ�ã�0-160��255��Ч
     xbsize =  Calc_Size();
     Speed_set=50000;                                   //��������ٶ�
     flag_all[0]=flag_all[1];                           //����n�μ�⵽���ű�λ��
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
     {                                  //�����⵽������������⵽��ֵ��û�У�˵���ű��Ѿ����ˣ�����һ����̺�תȦ״̬
       if(flag_all[5]>80)               //���һ�μ�⵽��ֵС��80����ת������80����
       {
      duty_st=14000; 
       }
       else
       {
       duty_st=20000;
       }
    // State_Reset();

     duty_dianji_expect=20000;                  //ͻȻ�����뿪
     duty_dianji=duty_dianji_expect;
     State_Reset();
     time_delay_ms(30);
     temp=0;
     
     }
     else if((flag_all[0]==255)&&(flag_all[1]==255)&&(flag_all[2]==255)&&(flag_all[3]==255)&&(flag_all[4]==255)&&(flag_all[5]==255)&&(flag_all[6]==255))
     {                                                  //һֱû��⵽��תȦ
       
     duty_dianji_expect=20000;                                 //תȦ�ٶ�
     
     if(duty_st>17000)               //���һ�μ�⵽��ֵС��80����ת������80����
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
     else           //��⵽��״̬
     {  
       if(((flag<10)||(flag>150))&&(flag_all[0]==255)&&(flag_all[1]==255)&&(flag_all[2]==255)&&(flag_all[3]==255)&&(flag_all[4]==255)&&(flag_all[5]==255)&&(flag_all[6]!=255))
     {  
                              //������ⲻ����һ��⵽���
      if(flag<80)
      {
       duty_st=20000;
      }
      else
      {
        duty_st=14000;
      }
      //Ŀ�귽��
   //    time_delay_ms(50);
      duty_dianji_expect=0;                                //��Ŀ�귽����
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
     if(flag<250)              //�ű���Ч
     {   
       ADJ[0]=ADJ[1];
       ADJ[1]=flag;
       if ((ADJ[1]<(ADJ[0]+3)&&ADJ[1]>(ADJ[0]-3))/*||((ADJ[1]>(ADJ[0]+30))||(ADJ[1]<(ADJ[0]-30)))*/)
         ADJ[1]=ADJ[0];
       flag_adj=ADJ[1];                         //ȥ��΢С����
       
//       RAG=(flag-80)*(1.732)/80/4.0;    //�����ű�Ƕ�
//       duty_dianji_expect=(RAG-(RAG*RAG*RAG)/3.0+(RAG*RAG*RAG*RAG*RAG)/5.0)*180/3.1415926*2000/30.0+17000;//��Ϊ����Ƕ�
//       E[0]=E[1];                   
//       E[1]=E[2];
//       E[2]=duty_dianji_expect;           //ǰ�������
     }
//     char  flagtxt[16];                    //��flag ת��Ϊtxt
//     char  dutytxt[16];
//     char  sizetxt[16];
//     char  out2txt[16];
//     char  duty_dianjitxt[16];
//     char  duty_dianji_expecttxt[16];
//     char  speedtxt[16];
//     
//     sprintf(flagtxt,"%u",flag_adj);              //��ʾ
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
     
     if((0<xbsize)&&(xbsize<3))                         //�ű����
     {
     duty_dianji_expect=30000+20000/xbsize  ;            //�����ٶ�
     }
     else if((xbsize>2)&&(xbsize<20))
     {
     duty_dianji_expect=5000+(80000/xbsize);
     }
     if (flag<250)                 //�����⵽��Ч
     {
       E[0]=E[1];
       E[1]=E[2];
       E[2]=flag_adj-80;                //��Чƫ��
       
//      if(xbsize<7)
  //    {
       out=(E[2])*(Kp*(1+T/Ti+Td/T))-Kp*(1+2*Td/T)*(E[1])+(E[0])*Kp*Td/T;                       //�������������
       //duty_st=15000;
       duty_st=duty_st+out;
       if(duty_st<11000)                                //�޷�
       {
       duty_st=11000;
       }
       if(duty_st>23000)
       {
       duty_st=23000;
       }
       
//    duty_st=17000-(Kp*(1+T/Ti+Td/T)*(E[2]-17000)-Kp*(1+2*Td/T)*(E[1]-17000)+Kp*Td/T*(E[0]-17000)); //pid����������


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
//      SendPicture();          //���ڷ������ݵ���λ��   
     }
       }
     
     
     
     sADJ[0]=sADJ[1];
       sADJ[1]=speed;
       if ((sADJ[1]<(sADJ[0]+10) && sADJ[1]>(sADJ[0]-10))/*||((ADJ[1]>(ADJ[0]+30))||(ADJ[1]<(ADJ[0]-30)))*/)
         sADJ[1]=sADJ[0];
       speed_adj=sADJ[1];                         //ȥ��΢С����  
     F[0]=F[1];
     F[1]=F[2];
     F[2]=duty_dianji_expect-speed_adj;
     
     
     char  flagtxt[16];                    //��flag ת��Ϊtxt
     char  dutytxt[16];
     char  sizetxt[16];
     char  out2txt[16];
     char  duty_dianjitxt[16];
     char  duty_dianji_expecttxt[16];
     char  speedtxt[16];
     
     sprintf(flagtxt,"%u",flag_adj);              //��ʾ
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
     if(duty_dianji<0)                                //�޷�
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
                                //״̬���
               //����Ƶ��
       
       } 
   }
   }
}
