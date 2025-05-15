# include  <Servo.h> // Подключение библиотеки Servo 
Servo rotateServo;
Servo stage1Servo;
Servo stage3Servo;
Servo stage2Servo;
Servo clawServo;

int curr_rotateServo;
int curr_stage1Servo;
int curr_stage2Servo;
int curr_stage3Servo;
int curr_clawServo;

Servo* servos[5] = {&rotateServo, &stage1Servo, &stage2Servo, &stage3Servo, &clawServo};

void  setup  ()  { 

	Serial.begin(115200); 
	Serial.setTimeout(5); 

  rotateServo.attach(3);
  rotateServo.write(90);

  stage1Servo.attach(4);
  stage1Servo.write(90);

  stage3Servo.attach(5);
  stage3Servo.write(90);

  stage2Servo.attach(6);
  stage2Servo.write(90);

  clawServo.attach(7);

  clawServo.write(0);

  curr_rotateServo = 90;
  curr_stage1Servo = 90;
  curr_stage2Servo = 90;
  curr_stage3Servo = 90;
  curr_clawServo = 0;

} 

void  loop  ()  { 

	while (!Serial.available()); 
	String x = Serial.readStringUntil('\n');
  char command = x.charAt(0);
  String valueStr = x.substring(1);

  if (command == 'r') {

    processServoCommand(valueStr);

  }

  else if (command == 'm') {

    int value = valueStr.substring(1).toInt();
    int servo_key = valueStr.charAt(0) - '0';

    servos[servo_key] -> write(value);

  }

}

// Функция для обработки строки с командами сервоприводов
void processServoCommand(String valueStr) {

  if (valueStr.length() % 4 != 0) {
    return;
  }

  String answer = "";
  for (unsigned int i = 0; i < valueStr.length(); i += 4) {

    moveServoSmoothly(valueStr.substring(i, i+4));
    
  }

  answer = "!";
  Serial.print(answer);
  
}

void moveServoSmoothly(String command) {
  if (command.length() < 4) return;

  int servo_key = command[0] - '0';
  String angleStr = command.substring(1, 4);
  int targetAngle = angleStr.toInt();

  if (targetAngle < 0 || targetAngle > 180) return;

  int stepSize = 1;
  int delayBetweenSteps = 15;

  Servo* servo = servos[servo_key];

  int currAnglePtr = servo -> read();

  if (currAnglePtr < targetAngle) {
    for (int i = currAnglePtr; i <= targetAngle; i += stepSize) {
      servo -> write(i);
      delay(delayBetweenSteps);
    }
  } else if (currAnglePtr > targetAngle) {
    for (int i = currAnglePtr; i >= targetAngle; i -= stepSize) {
      servo -> write(i);
      delay(delayBetweenSteps);
    }
  }
}