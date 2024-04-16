/* Realtime Repitch of Audio, by Robert Smith (@RobSmithDev)
 * https://youtube.com/c/robsmithdev
 *
 * See the video at https://youtu.be/UBEo4ezaw5c
 * 
 * This is free and unencumbered software released into the public domain.
 * See https://unlicense.org/ for more details
 */

#define AUDIO_INPUT     A5

#define SHAFT_ENCODER_CLK     8
#define SHAFT_ENCODER_DATA    9
#define SHAFT_ENCODER_BUTTON 10


// Prepare the Arduino
void setup() {
  // put your setup code here, to run once:
  pinMode(AUDIO_INPUT, INPUT);

  // Audio Output, AKA PORTD
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  // Input buttons
  pinMode(SHAFT_ENCODER_CLK, INPUT_PULLUP);
  pinMode(SHAFT_ENCODER_DATA, INPUT_PULLUP);
  pinMode(SHAFT_ENCODER_BUTTON, INPUT_PULLUP);

  // Disable Interrupts
  cli();

  //clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;
  
  ADMUX |= (1 << REFS0); //set reference voltage (AVcc with external capacitor at aref pin)
  ADMUX |= (1 << ADLAR); //left align the ADC value- so we can read highest 8 bits from ADCH register only
  ADMUX |= 5;            // Select A5
  
  ADCSRA |= (1 << ADPS2) |  (1 << ADPS0); // ADC takes 13 clocks, so with 32 prescaler- 16mHz/32=500kHz/13 = 38khz
  ADCSRA |= (1 << ADATE); //enabble auto trigger
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADIE); // Enable Interrupt
  ADCSRA |= (1 << ADSC); //start ADC measurements

  // Re-enable interrupts
  sei();
}


// The speed of playback
volatile unsigned char speed = 230;

// when new ADC value ready
ISR(ADC_vect) {
  static unsigned char data[512];

  static unsigned short inputPosition = 0;  
  static unsigned short outputPosition = 0;

  // Store current sample
  data[inputPosition] = ADCH;

  inputPosition = (inputPosition+1) & 511;
  outputPosition += speed;

  // Play from a different part
  PORTD = data[outputPosition >> 7] >> 1;
}


void loop() {
  delay(1);

  // Check for Reset
  if (digitalRead(SHAFT_ENCODER_BUTTON) == LOW) speed = 128;

  // Monitor shaft encoder
  static bool clkLastState = false;
  
  bool clkState = digitalRead(SHAFT_ENCODER_CLK) == HIGH;
  
  if (clkState != clkLastState) {
    clkLastState = clkState;
  
    bool dataState = digitalRead(SHAFT_ENCODER_DATA) == HIGH;

    if (dataState == clkState) {
      // anticlockwise
      if (speed>1) speed--;
    } else {
      // Clockwise
      if (speed<255) speed++;
    }
  }
}
