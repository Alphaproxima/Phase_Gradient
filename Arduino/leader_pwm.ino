#include <math.h>

//this code for generate the nonlinear oscillator from the leader
// Light emmitted from the leader
// pwm by timer0, interrupt timer by timer1

//this code using 8pino microcontroller
//Global Constant
const int SW = 0; //switch pin 0
const int PWM_pin = 1; //LED pin
const int time_L =30; //pika-pika interval..

const float dt = 0.001; // time resolution
const float omega0 = 10.0; // natural angular frequency rad/s
const float domega = 1.0; //angluar frequency variation

// Global Variables
int Switch_old = 0; //to prevent the chattering
int Status = 0; //change state of angular frequency
int Status_light = 0;      // LED Condition
int Status_phipast = 1;    // Positive negative state
int timer_LED = 0;         // Timer counter for LED
float phi=0.0;             // phase
float omega = 0.0;         // Angular frequency

int counter_w = 0;

//Phase update
void Phase_update(){
  phi += omega*dt;
  phi = Phase_reset(phi);
  Phase_LED(phi);
}
