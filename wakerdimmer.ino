// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       wakerdimmer.ino
    Created:	08/19/2019 17:09:41
    Author:     DESKTOP-F29UT1Q\Rambo Rambo
*/

// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// The setup() function runs once each time the micro-controller starts


/*
#define ENCODER_A PA12
#define ENCODER_B PA13
#define ENCODER_PUSH PA1
#define PIN PA0
#define MAX_LED PA2
#define ADD true
#define SUB false
#define OLED_RESET PA4*/
#define ENCODER_A 12
#define ENCODER_B 13
#define ENCODER_PUSH 1
#define PIN A0
#define MAX_LED 2
#define ADD true
#define SUB false
#define OLED_RESET 4

#include <Adafruit_NeoPixel.h>
#include <SCoop.h>
#include <Adafruit_SSD1306.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <DS3231.h>
//#include <MsTimer2.h>

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int change = 0;

int encoderVal = 0;
static int cursorPos = 0;
static int clickmode = 2;
static bool isTime = false;
static bool isLighted = false;
static bool configOK = false;
static int sysStage = 0;
static int dimmerVal = 255;
static bool isLED = false;
static int toDay = 1;
Time nowT;
static int usrconfig[] = { 0,0,0,0,1,25,0,0,0,0,0,0 };	//Hour,Min,duration_hr,duration_min,steps,bat self poweroff time,led1 R,G,B,led2 R,G,B

Adafruit_SSD1306 display(OLED_RESET);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(MAX_LED, PIN, NEO_RGB + NEO_KHZ800);
DS3231 rtc(SDA, SCL);
SCoop myScoop;


