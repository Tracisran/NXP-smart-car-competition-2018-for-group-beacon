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
#include "I2C.h"


volatile struct I2C_MemMap *I2Cx[2] = {I2C0_BASE_PTR, I2C1_BASE_PTR}; //��������ָ�����鱣�� I2Cx �ĵ�ַ


//-------------------------------------------------------------------------*
//������: IIC_init                                                        
//��  ��: ��ʼ��IIC                                                       
//��  ��: i2cn:�˿��� I2C0,I2C1                                                                                     
//��  ��: ��                                                              
//��  ��: IIC_init(I2C1);              
//-------------------------------------------------------------------------*
void I2C_init(I2Cn i2cn)
{

    if(i2cn == I2C0)
    {
        /* ����ʱ�� */
        SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;         //���� I2C0ʱ��

        /* ���� I2C0���ܵ� GPIO �ӿ� */
        if(I2C0_SCL == PTB0)
        {  
           PORTB_PCR0 = PORT_PCR_MUX(2);
           PORTB_PCR0 = PORTB_PCR0|(1<<5);//����Ϊ��©ģʽ
        }
        else if(I2C0_SCL == PTB2)
        {
          PORTB_PCR2 = PORT_PCR_MUX(2);
          PORTB_PCR2 = PORTB_PCR2|(1<<5);//����Ϊ��©ģʽ
        }
        else if(I2C0_SCL == PTD8)
        {
          PORTD_PCR8 = PORT_PCR_MUX(2);
          PORTD_PCR8 = PORTD_PCR8|(1<<5);//����Ϊ��©ģʽ
        }
        else
            ;
        if(I2C0_SDA == PTB1)
        {
          PORTB_PCR1 = PORT_PCR_MUX(2);
          PORTB_PCR1 = PORTB_PCR1|(1<<5);//����Ϊ��©ģʽ
        }
        else if(I2C0_SDA == PTB3)
        {  
          PORTB_PCR3 = PORT_PCR_MUX(2);
          PORTE_PCR3 = PORTE_PCR3|(1<<5);//����Ϊ��©ģʽ
        }
        else if(I2C0_SDA == PTD9)
        {
          PORTD_PCR9 = PORT_PCR_MUX(2);
          PORTD_PCR9 = PORTD_PCR9|(1<<5);//����Ϊ��©ģʽ
        }
        else
           ;
    }
    else
    {
        /* ����ʱ�� */
        SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;         //���� I2C1ʱ��

        /* ���� I2C1���ܵ� GPIO �ӿ� */
        if(I2C1_SCL == PTE1)
        { PORTE_PCR1 = PORT_PCR_MUX(6);
          PORTE_PCR1 = PORTE_PCR1|(1<<5);//����Ϊ��©ģʽ
        }
        else if(I2C1_SCL == PTC10)
        {
          PORTC_PCR10 = PORT_PCR_MUX(2);
          PORTC_PCR10 = PORTC_PCR10|(1<<5);//����Ϊ��©ģʽ
        }
        else
            ;

        if(I2C1_SDA == PTE0)
        { PORTE_PCR0 = PORT_PCR_MUX(6);
          PORTE_PCR0 = PORTE_PCR0 | (1<<5);//���óɿ�©ģʽ   ֻ����SDA���ɣ�����������Ҳû��
        }
        else if (I2C1_SDA == PTC11)
        {
          PORTC_PCR11 = PORT_PCR_MUX(2);
          PORTC_PCR11 = PORTC_PCR11 | (1<<5);//����Ϊ��©ģʽ
        }
        else
          ;

    }

    /* ����Ƶ�� */
    I2C_F_REG(I2Cx[i2cn])  = I2C_F_MULT(0) | I2C_F_ICR(0) ;  //0x29
    // MULT=00  ��  mul = 1
    // ICR =14  ICRΪClock rateʱ��Ƶ��
    // �ӡ�k16 reference manual.pdf��P1460 �ɵã�
    // ICR    SCL Divider   SDA Hold Value    SCL Hold (Start) Value    SCL Hold (Stop) Value
    //  29       384              33                   190                        193
    //  17       128              21                   58                         65
    // I2C baud rate = bus speed (Hz)/(mul �� SCL divider)  ������ 90MHz/(1 ��128)=234.375kHz

    /* ʹ�� IIC1 */
    I2C_C1_REG(I2Cx[i2cn]) = I2C_C1_IICEN_MASK;
}

