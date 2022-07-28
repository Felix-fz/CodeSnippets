#include "l298n.h"

void TIM2_PWM_Init(u16 arr,u16 psc)
{
	//**�ṹ������**//
	GPIO_InitTypeDef GPIO_InitStructure;            //����GPIO
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; //������ʱ��
	TIM_OCInitTypeDef  TIM_OCInitStructure;         //����PWMͨ��
	
	//**ʱ��ʹ��**//
	RCC_APB1PeriphClockCmd(L298N_EN_CLK, ENABLE);	//ʹ�ܶ�ʱ��TIM2ʱ��
	RCC_APB2PeriphClockCmd(L298N_CLK, ENABLE);	 //ʹ��PA�˿�ʱ��
	
	//**PWM����**//
	GPIO_InitStructure.GPIO_Pin = EA | EB;				 //TIM2_CH1 --> PA0 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(L298N_EN_GPIO, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.0
	
	//**IN����**//	
	GPIO_InitStructure.GPIO_Pin = IN1	| IN2 | IN3 | IN4;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(L298N_GPIO, &GPIO_InitStructure);					 
	
	//��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(L298N_PWM_TIM, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	//��ʼ��TIM2 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
	TIM_OCInitStructure.TIM_Pulse = 50; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(L298N_PWM_TIM, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC1
	TIM_OC2Init(L298N_PWM_TIM, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC2

	TIM_OC1PreloadConfig(L298N_PWM_TIM, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	TIM_OC2PreloadConfig(L298N_PWM_TIM, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��
	
  TIM_ARRPreloadConfig(L298N_PWM_TIM, ENABLE); //ʹ��TIM2��ARR�ϵ�Ԥװ�ؼĴ���
 	
	TIM_Cmd(L298N_PWM_TIM, ENABLE);  //ʹ��TIM2   
	TIM_CtrlPWMOutputs(L298N_PWM_TIM,ENABLE);	//MOE �����ʹ��	
}

void L_MOTO(char state)
{
	if(state == GO)
	{
		IN1_HIGH;
		IN2_LOW;
	}
	if(state == BACK)
	{
		IN1_LOW;
		IN2_HIGH;
	}
	if(state == STOP)
	{
		IN1_LOW;
		IN2_LOW;
	}
}

void R_MOTO(char state)
{
	if(state == GO)
	{
		IN3_HIGH;
		IN4_LOW;
	}
	if(state == BACK)
	{
		IN3_LOW;
		IN4_HIGH;
	}
	if(state == STOP)
	{
		IN3_LOW;
		IN4_LOW;
	}	
}

void Car_Go(void)
{
	L_MOTO(GO);
	R_MOTO(GO);
}

void Car_Back(void)
{
	L_MOTO(BACK);
	R_MOTO(BACK);	
}

void Car_Turn_Left(void)
{
	L_MOTO(STOP);
	R_MOTO(GO);
}

void Car_Left(void)
{
	L_MOTO(BACK);
	R_MOTO(GO);
}

void Car_Turn_Right(void)
{
	L_MOTO(GO);
	R_MOTO(STOP);	
}

void Car_Right(void)
{
	L_MOTO(GO);
	R_MOTO(BACK);	
}

void Car_Stop(void)
{
	L_MOTO(STOP);
	R_MOTO(STOP);
}

//�����ٶ�
void setSpeed(u16 LSpeed , u16 RSpeed)
{
	TIM_SetCompare1(L298N_PWM_TIM , LSpeed);
	TIM_SetCompare2(L298N_PWM_TIM , RSpeed);
}

void Forward(u16 speed)
{
	setSpeed(speed, speed);
	Car_Go();
}
void Back(u16 speed)
{
	setSpeed(speed , speed);
	Car_Back();
}

void Stop(void)
{
	setSpeed(0 , 0);
	Car_Stop();
}

void LCorrect(u16 speed)//ǰ����������
{
	setSpeed(speed , speed);
//	Car_Left();
	Car_Turn_Left();
}
void LLCorrect(u16 speed)//ǰ������������
{
	setSpeed(speed , speed+4);
//	Car_Left();
	Car_Turn_Left();
}
void RCorrect(u16 speed)//ǰ����������
{
	setSpeed(speed , speed);
//	Car_Right();
	Car_Turn_Right();
}

void RRCorrect(u16 speed)//ǰ�����Ҵ������
{
	setSpeed(speed+4 , speed);
//	Car_Right();
	Car_Turn_Right();
}

void Left(u16 speed)
{
	setSpeed(speed, speed);
	Car_Left();
}

void Right(u16 speed)
{
	setSpeed(speed , speed);
	Car_Right();
}
