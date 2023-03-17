//relay setup and pump . send signal to pump when port is 1
#include <Arduino.h>
#include "Water_pump.h"
#include "menu_setup.h"

void turn_on_pump(int status)
{
    digitalWrite(PUMP_PORT,status);
    delay(2000);
    digitalWrite(PUMP_PORT,LOW);
    //if signal is in HIGH then PUMP_PORT IS HIGH
}


