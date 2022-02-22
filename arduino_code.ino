//ultrasonic pins
#define trigPin A4
#define echoPin A3
float new_delay;
int distance;

//led pins
int front_led1 = 12;
int front_led2 = A0;
int back_led1 = 11;
int back_led2 = A2;

//automatic driving
char last = 'r';
//application manual speed
int speed_reading = 100;
int real_speed = 255;
//buzzer pin
#define buzzer 13

//ir pins
int ir_middle = A5;
int ir_right = 4;
int ir_left = A1;
int ir_back = A6;

//variables for ir readings
int ir_middle_reading = 0;
int ir_right_reading = 0;
int ir_left_reading = 0;
int ir_back_reading = 0;

//motor driver pins
#define ENA 10
#define ENB 5
#define in_1 9
#define in_2 8
#define in_3 7
#define in_4 6

//declaring functions
void readUltrasonic();
void Forward(int speed_left, int speed_right);
void Backward(int speed_left, int speed_right);
void Left(int speed_left, int speed_right);
void Right(int speed_left, int speed_right);
void Stop();
void Follow_Line();

//variable for bluetooth reading
char message;
void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(front_led1, OUTPUT);
  pinMode(front_led2, OUTPUT);
  pinMode(back_led1, OUTPUT);
  pinMode(back_led2, OUTPUT);
  pinMode(ir_middle, INPUT);
  pinMode(ir_left, INPUT);
  pinMode(ir_right, INPUT);
  pinMode(ir_back, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(in_1, OUTPUT);
  pinMode(in_2, OUTPUT);
  pinMode(in_3, OUTPUT);
  pinMode(in_4, OUTPUT);
}

void loop()
{
  if (Serial.available())
  {
    message = Serial.read();
    //led
    if (message == 'W')
    {
      digitalWrite(front_led1, 1);
      digitalWrite(front_led2, 1);
    }

    if (message == 'w')
    {
      digitalWrite(front_led1, 0);
      digitalWrite(front_led2, 0);
    }

    if (message == 'U')
    {
      digitalWrite(back_led1, 1);
      digitalWrite(back_led2, 1);
    }

    if (message == 'u')
    {
      digitalWrite(back_led1, 0);
      digitalWrite(back_led2, 0);
    }



    //speed
    if (message == '0')
    {
      speed_reading = 0;
    }
    if (message == '1')
    {
      speed_reading = 10;
    }
    if (message == '2')
    {
      speed_reading = 20;
    }
    if (message == '3')
    {
      speed_reading = 30;
    }
    if (message == '4')
    {
      speed_reading = 40;
    }
    if (message == '5')
    {
      speed_reading = 50;
    }
    if (message == '6')
    {
      speed_reading = 60;
    }
    if (message == '7')
    {
      speed_reading = 70;
    }
    if (message == '8')
    {
      speed_reading = 80;
    }
    if (message == '9')
    {
      speed_reading = 90;
    }
    if (message == 'q')
    {
      speed_reading = 100;
    }

    real_speed = map(speed_reading, 0, 100, 0, 255);

    if (message == 'F')
    {
      Forward(real_speed, real_speed);
    }
    if (message == 'B')
    {
      Backward(real_speed, real_speed);
    }
    if (message == 'R')
    {
      Right(real_speed, real_speed);
    }
    if (message == 'L')
    {
      Left(real_speed, real_speed);
    }
    if (message == 'S')
    {
      Stop();
    }
    if (message == 'V')
    {
      tone(buzzer, 500);
    }
    if (message == 'v')
    {
      noTone(buzzer);
    }
    if (message == 'X')
    {
      Follow_Line();
    }
  }
}

void readUltrasonic()
{
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  new_delay = (distance * 3) + 30;
  //  Serial.print(distance);
  //  Serial.println("  cm");
}

void Stop()
{
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(in_1, 0);
  digitalWrite(in_2, 0);
  digitalWrite(in_3, 0);
  digitalWrite(in_4, 0);
}

void Forward(int speed_left, int speed_right)
{
  analogWrite(ENA, speed_right);
  analogWrite(ENB, speed_left);
  digitalWrite(in_1, 0);
  digitalWrite(in_2, 1);
  digitalWrite(in_3, 0);
  digitalWrite(in_4, 1);
}
void Backward(int speed_left, int speed_right)
{
  analogWrite(ENA, speed_right);
  analogWrite(ENB, speed_left);
  digitalWrite(in_1, 1);
  digitalWrite(in_2, 0);
  digitalWrite(in_3, 1);
  digitalWrite(in_4, 0);
}

void Left(int speed_left, int speed_right)
{
  analogWrite(ENA, speed_right);
  analogWrite(ENB, speed_left);
  digitalWrite(in_1, 0);
  digitalWrite(in_2, 1);
  digitalWrite(in_3, 1);
  digitalWrite(in_4, 0);
}
void Right(int speed_left, int speed_right)
{
  analogWrite(ENA, speed_right);
  analogWrite(ENB, speed_left);
  digitalWrite(in_1, 1);
  digitalWrite(in_2, 0);
  digitalWrite(in_3, 0);
  digitalWrite(in_4, 1);
}

void Read_ir()
{
  ir_right_reading = digitalRead(ir_right);
  ir_left_reading = digitalRead(ir_left);
  ir_middle_reading = digitalRead(ir_middle);
  ir_back_reading = digitalRead(ir_back);
}

void Follow_Line()
{
  while (message != 'x') {
    readUltrasonic();
    Read_ir();
    if (distance <= 15) {
      Stop();
      tone(buzzer, 800);
    }
    else {
      noTone(buzzer);
      Read_ir();
      if (ir_middle_reading == 1)
      {
        if (ir_left_reading == 1 && ir_right_reading == 1) {
          Stop();
          delay (2000);
          while (ir_right_reading == 1 || ir_left_reading == 1) {
            Forward(200, 200);
            Read_ir();
          }
        }
        else {
          Forward(200, 200);
        }
      }
      else {
        if (ir_left_reading == 1) {
          Left(200, 200);
          last = 'l';
        }
        else if (ir_right_reading == 1) {
          Right(200, 200);
          last = 'r';
        }
        else {
          if (last == 'r')
          {
            Right(150, 150);
          }
          else if (last == 'l')
          {
            Left(150, 150);
          }


        }
      }


    }
    message = Serial.read();
    if (message == 'F') {
      message = 'x';
    }
  }
}
