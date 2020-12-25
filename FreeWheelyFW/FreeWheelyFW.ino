#include <SoftwareSerial.h>

SoftwareSerial MyBlue(2, 3); // RX | TX
int flag = 0;
int LED = LED_BUILTIN;

void setup() {
  Serial.begin(9600);
  MyBlue.begin(9600);
  Serial.println("Ready to connect\nDefualt password is 1234 or 000");
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
    flag = MyBlue.read();
    Serial.println("available!");
    Serial.println(flag);

    if (flag == '1')
    {
      digitalWrite(LED, HIGH);
      Serial.println("LED On");
    }
    else if (flag == '0')
    {
      digitalWrite(LED, LOW);
      Serial.println("LED Off");
    }
  }
}
