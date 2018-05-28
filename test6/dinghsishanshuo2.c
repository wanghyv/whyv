#include"LPC11XX.H"
#define LED1_ON()  (LPC_GPIO2->DATA &= ~(1<<0))
#define LED1_OFF() (LPC_GPIO2->DATA |= (1<<0))

#define LED2_ON()  (LPC_GPIO2->DATA &= ~(1<<1))
#define LED2_OFF() (LPC_GPIO2->DATA |= (1<<1))

#define KEY1_DOWN() ((LPC_GPIO3->DATA & (1<<0)) != (1<<0))
//#define KEY2_DOWN() ((LPC_GPIO3->DATA & (1<<1))!=(1<<1))
unsigned int status;

void LedInit()
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16); // ʹ��IOCONʱ�� 
    LPC_IOCON->PIO2_0 &= ~0x07;    
    LPC_IOCON->PIO2_0 |= 0x00; //��P2.0������ΪGPIO
    LPC_IOCON->PIO2_1 &= ~0x07;   
    LPC_IOCON->PIO2_1 |= 0x00; //��P2.1������ΪGPIO
    LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16); // ����IOCONʱ��   
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);  //ʹ��AHBʱ�ӵ�GPIO��    
	
    LPC_GPIO2->DIR |= (1<<0); // ��P2.0����Ϊ�������
    LPC_GPIO2->DATA |= (1<<0); // ��P2.0����Ϊ�ߵ�ƽ
    LPC_GPIO2->DIR |= (1<<1); // ��P2.1����Ϊ�������
    LPC_GPIO2->DATA |= (1<<1); // ��P2.1����Ϊ�ߵ�ƽ
}

void SysTickInit()
{
	SysTick->CTRL &=~(1<<2);
	SysTick->LOAD =12499999;
	SysTick->VAL = 0;
	SysTick->CTRL |= ((1<<1)|(1<<0));
	NVIC_EnableIRQ(SysTick_IRQn);
}

void SysTick_Handler(void)
{
	status = LPC_GPIO2->DATA;
	LPC_GPIO2->DATA = ~status ;
}
/*************************/
void PIOINT3_IRQHandler()
{
    if((LPC_GPIO3->MIS & (1<<0))==(1<<0)) // �����P1.9������ж�
    {
        LED1_ON();
        while(KEY1_DOWN());
        LED1_OFF();
        LPC_GPIO3->IC = (1<<0);  // ���ж�
    }
    if((LPC_GPIO3->MIS & (1<<1))==(1<<1)) // �����P1.10������ж�
    {
        LED2_ON();
        while(KEY1_DOWN());
        LED2_OFF();
        LPC_GPIO3->IC = (1<<1);  // ���ж�
    }
}
int main(void)
{
	LedInit();
	SysTickInit();
	

    
   LPC_IOCON->PIO3_0 &= ~(0x07);//��P3_0����ΪGPIO
    LPC_GPIO3->DIR    &= ~(1<<1);//��P3_0����Ϊ����	
//    LPC_IOCON->PIO3_1 &= ~(0x07);//��P3_1����ΪGPIO
//    LPC_GPIO3->DIR &= ~(1<<1);//��P3_2����Ϊ����
	
    LPC_GPIO3->IE |= (1<<0); // ����P3.0�����ϵ��ж�
//    LPC_GPIO3->IE |= (1<<1); // ����P3.1�����ϵ��ж�
    LPC_GPIO3->IS &= ~(1<<0); // ����P3.0����Ϊ���ش���
//    LPC_GPIO3->IS &= ~(1<<1); // ����P3.1����Ϊ���ش���
    LPC_GPIO3->IEV &= ~(1<<0); // ����P3.0����Ϊ�½��ش���
//    LPC_GPIO3->IEV &= ~(1<<1); // ����P3.1����Ϊ�½��ش���
    NVIC_EnableIRQ(EINT3_IRQn); // ��GPIO3�ж�
	while(1)
	{
	}
}