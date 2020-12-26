#include <SoftwareSerial.h>
#include <avr/wdt.h>

/*
 * MOTOR
 */
#define CMD_FORWARD   '1' 
#define CMD_REVERSE   '2'
#define CMD_STOP      '3'
#define CMD_INC_SPEED '+'
#define CMD_DEC_SPEED '-'
#define CMD_KEEPALIVE '9'
 
#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

#define MOTOR_A1_PIN  7
#define MOTOR_B1_PIN  8
#define PWM_MOTOR_1   5
#define EN_PIN_1      A0

bool isFirstCmd = true;
short usSpeed = 150;  //default motor speed
unsigned short usMotor_Status = BRAKE;

/*
 * BLE
 */
 #define LED          LED_BUILTIN
SoftwareSerial MyBlue(2, 3); // RX | TX
char msg = 0;


void setup() {
  wdt_disable();
  Serial.begin(9600);
  ble_setUp();
  motor_setUp();

  //sanity
  pinMode(LED, OUTPUT);
  for (int i = 0 ; i < 20; i++) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
}

void loop()
{
  if (MyBlue.available()) {
    msg = MyBlue.read();
    Serial.println("available!");
    Serial.println(msg);

    execMotorCMD(msg);
    if(isFirstCmd) {
      startWD();
      isFirstCmd = false;
    }
  }
}

void ble_setUp() {
  MyBlue.begin(9600);
  Serial.println("Ready to connect\nDefualt password is 1234 or 000");
}

void motor_setUp() {
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);
  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(EN_PIN_1, OUTPUT);

  execMotorCMD(CMD_STOP);
  Serial.println("Begin motor control");
}

void startWD() {
  // make a delay before enable WDT
  // this delay help to complete all initial tasks
  delay(2000);
  wdt_enable(WDTO_2S);
}

void execMotorCMD(char cmd)
{
  digitalWrite(EN_PIN_1, HIGH);

  if (cmd == CMD_FORWARD)
  {
    Forward();
  }
  else if (cmd == CMD_REVERSE)
  {
    Reverse();
  }
  else if (cmd == CMD_STOP)
  {
    Stop();
  }
  else if (cmd == CMD_INC_SPEED)
  {
    IncreaseSpeed();
  }
  else if (cmd == CMD_DEC_SPEED)
  {
    DecreaseSpeed();
  }
  else if (cmd == CMD_KEEPALIVE)
  {
    wdt_reset();
  }
  else
  {
    Serial.println("Invalid option entered");
  }
}

void Stop()
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(usMotor_Status, 0);
}

void Forward()
{
  Serial.println("Forward");
  usMotor_Status = CW;
  motorGo(usMotor_Status, usSpeed);
}

void Reverse()
{
  Serial.println("Reverse");
  usMotor_Status = CCW;
  motorGo(usMotor_Status, usSpeed);
}

void IncreaseSpeed()
{
  usSpeed = usSpeed + 10;
  if (usSpeed > 255)
  {
    usSpeed = 255;
  }

  Serial.print("Speed +: ");
  Serial.println(usSpeed);

  motorGo(usMotor_Status, usSpeed);
}

void DecreaseSpeed()
{
  usSpeed = usSpeed - 10;
  if (usSpeed < 0)
  {
    usSpeed = 0;
  }

  Serial.print("Speed -: ");
  Serial.println(usSpeed);

  motorGo(usMotor_Status, usSpeed);
}

void motorGo(uint8_t direct, uint8_t pwm)         //Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);
{
    if (direct == CW)
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, HIGH);
    }
    else if (direct == CCW)
    {
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_B1_PIN, LOW);
    }
    else
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, LOW);
    }

    analogWrite(PWM_MOTOR_1, pwm);
}
