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

// Total element in the array
float SUM_phase(float dp[4]){
  int N_dp,i;
  float r;
  r=0.0;
//  N_dp = sizeof dp/ sizeof dp[0];
  N_dp = 4;
  for(i=0;i<N_dp;i++){
    r = r + dp[i];
  }
  return r;
}

// Sensor falling response
void Pin_stand(){
  int val;
  float sum_phi = 0.0;
  val = digitalRead(SENSOR);
  if(timer_pass>0){    // invalidation
    timer_pass++;
    if(timer_pass>time_pass){
      timer_pass=0;
    }
  }else{
    if(val==LOW){
      if(val_old==HIGH){
        N_S++;
        if(N_S<3){
          dphi[N_S-1] = -phi;
          sum_phi = SUM_phase(dphi);
          if(N_S>N_i){
            N_i = N_S;
          }
          omega = omega0 + kappa*sum_phi/(N_i);
        }
//        timer_pass=1;
        timer_pass=0;    //
      }
    }
  }
  val_old = val;
}


//Phase update
void Phase_update(){
  phi += omega*dt;
  phi = Phase_reset(phi);
  Phase_LED(phi);
}

//Reset phase
float Phase_reset(float p){
  float p_out=p;
  if(p_out>M_PI){
    p_out = p_out - 2*M_PI;
    Status_phipast = 0;
    if(N_S==N_Sold){
      N_i = N_S;
      if(N_i<2){
        dphi[1]=0.0;
        if(N_i<1){
          dphi[0]=0.0;
        }
        omega = omega0 + kappa*dphi[0];
      }
    }
    Show_sensing();
    N_Sold = N_S;
    N_S=0;
  }
  return p_out;
}

// LED lit by phase
void Phase_LED(float p){
  if(!Status_phipast){
    if(p>0){
      Status_phipast = 1;
//      digitalWrite(LED, HIGH);
      OCR0B = 2;    // Duty ratio
      Status_light = 1;
    }
  }
  if(Status_light){
    timer_LED++;
    if(timer_LED>time_L){
//      digitalWrite(LED, LOW);
      OCR0B = 4;    // デューティ比変更
      Status_light = 0;
      timer_LED = 0;
    }
  }
}

// Function for sensor of rise count
void Show_sensing(){
  if(N_S<4){
    digitalWrite(S1, N_S%2);
    digitalWrite(S2, (N_S/2)%2);
  }else{
    digitalWrite(S1,LOW);
    digitalWrite(S2,LOW);
  }
}

// Setup
void setup() {
  pinMode(PWM_pin, OUTPUT); 
  pinMode(SENSOR, INPUT);  
  pinMode(S1, OUTPUT);  
  pinMode(S2, OUTPUT);  
  // timer 割り込み設定
  // 詳細は TrinketTimers.pdf を参照
  cli();
//  TCCR1 |= (0<<CS13) | (1<<CS12) | (0<<CS11) | (0<<CS10);// 1[MHz]→周期1[μs]
  TCCR0A=0xF3;           // モード設定
  TCCR0B=0x0A;           // モード設定 1MHz
  OCR0A = 4;             //OCR0A+1が周期に相当
  OCR0B = 4;             //デューティ比(OCR0B+1)/(OCR0A+1)
//  TIMSK = (1 << OCIE0A);  // これにより関数を入れられる
  
  TCCR1=0x97;            // ここでモードを設定 125[kHz]
  OCR1C = 124;            // 1[ms]ごとにISR(TIM1_COMPA_vect) を実行
  TIMSK = (1 << OCIE1A);  // これにより関数を入れられる
  sei();
}

// Loop
void loop() {
}

ISR(TIM1_COMPA_vect)  
{
  Pin_stand();
  Phase_update();
}
