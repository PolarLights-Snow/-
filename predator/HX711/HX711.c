/************************************************************************************
						
*************************************************************************************/
#include "HX711.h"
//#include "delay.h"

int HX711_Buffer;
int Weight_Maopi;
int Weight_Shiwu;
int Flag_Error = 0;

//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValue 106.5

int get_HX711data(void)
{		
	Init_HX711pin();
	
//	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
//	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	
	Get_Maopi();				//��ëƤ����
	HAL_Delay(1000);
	HAL_Delay(1000);
	Get_Maopi();				//���»�ȡëƤ����
	
		Get_Weight();

//		print("������ = %d g\r\n",Weight_Shiwu); //��ӡ 
//		HAL_Delay(1000);

return Weight_Shiwu;
	
}
void Init_HX711pin()
{
	HAL_GPIO_WritePin(HX711_DOUT_GPIO_Port,HX711_DOUT_Pin,0);
	HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,0);
}



//****************************************************
//��ȡHX711
//****************************************************
int HX711_Read(void)	//����128
{
	unsigned long count; 
	unsigned char i; 
//  	HX711_DOUT=1; 
	HAL_GPIO_WritePin(HX711_DOUT_GPIO_Port,HX711_DOUT_Pin,1);
	HAL_Delay(1);
//  	HX711_SCK=0; 
	HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,0);
  	count=0; 
  	while(HAL_GPIO_ReadPin(HX711_DOUT_GPIO_Port,HX711_DOUT_Pin)); 
  	for(i=0;i<24;i++)
	{ 
//	  	HX711_SCK=1; 
		HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,1);
	  	count=count<<1; 
		HAL_Delay(1);
//		HX711_SCK=0; 
		HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,0);

		HAL_GPIO_WritePin(DJ_GPIO_Port,DJ_Pin,1);
	  	if(HAL_GPIO_ReadPin(HX711_DOUT_GPIO_Port,HX711_DOUT_Pin))
			count++; 
		HAL_Delay(1);
	} 
// 	HX711_SCK=1; 
	HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,1);
    count=count^0x800000;//��25�������½�����ʱ��ת������
	HAL_Delay(1);
//	HX711_SCK=0; 
	HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,0);	
	return(count);
}

//****************************************************
//��ȡëƤ����
//****************************************************
void Get_Maopi(void)
{
	Weight_Maopi = HX711_Read();	
} 

//****************************************************
//����
//****************************************************
void Get_Weight(void)
{
	HX711_Buffer = HX711_Read();
	if(HX711_Buffer > Weight_Maopi)			
	{
		Weight_Shiwu = HX711_Buffer;
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//��ȡʵ���AD������ֵ��
	
		Weight_Shiwu = (int)((float)Weight_Shiwu/GapValue); 	//����ʵ���ʵ������
																		//��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������GapValue���������
																		//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
																		//������Գ���������ƫСʱ����С����ֵ��
	}

	
}
