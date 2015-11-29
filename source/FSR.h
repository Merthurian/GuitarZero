const int FSR1 = A3;
const int FSR2 = A4;
const int FSR3 = A5;
const int FSR4 = A6;

class FSR
{
  public:	
	FSR()
	{
		sens = 1.0;
		thresh = 0.01;
		minimum = 0.0;
		maximum = 1.0;
      
		mode = 0;
		on = false;
		
		switch(id)
		{
			case 0:
				pin = FSR1;
				channel = 0xB0;
				cc = 64;
				break;
			
			case 1:
				pin = FSR2;
				channel = 0xB0;
				cc = 65;
				break;
			
			case 2:
				pin = FSR3;
				channel = 0xB0;
				cc = 66;
				break;
			
			case 3:
				pin = FSR4;
				channel = 0xB0;
				cc = 67;
				break;
			
			default:
				pin = FSR1;
				channel = 0xB0;
				cc = 64;
				break;
		}
		id++;
		id %= 4;
	}
    
    int pin;
    float sens;
    float thresh;
    float minimum;
    float maximum;
    byte channel;
    byte cc;
    
    float lvalue;
        
    int mode;
	
	static int id;
	static int modes;
    
    boolean on;
    
	void nextMode()
	{
		mode++;
		mode %= modes;
	}
	
	void go()
	{
		float v = analogRead(pin);
		float value = v/1023;
      
		value *= sens;
		
		byte val = byte(value * 127);
      
		switch(mode)
		{
			case 0:
				if (value > thresh){
					on = true;
						   
					Serial.write(channel);
					Serial.write(cc);
					Serial.write(127);          
				}
				else if (on)
				{
					Serial.write(channel);
					Serial.write(cc);
					Serial.write(0);
				  
					on = false;
				}        
				break;
		
			case 1:
				if ((value > thresh) && (lvalue < thresh))
				{
					on = !on;

					Serial.write(channel);
					Serial.write(cc);
					Serial.write(on * 127);
				}
				
				break;
				
			case 2:
				on = false;
				Serial.write(channel);
				Serial.write(12);
				Serial.write(val);
				break;
			
			case 3:
				Serial.write(channel);
				Serial.write(12);
				Serial.write(val);
			
				if (value > thresh){
					on = true;
						   
					Serial.write(channel);
					Serial.write(cc);
					Serial.write(127);          
				}
				else
				{
					if (on)
					{
						Serial.write(channel);
						Serial.write(cc);
						Serial.write(0);
					  
						on = false;
					}     
				}			
				break;
		}
		lvalue = value;      
    }
};

int FSR::id = 0;
int FSR::modes = 4;

FSR FSRs[4];// {fsr1, fsr2, fsr3, fsr4};
