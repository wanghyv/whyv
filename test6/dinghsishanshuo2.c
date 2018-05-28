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
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16); // 使能IOCON时钟 
    LPC_IOCON->PIO2_0 &= ~0x07;    
    LPC_IOCON->PIO2_0 |= 0x00; //把P2.0脚设置为GPIO
    LPC_IOCON->PIO2_1 &= ~0x07;   
    LPC_IOCON->PIO2_1 |= 0x00; //把P2.1脚设置为GPIO
    LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16); // 禁能IOCON时钟   
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);  //使能AHB时钟到GPIO域。    
	
    LPC_GPIO2->DIR |= (1<<0); // 把P2.0设置为输出引脚
    LPC_GPIO2->DATA |= (1<<0); // 把P2.0设置为高电平
    LPC_GPIO2->DIR |= (1<<1); // 把P2.1设置为输出引脚
    LPC_GPIO2->DATA |= (1<<1); // 把P2.1设置为高电平
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
    if((LPC_GPIO3->MIS & (1<<0))==(1<<0)) // 如果是P1.9引起的中断
    {
        LED1_ON();
        while(KEY1_DOWN());
        LED1_OFF();
        LPC_GPIO3->IC = (1<<0);  // 清中断
    }
    if((LPC_GPIO3->MIS & (1<<1))==(1<<1)) // 如果是P1.10引起的中断
    {
        LED2_ON();
        while(KEY1_DOWN());
        LED2_OFF();
        LPC_GPIO3->IC = (1<<1);  // 清中断
    }
}
int main(void)
{
	LedInit();
	SysTickInit();
	

    
   LPC_IOCON->PIO3_0 &= ~(0x07);//将P3_0设置为GPIO
    LPC_GPIO3->DIR    &= ~(1<<1);//将P3_0设置为输入	
//    LPC_IOCON->PIO3_1 &= ~(0x07);//将P3_1设置为GPIO
//    LPC_GPIO3->DIR &= ~(1<<1);//将P3_2设置为输入
	
    LPC_GPIO3->IE |= (1<<0); // 允许P3.0引脚上的中断
//    LPC_GPIO3->IE |= (1<<1); // 允许P3.1引脚上的中断
    LPC_GPIO3->IS &= ~(1<<0); // 设置P3.0引脚为边沿触发
//    LPC_GPIO3->IS &= ~(1<<1); // 设置P3.1引脚为边沿触发
    LPC_GPIO3->IEV &= ~(1<<0); // 设置P3.0引脚为下降沿触发
//    LPC_GPIO3->IEV &= ~(1<<1); // 设置P3.1引脚为下降沿触发
    NVIC_EnableIRQ(EINT3_IRQn); // 打开GPIO3中断
	while(1)
	{
	}
}