//-------------------------------------------------------------------------*
// �ļ���:sysinit.c                                                        *
// ˵  ��: ϵͳ�����ļ�                                                    *
//-------------------------------------------------------------------------*

#include "sysinit.h"	//ͷ�ļ�
#include "include.h"
#include "PLL.h"

//ȫ�ֱ�������
int core_clk_khz;
int core_clk_mhz;
int periph_clk_khz;

//-------------------------------------------------------------------------*
//������: sysinit                                                          *
//��  ��: ϵͳ����                                                         * 
//��  ��: ��						  	           *	
//��  ��: ��                                                               *
//˵  ��: ��                                                               *
//-------------------------------------------------------------------------*
void sysinit (void)
{
    //ʹ��IO�˿�ʱ��    
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                              | SIM_SCGC5_PORTB_MASK
                              | SIM_SCGC5_PORTC_MASK
                              | SIM_SCGC5_PORTD_MASK
                              | SIM_SCGC5_PORTE_MASK );

    //����ϵͳʱ��
    core_clk_mhz = pll_init(PLL180);
    //ͨ��pll_init�����ķ���ֵ�������ں�ʱ�Ӻ�����ʱ��
    core_clk_khz = core_clk_mhz * 1000;
    periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
    //ʹ�ܸ���ʱ�ӣ����ڵ���
    trace_clk_init();	
    //FlexBusʱ�ӳ�ʼ��
    fb_clk_init();

}

//-------------------------------------------------------------------------*
//������: trace_clk_init                                                   *
//��  ��: ����ʱ�ӳ�ʼ��                                                   * 
//��  ��: ��							  	   *	
//��  ��: ��                                                               *
//˵  ��: ���ڵ���                                                         *
//-------------------------------------------------------------------------*
void trace_clk_init(void)
{
    //���ø���ʱ��Ϊ�ں�ʱ��
    SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;	
    //��PTA6������ʹ��TRACE_CLKOU����
    PORTA_PCR6 = ( PORT_PCR_MUX(0x7));
}

//-------------------------------------------------------------------------*
//������: fb_clk_init                                                      *
//��  ��: FlexBusʱ�ӳ�ʼ��                                                * 
//��  ��: ��								   *	
//��  ��: ��                                                               *
//˵  ��:                                                                  *
//-------------------------------------------------------------------------*
void fb_clk_init(void)
{
    //ʹ��FlexBusģ��ʱ��
    SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;
    //��PTA6������ʹ��FB_CLKOUT����
    PORTC_PCR3 = ( PORT_PCR_MUX(0x5));
}

