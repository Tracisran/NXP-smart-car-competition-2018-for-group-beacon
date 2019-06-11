/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：UART.H
 * 功能             ：设置UART工作模式
 * 备注             ：官方例程上修改
 * 日期             ：2016-09-01
 * 实验平台         ：龙丘 K60VG核心板 
 * 作者             ：龙丘技术 006
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/

#ifndef __UART_H__
#define __UART_H__



//定义模块号
typedef enum
{
    UART0,
    UART1,
    UART2,
    UART3,
    UART4,
    UART5,

    UART_MAX,
} UARTn_e;

extern volatile struct UART_MemMap *UARTN[UART_MAX];

/**********************************  UART(引脚复用) ***************************************/
//      模块通道    端口          可选范围                          建议
#define UART0_RX    PTA15       //PTA1、PTA15、PTB16、PTD6         PTA1不要用（与J-LINK冲突）
#define UART0_TX    PTA14        //PTA2、PTA14、PTB17、PTD7        PTA2不要用（与J-LINK冲突）

#define UART1_RX    PTC3        //PTC3、PTE1
#define UART1_TX    PTC4        //PTC4、PTE0

#define UART2_RX    PTD2        //PTD2
#define UART2_TX    PTD3        //PTD3

#define UART3_RX    PTC16       //PTB10、PTC16、PTE5
#define UART3_TX    PTC17       //PTB11、PTC17、PTE4

#define UART4_RX    PTE25       //PTC14、PTE25
#define UART4_TX    PTE24       //PTC15、PTE24

#define UART5_RX    PTE9        //PTD8、PTE9
#define UART5_TX    PTE8        //PTD9、PTE8


/**********************************  UART(引脚复用) ***************************************/







/*********************** UART功能函数 **************************/
//初始化
extern void UART_Init      (UARTn_e, uint32 baud);             //初始化uartx模块

//接收相关代码
extern  char UART_Get_Char (UARTn_e uratn);                //等待接受1个字节
extern char UART_Query_Char (UARTn_e, char *ch);                //查询接收1个字符
extern char UART_Query_Str  (UARTn_e, char *str, uint32 max_len); //查询接收字符串
extern char UART_Query    (UARTn_e);                          //查询是否接收到一个字节
//发送相关代码
extern void UART_Put_Char   (UARTn_e, char ch);                 //发送1个字节
extern void UART_Put_Buff   (UARTn_e , s8 *buff, uint32 len);//发送len个字节buff
extern void UART_Put_Str    (UARTn_e , char *str);       //发送字符串

void UART_Irq_En(UARTn_e uratn);
void UART_Irq_Dis(UARTn_e uratn);
extern void UART0_IRQHandler(void);
extern void UART1_IRQHandler(void);
extern void UART2_IRQHandler(void);
extern void UART3_IRQHandler(void);
extern void UART4_IRQHandler(void);
extern void UART5_IRQHandler(void);
/********************************************************************/

#endif 
