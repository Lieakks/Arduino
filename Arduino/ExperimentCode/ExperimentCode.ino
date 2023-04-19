/*
 Name:		Experiment_Code.ino
 Created:	2022/7/22 20:08:05
 Author:	Daipei
*/

// pwm parameters
const int pwm_pin = 7;		 // PWM pin for MOS
int mos_state = LOW;		 // MOS state to check
unsigned long prev_mill = 0; // previous millis()
const long pwm_period = 100; // PWM period in ms

// the setup function runs once when you press reset or power the board
void setup()
{
	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop()
{
	// set pwm signal
	unsigned long curr_mill = millis(); // store current time
	if (curr_mill - prev_mill >= pwm_period)
	{
		prev_mill = curr_mill;
		if (mos_state == LOW)
		{
			mos_state = HIGH;
		}
		else
		{
			mos_state = LOW;
		}
		digitalWrite(pwm_pin, mos_state);
		// Serial.print("mos state"); Serial.print(mos_state); Serial.println("");
	}
}
