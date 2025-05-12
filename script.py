from PyQt5 import QtWidgets, uic
import sys
import os
import serial 

class ManipulatorInterface(QtWidgets.QMainWindow):

    def __init__(self):
        super().__init__()
        
        uic.loadUi('design.ui', self)
        
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
    
    def open_close_slider_handler(self):
        value = self.OpenCloseSlider.value()
        arduino.write(bytes("a" + str(value) + "\n", 'utf-8'))
    
    def stage1_slider_handler(self):
        value = self.Stage1Slider.value()
        arduino.write(bytes("b" + str(value) + "\n", 'utf-8'))
    
    def stage2_slider_handler(self):
        value = self.Stage2Slider.value()
        arduino.write(bytes("c" + str(value) + "\n", 'utf-8'))
    
    def stage3_slider_handler(self):
        value = self.Stage3Slider.value()
        arduino.write(bytes("d" + str(value) + "\n", 'utf-8'))

    def left_right_slider_handler(self):
        value = self.LeftRightSlider.value()
        arduino.write(bytes("e" + str(180-value), 'utf-8'))
    

def main():
    qt_plugin_path = os.path.join(os.path.dirname(sys.executable), 'Lib', 'site-packages', 'PyQt5', 'Qt', 'plugins')
    QtWidgets.QApplication.addLibraryPath(qt_plugin_path)
    app = QtWidgets.QApplication(sys.argv)
    window = ManipulatorInterface()
    window.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    arduino = serial.Serial(port='COM3', baudrate=115200, timeout=.2) 
    main() 
