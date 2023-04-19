const int motorPin_pwm = 3; // PWM引脚，用于控制电机速度
const int motorPin_A = 14;  // 正转引脚
const int motorPin_B = 15;  // 反转引脚
const int Hal_c1 = 18;      // 霍尔编码器引脚
const int Hal_c2 = 19;      // 霍尔编码器引脚
const int ppr = 7;          // 每转脉冲数
const int gear_ratio = 30;  // 减速比
const int max_speed = 380;  // 最大转速（RPM）

volatile int count = 0; // 脉冲计数器
int rpm = 0;            // 电机转速
int speed = 0;          // 目标转速

void setup()
{
    Serial.begin(9600);                                                  // 开启串口通信，波特率9600
    Serial.println("Serial communication started");                      // 打印串口通信开始信息
    pinMode(motorPin_pwm, OUTPUT);                                       // 将PWM引脚设为输出模式
    pinMode(motorPin_A, OUTPUT);                                         // 将正转引脚设为输出模式
    pinMode(motorPin_B, OUTPUT);                                         // 将反转引脚设为输出模式
    pinMode(Hal_c1, INPUT_PULLUP);                                       // 将霍尔编码器引脚设为输入模式，上拉输入
    pinMode(Hal_c2, INPUT_PULLUP);                                       // 将霍尔编码器引脚设为输入模式，上拉输入
    attachInterrupt(digitalPinToInterrupt(Hal_c1), count_pulse, RISING); // 霍尔编码器引脚上升沿触发中断计数
    Serial.println("Please enter the desired speed:");                   // 打印提示信息
}

void loop()
{
    if (Serial.available() > 0)
    {                                           // 如果串口缓冲区中有数据
        speed = Serial.parseInt();              // 读取数据并转换为整型
        speed = constrain(speed, 0, max_speed); // 将目标转速限制在0到最大转速之间
        Serial.print("Speed set to: ");         // 打印提示信息
        Serial.println(speed);                  // 打印目标转速
    }
    // 控制电机正反转和速度
    if (rpm < speed)
    {                                   // 如果当前转速小于目标转速
        digitalWrite(motorPin_A, HIGH); // 将正转引脚设为高电平
        digitalWrite(motorPin_B, LOW);  // 将反转引脚设为低电平
        analogWrite(motorPin_pwm, 255); // 设置PWM信号为最大值（255）
    }
    else if (rpm > speed)
    {                                   // 如果当前转速大于目标转速
        digitalWrite(motorPin_A, LOW);  // 将正转引脚设为低电平
        digitalWrite(motorPin_B, HIGH); // 将反转引脚设为高电平
        analogWrite(motorPin_pwm, 255); // 设置PWM信号为最大值（255）
    }
    else
    {                                  // 如果当前转速等于目标转速
        digitalWrite(motorPin_A, LOW); // 将正转引脚设为低电平
        digitalWrite(motorPin_B, LOW); // 将反转引脚设为低电平
        analogWrite(motorPin_pwm, 0);  // 设置PWM信号为0
    }
    // 计算电机转速
    rpm = (count * 60 * 10) / (ppr * gear_ratio); // 计算转速
    rpm = min(rpm, max_speed);                    // 转速限制在最大转速以内
    Serial.print("rpm: ");                        // 打印转速信息
    Serial.println(rpm);                          // 打印转速信息
    // 重置脉冲计数器
    count = 0;
    delay(100); // 延时100ms
}

void count_pulse()
{
    count++; // 脉冲计数器加1
}
