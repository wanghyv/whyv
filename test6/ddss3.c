#include "LPC11xx.h"                             //LPC1114ͷ�ļ�
#define LED         (1ul << 0)
#define LEDOFF()    LPC_GPIO1->DATA |=  LED  //Ϩ��LED
#define LEDON()     LPC_GPIO1->DATA &= ~LED  //����LED
void LedInit()
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);       //ʹ��IOCONʱ�� 
    LPC_IOCON->R_PIO1_0 &= ~0x07;    
    LPC_IOCON->R_PIO1_0 |= 0x01;                            //��P1.0������ΪGPIO
    LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<16);      //����IOCONʱ��
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);            //ʹ��GPIOʱ��
    LPC_GPIO1->DIR |= LED;                                      //��P1.0����Ϊ�������
    LPC_GPIO1->DATA |= LED;                                     //��P1.0����Ϊ�ߵ�ƽ
}
void TIMER32_0_IRQHandler (void)
{
    LPC_TMR32B0->IR = 0x01;                                 //��ƥ��ͨ��0д1����ж�
    if (LPC_GPIO1->DATA & LED) 
    {
            LEDON();                                                        //����LED
    } 
    else 
    {
            LEDOFF();                                                       //Ϩ��LED
    }
}
void Timer0Init (void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 9);  //ʹ��TIM32B0ʱ��
    LPC_TMR32B0->IR      = 0x01; //MR0�жϸ�λ,�����жϣ�bit0:MR0, bit1:MR1, bit2:MR2, bit3:MR3, bit4:CP0��
    LPC_TMR32B0->PR      = 0;                           //ÿһ��PCLK��1
    LPC_TMR32B0->MCR     = 0x03;                    //MR0ƥ��ʱ��λTC���������ж�
    LPC_TMR32B0->MR0     = SystemCoreClock; //ƥ��ʱ�䣬Ҳ���Ƕ�ʱʱ��
    LPC_TMR32B0->TCR     = 0x01;                    //������ʱ����TCR[0]=1;
    NVIC_EnableIRQ(TIMER_32_0_IRQn);            //ʹ���ж�
}
int main(void)
{
    LedInit();
    Timer0Init();
    while (1) ;
}

