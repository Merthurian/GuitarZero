#include "mariamole_auto_generated.h"
#include "ADXL.h"
#include "FSR.h"
#include "Color.h"
#include "RotaryEncoder.h"

void setup(){
	//Serial.begin(9600);
	Serial.begin(31250);
	analogReference(EXTERNAL);

	pinMode(RE_SW_PIN, INPUT_PULLUP);
	pinMode(RE_A_PIN, INPUT);
	digitalWrite(RE_A_PIN, HIGH);                // Turn on internal pullup resistor
	pinMode(RE_B_PIN, INPUT);
	digitalWrite(RE_B_PIN, HIGH);                // Turn on internal pullup resistor
	attachInterrupt(0, isr_2, FALLING);   // Call isr_2 when digital pin 2 goes LOW
	attachInterrupt(1, isr_3, FALLING);   // Call isr_3 when digital pin 3 goes LOW
	attachInterrupt(5, buttonInterruptChange, CHANGE);
		  
	ledColor = white;
	ledBrightness = 1.0;
}

double loopfreq = 200.0;
int loopdelay = int(1000.0/loopfreq);
byte lfo = 0;

void changeColor(){
  switch (selected) {
    case 0:
      ledColor = red;
      break;
    case 1:
      ledColor = green;
      break;
    case 2:
      ledColor = blue;
      break;
    case 3:
      ledColor = yellow;
      break;
    case 4:
      ledColor = pink;
      break;
    case 5:
      ledColor = skyblue;
      break;
    case 6:
      ledColor = white;
      break;
    default:
      ledColor = black;
      break;
  }
}

void loop(){
	lfo += 4;
	lfo = lfo%256;

	int change = 0;
	if (rotary != lastRotary)
	{
		change = rotary - lastRotary;
	}

	selected = (selected + change)%7;

	if (selected <= 3)
	{
		if (pressed)
		{ 
			FSRs[selected].nextMode();
			pressed = false;
		}

		switch(FSRs[selected].mode)
		{
			case 0:
				if (lfo % 13 == 0)
					ledBrightness = 1.0/255 * lfo;
				else 
					ledBrightness = 0;
				break;			
						
			case 1:
				if (lfo < 127)
					ledBrightness = 0;
				else 
					ledBrightness = 255;
				break;
				
			case 2:
				ledBrightness = 1.0/255 * lfo;
				break;
			
			case 3:
				if (lfo < 220)
					ledBrightness = 0;
				else 
					ledBrightness = 255;
				break;
			
			default:
				break;			
		}
	}
	else {
	if (buttonState == true)
	{
	switch (selected){
	case 4:
	//adxl.xphase += double(change/100.0);
	break;
	case 5:
	//adxl.yphase += double(change/100.0);
	break;
	case 6:
	//adxl.zphase += double(change/100.0);
	break;
	default:
	break;
	}

	ledBrightness = 0;
	}
  }
   
  changeColor();
  ledDo();
  
  for (int i = 0; i < 4; i++)
  {
    FSRs[i].go();
  }
      
  lastRotary = rotary;
  
  adxl.go();
  delay(loopdelay);
}
