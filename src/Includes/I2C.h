/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：I2C.H
 * 功能             ：设置I2C总线
 * 备注             ：官方例程上修改
 * 日期             ：2016-09-01
 * 实验平台         ：龙丘 K60VG核心板 
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/

#ifndef _I2C_H_
#define _I2C_H_

/**********************************  IIC(引脚复用) ***************************************/

#define I2C0_SCL    PTD8        // PTB0、PTB2、PTD8
#define I2C0_SDA    PTD9        // PTB1、PTB3、PTD9

#define I2C1_SCL    PTE1       // PTE1、PTC10
#define I2C1_SDA    PTE0       // PTE0、PTC11

/**********************************  IIC(引脚复用) ***************************************/

//定义模块号
typedef enum I2Cn
{
    I2C0  = 0,
    I2C1  = 1
} I2Cn;

//定义读写选项
typedef enum MSmode
{
    write =   0x00,  /* Master write  */
    read =   0x01   /* Master read */
} MSmode;







#define I2C_DisableAck(I2Cn)        I2C_C1_REG(I2Cx[I2Cn]) |= I2C_C1_TXAK_MASK

//
#define I2C_RepeatedStart(I2Cn)     I2C_C1_REG(I2Cx[I2Cn]) |= I2C_C1_RSTA_MASK

//启动信号
#define I2C_Start(I2Cn)             I2C_C1_REG(I2Cx[I2Cn]) |= I2C_C1_TX_MASK+I2C_C1_MST_MASK;\
                                    //I2C_C1_REG(I2Cx[I2Cn]) |= I2C_C1_MST_MASK

//暂停信号
#define I2C_Stop(I2Cn)              I2C_C1_REG(I2Cx[I2Cn]) &= ~(I2C_C1_MST_MASK+I2C_C1_TX_MASK);\
                                    //I2C_C1_REG(I2Cx[I2Cn]) &= ~I2C_C1_TX_MASK

//进入接收模式(应答)
#define I2C_EnterRxMode(I2Cn)       I2C_C1_REG(I2Cx[I2Cn]) &= ~I2C_C1_TX_MASK;\
                                    I2C_C1_REG(I2Cx[I2Cn]) &= ~I2C_C1_TXAK_MASK
//进入接收模式(不应答)
#define I2C_PutinRxMode(I2Cn)       I2C_C1_REG(I2Cx[I2Cn]) &= ~I2C_C1_TX_MASK

//等待 I2C0_S
#define I2C_Wait(I2Cn)              while(( I2C_S_REG(I2Cx[I2Cn]) & I2C_S_IICIF_MASK)==0) {} \
                                    I2C_S_REG(I2Cx[I2Cn]) |= I2C_S_IICIF_MASK;

//写一个字节
#define I2C_write_byte(I2Cn,data)   I2C_D_REG(I2Cx[I2Cn]) = data


void  I2C_Init(I2Cn);                                         //初始化I2C
void  I2C_WriteAddr(I2Cn, u8 SlaveID, u8 Addr, u8 Data);      //读取地址里的内容
void  I2C_StartTransmission (I2Cn, u8 SlaveID, MSmode);       //启动传输
u8    I2C_ReadAddr(I2Cn, u8 SlaveID, u8 Addr);                //往地址里写入内容



#endif