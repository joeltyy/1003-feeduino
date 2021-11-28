
#define BLYNK_TEMPLATE_ID "TMPLJI6MmHuh"
#define BLYNK_DEVICE_NAME "V1"
#define BLYNK_AUTH_TOKEN "v9Fdv0JMG61oGOPsd-VrUHlNCcQxo9f2"
#define BLYNK_PRINT Serial
#define TURN_TIME 120 //Time is in miliseconds
#define DELAY_TIME 10000 // 3 Seconds- Simulate 8 hour intervals

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Gan";
char pass[] = "1a2b3c4d";

int V1State = 0;
int V2State = 0;
const long interval = 5000;
unsigned long previousMillis = 0;



Servo servo;

void setup() {
  Blynk.begin(auth, ssid, pass);
  servo.attach(2);
}

BLYNK_WRITE(V1)
{
  V1State = param.asInt();
}
BLYNK_WRITE(V2)
{
  V2State = param.asInt();
}



void loop()

{
  Blynk.run();
  unsigned long currentMillis = millis();
  if (V1State == 1 && V2State == 0) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      servo.write(0);
      delay(TURN_TIME);
      servo.write(90);
    }
  }

  if (V2State == 1) {
    servo.write(0);
    delay(TURN_TIME);
    servo.write(90);
  }
}
