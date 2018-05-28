#include "LPC11XX.H"

#define LED      (1ul<<0)

#define LED1_ON()   (LPC_GPIO2->DATA&= ~(1<<0))
#define LED1_OFF()  (LPC_GPIO2->DATA|= (1<<0))

#define LED2_ON()  (LPC_GPIO2->DATA&= ~(1<<1))
#define LED2_OFF()  (LPC_GPIO2->DATA|= (1<<1))

#define KEY1_DOWN()  ((LPC_GPIO3->DATA&(1<<0)) != (1<<0))
#define KEY2_DOWN()  ((LPC_GPIO3->DATA&(1<<1))!= (1<<1))
static volatile unsigned int ticks;  //fFF

void LedInit(void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16); 
    LPC_IOCON->PIO2_0 &= ~0x07;    
    LPC_IOCON->PIO2_0 |= 0x00; 
    LPC_IOCON->PIO2_1 &= ~0x07;   
    LPC_IOCON->PIO2_1 |= 0x00; 
		LPC_IOCON->PIO2_2 &= ~0x07;    
    LPC_IOCON->PIO2_2 |= 0x00; 
    LPC_IOCON->PIO2_3 &= ~0x07;   
    LPC_IOCON->PIO2_3 |= 0x00;
		LPC_IOCON->PIO2_4 &= ~0x07;    
    LPC_IOCON->PIO2_4 |= 0x00; 
    LPC_IOCON->PIO2_5 &= ~0x07;   
    LPC_IOCON->PIO2_5 |= 0x00; 
		LPC_IOCON->PIO2_6 &= ~0x07;    
    LPC_IOCON->PIO2_6 |= 0x00; 
    LPC_IOCON->PIO2_7 &= ~0x07;   
    LPC_IOCON->PIO2_7 |= 0x00;
	
    LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);
    LPC_SYSCON->SYSAHBCLKCTRL |= ~(1<<6);
	
    
    LPC_GPIO2->DIR |= (1<<0); 
   // LPC_GPIO2->DATA |= (1<<0); 
    LPC_GPIO2->DIR |= (1<<1); 
   // LPC_GPIO2->DATA |= (1<<1);
		LPC_GPIO2->DIR |= (1<<2); 
   // LPC_GPIO2->DATA |= (1<<2); 
    LPC_GPIO2->DIR |= (1<<3); 
    //LPC_GPIO2->DATA |= (1<<3);
		LPC_GPIO2->DIR |= (1<<4); 
    //LPC_GPIO2->DATA |= (1<<4); 
    LPC_GPIO2->DIR |= (1<<5); 
   // LPC_GPIO2->DATA |= (1<<5);
		LPC_GPIO2->DIR |= (1<<6); 
   // LPC_GPIO2->DATA |= (1<<6); 
    LPC_GPIO2->DIR |= (1<<7); 
   // LPC_GPIO2->DATA |= (1<<7);
}

/*void PIOINT3_IRQHandler()
{
	if((LPC_GPIO3->MIS&(1<<0)) == (1<<0))
	{
		LED1_ON();
		while(KEY1_DOWN());
		LED1_OFF();
		LPC_GPIO3->IC = (1<<0);
	}
	if((LPC_GPIO3->MIS&(1<<1)) == (1<<1))
	{
		LED2_ON();
		while(KEY2_DOWN());
		LED2_OFF();
		LPC_GPIO3->IC = (1<<1);
	}
}*/

void TIMER32_0_IRQHandler(void)
{
	LPC_TMR32B0 -> IR = 0x01;//这个就是这个寄存器特殊的清零操作  老师说这是做芯片时候集成它们的规定 我们理解不了 这里是把1赋给这个寄存器竟然能达到清零的效果  详情见书165页最上面
	
	static int count = 0;//我定义了一个静态变量  用来计数的一个变量而已  意思是说 移位操作了7次以后该从头再来了  达到循环的效果
	if(count == 7)
	{
		LPC_GPIO2->DATA = 0XFE;//重新初始化寄存器
		count = 0;
		return ;//这里这个返回的意思是   没有它的话第一个灯在循环的时候不会亮 因为这里它刚亮 下面的代码移位了以后就让第二个灯亮了 所以这里要退出子函数
	}
	
	LPC_GPIO2->DATA = ~(~LPC_GPIO2->DATA << 1);//这里是用的双取反  因为是共阳极1是灭0是亮
//	所以最开始赋值是1111 1110取反0000 0001移位0000 0010再取反1111 1101 就是这样  如果不双取反的话直接移位就是从1111 1110 变成1111 1100 移位的初始位是自动补零的
	
	count++;//计数器自增
}

void Timer0Init (void)
{
	LPC_SYSCON -> SYSAHBCLKCTRL |= (1<<9);
	LPC_TMR32B0 -> IR = 0x01;
	LPC_TMR32B0 -> PR = 0;
	LPC_TMR32B0 -> MCR = 0x03;
	LPC_TMR32B0 -> MR0 = SystemCoreClock / 4;
	LPC_TMR32B0 -> TCR = 0x01;
	NVIC_EnableIRQ(TIMER_32_0_IRQn );
}

int main(void) 
{   
	 LedInit();// LED的初始化  这个不用说了把  把所有8个灯都初始化了
		
	Timer0Init () ;//定时器初始化
	
	LPC_GPIO2->DATA = 0XFE;//将这个寄存器里的值初始化为FE 也就是1111 1110这些灯是共阳极的 所以0是亮1是灭 这是初始化先让第一个灯亮 
	
    while(1);//使整个程序保持
}