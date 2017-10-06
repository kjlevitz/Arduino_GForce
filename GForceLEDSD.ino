//Init Librarys

#include "I2Cdev.h" //I2C library
#include <MPU6050.h> //Gyro Library
#include <SPI.h> //OLED Library
#include <SD.h> //SD Library
#include <Adafruit_GFX.h> //OLED Library
#include <Adafruit_SSD1306.h> //OLED Library


//OLED Start

#define OLED_RESET 4 
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16


static const unsigned char PROGMEM logo16_glcd_bmp[] =
{
    B00000000, B11000000,
    B00000001, B11000000,
    B00000001, B11000000,
    B00000011, B11100000,
    B11110011, B11100000,
    B11111110, B11111000,
    B01111110, B11111111,
    B00110011, B10011111,
    B00011111, B11111100,
    B00001101, B01110000,
    B00011011, B10100000,
    B00111111, B11100000,
    B00111111, B11110000,
    B01111100, B11110000,
    B01110000, B01110000,
    B00000000, B00110000
}
;
#if (SSD1306_LCDHEIGHT != 64)

#endif

//OLED END


//SD Start

double pinCS = 53; // SD bus
File telFile; //SD save file

//Declare Variables, declaring g force values to use in calculations

float maxGL; //Max G Force //Variable used below.
float maxGZ; //Max G Force //Variable used for max y GForce.

float lateralForce; //Used for X and Y axis G Force
float zGForce; //Used for Z Axis G Force


MPU6050 mpu; //Gyro

//MPU6050 accelgyro(0x68); //Used to define I2C Gyro address.


//Gyro Calibration

void setup() {

  // Init Setup
  
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Init display and call i2c port
  pinMode(pinCS, OUTPUT); //SD Pin setup pin 53

  if (SD.begin()) //Statement reports if card was found.
  {
    Serial.println("SD card initialized"); //Found
    
  } else
  {
    Serial.println("SD card initialization failed"); //Not found
    return;
  }


  // Initialize MPU6050

  Serial.println("Initialize MPU6050");
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }


  //Auto Calibration
  
  mpu.calibrateGyro(); // Auto Calibrates sensor.

  
  //Manual Calibration remove comment to calibrate.
  
  //mpu.setGyroOffsetX(-10); 
  //mpu.setGyroOffsetY(11);
  //mpu.setGyroOffsetZ(-3);  
  //mpu.setAccelOffsetX(-623);
  //mpu.setAccelOffsetY(1055);
  //mpu.setAccelOffsetZ(1335);


  //Sensitivity of Gyro, default value is 3.
  
  mpu.setThreshold(3);

  checkSettings(); //Run below function to verify gyro.

}

void checkSettings()
{
  Serial.println();
  Serial.print(" * Sleep Mode:            ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  Serial.print(" * Clock Source:          ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Accelerometer:         ");
  switch(mpu.getRange())
  {
    case MPU6050_RANGE_16G:            Serial.println("+/- 16 g"); break;
    case MPU6050_RANGE_8G:             Serial.println("+/- 8 g"); break;
    case MPU6050_RANGE_4G:             Serial.println("+/- 4 g"); break;
    case MPU6050_RANGE_2G:             Serial.println("+/- 2 g"); break;
  }  

  Serial.print(" * Accelerometer offsets: ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());
  Serial.println();
}



void loop() {
  
  //Sensor probe
  
  Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();

  
  //Calculating G Force
  
  lateralForce = (sqrt(rawAccel.XAxis * rawAccel.XAxis + rawAccel.YAxis * rawAccel.YAxis)/17000); //This is working
  zGForce = abs(rawAccel.ZAxis / 17000); //This is working


  //Max G Force logic
  
  if (maxGL <= lateralForce) maxGL = lateralForce; 
  if (maxGZ <= zGForce) maxGZ = zGForce; 

  
  //OLED Print out data
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(30,0);
  display.print("Max G: ");
  display.println(maxGL);
  display.setTextColor(BLACK, WHITE);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(1,10);
  display.print("G: ");
  display.println(lateralForce);
  display.display();


  // Serial print out of data

  Serial.print("Lateral G Force    :");Serial.println(lateralForce); //This prints to the serial your Lateral G Force
  Serial.print("Y-Axis G Force:");Serial.println(zGForce); //This prints to the serial your Z Axis G Force


  // SD Card data logging, logs if G Force is greater than 0.25 on any axis.

  if (lateralForce > 0.25 || zGForce > 0.25)
{
    telFile = SD.open("teletes.txt", FILE_WRITE); //This opens/creates a file to send data to
    telFile.print(lateralForce);telFile.print(",");Serial.println(maxGL); //Prints this text or variable SD.
    telFile.print(zGForce);telFile.print(",");Serial.println(maxGZ); //Prints this text or variable SD.
    //telFile.println(maxGD); //Prints this text or variable SD.
    telFile.close(); //Closes after save, if this isn't done data may not be saved
}


  
  delay(50);

}
