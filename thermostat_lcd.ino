
/* Cold-Junction-Compensated K-Thermocoupleto-
Digital Converter
Output to LCD

*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

#include "max6675.h"
//int vccPin = 3;// +
//int gndPin = 2; // -
//int thermoDO = 4; //6
//int thermoCS = 5; //CS
//int thermoCLK = 6; //4

//thermowell max6675
int vccPin = 3;
int gndPin = 2;
int thermoDO = 6;
int thermoCS = 5;
int thermoCLK = 4;

//sainsmart 2 channel relay
int RELAY1 = 10;
int RELAY2 = 11;

//minimum amount of time for fridge to run. We don't want to cycle back and forth.
// mutliplying 60 * 1000 does not work
const long MIN_COOLING_TIME = 480000;
//if you double time, have to double modulus - use 4 for 30000
int modulus = 4;

unsigned long previousMillis = 0;


//temp controls

float max_temp = 40.00;
//float min_temp = 69.95;
float min_temp = max_temp - 1;


// 2 -
// 3 +
// 4 SCK
// 5 CS
// 6 SO

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void lcdBackground()
{
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");

  lcd.setCursor(0, 1);
  lcd.print("Cooling: ");
  lcd.setCursor(9, 1);
  //lcd.print("OFF");

  lcd.setCursor(11, 0);
  lcd.print("F");
  lcd.setCursor(12, 0);
  lcd.print((char)223);
}

void setup() {
  Serial.begin(9600);

  lcd.init();                      // initialize the lcd
  Serial.println("Initialize LCD");
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);

  // LCD Arduino pins
  pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
  pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);

  //relay switch
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  // wait for MAX thermocoupler chip to stabilize
  // Serial.println("MAX6675 test.");
  //delay so temp sensor can adjust
  delay(5000);
  Serial.println("MAX6675 thermo ready.");
  displayThermostat();
  //Serial.flush();
}

void displayThermostat()
{

  lcd.setCursor(0, 2);
  lcd.print("Thermostat: ");
  lcd.print(min_temp);
  lcd.print((char)223);
  lcd.print("F");
}

void displayCurrentTemp(float currentTemp)
{
  lcd.setCursor(6, 0);
  lcd.print(currentTemp);

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");

  lcd.setCursor(11, 0);
  lcd.print((char)223);
  lcd.print("F");
}

long loopCount = 0;



void incrLoopCount()
{
  lcd.setCursor(14, 0);
  lcd.print("#");
  lcd.print(++loopCount);
  //Serial.println("Loop count# ");
  Serial.print(loopCount);
}
void loop() {


  unsigned long currentMillis = millis();
  //run every MIN_COOLING_TIME seconds
  //avoiding delay as that seems to cause issues
  //run if initial loop
  // Serial.println(currentMillis - previousMillis);

  //  Serial.println(loopCount);
  //run on first loop
  if ((loopCount == 0) || (currentMillis - previousMillis >= MIN_COOLING_TIME)) {
    //    Serial.println("in loop");
    // save the last time you ran
    previousMillis = currentMillis;

    incrLoopCount();
    lcdBackground();
    float nearest = (roundf(thermocouple.readFahrenheit() * 10)) / 10;

    // Serial.println(thermocouple.readCelsius());
    displayCurrentTemp(nearest);
    lcd.setCursor(9, 1);

    //just read the temp - give it a second!
    //delay(1000);
    //cool
    if (nearest >= max_temp)
    {

      digitalWrite(RELAY1, LOW);
      digitalWrite(RELAY2, LOW);
      lcd.print("ON ");
      // displayThermostat();
      //turn off cooling/heat
      //  chillout();
    } else {

      digitalWrite(RELAY1, HIGH);
      digitalWrite(RELAY2, HIGH);
      lcd.print("OFF");
      // displayThermostat();
      // chillout();
    }
    // wait();
    //can't call this in this nonblocking loop
    //lcd.clear();
  }
  //Serial.flush();

}

void chillout()
{
  /*
    lcd.setCursor(0, 3);

    for (int y = 0; y < (MIN_COOLING_TIME / 1000); y++)
    {

      if (y % modulus == 0)
      {
        //lcd.print(".");

      }
      delay(1000);

    }
    */
  delay(MIN_COOLING_TIME);

}
void wait()
{

  lcd.setCursor(0, 2);
  lcd.print("Checking temp in: ");
  for (int i = 5; i > 0; i--)
  {

    lcd.setCursor(18, 2);
    lcd.print(i);
    delay(1000);

  }



}


