/*
 作者：天下吾贼
 时间：2022年3月4日
 IDE版本号：1.8.13
 作用:用Arduino中的PID库，实现直流电机速度稳定在800r/min
*/
int motor_c_ENA = 6; // 控制板与驱动板的引脚连接

#define ENCODER_A_PIN 2 // 编码器A相接控制板2号引脚，对应0号外部中断
#define ENCODER_B_PIN 3 // 编码器B相接控制板3号引脚，
long pulse_number = 0;  // 脉冲计数
int rpm;

#include <MsTimer2.h> //定时器库的头文件
#include <PID_v1.h>

// Define Variables we'll be connecting to
double Setpoint, Input, Output;

// Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, 0.3, 0.2, 0.01, DIRECT);

void setup()
{
    pinMode(motor_c_ENA, OUTPUT); // 电机C使能和PWM调速口
    pinMode(11, OUTPUT);          // 数字口要选择带#号的具有pwm功能的输出口

    MsTimer2::set(500, send); // 中断设置函数，每 500ms 进入一次中断
    MsTimer2::start();        // 开始计时

    pinMode(ENCODER_A_PIN, INPUT);
    pinMode(ENCODER_B_PIN, INPUT);
    attachInterrupt(0, read_quadrature, FALLING); // EN_A脚下降沿触发中断
    Serial.begin(9600);                           // 初始化Arduino串口

    // initialize the variables we're linked to
    Input = rpm;
    Setpoint = 800;

    // turn the PID on
    myPID.SetMode(AUTOMATIC);
}

void loop()
{
    Input = rpm;
    myPID.Compute();
    analogWrite(motor_c_ENA, 255 - Output);
}

void send() // 速度串行传送
{
    rpm = int(pulse_number / 0.53);
    // 编码器精度为224线，减速器减速比为1:20，故系数=(16/(60/0.5))*1=0.53
    Serial.print("rpm: ");
    Serial.println(rpm, DEC);
    pulse_number = 0;
}

void read_quadrature() // 编码器脉冲计数中断函数
{
    if (digitalRead(ENCODER_A_PIN) == LOW)
    {
        if (digitalRead(ENCODER_B_PIN) == LOW) // 查询EN_B的电平以确认正转
        {
            pulse_number++;
        }
        if (digitalRead(ENCODER_B_PIN) == HIGH) // 查询EN_B的电平以确认反转
        {
            pulse_number--;
        }
    }
}