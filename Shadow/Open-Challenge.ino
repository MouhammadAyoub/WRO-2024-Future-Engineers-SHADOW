#include <Servo.h>

// Loops Counter
int Corners_Count = 0;
int threshold = 20;

// Servo Variables
Servo myServo;
int Servo_Pin = 11;

// Ultrasonic Variables
int Echo_Front = 13;
int Trig_Front = 12;

int Echo_Back = 8;
int Trig_Back = 7;

int Echo_Right = 10;
int Trig_Right = 9;

int Echo_Left = 6;
int Trig_Left = 4;

// Distance Variables
float Distance_Front;
float Distance_Back;
float Distance_Right;
float Distance_Left;

long Initial_Front_Position ;

// Motor Variables
int Forward_Motor = 5;
int Backward_Motor = 3;
int Enable_Motor = 2;


float Calculate_Distance(int trigPin, int echoPin) {

  float distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;
  return distance;
}

void Get_Sensors_Values() {

  Distance_Front = Calculate_Distance(Trig_Front, Echo_Front);
  Distance_Back = Calculate_Distance(Trig_Back, Echo_Back );
  Distance_Right = Calculate_Distance(Trig_Right, Echo_Right );
  Distance_Left = Calculate_Distance(Trig_Left, Echo_Left );
}

void Move_Forward(int Speed) {

  analogWrite(Forward_Motor, Speed);
  analogWrite(Backward_Motor, 0);
}

int button = 0 ;
bool state = false ;
void setup() {

  myServo.attach(Servo_Pin);

  pinMode(Trig_Front, OUTPUT);
  pinMode(Echo_Front, INPUT);

  pinMode(Trig_Back, OUTPUT);
  pinMode(Echo_Back, INPUT);

  pinMode(Trig_Right, OUTPUT);
  pinMode(Echo_Right, INPUT);

  pinMode(Trig_Left, OUTPUT);
  pinMode(Echo_Left, INPUT);

  pinMode(Forward_Motor, OUTPUT);
  pinMode(Backward_Motor, OUTPUT);
  pinMode(Enable_Motor, OUTPUT);

  digitalWrite(Enable_Motor, HIGH);

  pinMode(A2, INPUT);
  Serial.begin(9600);
  while (state == false) {
    button = analogRead(A2);// enter built in button here
    if (button > 1000) {
      state = true ;
    }
  }
  

  Get_Sensors_Values();

  Initial_Front_Position = Distance_Front;

  myServo.write(79);
  delay(500);
}

void loop() {

  if ( Distance_Right < 90  && Distance_Left < 90 && abs (Distance_Right - Distance_Left) > threshold && Distance_Front > 90) {
    if ( (Distance_Right - Distance_Left) >= 0) Turn_CorrectionRight();
    else Turn_CorrectionLeft();
  }
  else if (Distance_Front < 70 && (Distance_Right < 85 || Distance_Left < 85)){
    if (Distance_Right > Distance_Left) Turn_CornerRight();
    else Turn_CornerLeft();
  }
  else if (Corners_Count == 12) {
    digitalWrite(Enable_Motor, LOW);
    while(true);
  }
  else Move_Forward(85);

  Get_Sensors_Values();
}

void Turn_CornerRight() {

  myServo.write(160);
  Move_Forward(90);
  delay(950);
  myServo.write(79);
  Move_Forward(95);
  delay(1100);
  Corners_Count++;
}

void Turn_CornerLeft() {

  myServo.write(40);
  Move_Forward(90);
  delay(800);
  myServo.write(81);
  Move_Forward(95);
  delay(1300);
  Corners_Count++;
}

void Turn_CorrectionLeft() {

  myServo.write(70);
  Move_Forward(85);
  delay(150);
  myServo.write(81);
}

void Turn_CorrectionRight() {

  myServo.write(95);
  Move_Forward(85);
  delay(150);
  myServo.write(79);
}
