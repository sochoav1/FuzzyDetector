#include "PWM_arduino_menu.h"
float dc;

void setup() {
    setupMenu();
// RTM_TimerCalc 1.40,  RuntimeMicro.com
// Timer-1 16-bit, Mode-14 Fast, Top=ICR
// 10,000 Hz Frequency, Clock is 16 MHz

TCCR1B = 0x18; // 0001 1000, Disable Timer 
TCCR1A = 0xA2; // 1010 0010

ICR1 = 1600-1;
OCR1A = (int) (ICR1 * 0.25);
OCR1B = (int) (ICR1 * 0.50);
TCNT1=0x0;

// UnComment following lines for UNO-NANO Timer-1 Pins 
 pinMode(9, OUTPUT);  // OC1a
// pinMode(10, OUTPUT); // OC1b

// UnComment following lines for 2560 Timer-1 Pins 
// pinMode(11, OUTPUT);  // OC1a
// pinMode(12, OUTPUT);  // OC1b

TCCR1B |= 1; // Prescale=1, Enable Timer 


  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
    taskManager.runLoop();

  OCR1A = (int)(map(analogRead(A0), 0, 1023, 0, ICR1));
  dc = ((float)OCR1A / (float)ICR1) * 100;
  Serial.println(dc);
  delay(1000);
}
