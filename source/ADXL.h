//adxl335 - Accelerometer

const int ACC_X_PIN = A0;
const int ACC_Y_PIN = A1;
const int ACC_Z_PIN = A2;

class ADXL
{   
	public:
	ADXL()
	{
		minVal = 495;
		maxVal = 610;
	}
	
	int minVal;
	int maxVal;

	double x;
	double y;
	double z;

	void go()
	{
		int xRead = analogRead(ACC_X_PIN);
		int yRead = analogRead(ACC_Y_PIN);
		int zRead = analogRead(ACC_Z_PIN);

		//convert read values to degrees -90 to 90 - Needed for atan2
		int xAng = map(xRead, minVal, maxVal, -180, 180);
		int yAng = map(yRead, minVal, maxVal, -180, 180);
		int zAng = map(zRead, minVal, maxVal, -180, 180);

		//Caculate 360deg values like so: atan2(-yAng, -zAng)
		//atan2 outputs the value of -π to π (radians)
		//We are then converting the radians to degrees
		x = RAD_TO_DEG * (atan2(-yAng, -zAng) + (PI));
		x -= 180;
		x = abs(x);

		y = RAD_TO_DEG * (atan2(-xAng, -zAng) + (PI));
		y -= 180;
		y = abs(y);

		z = RAD_TO_DEG * (atan2(-yAng, -xAng) + (PI));
		z -= 180;
		z = abs(z);    

		//byte bx = byte(map(x, 0, 180, 127, 0));
		byte by = byte(map(y, 0, 180, 127, 0));
		byte bz = byte(map(z, 0, 180, 127, 0));

		//Serial.write(0xB0);
		//Serial.write(12);
		//Serial.write(bx);
		//Only using 2 axis at the moment for testing.
		
		Serial.write(0xB0);
		Serial.write(13);
		Serial.write(by);

		Serial.write(0xB0);
		Serial.write(14);
		Serial.write(bz);
	} 
};

ADXL adxl = ADXL();
