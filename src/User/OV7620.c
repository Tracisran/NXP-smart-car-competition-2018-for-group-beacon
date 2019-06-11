#include "include.h"

unsigned char Image_data[ROW][LIN] = {0};
unsigned char Copy_Image[R60][L80] = {0};
unsigned char PianCha[R60]= {0};
unsigned char XIANGJIAN[R60]= {0};
unsigned char ROW_Count = 0;
unsigned char size = 0;
unsigned char FieldCounter = 0;
unsigned char Is_SendPhoto = 0;

unsigned char a=1;
int HIGHT=1;

void OV7620_Init(void)
{
  u16 i=0,j=0;
  
  EXTI_Init(PTC,6,rising_down);   //行中断
  EXTI_Init(PTC,7,falling_up);    //场中断 
  
  for(i=0; i<ROW; i++)
   {
     for(j=0;j<LIN ;j++)
      {
        Image_data[i][j] = 0;     //将摄像头数据清零
       }
    }
}

/*************************************************************************
*                    北京龙邱智能科技 大家玩开发板
*
*  函数名称：void SendPicture()
*  功能说明：摄像头数据发送
*  参数说明：无
*  函数返回：无
*  备    注：
*************************************************************************/
void SendPicture(void)
{
   int i = 0, j = 0;
   UART_Put_Char(UART4,0xff);//发送帧头标志
   for(i=0;i<ROW;i++)      //输出
   {
     for(j=0;j<LIN;j++)    //输出从第0列到320列，用户可以选择性的输出合适的列数
      {
        if(Image_data[i][j]==0xff)
         {
         Image_data[i][j]=0xfe;//防止发送标志位
         }
         UART_Put_Char(UART4,Image_data[i][j]);
      }
   }
}

void Copy_Arr(unsigned char aa[],unsigned char b[])            //拷贝图像
{
  unsigned int i,n=L80*R60;
  for(i= 0;i < n;i++)
    b[i] = aa[i<<2];
}

void BinaryZation(unsigned char (*p)[L80])  //二值化
{
    unsigned int i,j;
    for(i=0;i<R60;i++)
      for(j=0;j<L80;j++)
      {
        if(*(*(p + i)+ j)<250)   //固定阈值
        {
          *(*(p + i)+ j) = 0;    //黑色是1
        }
        else
        {
          *(*(p + i)+ j) = 1;    //白色是0
        }
      }
}

void Peng_Fushi(unsigned char (*t)[L80],unsigned char (*temp)[L80],unsigned char b)
{
   a = b;
    unsigned int i,j;
    for(i=1;i<R60-1;i++)
      for(j=1;j<L80-1;j++)
          if(t[i][j-1]==a||t[i][j+1]==a||t[i-1][j]==a||t[i+1][j]==a)  //对图像内部进行置一处理
               temp[i][j]=a;
    for(i=1;i<R60-1;i++)
    {
        if(t[i-1][0]==a||t[i+1][0]==a)         //对图像第一列数据置一处理
             temp[i][0]=a;
        if(t[i-1][L80-1]==a||t[i+1][L80-1]==a) //对图像最后一列置一处理
             temp[i][L80-1]=a;
    }
    for(j=0;j<L80-1;j++)
    {
        if(t[0][j-1]==a||t[0][j+1]==a)        //对第一行进行置一处理
             temp[0][j]=a;
        if(t[R60-1][j-1]==a||t[R60-1][j+1]==a)//对最后一行进行置一处理
             temp[R60-1][j]=a;
    }
    if(t[0][1]==a||t[1][0]==a)                //对四个角进行置一处理
           temp[0][0]=a;
    if(t[0][L80-2]==a||t[1][L80-1]==a)
           temp[0][L80-1]=a;
    if(t[R60-2][0]==a||t[R60-1][1]==a)
           temp[R60-1][0]=a;
    if(t[R60-2][L80-1]==a||t[R60-1][L80-2]==a)
           temp[R60-1][L80-1]=a; 
}

void Image_Filter(void)                   //一步到位函数，二值化，滤波
{
   //unsigned char t[R60][L80];            //定义参考图像
    
   Copy_Arr(Image_data[0],Copy_Image[0]);  //拷贝图像
   BinaryZation(Copy_Image);               //二值化
   unsigned int i,j;
    
    for(i=1;i<R60-1;i++)
    {      
      for(j=1;j<L80-1;j++)     
      {
        if((Copy_Image[i][j-1]==0)&&(Copy_Image[i][j]==1)&&(Copy_Image[i][j+1]==0)&&(Copy_Image[i][j+1]==0)&&(Copy_Image[i][j-1]==0))   //固定阈值
        {
          Copy_Image[i][j]=0;
        }       
      }      
    } 
   //Copy_Arr(Copy_Image[0],t[0]);           //拷贝出来一份参考图像    
  /**********************膨胀腐蚀闭处理**********************/     
   //Peng_Fushi(t,Copy_Image,1);           //膨胀处理
   //Peng_Fushi(t,Copy_Image,0);           //腐蚀处理
}

void Seek_Becon_Dots(void)  //二值化
{
    unsigned int i,j;
    unsigned char lside=80,rside=0;
    
    for(i=1;i<R60-1;i++)
    {
      lside=80,rside=0;
      for(j=1;j<L80-2;j++)     
      {
        if((Copy_Image[i][j-1]==0)&&(Copy_Image[i][j]==1)&&(Copy_Image[i][j+1]==1))   //固定阈值
        {
          lside=j;
        }
        if((Copy_Image[i][j-1]==1)&&(Copy_Image[i][j]==1)&&(Copy_Image[i][j+1]==0))   //固定阈值
        {
          rside=j;
        } 
        if(Copy_Image[i][79]==1) rside=79;
         if(Copy_Image[i][1]==1)  lside=1;
      }
      if(lside<rside) 
      {
        PianCha[i]=(lside+rside);
        XIANGJIAN[i]=rside-lside;
      }//最大数值为160
      else            
      {
        PianCha[i]=0xff;
        XIANGJIAN[i]=0xff;
      }
    }
}
//计算信标灯所在位置，有效值为0--80，无效返回为0XFF
u8 Calc_Offset(void)
{
      int t=0;             
     int i=0; 
     int TOP=1;
     int LOW=1;
  //  HIGHT=1;     
     //开启摄像头采集
     enable_irq(89);  //打开PORTC中断
     Image_Filter();    //滤波
     Seek_Becon_Dots();            //计算信标灯存在位置   
     Draw_Road();

     for(i=1;i<R60-1;i++)
     {

        if((PianCha[i]!=0xff)&&(PianCha[i+1]!=0xff))     //连续四个点表示信号为信标灯
        {   
          
          if(t==0)
          {
          TOP=i;
          t=1;
          }
          else
          {
          LOW=i+1;
          }

          HIGHT=(LOW+TOP)/2; 
          size=XIANGJIAN[HIGHT];
          
         }
        if(((PianCha[i]!=0xff)&&(PianCha[i+1]==0xff)||(LOW==59)))
 //         size=XIANGJIAN[HIGHT];
           return PianCha[HIGHT]; 
     }
//     if((PianCha[TOP]+PianCha[TOP])/2!=0)
  //   {
   //  return (PianCha[HIGHT]+PianCha[HIGHT])/2; //只判断最远处
 //    }
   //  else
     //{
     return 0xff; 
     //}
}

u8 Calc_Size(void)
{       
  
  
  return size;



}
