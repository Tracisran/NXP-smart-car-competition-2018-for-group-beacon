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
  
  EXTI_Init(PTC,6,rising_down);   //���ж�
  EXTI_Init(PTC,7,falling_up);    //���ж� 
  
  for(i=0; i<ROW; i++)
   {
     for(j=0;j<LIN ;j++)
      {
        Image_data[i][j] = 0;     //������ͷ��������
       }
    }
}

/*************************************************************************
*                    �����������ܿƼ� ����濪����
*
*  �������ƣ�void SendPicture()
*  ����˵��������ͷ���ݷ���
*  ����˵������
*  �������أ���
*  ��    ע��
*************************************************************************/
void SendPicture(void)
{
   int i = 0, j = 0;
   UART_Put_Char(UART4,0xff);//����֡ͷ��־
   for(i=0;i<ROW;i++)      //���
   {
     for(j=0;j<LIN;j++)    //����ӵ�0�е�320�У��û�����ѡ���Ե�������ʵ�����
      {
        if(Image_data[i][j]==0xff)
         {
         Image_data[i][j]=0xfe;//��ֹ���ͱ�־λ
         }
         UART_Put_Char(UART4,Image_data[i][j]);
      }
   }
}

void Copy_Arr(unsigned char aa[],unsigned char b[])            //����ͼ��
{
  unsigned int i,n=L80*R60;
  for(i= 0;i < n;i++)
    b[i] = aa[i<<2];
}

void BinaryZation(unsigned char (*p)[L80])  //��ֵ��
{
    unsigned int i,j;
    for(i=0;i<R60;i++)
      for(j=0;j<L80;j++)
      {
        if(*(*(p + i)+ j)<250)   //�̶���ֵ
        {
          *(*(p + i)+ j) = 0;    //��ɫ��1
        }
        else
        {
          *(*(p + i)+ j) = 1;    //��ɫ��0
        }
      }
}

void Peng_Fushi(unsigned char (*t)[L80],unsigned char (*temp)[L80],unsigned char b)
{
   a = b;
    unsigned int i,j;
    for(i=1;i<R60-1;i++)
      for(j=1;j<L80-1;j++)
          if(t[i][j-1]==a||t[i][j+1]==a||t[i-1][j]==a||t[i+1][j]==a)  //��ͼ���ڲ�������һ����
               temp[i][j]=a;
    for(i=1;i<R60-1;i++)
    {
        if(t[i-1][0]==a||t[i+1][0]==a)         //��ͼ���һ��������һ����
             temp[i][0]=a;
        if(t[i-1][L80-1]==a||t[i+1][L80-1]==a) //��ͼ�����һ����һ����
             temp[i][L80-1]=a;
    }
    for(j=0;j<L80-1;j++)
    {
        if(t[0][j-1]==a||t[0][j+1]==a)        //�Ե�һ�н�����һ����
             temp[0][j]=a;
        if(t[R60-1][j-1]==a||t[R60-1][j+1]==a)//�����һ�н�����һ����
             temp[R60-1][j]=a;
    }
    if(t[0][1]==a||t[1][0]==a)                //���ĸ��ǽ�����һ����
           temp[0][0]=a;
    if(t[0][L80-2]==a||t[1][L80-1]==a)
           temp[0][L80-1]=a;
    if(t[R60-2][0]==a||t[R60-1][1]==a)
           temp[R60-1][0]=a;
    if(t[R60-2][L80-1]==a||t[R60-1][L80-2]==a)
           temp[R60-1][L80-1]=a; 
}

void Image_Filter(void)                   //һ����λ��������ֵ�����˲�
{
   //unsigned char t[R60][L80];            //����ο�ͼ��
    
   Copy_Arr(Image_data[0],Copy_Image[0]);  //����ͼ��
   BinaryZation(Copy_Image);               //��ֵ��
   unsigned int i,j;
    
    for(i=1;i<R60-1;i++)
    {      
      for(j=1;j<L80-1;j++)     
      {
        if((Copy_Image[i][j-1]==0)&&(Copy_Image[i][j]==1)&&(Copy_Image[i][j+1]==0)&&(Copy_Image[i][j+1]==0)&&(Copy_Image[i][j-1]==0))   //�̶���ֵ
        {
          Copy_Image[i][j]=0;
        }       
      }      
    } 
   //Copy_Arr(Copy_Image[0],t[0]);           //��������һ�ݲο�ͼ��    
  /**********************���͸�ʴ�մ���**********************/     
   //Peng_Fushi(t,Copy_Image,1);           //���ʹ���
   //Peng_Fushi(t,Copy_Image,0);           //��ʴ����
}

void Seek_Becon_Dots(void)  //��ֵ��
{
    unsigned int i,j;
    unsigned char lside=80,rside=0;
    
    for(i=1;i<R60-1;i++)
    {
      lside=80,rside=0;
      for(j=1;j<L80-2;j++)     
      {
        if((Copy_Image[i][j-1]==0)&&(Copy_Image[i][j]==1)&&(Copy_Image[i][j+1]==1))   //�̶���ֵ
        {
          lside=j;
        }
        if((Copy_Image[i][j-1]==1)&&(Copy_Image[i][j]==1)&&(Copy_Image[i][j+1]==0))   //�̶���ֵ
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
      }//�����ֵΪ160
      else            
      {
        PianCha[i]=0xff;
        XIANGJIAN[i]=0xff;
      }
    }
}
//�����ű������λ�ã���ЧֵΪ0--80����Ч����Ϊ0XFF
u8 Calc_Offset(void)
{
      int t=0;             
     int i=0; 
     int TOP=1;
     int LOW=1;
  //  HIGHT=1;     
     //��������ͷ�ɼ�
     enable_irq(89);  //��PORTC�ж�
     Image_Filter();    //�˲�
     Seek_Becon_Dots();            //�����ű�ƴ���λ��   
     Draw_Road();

     for(i=1;i<R60-1;i++)
     {

        if((PianCha[i]!=0xff)&&(PianCha[i+1]!=0xff))     //�����ĸ����ʾ�ź�Ϊ�ű��
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
   //  return (PianCha[HIGHT]+PianCha[HIGHT])/2; //ֻ�ж���Զ��
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
