/*
   Alcohol indicator from breathed air using MQ4
   The analog value of obtained readings is presented as percentage to the LCD(using I2C display)
   The additional LEDs(Common cathode RGB used) are used for some customized readings
   The MQ sensors require some time for heating up the element before detection can proceed,
   the delay is introduced in the Setup fuction to allow heating to take palce

    Using built-in I2C scanner example(File->Examples->Wire->i2c scanner)
   scan its I2C address(this address is used when creating an instance of LiquidCrystal_I2C)
*/

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define MQ_PIN A3

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int numRows = 2, numCols = 16;
LiquidCrystal_I2C screen(0x27, numCols, numRows);

enum LEDs {
  red = 0, blue = 1, green = 2,
};

const int  array_size = 3;
int LED[array_size] = {5, 6, 7}, last_reading = 0, MQ_reading; //LEDs order=> red, blue, green

void update_LED(int readings);
String alcohol_status(int readings);
void update_data(int readings);

void setup() {
  // set up the LCD's number of columns and rows:
  screen.begin();
  screen.clear();
  screen.setCursor(3, 0);
  screen.backlight();
  screen.print("PROJECT BY:");
  screen.setCursor(4, 1);
  screen.print("JOVINE");
  delay(1000);
  screen.clear();
  screen.setCursor(0, 0);
  screen.print("Preparing device: ");

  //printing dots(.) on the screen
  for (int runs = 0; runs <= 20; runs++) {
    screen.setCursor(0, 1);
    screen.print(".");
    for (int pos = 1; pos < 3; pos++) {
      screen.setCursor(pos, 1);
      screen.print(".");
      delay(100);
    }
    screen.clear();
  }

  for (int i = 0; i < array_size; i++) {
    pinMode(LED[i], OUTPUT);
  }
  pinMode(MQ_PIN, INPUT);

  //  turn on blue light to indicate device is ready
  digitalWrite(LED[green], HIGH);
  //indicate the device is ready for detection
  screen.setCursor(0, 0);
  screen.print("Device is ready");

  delay(2000);
  screen.clear();
  screen.setCursor(0, 0);
  screen.print("Alcohol measurement: ");

  //Scrolling the text to the left
  for (int positionCounter = 0; positionCounter < 22; positionCounter++) {
    screen.scrollDisplayLeft();
    delay(150);
  }
  screen.clear();


}

void loop() {

  //obtain sensor readings and mapping them to percentage
  MQ_reading = map(analogRead(MQ_PIN), 0, 1024, 0, 100);
  //updating MQ data on the screen and the LED state
  update_data(MQ_reading);
}


void update_data(int readings) {
  //update data whenever there is a change
  if (last_reading != readings) {
    screen.setCursor(0, 1);
    screen.print("Alcohol: ");
    screen.print(readings);
    screen.print(" %");
    screen.setCursor(1, 1);
    screen.print(alcohol_status(readings));
    update_LED(readings);
    last_reading = readings;
    delay(100);
  }
}

void update_LED(int readings) {
  // the percentage below 70% was found to be normal, otherwise the person was drunk
  digitalWrite(LED[green], LOW);
  if (readings < 70) {
    digitalWrite(LED[red], LOW);
    digitalWrite(LED[blue], HIGH);
  }
  else {
    digitalWrite(LED[red], HIGH);
    digitalWrite(LED[blue], LOW);
  }
}

String alcohol_status(int readings) {
  if (readings < 70) {
    return "No Alcohol";
  } else {
    return "Alcohol Available";
  }
}
