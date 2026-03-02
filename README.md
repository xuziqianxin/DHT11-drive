# DHT11-drive
DHT11的驱动库，移植快捷
使用示例（本驱动库支持多机管理）：
```
// 使用官方例程 CH32V307EVT\EVT\EXAM\GPIO\GPIO_Toggle
#include "DHT11.h"

DHT11_Typedef DHT11 = {(uint32_t *)GPIOA, GPIO_Pin_0};
double temperature = 0;
double humidity = 0;
uint32_t Ecode = 0;

int main()
{
  // 省略初始化......
  while(1)
  {
        Ecode = DHT11_Get_Value(DHT11, &temperature, &humidity);
        printf("Ecode %d T: %.2lf H: %.2lf\n", Ecode, temperature, humidity);
        Delay_Ms(100);
  }
}
```
移植时需要注意 US_CORRECTION_PARAMETER 设置是否正确，如果出现错误码5尝试修改加大直到正确稳定输出数据。
