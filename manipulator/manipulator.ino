# include  <Servo.h>
Servo rotateServo;
Servo stage1Servo;
Servo stage3Servo;
Servo stage2Servo;
Servo clawServo;

Servo* servos[5] = {&rotateServo, &stage1Servo, &stage2Servo, &stage3Servo, &clawServo};

void  setup  ()  { 

	Serial.begin(115200); 
	Serial.setTimeout(5); 

  rotateServo.attach(3);
  stage1Servo.attach(4);
  stage3Servo.attach(5);  
  stage2Servo.attach(6);  
  clawServo.attach(7);

  initial_pos();

  Serial.println("!");

} 

void  loop  ()  { 

	while (!Serial.available()); 
	String commandLine = Serial.readStringUntil('\n');

  if (commandLine.length() < 2) {
    return;
  }

  char command = commandLine.charAt(0);
  String valueStr = commandLine.substring(1);

  if (command == 'r') {

    processServoCommand(valueStr);

  }

  else if (command == 'm') {

    int value = valueStr.substring(1).toInt();
    int servo_key = valueStr.charAt(0) - '0';

    if (servo_key == 9) {
      Serial.println("!");
      initial_pos();
      return;
    }

    if (servo_key >= 0 && servo_key < 5) {
      servos[servo_key]->write(value);
    } else {
      Serial.write('x');
    }

  }

}

/**
 * @brief Функция для обработки строки с командами сервоприводов при воспроизведении действий
 *
 * @param valueStr Строка с командами
 */
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

/**
 * @brief Исходное положение сервоприводов
 */
void initial_pos() {

  moveServoSmoothly("0090");
  moveServoSmoothly("1090");
  moveServoSmoothly("2090");
  moveServoSmoothly("3090");
  moveServoSmoothly("4000");
  
}

/**
 * @brief Постепенно двигает сервопривод
 *
 * @param command строка из 4 символов: команда изменения угла сервопривода
 */
void moveServoSmoothly(String command) {
  if (command.length() != 4) return;

  int servo_key = command[0] - '0';

  if (servo_key == 9) {
    initial_pos();
    return;
  }

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