void setup()
{
	
	//Serial.begin(115200);
	pinMode(11, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(ENCODER_A, INPUT_PULLUP);
	pinMode(ENCODER_B, INPUT_PULLUP);
	pinMode(ENCODER_PUSH, INPUT);	//rtc.setDate(18, 8, 2019);
	pinMode(ENCODER_PUSH, INPUT_PULLUP);
	digitalWrite(11, HIGH);
	Wire.begin();
	rtc.begin();
	strip.begin();
	//char *app = rtc.getTimeStr(); +rtc.getTemp();
	toDay = rtc.getTime().date;
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.fillScreen(BLACK);
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.setCursor(0, 0);
	display.println(rtc.getTimeStr());
	display.setCursor(0, 16);
	display.println(rtc.getTemp());
	display.setCursor(90, 16);
	display.print("C");
	display.display(); // show splashscreen
	delay(5000);
	display.clearDisplay();   // clears the screen and buffer
	delay(300);
	myScoop.start();
	
}

// Add the main program code into the continuous loop() function
void loop()
{
	yield();
}
static char* tStr =	"24hr-00:00";
static char* ph3Str = "000sec";
static char* ph4Str = "000000000";
static char* ph2Str = "0h00m   00";


void testUpdate() {
	int t = usrconfig[0] / 10;
	int t1 = usrconfig[1] / 10;
	tStr[5] = t + '0';
	tStr[6] =  usrconfig[0] % 10 + '0'; // (usrconfig[0] - (t1*(usrconfig[0] % 10))) + '0';
	tStr[8] = t1 + '0';
	tStr[9] = usrconfig[1] % 10 + '0';
}

void updateLedConfig(int ledNum) {
	//	param1 Green
	//	param2 RED
	//	param3 BLUE
		int led1_R = usrconfig[6] / 100;
		int led1_G = usrconfig[7] / 100;
		int led1_B = usrconfig[8] / 100;
		int led2_R = usrconfig[9] / 100;
		int led2_G = usrconfig[10] / 100;
		int led2_B = usrconfig[11] / 100;
		//static uint32_t color = strip.Color(usrconfig[7], usrconfig[6], usrconfig[8]);
		//strip.setPixelColor(0, color);
		if (ledNum == 1) {
			ph4Str[0] = led1_R + '0';
			ph4Str[1] = ((int)(usrconfig[6] % 100) / 10) + '0';
			ph4Str[2] = usrconfig[6] % 10 + '0';

			ph4Str[3] = led1_G + '0';
			ph4Str[4] = ((int)(usrconfig[7] % 100) / 10) + '0';
			ph4Str[5] = usrconfig[7] % 10 + '0';

			ph4Str[6] = led1_B + '0';
			ph4Str[7] = ((int)(usrconfig[8] % 100) / 10) + '0';
			ph4Str[8] = usrconfig[8] % 10 + '0';
		}
		else {
			ph4Str[0] = led2_R + '0';
			ph4Str[1] = ((int)(usrconfig[9] % 100) / 10) + '0';
			ph4Str[2] = usrconfig[9] % 10 + '0';

			ph4Str[3] = led2_G + '0';
			ph4Str[4] = ((int)(usrconfig[10] % 100) / 10) + '0';
			ph4Str[5] = usrconfig[10] % 10 + '0';

			ph4Str[6] = led2_B + '0';
			ph4Str[7] = ((int)(usrconfig[11] % 100) / 10) + '0';
			ph4Str[8] = usrconfig[11] % 10 + '0';
		}
}
bool is2MinEal() {
	if (nowT.hour == usrconfig[0] && nowT.min == usrconfig[1] - 2 && toDay+1==nowT.date) {
		isLED = true;
		return true;
	}
	else {
		isLED = false;
		return false;
	}
}

bool isLater() {
	nowT = rtc.getTime();
	//Serial.print("NOW-");	Serial.print(nowT.hour);	Serial.println(nowT.min);
//	Serial.print("SET-");	Serial.print(usrconfig[0]);	Serial.println(usrconfig[1]);


	if (nowT.hour > usrconfig[0]&&nowT.date==toDay) {
		isTime = true;
		return true;
	}
	else if (nowT.hour == usrconfig[0] && nowT.date == toDay)
	{
		if (nowT.min >= usrconfig[1]) {
			isTime = true;
			return true;
		}
		else {
			isTime = false;
			return false;
		}
	}
}
defineTaskLoop(Sysconfiger)
{
	cursorPos = 0;
	encoderVal = 0;
	while (sysStage == 0) {
		if (clickmode == 1) {
			mySCoop.Atomic++;
			clickmode = 2;
			sysStage = 1;
			mySCoop.Atomic--;
		}
		if (clickmode == 0) {
			if (cursorPos == 0) {
				cursorPos++;
				encoderVal = 0;
				clickmode = 2;
			}
			else {
				cursorPos = 0;
				encoderVal = usrconfig[cursorPos];
				clickmode = 2;
			}
		}
		if (encoderVal < 60 && encoderVal >= 0) {
			if (cursorPos == 1) {
				usrconfig[cursorPos] = encoderVal;
			}
			if (cursorPos == 0 && encoderVal < 24) {
				usrconfig[cursorPos] = encoderVal;
			}
			else if (encoderVal >= 24 && cursorPos == 0) {
				encoderVal = 0;
			}
		}
		else if (encoderVal >= 60 && cursorPos == 1) {
			encoderVal = 0;
		}
		else if (cursorPos == 0 && encoderVal < 24) {
			encoderVal = 23;
		}
		else if (cursorPos == 1 && encoderVal < 0)
		{
			encoderVal = 59;
		}
		testUpdate();
		display.clearDisplay();
		display.setCursor(0, 0);
		display.println("START TIME");
		display.setCursor(0, 16);
		display.println(tStr);
		display.display();
		sleep(500);
	}
	cursorPos = 2;
	encoderVal = 0;

	while (sysStage == 1) {
		if (clickmode == 1) {

			mySCoop.Atomic++;
			clickmode = 2;
			sysStage = 2;
			mySCoop.Atomic--;
		}
		if (clickmode == 0) {
			if (cursorPos >= 2 && cursorPos < 4) {
				cursorPos++;
				encoderVal = usrconfig[cursorPos];
				clickmode = 2;
			}
			else {
				mySCoop.Atomic++;
				cursorPos = 2;
				encoderVal = usrconfig[cursorPos];
				clickmode = 2;
				mySCoop.Atomic--;
			}
		}
		else {
			if (encoderVal > 9 && cursorPos == 2) {
				encoderVal = 0;
			}
			else if (cursorPos == 2 && encoderVal < 0) {
				encoderVal = 9;
			}
			else if (cursorPos == 2 && encoderVal >= 0 && encoderVal <= 9)
			{
				usrconfig[2] = encoderVal;
			}
			if (cursorPos == 3 && encoderVal > 59) {
				encoderVal = 0;
			}
			else if (cursorPos == 3 && encoderVal < 0) {
				encoderVal = 59;
			}
			else if (cursorPos == 3 && encoderVal >= 0 && encoderVal < 60)
			{
				usrconfig[3] = encoderVal;
			}
			if (cursorPos == 4 && encoderVal > 99) {
				encoderVal = 0;
			}
			else if (cursorPos == 4 && encoderVal <= 0) {
				encoderVal = 99;
			}
			else if (cursorPos == 4 && encoderVal <= 99 && encoderVal >= 0)
			{
				usrconfig[4] = encoderVal;
			}
		}
		int t = usrconfig[3] / 10;
		int t1 = usrconfig[4] / 10;
		ph2Str[0] = usrconfig[2] + '0';
		ph2Str[2] = t + '0';
		ph2Str[3] = usrconfig[3] % 10 + '0';
		ph2Str[8] = t1 + '0';
		ph2Str[9] = usrconfig[4] % 10 + '0';
		display.clearDisplay();
		display.setCursor(0, 0);
		display.println("DUR & STEP");
		display.setCursor(0, 16);
		display.println(ph2Str);
		display.display();
		sleep(300);
	}
	encoderVal = 20;
	while (sysStage == 2) {
		if (clickmode == 1) {
			mySCoop.Atomic++;
			clickmode = 2;
			sysStage = 3;
			mySCoop.Atomic--;
		}
		if (encoderVal <= 300) {
			usrconfig[5] = encoderVal;
		}
		if (encoderVal > 300) {
			encoderVal = 0;
		}
		if (encoderVal < 0) {
			encoderVal = 300;
		}
		int hud = usrconfig[5] / 100;
		ph3Str[0] = hud + '0';
		ph3Str[1] = ((int)(usrconfig[5] % 100) / 10) + '0';
		ph3Str[2] = usrconfig[5] % 10 + '0';
		display.clearDisplay();
		display.setCursor(0, 0);
		display.println("BATT OFF T");
		display.setCursor(0, 16);
		display.println(ph3Str);
		display.display();
		sleep(400);
	}
	encoderVal = 0;
	cursorPos = 6;

	while (sysStage == 3)
	{
		isLED = true;
		if (clickmode == 1) {
			mySCoop.Atomic++;
			clickmode = 2;
			sysStage = 4;
			mySCoop.Atomic--;
		}

		if (cursorPos >= 6 && cursorPos <= 8) {

			if (encoderVal <= 255 && encoderVal >= 0)
			{
				usrconfig[cursorPos] = encoderVal;
			}
			else {
				if (encoderVal > 255)
				{
					encoderVal = 0;
				}
				else {
					encoderVal = 255;
				}
			}
			if (clickmode == 0) {
				cursorPos++;
				encoderVal = usrconfig[cursorPos];
				clickmode = 2;
			}
		}
		else
		{
			cursorPos = 6;
		}
		mySCoop.Atomic++;
		updateLedConfig(1);
		mySCoop.Atomic--;
		display.clearDisplay();
		display.setCursor(0, 0);
		display.println("LED1 R G B");
		display.setCursor(0, 16);
		display.println(ph4Str);
		display.display();
		sleep(300);
	}
	encoderVal = 0;
	cursorPos = 9;
	while (sysStage == 4)
	{
		isLED = true;

		if (clickmode == 1) {
			mySCoop.Atomic++;
			clickmode = 2;
			sysStage = 5;
			mySCoop.Atomic--;
			break;
		}

		if (cursorPos >= 9 && cursorPos <= 11) {

			if (encoderVal <= 255 && encoderVal >= 0)
			{
				usrconfig[cursorPos] = encoderVal;
			}
			else {
				if (encoderVal > 255)
				{
					encoderVal = 0;
				}
				else {
					encoderVal = 255;
				}
			}
			if (clickmode == 0) {
				cursorPos++;
				encoderVal = usrconfig[cursorPos];
				clickmode = 2;
			}
		}
		else
		{
			cursorPos = 9;
		}
		mySCoop.Atomic++;
		updateLedConfig(2);
		mySCoop.Atomic--;
		display.clearDisplay();
		display.setCursor(0, 0);
		display.println("LED2 R G B");
		display.setCursor(0, 16);
		display.println(ph4Str);
		display.display();
		sleep(300);
	}
	while (sysStage == 5) {

		display.clearDisplay();
		display.setCursor(0, 0);
		display.println("CONFIGURED");
		display.setCursor(0, 16);
		display.println("SYS ACTIVE");
		display.display();
		sleep(2000);
		display.fillScreen(BLACK);
		display.display();
		mySCoop.Atomic++;
		isLED = false;
		clickmode = 2;
		sysStage = 6;
		mySCoop.Atomic--;
	}
	while (sysStage == 6) {
		configOK = true;
		while (configOK)
		{
			if (!isLighted) {
				isLater();
				//Serial.println(isLater());
			}
			sleep(4200);
			break;
		}
		//sleep(5000);
		break;
	}
}
defineTaskLoop(blink) {
	while (isLED) {
		strip.setPixelColor(0, usrconfig[7], usrconfig[6], usrconfig[8]);
		strip.setPixelColor(1, usrconfig[10], usrconfig[9], usrconfig[11]);
		strip.show();
		sleep(700);
		strip.setPixelColor(1, 0, 0, 0);
		strip.setPixelColor(0, 0, 0, 0);
		strip.show();
		sleep(700);
		//break;
	}
		sleep(300);
}
defineTaskLoop(TaskEncoderCont) {
	//if (isTime) {


	//}
	static int oldA = HIGH;
	static int oldB = HIGH;
	int change = 0;
	int result = 0;
	int newA = digitalRead(ENCODER_B);//Reversed design(by mistake)
	int newB = digitalRead(ENCODER_A);
	if (digitalRead(ENCODER_PUSH) == 1) {
		sleep(100);
		if (digitalRead(ENCODER_PUSH) == 1) {
			sleep(300);
			if (digitalRead(ENCODER_PUSH) == 1) {
				//Serial.println("Long click");
				clickmode = 1;
			}
			else
			{
				//Serial.println("clicked");
				clickmode = 0;
			}
		}
		else {
			clickmode = 2;
		}
	}



	if (newA != oldA || newB != oldB) {
		if (oldA == HIGH && newA == LOW) {
			result = (oldB * 2 - 1);
		}
	}
	oldA = newA;
	oldB = newB;
	change = result;
	encoderVal = encoderVal + change;
	//if (change != 0) {
		//Serial.println(encoderVal);
	//}
}
defineTaskLoop(pwr) {
		//Serial.println("PWR HACKED");
	if (usrconfig[5] < 10) {
		yield();
	}
		mySCoop.Atomic++;
		digitalWrite(6, HIGH);
		sleep(200);
		digitalWrite(6, LOW);
		mySCoop.Atomic--;
		sleep(1000 * usrconfig[5]);
	}
defineTaskLoop(PWMLED) {
		is2MinEal();
		while (isTime)
		{
		/*	Serial.print("dimmerVal=");
			Serial.println(dimmerVal);
			Serial.print("isLED:is2al=");
			Serial.print(isLED);
			Serial.println();
			*/
			if (dimmerVal <= 0) {
				isLED = false;
				dimmerVal = 0;
				isTime = false;
				digitalWrite(11, LOW);
				isLighted = true;
				break;
			}
			else {	
				
				unsigned long inteval = (1000 * (usrconfig[2] * 3600L + usrconfig[3] * 60L))/usrconfig[4];
			//	Serial.print("LIGHTED!intval="); 
				//Serial.println(inteval);// * 3600 + (60 * usrconfig[3])) / usrconfig[4]);
				//Serial.print("conf[3]="); Serial.println(usrconfig[3]);
				//Serial.print("conf[4]="); Serial.println(usrconfig[4]);

				mySCoop.Atomic++;
				isLED = false;
				dimmerVal = dimmerVal - (int)(255 / usrconfig[4]);
				analogWrite(11, dimmerVal);
				sleep(inteval);

				mySCoop.Atomic--;

			//	break;
			}
		}
	}
