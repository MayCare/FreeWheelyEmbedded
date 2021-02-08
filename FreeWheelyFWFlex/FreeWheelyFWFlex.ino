#define FLEX_PIN        A0
#define FLEX_START_VAL  750
#define FLEX_END_VAL    820//900

#define SPEED_PIN         5 
#define SPEED_PWM_START   41
#define SPEED_PWM_END     255//215

#define NUM_OF_SAMPLES    10

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SPEED_PIN, OUTPUT);
} 

void loop() {
  long sum = 0;
  for (int i = 0; i < NUM_OF_SAMPLES; i++)
  {
    sum += analogRead(FLEX_PIN);
    delay(1);
  }

  // put your main code here, to run repeatedly:
  int flexVal = sum / NUM_OF_SAMPLES;
  //Serial.println(sum);                    
  Serial.println(flexVal);            
  flexVal = flexVal < FLEX_START_VAL ? FLEX_START_VAL : flexVal; 
  flexVal = flexVal > FLEX_END_VAL ? FLEX_END_VAL : flexVal; 
          
  int speedPWM = map(flexVal, FLEX_START_VAL, FLEX_END_VAL, SPEED_PWM_START, SPEED_PWM_END);
  //speedPWM = SPEED_PWM_END - speedPWM;
  Serial.println(speedPWM);
  analogWrite(SPEED_PIN, speedPWM);
  //Serial.println(speedPWM);
  
  delay(10);                                //Small delay
  
}
