// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       nndimmer.ino
    Created:	08/18/2019 22:56:47
    Author:     DESKTOP-F29UT1Q\Rambo Rambo
*/

// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//


// Define Functions below here or use other .ino or cpp files
//

// The setup() function runs once each time the micro-controller starts
#define ENCODER_A 12
#define ENCODER_B 13
#define ENCODER_PUSH 1
#define PIN A0
#define MAX_LED 2
#define ADD true
#define SUB false
#define OLED_RESET 4


#include <Arduino_FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <Adafruit_NeoPixel.h>
#include <DS3231.h>
#include <splash.h>
#include <Adafruit_SSD1306.h>
#include <gfxfont.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GFX.h>
#include <queue.h>

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int change = 0;
int encoderVal = 0;
int currentEncoderVal = 0;
int cursorPos = 0;
static int clickmode = 2;

Adafruit_SSD1306 display(OLED_RESET);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(MAX_LED, PIN, NEO_RGB + NEO_KHZ800);
DS3231 rtc(SDA, SCL);

QueueHandle_t queue; 
void TaskLEDFlash(void *pvParameters);
static void TaskDisplay(void *pvParameters);
static void TaskEncoder(void *pvParameters);
void TaskExtPower(void *pvParameters);
static void TaskClickmode(void *pvParameters);
void Tasktest(void *pvParameters);


struct frame {
	int clickmode;
	int encodervalue;
	int cursor;
	bool isFin;
};

void setup()
{
	queue = xQueueCreate(5, sizeof(struct frame));
	Serial.begin(115200);
	pinMode(11, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(ENCODER_A, INPUT_PULLUP);
	pinMode(ENCODER_B, INPUT_PULLUP);
	pinMode(ENCODER_PUSH, INPUT);	//rtc.setDate(18, 8, 2019);
	rtc.begin();
	strip.begin();
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	delay(300);
	display.display(); // show splashscreen
	delay(2000);
	display.clearDisplay();   // clears the screen and buffer
	delay(300);
	//xTaskCreate(TaskLEDFlash, (const portCHAR *) "WS2812", configMINIMAL_STACK_SIZE+50, NULL, 0, NULL);
	xTaskCreate(TaskEncoder, (const portCHAR *) "ENCODER", configMINIMAL_STACK_SIZE,NULL, 2, NULL);
	xTaskCreate(TaskDisplay, (const portCHAR *) "DISPLAY", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate(TaskClickmode, (const portCHAR *) "CLICK", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	//xTaskCreate(Tasktest, "Test", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	//vTaskStartScheduler();	
	//rtc.setTime(23, 11, 30);

}

// Add the main program code into the continuous loop() function
void loop(){
	yield();
}

void displayTime() {
	char * realT = rtc.getTimeStr();
	char * realD = rtc.getDateStr();
	display.clearDisplay();
	//Time rawTime = rtc.getTime();
	//sprintf(timeStr, "%u:%u:%u", rawTime.hour, rawTime.min, rawTime.sec);
	//timeStr+= rawTime.min + ":" + rawTime.sec;
	//Serial.println(rawTime.sec);
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.setCursor(0, 0);
	display.println(realD);
	display.setCursor(0, 16);
	display.println(realT);
	display.setTextColor(WHITE); 
	display.display();

}
static void TaskLEDFlash(void *pvParameters) {
	for (;;) {
		uint32_t color = strip.Color(0, 100, 255);
		uint32_t color1 = strip.Color(0, 255, 0);
		uint32_t off = strip.Color(0, 255, 0);
		strip.setPixelColor(0, color);
		strip.setPixelColor(1, color1);
		strip.show();
		vTaskDelay(30);
		strip.setPixelColor(0, off);
		strip.setPixelColor(0, off);
		strip.show();
	}
}
static void Tasktest(void *pvParameters) {
	(void)pvParameters;
	for (;;) {
		Serial.println("alive");
		vTaskDelay(20);
	}
}
static void TaskDisplay(void *pvParameters) {
	(void)pvParameters;
	while (true) {
		display.clearDisplay();
		//Time rawTime = rtc.getTime();
		//sprintf(timeStr, "%u:%u:%u", rawTime.hour, rawTime.min, rawTime.sec);
		//timeStr+= rawTime.min + ":" + rawTime.sec;
		//Serial.println(rawTime.sec);
		display.setTextSize(2);
		display.setTextColor(WHITE);
		display.setCursor(0, 0);
		display.println(rtc.getDateStr());
		display.setCursor(0, 16);
		display.println(rtc.getTimeStr());
		display.display();
		//yield();
		vTaskDelay(60);
	}
}
static void TaskEncoder(void *pvParameters) {
	(void)pvParameters;
	for (;;) {
		static int oldA = HIGH;
		static int oldB = HIGH;
		int change = 0;
		int result = 0;
		int newA = digitalRead(ENCODER_B);//Reversed design(by mistake)
		int newB = digitalRead(ENCODER_A);
		if (newA != oldA || newB != oldB) {
			if (oldA == HIGH && newA == LOW) {
				result = (oldB * 2 - 1);
			}
		}
		oldA = newA;
		oldB = newB;
		change = result;
		encoderVal = encoderVal + change;
		if (clickmode == 0)
		{
			encoderVal = 0;
			clickmode = 2; //reset click pending bits
		}
		if (change != 0) {
			Serial.println(encoderVal);
		}
		vTaskDelay(20);
	}
}
static void TaskClickmode(void *pvParameters) {
	(void)pvParameters;
	while (1) {
		if (digitalRead(ENCODER_PUSH) == 1) {
			vTaskDelay(10);
			if (digitalRead(ENCODER_PUSH) == 1) {
				vTaskDelay(32);
				if (digitalRead(ENCODER_PUSH) == 1) {
					Serial.println("Long click");
					clickmode = 1;
				}
				else
				{
					Serial.println("clicked");
					clickmode = 0;
				}
			}
		}
		clickmode = 2;
	}
	vTaskDelay(20);
}
static void TaskExtPower(void *pvParameters) {
	(void)pvParameters;
	digitalWrite(6, HIGH);
	vTaskDelay(150 / portTICK_PERIOD_MS);
	digitalWrite(6, LOW);
}
/*
void ledFlash() {
	/*
		
	
	
}

void powerEnabler() {
	digitalWrite(6, HIGH);
	delay(150);
	digitalWrite(6, LOW);
}


int getEncoderTurn()
{
	static int oldA = HIGH;
	static int oldB = HIGH;
	int result = 0;
	int newA = digitalRead(ENCODER_B);//Reversed design(by mistake)
	int newB = digitalRead(ENCODER_A);
	if (newA != oldA || newB != oldB) {
		if (oldA == HIGH && newA == LOW) {
			result = (oldB * 2 - 1);
		}
	}
	oldA = newA;
	oldB = newB;
	return result;
}

int isClick() {
	/**
		1 for long click
		0 for single click
		2 for non click
	
	if (digitalRead(ENCODER_PUSH) == 1) {
		delay(150);
		if (digitalRead(ENCODER_PUSH) == 1) {
			delay(500);
			if (digitalRead(ENCODER_PUSH) == 1) {
				Serial.println("Long click");
				return 1;
				delay(150);
			}
			else
			{
				Serial.println("clicked");
				return 0;
			}
		}
	}
	return 2;
}

int getEncoderVal() {
	int change = getEncoderTurn();
	encoderVal = encoderVal + change;
	if (isClick() == 0)
	{
		encoderVal = 0;
	}
	if (change != 0) {
		Serial.println(encoderVal);
	}
	return encoderVal;
}
*/
