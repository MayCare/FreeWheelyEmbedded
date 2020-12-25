#include <SoftwareSerial.h>

/*
 * MOTOR
 */
#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

#define MOTOR_A1_PIN  7
#define MOTOR_B1_PIN  8
#define PWM_MOTOR_1   5
#define EN_PIN_1      A0

short usSpeed = 150;  //default motor speed
unsigned short usMotor_Status = BRAKE;

/*
 * BLE
 */
 #define LED          LED_BUILTIN
SoftwareSerial MyBlue(2, 3); // RX | TX
char msg = 0;


void setup() {
  Serial.begin(9600);
  ble_setUp();
  motor_setUp();

  //sanity
  pinMode(LED, OUTPUT);
  for (int i = 0 ; i < 20; i++) {
    digitalWrite(LED, HIGH);
    delay(300);
    digitalWrite(LED, LOW);
    delay(300);
  }
}

void loop()
{
  if (MyBlue.available()) {
    msg = MyBlue.read();
    Serial.println("available!");
    Serial.println(msg);

    execMotorCMD(msg);
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

  Serial.println("Begin motor control");
}

void execMotorCMD(char cmd)
{
  digitalWrite(EN_PIN_1, HIGH);

  if (cmd == '0')
  {
    Stop();
  }
  else if (cmd == '1')
  {
    Forward();
  }
  else if (cmd == '2')
  {
    Reverse();
  }
  else if (cmd == '+')
  {
    IncreaseSpeed();
  }
  else if (cmd == '-')
  {
    DecreaseSpeed();
  }
  else
  {
    Serial.println("Invalid option entered.");
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
