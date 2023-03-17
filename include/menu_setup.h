#include <LiquidCrystal.h>
#include "Adafruit_seesaw.h"

#define BUTTON_PORT 7 //analog pin A0
#define BUZZER_PORT 13
#define MOIST_PORT A0

#define TEST_MODE   3
#define MAN_MODE    2
#define AUTO_MODE   1

#define rs 12
#define en 11
#define d4 5
#define d5 4
#define d6 3
#define d7 2

void water_plant(Adafruit_seesaw ss,LiquidCrystal lcd,int mode,bool once);
void change_button_state();
void buzz();
int give_stats();