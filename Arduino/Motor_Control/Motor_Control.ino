/*AB相正交霍尔传感器驱动电机*/
#include <MsTimer2.h>
#include <PID_v2.h>

// 声明函数
void readspeed();   // read speed from serial
void count_pulse(); // count pulse
float get_rpm();    // calculate rpm

// 声明引脚
const int motorPin_ENA = 3;   // 控制板使能信号
const int motorPin_int3 = 14; // 控制板方向信号
const int motorPin_int4 = 15; // 控制板方向信号
const int Hal_c1 = 18;        // 霍尔传感器引脚c1
const int Hal_c2 = 19;        // 霍尔传感器引脚c2

// 声明变量
const int ppr = 7;         // 脉冲数
const int gear_ratio = 30; // 传动比
const int max_speed = 730; // 最大转速

volatile long count = 0; // 脉冲计数
float rpm = 0;           // 电机转速
float Exp_speed = 0;     // 期望转速

double Setpoint, Input, Output; // PID参数

PID myPID(&Input, &Output, &Setpoint, 0.3, 0.2, 0.01, DIRECT);

void setup()
{
    pinMode(motorPin_ENA, OUTPUT);                                        // 设置引脚为输出
    pinMode(motorPin_int3, OUTPUT);                                       // 设置引脚为输出
    pinMode(motorPin_int4, OUTPUT);                                       // 设置引脚为输出
    pinMode(Hal_c1, INPUT_PULLUP);                                        // 设置引脚为输入
    pinMode(Hal_c2, INPUT_PULLUP);                                        // 设置引脚为输入
    attachInterrupt(digitalPinToInterrupt(Hal_c1), count_pulse, FALLING); // 中断函数
    Serial.println("please input the motor speed (0-780):");

    Input = get_rpm();    // 输入等于读取的转速
    Setpoint = Exp_speed; // 期望转速等于输入的转速

    myPID.Start(Input, 0, Setpoint); // 开始PID，输入，起始输出，期望值

    Serial.begin(9600);                             // 串口初始化
    Serial.println("Serial communication started"); // 串口通信开始
}

void loop()
{
    Input = get_rpm(); // 输入等于读取的转速
    Output = myPID.Run(Input);
    analogWrite(motorPin_ENA, 255 - Output); // 输出PWM信号

    readspeed(); // 读取期望转速
    // set motor direction
    // digitalWrite(motorPin_int3, HIGH);
    // digitalWrite(motorPin_int4, LOW);
    // set pwm signal
    analogWrite(motorPin_ENA, );
    // check hall sensor
    int Hal_c1_state = digitalRead(Hal_c1);
    int Hal_c2_state = digitalRead(Hal_c2);
    Serial.print("Hal_c1_state: ");
    Serial.print(Hal_c1_state);
    Serial.print(" Hal_c2_state: ");
    Serial.println(Hal_c2_state);
    delay(100);
}

float readspeed() // 从串口读取预期转速
{
    if (Serial.available() > 0)
    {
        Exp_speed = Serial.parseInt();
        if (Exp_speed > max_speed)
        {
            Exp_speed = max_speed;
        }
        else if (Exp_speed < 0)
        {
            Exp_speed = 0;
        }
        return Exp_speed;
    }
}

float get_rpm() // get rpm
{
    rpm = (float)count * 60 * 10 / (ppr * gear_ratio);
    count = 0;
    return rpm;
}

void count_pulse() // count pulse
{
    if (digitalRead(Hal_c1) == LOW)
    {
        if (digitalRead(Hal_c2) == LOW)
        {
            count++;
        }
        else
        {
            count--;
        }
    }
}