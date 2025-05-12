# include  <Servo.h> // Подключение библиотеки Servo 
Servo rotateServo;
Servo stage1Servo;
Servo stage3Servo;
Servo stage2Servo;
Servo clawServo;

void  setup  ()  { 

	Serial.begin(115200); 
	Serial.setTimeout(2); 

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

} 

void  loop  ()  { 

	while (!Serial.available()); 
	String x = Serial.readStringUntil('\n');
  char command = x.charAt(0);
  String numberStr = x.substring(1);
  int value = numberStr.toInt();

  if (command == 'a') {
    // Открытие/Закрытие клешнки
    clawServo.write(value);
  } 
  else if (command == 'b') {
    // 1 ступень
    stage1Servo.write(value);
  }
  else if (command == 'c') {
    // 2 ступень
    stage2Servo.write(value);
  }
  else if (command == 'd') {
    // 3 ступень
    stage3Servo.write(value);
  }
  else if (command == 'e') {
    // Поворот
    rotateServo.write(value);
  }

}