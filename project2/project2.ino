/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/
//Include LCD library
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// used for Serial.read().
char incoming_char; // each character on the line
char incoming_message[30]; // whole message
int index = 0; // for tracking each index of incoming_message

// flags, need to refactor later
bool message_sent = false; 
bool more_messages = false;

void serialRead(){ // have Arduino Serial check to see if any data is being sent its way.

  if (message_sent){ 
      // re-initialize Serial message variables if a message has already been sent
      
  }
  
  while(Serial.available() > 0){ // Don't read unless you know there is data.
      
      incoming_char = Serial.read(); // get a char from the Serial connect.
      incoming_message[index] = incoming_char; // put the char into an array.
      index++;
      message_sent = true; // a message has been sent
  }
}

void printLCDWithLibrary(){

  // prompt the user
  if(more_messages == false){
      Serial.println("Waiting for input. Please enter your message to print out.");
  }

  serialRead();

  // if a message has been sent AND there is nothing left in the Arduino Serial buffer
  if((Serial.available() == -1 || Serial.available() == 0) && message_sent){
      Serial.print("\n\nThe display should print: ");
      Serial.println(incoming_message);
      lcd.print(incoming_message); // print the whole message using the LCD library
      

      delay(1000);
      Serial.print("\n\nWould you like to print another message? (Y)es or (N)o");
      while(more_messages == false){
        
        serialRead();
//        if(
      }
      
  }

}

// send 0's or 1's to corresponding params
// example: sendBitsLCD(0,0,0,0,1,0);
void sendBitsLCD(int RS, int RW, int DB7, int DB6, int DB5, int DB4){

  digitalWrite(12, RS);
  // RW is already grounded
  digitalWrite(2, DB7);
  digitalWrite(3, DB6);
  digitalWrite(4, DB5);
  digitalWrite(5, DB4);
  
}

void writeCharacter(){
  // example: write 'E' to LCD
  // steps: 1. get ascii code for 'E'. (0100 0101)
  //        2. consult https://www.sparkfun.com/datasheets/LCD/HD44780.pdf documentation, page 42.
  //        3. step 6 on page 42 shows writing H to LCD - there are two steps:
  //          a. sendBitsLCD(1,0,0,1,0,0);
  //          b. sendBitsLCD(1,0,1,0,0,0);
  //          sendBitsLCD()'s latter 4 params correspond with H's ascii code: 0100 1000
  //        4. so, to write 'E' (0100 0101), we can use:
  //          a. sendBitsLCD(1,0,0,1,0,0)
  //          b. sendBitsLCD(1,0,0,1,0,1)
}

void initializeLCDWithoutLibrary(){

  // HD447780U LCD display usage with no LCD library
  // LCD usage from https://www.sparkfun.com/datasheets/LCD/HD44780.pdf p. 42
  // pin 11 is e pin

  // 2. Function set:
  sendBitsLCD(0,0,0,0,1,0);

  // 4. Display on/off control:
  sendBitsLCD(0,0,0,0,0,0);
  sendBitsLCD(0,0,1,1,1,0);

  // 5. Entry mode set:
  sendBitsLCD(0,0,0,0,0,0);
  sendBitsLCD(0,0,0,1,1,0);

  // 6. Write data to CGRAM/DDRAM:
  sendBitsLCD(1,0,0,1,0,0);
  sendBitsLCD(1,0,1,0,0,0);
  
  
}



void setup() {
  // set up the LCD's number of columns and rows: 
//  lcd.begin(16, 2);
  // Print a message to the LCD.
//  lcd.print("LCD initialized");

//  delay(2000);
//  lcd.setCursor(0, 1);

  Serial.begin(9600);
}

void loop() {


  initializeLCDWithoutLibrary();
  delay(1000);
  
}
