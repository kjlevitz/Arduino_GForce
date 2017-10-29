# Arduino_GForce
Arduino project created to record G Force levels on X, Y and Z planes and record max GForce to an LCD screen and SD  card.

    G-Force, LED, SDCard Version 1.0.0 (Display G-Force to LED and save data to SD card)
    Copyright (C) 2017  Kevin Levitz
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
    
Used for Student SAE race team to display G-Force to small LED and to log detailed information to SD card.  
Parts used;  
Arduino Mega (Due to the size of libraries you need a Mega)  
I2C IIC Serial 128x64 OLED SSD1306 Display (LED Display) (Plug into 1st set of SDA/SCL on Mega or use circuit, 3v power)  
GY-521 MPU-6050 (Gyro) (Plug into 2nd set of SDA/SCL on Mega or use circuit, 5v power)  
SD Card Micro SDHC Mini TF (SD Reader)(GND -> GND, VCC -> 5v, MISO -> 50, MOSI -> 51, SCK -> 52, CS -> 53)  
Note: If you are not using a circiut you can use the 2nd 5v power from the Arduino's ICSP. Do not use it the ICSP ground though because it will prevent startup.)  
  
Reference code was used for Gyro Setup and Void function.  
Created by Korneliusz Jarzebski's free_fall program released under GNU license.  
https://github.com/jarzebski/Arduino-MPU6050  
https://github.com/jarzebski/Arduino-MPU6050/blob/master/MPU6050_free_fall/MPU6050_free_fall.ino (GNU License)  
  
Libaries used; (All credit goes to orignal authors of these libraries.)  
I2Cdev.h //I2C library https://github.com/jrowberg/i2cdevlib/blob/master/Arduino/I2Cdev/I2Cdev.h   
MPU6050.h //Gyro Library https://github.com/jrowberg/i2cdevlib/blob/master/Arduino/MPU6050/MPU6050.h  
SPI.h //OLED Library https://www.arduino.cc/en/Reference/SPI  
SD.h //SD Library https://www.arduino.cc/en/Reference/SD  
Adafruit_GFX.h //OLED Library https://github.com/adafruit/Adafruit-GFX-Library  
Adafruit_SSD1306.h //OLED Library https://github.com/adafruit/Adafruit_SSD1306  
