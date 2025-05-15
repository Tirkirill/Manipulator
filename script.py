import sys
import os
from PyQt5 import QtWidgets, uic
import serial 
import time

class ManipulatorInterface(QtWidgets.QMainWindow):

    def __init__(self):
        super().__init__()
        
        uic.loadUi('design.ui', self)

        self.record = []
        self.is_recoring = False
        
        self.LeftRightSlider.valueChanged.connect(self.left_right_slider_handler)
        self.OpenCloseSlider.valueChanged.connect(self.open_close_slider_handler)
        self.Stage1Slider.valueChanged.connect(self.stage1_slider_handler)
        self.Stage2Slider.valueChanged.connect(self.stage2_slider_handler)
        self.Stage3Slider.valueChanged.connect(self.stage3_slider_handler)
        self.LeftRightSlider.setValue(90)
        self.OpenCloseSlider.setValue(0)
        self.Stage1Slider.setValue(90)
        self.Stage2Slider.setValue(90)
        self.Stage3Slider.setValue(90)
        self.StartButton.clicked.connect(self.start_button_handler)
        self.PlayButton.clicked.connect(self.play_button_handler)
    
    def open_close_slider_handler(self):
        value = self.OpenCloseSlider.value()
        self.send_move_command(value, "4")
    
    def stage1_slider_handler(self):
        value = self.Stage1Slider.value()
        self.send_move_command(value, "1")
    
    def stage2_slider_handler(self):
        value = self.Stage2Slider.value()
        self.send_move_command(value, "2")
    
    def stage3_slider_handler(self):
        value = self.Stage3Slider.value()
        self.send_move_command(value, "3")

    def left_right_slider_handler(self):
        value = self.LeftRightSlider.value()
        self.send_move_command(180-value, "0")

    def send_move_command(self, value:int, servo_id: str):
        str_value = str(value)
        if value < 10:
            str_value = "00" + str_value
        elif value < 100:
            str_value = "0" + str_value
        command = servo_id + str(str_value)
        if self.is_recoring:
            self.record.append(command)

        message = "m" + command 
        self.send_message(message)

    def send_message(self, message):
        arduino.write(bytes(message + "\n", 'utf-8'))

    def start_button_handler(self):
        if self.is_recoring:
            self.StartButton.setText("Начать запись")
            self.is_recoring = False
        else:
            self.StartButton.setText("Закончить запись")
            self.is_recoring = True
            self.record = []

        print("Запись начата" if self.is_recoring else "Запись остановлена")
    
    def play_button_handler(self): 

        self.minimize_record()           

        # Если слишком много записей, отправляем по одной (можно было бы батчами)
        if len(self.record) >= 50:
            print("По одной")
            for record in self.record:
                self.send_move_command(int(record[1:]), record[0]) 
                time.sleep(1)
        else:
            print("Скопом")
            arduino.write(bytes("r" + "".join(self.record) + "\n", 'utf-8'))
            response = self.wait_for_arduino_response(timeout=15)
            print(response)

    def wait_for_arduino_response(self, timeout=None):
        """Ожидает данные от Arduino. Возвращает строку ответа или None, если таймаут истёк."""
        start_time = time.time()
        while True:
            if arduino.in_waiting > 0:
                response = arduino.readline().decode('utf-8').strip()
                return response
            if timeout is not None and time.time() - start_time > timeout:
                return None
            time.sleep(0.01)

    def minimize_record(self):
        """Сократим self.record. Будем искать последовательности и брать только финальное положение"""
        new_record = []
        curr_servo_id = None
        last_value = None
        for record in self.record:
            servo_id = record[0]
            if curr_servo_id == None or curr_servo_id != servo_id:
                if curr_servo_id != None:
                    new_record.append(curr_servo_id + last_value)
                curr_servo_id = servo_id

            last_value = record[1:]

        if curr_servo_id != None:
            new_record.append(curr_servo_id + last_value)   

        self.record = new_record

def main():
    
    qt_plugin_path = os.path.join(os.path.dirname(os.path.dirname(sys.executable)), 'Lib', 'site-packages', 'PyQt5', 'Qt5', 'plugins')
    QtWidgets.QApplication.addLibraryPath(qt_plugin_path)
    app = QtWidgets.QApplication(sys.argv)
    window = ManipulatorInterface()
    window.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    arduino = serial.Serial(port='COM3', baudrate=115200, timeout=.5) 
    main() 
