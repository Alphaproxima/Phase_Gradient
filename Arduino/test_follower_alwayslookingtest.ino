#include<math.h>

//This code is running in 8pino microcontroller

//       Global Constant       //
const int SENSOR = 0;
const int PWM_pin = 1; //pin 1 in 8pino is PWM pin
const int S1 = 3;                     // sensing number 1 pin
const int S2 = 4;                     // sensing number 2 pin
const int time_L = 30;                // LED blinking time
const int time_pass = 10000;          // Sensor ignoring time
const float dt=0.001;                 // time resolution
const float omega0 = 10.0;            // Natural angular frequency
const float kappa = omega0/(2*M_PI);  // interaction constant
//const float kappa = 2.1;

//        Global Variable        //
int val_old = 0;                         // Prevent chattering
int Status_light = 0;                   // LD lit condition
int Status_phipast = 1;                 // positive or negative
int timer_LED = 0;                      // time counter for led
int timer_pass = 0;                     // Sensor ignorance time
int N_S = 0;                            // Number of sensor ties\\s
int N_Sold = N_S;                       // number of time storage
int N_i = 0;                            // number of adjuscene robot
float phi=0.0;                          // phase
float omega=omega0;                     // Angular frequency
float dphi[4]={0.0,0.0,0.0,0.0};        // Phase difference storage
