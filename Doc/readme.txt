/* 代码例程 */


// 延时函数
// 系统时钟频率
#define SYSCLK_FREQ 72000000

// 初始化SysTick
void SysTick_Init(void) {
    // 设置SysTick的时钟源为系统时钟
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
}

// 微秒级延时函数
void delay_us(uint32_t us) {
    uint32_t ticks = us * (SYSCLK_FREQ / 1000000);
    SysTick->LOAD = ticks - 1;  // 设置计数值
    SysTick->VAL = 0;  // 清零当前计数值
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;  // 启动SysTick

    // 等待计数完成
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));

    SysTick->CTRL = 0;  // 关闭SysTick
}

// 毫秒级延时函数
void delay_ms(uint32_t ms) {
    uint32_t ticks = ms * (SYSCLK_FREQ / 1000);
    SysTick->LOAD = ticks - 1;  // 设置计数值
    SysTick->VAL = 0;  // 清零当前计数值
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;  // 启动SysTick

    // 等待计数完成
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));

    SysTick->CTRL = 0;  // 关闭SysTick
}

// 秒级延时函数
void delay_s(uint32_t s) {
    for (uint32_t i = 0; i < s; i++) {
        delay_ms(1000);  // 调用毫秒延时函数
    }
}

	/* 标准库配置 PA0 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);
	
	
	/* 位带操作 */
	int main(void){
		RCC->APB2ENR = 0x04;
		GPIOA->CRL = 0x03;
		OLED_Init();

		while(1){
			// PAout(0) = 1;
			// WritePin(A,0) = 1;
			TogglePin(A,0);
			OLED_ShowNum(1,1,ReadPin(A,0),4);
			Delay_ms(250);
			
			// PAout(0) = 0;
			// WritePin(A,0) = 1;
			TogglePin(A,0);
			OLED_ShowNum(1,1,ReadPin(A,0),4);
			Delay_ms(250);
		}
	}
	
	/* 配置寄存器 闪烁PC13 */
	int main(void){

		RCC->APB2ENR = 0x10;
		GPIOC->CRH = 0x00300000;
		/* GPIOC->ODR = 0X2000; */

		while(1){
			GPIOC->BSRR = 0X2000;
			Delay_ms(200);
			
			GPIOC->BRR = 0x2000;
			Delay_ms(200);
		}
	}
	
	
	
	
	
	/*	以上为托管代码	*/