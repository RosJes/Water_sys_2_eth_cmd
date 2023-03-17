#include <Arduino.h>
#include "Water_pump.h"
#include "menu_setup.h"
#include <LiquidCrystal.h>
#include "Adafruit_seesaw.h"
//logic for menu auto or manual settings
//int mode=TEST_MODE;//AUTO_MODE;//default
//powersave options
int button_disable=0;
const int dry = 796; // value for dry sensor
const int wet = 239; // value for wet sensor
void change_button_state()
{
    digitalWrite(BUTTON_PORT,LOW);
    //if signal is in HIGH then PUMP_PORT IS HIGH
}
int give_stats()
{
    int sensorVal = analogRead(A0);
    delay(100);
    return  map(sensorVal, wet, dry, 100, 0); 
}
void write_status(LiquidCrystal lcd,const char* message1,const char*message2,int message3,const char* message4)
{
     lcd.clear();
        lcd.print(message1);
        lcd.setCursor(0, 1);
        lcd.print(message2);
        lcd.print(message3);
        lcd.print(message4);
}
void buzz()
{
    digitalWrite(BUZZER_PORT,HIGH);
    delay(200);
    digitalWrite(BUZZER_PORT,LOW);

}
void water_plant(Adafruit_seesaw ss,LiquidCrystal lcd,int mode,bool once)
{
    
    if (mode==TEST_MODE)
    {
        int sensorVal = analogRead(A0);
        delay(100);
       if (once==0) //when changing channel set this to zero
       {
            write_status(lcd,"Mode: TEST","sensor ",sensorVal," ");
            once =1;
       }
        while (digitalRead(BUTTON_PORT)==LOW) //this button will be an interrupt
        {
            buzz();
            digitalWrite(LED_BUILTIN,HIGH);
            turn_on_pump(HIGH);
        }
        
        digitalWrite(LED_BUILTIN,LOW);
        turn_on_pump(LOW);
       
    }
    if (mode==MAN_MODE)
    {
        int sensorVal = analogRead(A0);
        delay(100);
        int percentageHumididy = map(sensorVal, wet, dry, 100, 0); //implement timer for polling
        if (once==0) //when changing channel set this to zero, //if state changes via timer, if timer is done set once to 0 then 1 again
       {
        lcd.clear();
        lcd.print("Mode: MANUAL");
        lcd.setCursor(0, 1);
        if (percentageHumididy>50)
        {
            lcd.print("I AM FULL!");
        }
        if (percentageHumididy<=50 && percentageHumididy>=10)
        {
            lcd.print("I FEEL GOOD");
        }
        if (percentageHumididy<10 && percentageHumididy>=5)
        {
            lcd.print("THIRSTY!!");
        }
         if (percentageHumididy<5&& percentageHumididy>=1)
        {
            lcd.print("I NEED H2O NOW"); 
        }
        if (percentageHumididy<1)
        {
            lcd.print("I AM DEAD...");
        }
       
        once =1;
       }
      
    }

     if (mode==AUTO_MODE)
    {
        int sensorVal = analogRead(A0);
        delay(100);
        int percentageHumididy = map(sensorVal, wet, dry, 100, 0); //implement timer for polling
        
       if (once==0) //when changing channel set this to zero or state changes
       {
        write_status(lcd,"Mode: AUTO","H2O LVL: ",percentageHumididy,"%");
        once=1;
       }
       if (percentageHumididy < 5 )
       {

        while (percentageHumididy<=50)
        {
            turn_on_pump(HIGH);//pump should poll humidity levels before turning off
            sensorVal = analogRead(A0);
            percentageHumididy = map(sensorVal, wet, dry, 100, 0);
            write_status(lcd,"Mode: AUTO","H2O LVL: ",percentageHumididy,"%");
            delay(500);
            
        }
       write_status(lcd,"Mode: AUTO","H2O LVL: ",percentageHumididy,"%");
       }
       
    }
    
    //write out text on LCD
}