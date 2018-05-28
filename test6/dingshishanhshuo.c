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
	LPC_TMR32B0 -> IR = 0x01;//�����������Ĵ���������������  ��ʦ˵������оƬʱ�򼯳����ǵĹ涨 ������ⲻ�� �����ǰ�1��������Ĵ�����Ȼ�ܴﵽ�����Ч��  �������165ҳ������
	
	static int count = 0;//�Ҷ�����һ����̬����  ����������һ����������  ��˼��˵ ��λ������7���Ժ�ô�ͷ������  �ﵽѭ����Ч��
	if(count == 7)
	{
		LPC_GPIO2->DATA = 0XFE;//���³�ʼ���Ĵ���
		count = 0;
		return ;//����������ص���˼��   û�����Ļ���һ������ѭ����ʱ�򲻻��� ��Ϊ���������� ����Ĵ�����λ���Ժ���õڶ��������� ��������Ҫ�˳��Ӻ���
	}
	
	LPC_GPIO2->DATA = ~(~LPC_GPIO2->DATA << 1);//�������õ�˫ȡ��  ��Ϊ�ǹ�����1����0����
//	�����ʼ��ֵ��1111 1110ȡ��0000 0001��λ0000 0010��ȡ��1111 1101 ��������  �����˫ȡ���Ļ�ֱ����λ���Ǵ�1111 1110 ���1111 1100 ��λ�ĳ�ʼλ���Զ������
	
	count++;//����������
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
	 LedInit();// LED�ĳ�ʼ��  �������˵�˰�  ������8���ƶ���ʼ����
		
	Timer0Init () ;//��ʱ����ʼ��
	
	LPC_GPIO2->DATA = 0XFE;//������Ĵ������ֵ��ʼ��ΪFE Ҳ����1111 1110��Щ���ǹ������� ����0����1���� ���ǳ�ʼ�����õ�һ������ 
	
    while(1);//ʹ�������򱣳�
}