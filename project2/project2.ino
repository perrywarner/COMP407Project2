//#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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
      //lcd.print(incoming_message); // print the whole message using the LCD library
      

      delay(1000);
      Serial.print("\n\nWould you like to print another message? (Y)es or (N)o");
      while(more_messages == false){
        
        serialRead();
//        if(
      }
      
  }

}

void myDelayMicros(int num_micros){
  // could have used delayMicroseconds(), but Prof. Black likes this more.
  uint8_t t = micros();
  while(micros() - t < num_micros){
    
  }
//  delay(1);
}

// This function pulses the E (enable) pin, and is must be done after each character is sent to the LCD.
void clock(){
  
  /* I had to reverse-engineer the Arduino library to get this one right:
   *  https://github.com/arduino-libraries/LiquidCrystal/blob/master/src/LiquidCrystal.cpp
   *  My solution is similar to void LiquidCrystal::pulseEnable(void).
   */
  digitalWrite(11, LOW);
  myDelayMicros(1000);
  digitalWrite(11, HIGH);
  myDelayMicros(1000);
  digitalWrite(11, LOW);
  // library says that commands need > microsecond to settle. Prof. Black recommends a full millisecond.
  myDelayMicros(1000);
}


// send 0's or 1's to corresponding params
// example: sendBitsLCD(0,0,0,0,1,0);
void sendBitsLCD(int RS, int RW, int DB7, int DB6, int DB5, int DB4){

  // Note: datasheet says that commands take up to 4.1ms to process. 
  //       Thus, I choose to waste 10ms before LCD commands are sent.
  delay(10);

  // Next: set each LCD pin value
  digitalWrite(12, RS);
  //       RW is already grounded. Whatever value is passed doesn't matter. 
  digitalWrite(2, DB7);
  digitalWrite(3, DB6);
  digitalWrite(4, DB5);
  digitalWrite(5, DB4);
  clock(); 
}

void writeCharacters(String chars){
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
  //        5. note: E pin on LCD must also be pulsed between each character write.
  //          a. I wrote a helper function clock() that handles this.

  int message_length = chars.length();
  char current_char;
  int message_bits[8];
  for (int i = 0; i < message_length; i++){
    
    current_char = chars.charAt(i);
    /* We have our char, now need to get the ASCII code for each char.
     *  'A' == 65 == 0b1000001
     * Arduino chars are 1 byte, or 8 bits. We can only send 4 bits to LCD at a time!
     * We can work around the limitation of quantity of bits to send
     *  by breaking each char in half, like so:
     */
    
    for (int j = 0; j < 8; j++){
      // build up message_bits[] with each char bit, starting at [0], up to [7].
      message_bits[j] = current_char & 1;
      current_char = current_char >> 1;
    }

    // send the four higher order bits
    // Remember: RS must be on, RW doesn't matter, and we want the message in order:
    sendBitsLCD(1, 0, message_bits[7], message_bits[6], message_bits[5], message_bits[4]);
    
    // send the four lower order bits
    sendBitsLCD(1, 0, message_bits[3], message_bits[2], message_bits[1], message_bits[0]);
    
  }
  
}

void initializeLCDWithoutLibrary(){
  
  
  
  // HD447780U LCD display usage with no LCD library
  // LCD usage from https://www.sparkfun.com/datasheets/LCD/HD44780.pdf p. 42

  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50ms

  
  delayMicroseconds(50000);

  // 2. Function set (4 bit operation):
  sendBitsLCD(0,0,0,0,1,0);
  // 3. Function set (4 bit operation, 1 line display, 5x8 font)
  sendBitsLCD(0,0,0,0,1,0);
  sendBitsLCD(0,0,0,0,0,0);
  // 4. Display on/off control:
  sendBitsLCD(0,0,0,0,0,0);
  sendBitsLCD(0,0,1,1,1,0);
  // 5. Entry mode set:
  sendBitsLCD(0,0,0,0,0,0);
  sendBitsLCD(0,0,0,1,1,0);
  // 6. Write data to CGRAM/DDRAM:
//  sendBitsLCD(1,0,0,1,0,0);
//  sendBitsLCD(1,0,1,0,0,0);
  
}



void setup() {
  // set up the LCD's number of columns and rows: 
//  lcd.begin(16, 2);
  // Print a message to the LCD.
//  lcd.print("LCD initialized");

//  delay(2000);
//  lcd.setCursor(0, 1);

  // Enable Arduino pins corresponding to:
  // RS, E, D7, D6, D5, D4, in that order. 
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  

  Serial.begin(9600);
  Serial.println("Initializing LCD display without library.");
  digitalWrite(11,0);
  initializeLCDWithoutLibrary();  
  writeCharacters("HELLO");
  Serial.println("Attempting to send message: HELLO\n");
}

void loop() {

  delay(1000);
  
  
}
