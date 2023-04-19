const int motorPin_pwm = 3; // PWM pin
const int motorPin_A = 14;  // direction pin
const int motorPin_B = 15;  // direction pin
const int Hal_c1 = 18;      // Hall sensor pin
const int Hal_c2 = 19;      // Hall sensor pin

void setup()
{
    Serial.begin(9600);
    Serial.println("Serial communication started");
    pinMode(motorPin_pwm, OUTPUT);
    pinMode(motorPin_A, OUTPUT);
    pinMode(motorPin_B, OUTPUT);
    pinMode(Hal_c1, INPUT);
    pinMode(Hal_c2, INPUT);
}

void loop()
{
    // set motor direction
    digitalWrite(motorPin_A, LOW);
    digitalWrite(motorPin_B, LOW);
    // set pwm signal
    analogWrite(motorPin_pwm, 50);
    // check hall sensor
    int Hal_c1_state = digitalRead(Hal_c1);
    int Hal_c2_state = digitalRead(Hal_c2);
    Serial.print("Hal_c1_state: ");
    Serial.print(Hal_c1_state);
    Serial.print(" Hal_c2_state: ");
    Serial.println(Hal_c2_state);
    delay(1000);
}