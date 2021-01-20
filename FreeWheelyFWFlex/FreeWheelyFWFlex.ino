#define FLEX_PIN        A0
#define FLEX_START_VAL  700
#define FLEX_END_VAL    900

#define SPEED_PIN         5 
#define SPEED_PWM_START   41
#define SPEED_PWM_END     215

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SPEED_PIN, OUTPUT);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  int flexVal = analogRead(FLEX_PIN);         
  Serial.println(flexVal);                    
  int speedPWM = map(flexVal, FLEX_START_VAL, FLEX_END_VAL, SPEED_PWM_START, 200);
  //speedPWM = SPEED_PWM_END - speedPWM;
  analogWrite(SPEED_PIN, speedPWM);
  Serial.println(speedPWM);
  
  delay(100);                                //Small delay
  
}