//-------------------------------------------------------------------------*
//������: pll_init                                                         *
//��  ��: pll��ʼ��                                                        * 
//��  ��: clk_option:ʱ��ѡ��						   * 
//		  crystal_val:ʱ��ֵ                                       *	
//��  ��: ʱ��Ƶ��ֵ                                                       *
//˵  ��:                                                                  *
//-------------------------------------------------------------------------*
unsigned char pll_init(unsigned char clk_option, unsigned char crystal_val)
{
    unsigned char pll_freq;
    
    if (clk_option > 3) {return 0;}   //���û��ѡ����õ�ѡ���򷵻�0
    if (crystal_val > 15) {return 1;} // ���������õľ���ѡ������򷵻�1
    
    //���ﴦ��Ĭ�ϵ�FEIģʽ
    //�����ƶ���FBEģʽ
    #if (defined(K60_CLK) || defined(ASB817))
             MCG_C2 = 0;
    #else
             //ʹ���ⲿ����
             MCG_C2 = MCG_C2_RANGE(2) | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK;
    #endif
    
    //��ʼ��������ͷ�����״̬��������GPIO
    SIM_SCGC4 |= SIM_SCGC4_LLWU_MASK;
    LLWU_CS |= LLWU_CS_ACKISO_MASK;
    
    //ѡ���ⲿ���񣬲ο���Ƶ������IREFS�������ⲿ����
    MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);
    
    //�ȴ������ȶ�	
    #if (!defined(K60_CLK) && !defined(ASB817))
    while (!(MCG_S & MCG_S_OSCINIT_MASK)){};  
    #endif
    
    //�ȴ��ο�ʱ��״̬λ����
    while (MCG_S & MCG_S_IREFST_MASK){}; 
    //�ȴ�ʱ��״̬λ��ʾʱ��Դ�����ⲿ�ο�ʱ��
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}; 
    
    //����FBEģʽ
    #if (defined(K60_CLK))
    MCG_C5 = MCG_C5_PRDIV(0x18);
    #else
    
    //����PLL��Ƶ����ƥ�����õľ���
    MCG_C5 = MCG_C5_PRDIV(crystal_val); 
    #endif
    
    //ȷ��MCG_C6���ڸ�λ״̬����ֹLOLIE��PLL����ʱ�ӿ���������PLL VCO��Ƶ��
    MCG_C6 = 0x0;
    //ѡ��PLL VCO��Ƶ����ϵͳʱ�ӷ�Ƶ��ȡ����ʱ��ѡ��
    switch (clk_option) {
    case 0:
      //����ϵͳ��Ƶ��
      //MCG=PLL, core = MCG, bus = MCG, FlexBus = MCG, Flash clock= MCG/2
      set_sys_dividers(0,0,0,1);
      //����VCO��Ƶ����ʹ��PLLΪ50MHz, LOLIE=0, PLLS=1, CME=0, VDIV=1
      MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(1); //VDIV = 1 (x25)
      pll_freq = 50;
      break;
    case 1:
      //����ϵͳ��Ƶ��
      //MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
      set_sys_dividers(0,1,1,3);
      //����VCO��Ƶ����ʹ��PLLΪ100MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26
      MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(26); //VDIV = 26 (x50)
      pll_freq = 100;
      break;
    case 2:
      //����ϵͳ��Ƶ��
      //MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
      set_sys_dividers(0,1,1,3);
      //����VCO��Ƶ����ʹ��PLLΪ96MHz, LOLIE=0, PLLS=1, CME=0, VDIV=24
      MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(24); //VDIV = 24 (x48)
      pll_freq = 96;
      break;
    case 3:
      //����ϵͳ��Ƶ��
      //MCG=PLL, core = MCG, bus = MCG, FlexBus = MCG, Flash clock= MCG/2
      set_sys_dividers(0,0,0,1);
      //����VCO��Ƶ����ʹ��PLLΪ48MHz, LOLIE=0, PLLS=1, CME=0, VDIV=0
      MCG_C6 = MCG_C6_PLLS_MASK; //VDIV = 0 (x24)
      pll_freq = 48;
      break;
    }
    while (!(MCG_S & MCG_S_PLLST_MASK)){}; // wait for PLL status bit to set
    
    while (!(MCG_S & MCG_S_LOCK_MASK)){}; // Wait for LOCK bit to set
    
    //����PBEģʽ
    
    //ͨ������CLKSλ������PEEģʽ
    // CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
    MCG_C1 &= ~MCG_C1_CLKS_MASK;
    
    //�ȴ�ʱ��״̬λ����
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){};
    
    //��ʼ����PEEģʽ
    
    return pll_freq;
}

//-------------------------------------------------------------------------*
//������: set_sys_dividers                                                 *
//��  ��: ����ϵϵͳ��Ƶ��                                                 * 
//��  ��: Ԥ��Ƶֵ   							   *	
//��  ��: ��                                                               *
//˵  ��: �˺����������RAM��ִ�У�������������e2448����FLASHʱ�ӷ�Ƶ�ı�* 
//        ʱ�������ֹFLASH��Ԥȡ���ܡ���ʱ�ӷ�Ƶ�ı�֮�󣬱�����ʱһС��ʱ*
//	 ��ſ��Դ���ʹ��Ԥȡ���ܡ�                                        * 
//-------------------------------------------------------------------------*
__ramfunc void set_sys_dividers(uint32 outdiv1, uint32 outdiv2, uint32 outdiv3, uint32 outdiv4)
{
    uint32 temp_reg;
    uint8 i;
    //����FMC_PFAPR��ǰ��ֵ
    temp_reg = FMC_PFAPR;
    
    //ͨ��M&PFD��λM0PFD����ֹԤȡ����
    FMC_PFAPR |= FMC_PFAPR_M7PFD_MASK | FMC_PFAPR_M6PFD_MASK | FMC_PFAPR_M5PFD_MASK
                     | FMC_PFAPR_M4PFD_MASK | FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
                     | FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;
    
    //��ʱ�ӷ�Ƶ����������ֵ  
    SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(outdiv1) | SIM_CLKDIV1_OUTDIV2(outdiv2) 
                      | SIM_CLKDIV1_OUTDIV3(outdiv3) | SIM_CLKDIV1_OUTDIV4(outdiv4);
    
    //�ȴ���Ƶ���ı�
    for (i = 0 ; i < outdiv4 ; i++)
    {}
    
    //���´�FMC_PFAPR��ԭʼֵ
    FMC_PFAPR = temp_reg; 
    
    return;
}






