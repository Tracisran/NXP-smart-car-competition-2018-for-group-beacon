/******************** LQ_K60_������ v1.0 ********************
 * �ļ���           ��common.h
 * ����             ��
 * ��ע             ���ٷ��������޸�
 * ����             ��2016-09-01
 * ʵ��ƽ̨         ������ K60VG���İ� 
 * ����             �������� 006
 * �Ա���           ��https://longqiu.taobao.com
 * �������ܳ�����Ⱥ ��202949437
*************************************************************/
  
#ifndef _COMMON_H_
#define _COMMON_H_


#define ARM_INTERRUPT_LEVEL_BITS     4//�ж����ȼ��궨��
#define EnableInterrupts asm(" CPSIE i");//�����ж�
#define DisableInterrupts asm(" CPSID i");//�����ж�
    
 
/*****************************������������*****************************/  


typedef unsigned char	uint8;  /*  8 bits */
typedef unsigned short int	uint16; /* 16 bits */
typedef unsigned long int	uint32; /* 32 bits */

typedef char		int8;   /*  8 bits */
typedef short int	       	int16;  /* 16 bits */
typedef int		       	int32;  /* 32 bits */

typedef volatile int8	vint8;  /*  8 bits */
typedef volatile int16	vint16; /* 16 bits */
typedef volatile int32	vint32; /* 32 bits */

typedef volatile uint8	vuint8;  /*  8 bits */
typedef volatile uint16	vuint16; /* 16 bits */
typedef volatile uint32	vuint32; /* 32 bits */


typedef unsigned char         u8;	  //�޷�����
typedef unsigned short int    u16;
typedef unsigned int          u32;
typedef unsigned long int     u64;

typedef char		        s8;	  //�з�����
typedef short int	        s16;
typedef int		        s32;
typedef long int    	        s64;

#if defined( DEBUG )
#define ASSERT(expr) \
    if (!(expr)) \
        assert_failed(__FILE__, __LINE__)
#else
#define ASSERT(expr)
#endif
/*****************************������������*****************************/ 


typedef enum
{
    /*  PTA�˿�    */ //0~31
    PTA0,  PTA1,  PTA2,  PTA3,  PTA4,  PTA5,  PTA6,  PTA7,  PTA8,  PTA9,  PTA10, PTA11, PTA12, PTA13, PTA14, PTA15,
    PTA16, PTA17, PTA18, PTA19, PTA20, PTA21, PTA22, PTA23, PTA24, PTA25, PTA26, PTA27, PTA28, PTA29, PTA30, PTA31,

    /*  PTB�˿�    */ //32~63
    PTB0,  PTB1,  PTB2,  PTB3,  PTB4,  PTB5,  PTB6,  PTB7,  PTB8,  PTB9,  PTB10, PTB11, PTB12, PTB13, PTB14, PTB15,
    PTB16, PTB17, PTB18, PTB19, PTB20, PTB21, PTB22, PTB23, PTB24, PTB25, PTB26, PTB27, PTB28, PTB29, PTB30, PTB31,

    /*  PTC�˿�    */
    PTC0,  PTC1,  PTC2,  PTC3,  PTC4,  PTC5,  PTC6,  PTC7,  PTC8,  PTC9,  PTC10, PTC11, PTC12, PTC13, PTC14, PTC15,
    PTC16, PTC17, PTC18, PTC19, PTC20, PTC21, PTC22, PTC23, PTC24, PTC25, PTC26, PTC27, PTC28, PTC29, PTC30, PTC31,

    /*  PTD�˿�    */
    PTD0,  PTD1,  PTD2,  PTD3,  PTD4,  PTD5,  PTD6,  PTD7,  PTD8,  PTD9,  PTD10, PTD11, PTD12, PTD13, PTD14, PTD15,
    PTD16, PTD17, PTD18, PTD19, PTD20, PTD21, PTD22, PTD23, PTD24, PTD25, PTD26, PTD27, PTD28, PTD29, PTD30, PTD31,

    /*  PTE�˿�    */
    PTE0,  PTE1,  PTE2,  PTE3,  PTE4,  PTE5,  PTE6,  PTE7,  PTE8,  PTE9,  PTE10, PTE11, PTE12, PTE13, PTE14, PTE15,
    PTE16, PTE17, PTE18, PTE19, PTE20, PTE21, PTE22, PTE23, PTE24, PTE25, PTE26, PTE27, PTE28, PTE29, PTE30, PTE31,
} PTXn_e;



//-------------------------------------------------------------------------*
//������: stop                                                             *
//��  ��: ����CPUΪSTOPģʽ                                                * 
//-------------------------------------------------------------------------*
void stop (void);

//-------------------------------------------------------------------------*
//������: wait                                                             *
//��  ��: ����CPUΪWAITģʽ                                                * 
//-------------------------------------------------------------------------*
void wait (void);

//------------------------------------------------------------------------- *
//������: write_vtor                                                        *
//��  ��: �����ж�������ƫ�ƼĴ�����ֵ                                       * 
//��  ��: Ҫ���ĵ�ֵ    						           *	
//-------------------------------------------------------------------------*
void write_vtor (int);

//-------------------------------------------------------------------------*
//������: enable_irq                                                       *
//��  ��: ʹ��irq�ж�                                                      * 
//��  ��: irq:irq��       						  *	
//˵  ��: irq�Ų����ж�������                                              *
//-------------------------------------------------------------------------*
void enable_irq (int);

//-------------------------------------------------------------------------*
//������: disable_irq                                                      *
//��  ��: ��ֹirq�ж�                                                      * 
//��  ��: irq:irq��       				                  *	
//˵  ��: irq�Ų����ж�������                                              *
//-------------------------------------------------------------------------*
void disable_irq (int);

//-------------------------------------------------------------------------*
//������: set_irq_priority                                                 *
//��  ��: ����irq�жϺ����ȼ�                                              * 
//��  ��: irq:irq��         					          *	
//        prio:���ȼ�						          *	
//˵  ��: irq�Ų����ж�������                                              *
//-------------------------------------------------------------------------*
void set_irq_priority (int, int);

//-------------------------------------------------------------------------*
//������: main                                                             *
//��  ��: ����������                                                       * 
//-------------------------------------------------------------------------*
void main(void);

    
#endif 
