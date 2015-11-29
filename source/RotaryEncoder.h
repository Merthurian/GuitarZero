//RGB Rotary encoder
const int RE_LED_R_PIN = 11;
const int RE_LED_G_PIN = 12;
const int RE_LED_B_PIN = 13;
const int RE_SW_PIN = 18;
const int RE_A_PIN = 2;
const int RE_B_PIN = 3;
volatile boolean halfleft = false;
volatile boolean halfright = false;

volatile int buttonState = LOW;
volatile int state = 0;
volatile int rotary = 0;
int lastRotary = 0;

Color ledColor = {0,0,0};
float ledBrightness = 1.0;

int selected = 0; //The currently selected sensor

volatile boolean pressed = false;

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

void ledDo()
{ 
	byte r = byte(255 - ledColor.r*ledBrightness*255);
	byte g = byte(255 - ledColor.g*ledBrightness*255);
	byte b = byte(255 - ledColor.b*ledBrightness*255);

	analogWrite(RE_LED_R_PIN, r);
	analogWrite(RE_LED_G_PIN, g);
	analogWrite(RE_LED_B_PIN, b);
}

void isr_2()
{
	// Pin2 went LOW
	delay(1);// Debounce time
	
	if(digitalRead(RE_A_PIN) == LOW)
	{// Pin2 still LOW ?
		if(digitalRead(RE_B_PIN) == HIGH && halfright == false)
		{      // -->
			halfright = true;// One half click clockwise
		} 
		if(digitalRead(RE_B_PIN) == LOW && halfleft == true)
		{         // <--
			halfleft = false;// One whole click counter-
			rotary--;// clockwise
		}
	}
}
void isr_3()
{
	// Pin3 went LOW
	delay(1);// Debounce time
	
	if(digitalRead(RE_B_PIN) == LOW)
	{                              // Pin3 still LOW ?
		if(digitalRead(RE_A_PIN) == HIGH && halfleft == false)
		{      // <--
			halfleft = true;// One half  click counter-
		}// clockwise
		if(digitalRead(RE_A_PIN) == LOW && halfright == true)
		{       // -->
			halfright = false;// One whole click clockwise
			rotary++;
		}
	}
}

void isr_sw1()
{// Pin3 went LOW
	delay(1);
	if(digitalRead(RE_A_PIN) == HIGH)
	{
		state = (state + 1)%7;
	}
}

long debouncing_time = 15; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;

void buttonInterruptChange() 
{
	if((long)(micros() - last_micros) >= debouncing_time * 1000)
	{
		buttonState = !buttonState;
		if (buttonState == false)
		{
			if (selected <= 3)
			{
				pressed = true;
			}      
		}
		last_micros = micros();
	}
}
