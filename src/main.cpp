#include <Arduino.h>
#include <Servo.h>

//HC-SR04 Ultrasonic Distance Sensor
const int trig = 22;
const int echo = 23;

//Two MG90S servos
const int servo_pins[2] = {18, 19};
Servo servos[2];

#define uS_TO_S_FACTOR 1000000 //Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP 5        //Time ESP32 will go to sleep (in seconds)

void do_the_servo_dance();
void setup_servos();
void measure_distance();
void setup_distance_sensor();

void setup()
{
  Serial.begin(9600);
  delay(1000);

  setup_distance_sensor();
  setup_servos();

  measure_distance();

  delay(1000);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void setup_distance_sensor()
{
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void setup_servos()
{
  for (int i = 0; i < 2; i++)
  {
    servos[i].attach(servo_pins[i]);
  }
}

void write_servos(int degrees)
{
  for (int i = 0; i < 2; i++)
  {
    servos[i].write(degrees);
  }
}

void do_the_servo_dance()
{
  for (int posDegrees = 0; posDegrees <= 180; posDegrees++)
  {
    write_servos(posDegrees);
    delay(5);
  }
  for (int posDegrees = 180; posDegrees >= 0; posDegrees--)
  {
    write_servos(posDegrees);
    delay(5);
  }
}

void measure_distance()
{
  long echo_time, distance_cm;

  for (int i = 0; i < 50; i++)
  {
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    echo_time = pulseIn(echo, HIGH);
    distance_cm = (echo_time / 2) / 28;

    Serial.print(distance_cm);
    Serial.println(" cm");

    if (distance_cm > 0 && distance_cm < 5)
    {
      do_the_servo_dance();
      Serial.println("Ok, go out of the loop.");
      break;
    }
    delay(500);
  }
}

void loop()
{
}
