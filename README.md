# DHT11-drive
DHT11的驱动库，移植快捷
使用示例（本驱动库支持多机管理）：
```
#include "stdio.h"
#include "DHT11.h"

DHT11_Typedef DHT11 = { 0 };
double temperature = 0;
double humidity = 0;
uint8_t string[20] = { 0 };
uint32_t retval = 0;

int main()
{
  // 省略初始化......
  DHT11.GPIO_Port = (uint32_t *)GPIOA;
  while(1)
  {
    // 1号脚的DHT11
		DHT11.GPIO_Pin = GPIO_PIN_1;
		retval = DHT11_Get_Value(DHT11, &temperature, &humidity);
		sprintf((char *)string, "%d T:%.2lf	H:%.2lf \n", retval, temperature, humidity);
		HAL_UART_Transmit(&huart1, string, sizeof(string), 0xff);

    // 2号脚的DHT11
		DHT11.GPIO_Pin = GPIO_PIN_2;
		retval = DHT11_Get_Value(DHT11, &temperature, &humidity);
		sprintf((char *)string, "%d T:%.2lf	H:%.2lf \n", retval, temperature, humidity);
		HAL_UART_Transmit(&huart1, string, sizeof(string), 0xff);
		HAL_Delay(200);
  }
}
```
移植时需要注意 US_CORRECTION_PARAMETER 设置是否正确，如果出现错误码5尝试修改加大直到正确稳定输出数据。