//-------------------------------------------------------------------------*
//������: I2C_StartTransmission                                                        
//��  ��: ����IIC����                                                        
//��  ��: i2cn    :�˿��� I2C0,I2C1 
//        SlaveID :�ӻ���ַ 
//        Mode    :����ģʽ
//��  ��: ��                                                              
//��  ��: IIC_init(I2C1);              
//-------------------------------------------------------------------------*
void I2C_StartTransmission (I2Cn i2cn, u8 SlaveID, MSmode Mode)
{


    SlaveID = ( SlaveID << 1 ) | Mode ;            //ȷ��д��ַ�Ͷ���ַ

    /* send start signal */
    I2C_Start(i2cn);

    /* send ID with W/R bit */
    I2C_write_byte(i2cn, SlaveID);
}

//-------------------------------------------------------------------------*
//������: Pause                                                        
//��  ��: ��ʱ                                                        
//��  ��: ��
//��  ��: ��                                                              
//��  ��: Pause;              
//-------------------------------------------------------------------------*
void Pause(void)
{
    u16 n;
    for(n = 1; n < 50000; n++)      //����̫С
    {
        asm("nop");
    }
}



//-------------------------------------------------------------------------*
//������: I2C_ReadAddr                                                        
//��  ��: ��ȡIIC�豸ָ����ַ�Ĵ���������                                                        
//��  ��: i2cn    :�˿��� I2C0,I2C1 
//        SlaveID :�ӻ���ַ 
//        Addr    :�ӻ��ļĴ�����ַ
//��  ��: result                                                              
//��  ��: p[0]  = I2C_ReadAddr(i2cn,SlaveID,OUT_X_MSB_REG);              
//-------------------------------------------------------------------------*
u8 I2C_ReadAddr(I2Cn i2cn, u8 SlaveID, u8 Addr)
{
    u8 result;

    /* Send Slave Address */
    I2C_StartTransmission (i2cn, SlaveID, write);
    I2C_Wait(i2cn);

    /* Write Register Address */
    I2C_write_byte(i2cn, Addr);
    I2C_Wait(i2cn);

    /* Do a repeated start */
    I2C_RepeatedStart(i2cn);

    /* Send Slave Address */
    I2C_write_byte(i2cn, ( SlaveID << 1) | read );
    I2C_Wait(i2cn);

    /* Put in Rx Mode */
    I2C_PutinRxMode(i2cn);

    /* Turn off ACK since this is second to last byte being read*/
    I2C_DisableAck(i2cn); //��Ӧ��

    /* Dummy read ��ٶ�ȡ*/
    result = I2C_D_REG(I2Cx[i2cn]);
    I2C_Wait(i2cn);

    /* Send stop since about to read last byte */
    I2C_Stop(i2cn);

    /* Read byte */
    result = I2C_D_REG(I2Cx[i2cn]);

    return result;
}


//-------------------------------------------------------------------------*
//������: I2C_ReadAddr                                                        
//��  ��: ��ȡIIC�豸ָ����ַ�Ĵ���������                                                        
//��  ��: i2cn    :�˿��� I2C0,I2C1 
//        SlaveID :�ӻ���ַ 
//        Addr    :�ӻ��ļĴ�����ַ
//        Data    :����
//��  ��: ��                                                              
//��  ��: I2C_WriteAddr(I2C1, SlaveAddress2100, CTRL_REG1_2100, 0x02);             
//-------------------------------------------------------------------------*
void I2C_WriteAddr(I2Cn i2cn, u8 SlaveID, u8 Addr, u8 Data)
{
    /* send data to slave */
    I2C_StartTransmission(i2cn, SlaveID, write);    //��������
    I2C_Wait(i2cn);

    I2C_write_byte(i2cn, Addr);                    //д��ַ
    I2C_Wait(i2cn);

    I2C_write_byte(i2cn, Data);                    //д����
    I2C_Wait(i2cn);

    I2C_Stop(i2cn);

    Pause();                                        //��ʱ̫�̵Ļ�������д����
}

