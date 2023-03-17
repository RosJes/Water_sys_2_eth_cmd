#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#include "menu_setup.h"
#include "Water_pump.h"
#include <LiquidCrystal.h>

#define _OPEN_SYS_ITOA_EXT
 //eth variables
 byte mac[] = {
  0xA8, 0x61, 0x0A, 0xAE, 0x18, 0xC1
};
IPAddress ip(192, 168, 0, 4);
unsigned int localPort = 8888;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
//char ReplyBuffer[] = "Received";        // a string to send back
EthernetUDP Udp;
//water sys setup
String message= "Test Mode";
String reply="";
 LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 const int RECV_PIN = 1;
 Adafruit_seesaw ss;
 long remote_code=0;
 int mode=TEST_MODE;
 bool once_is_on=0;
 int counter=0;
char buffer [sizeof(int)*8+1];
void setup() {
//eth setup
  Ethernet.begin(mac);
  Ethernet.setLocalIP(ip); 
  Serial.begin(9600);
  // start UDP
  Udp.begin(localPort);

  // initialize the digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PUMP_PORT,OUTPUT);
  pinMode(BUTTON_PORT,INPUT_PULLUP);
  pinMode(MOIST_PORT,INPUT);
  //attachInterrupt(BUTTON_PORT,change_button_state,FALLING);
  
  Serial.begin(9600);
  lcd.begin(16, 2);
  water_plant(ss,lcd,mode,once_is_on);
  once_is_on=1;

}

void loop() {
   
  // put your main code here, to run repeatedly:
  //switch for incoming packets
  // if there's data available, read a packet
  memset(packetBuffer, '\0', sizeof(packetBuffer)); 
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    IPAddress remote = Udp.remoteIP();
    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    message= String(packetBuffer);
    if (message=="status") //bugg flushar inte minnet
   {
    Serial.println(give_stats());
   }
   if (message=="water") //bugg flushar inte minnet
   {
     buzz();
     turn_on_pump(HIGH);
   }
   if (message=="changemode1")
   {
      buzz();
      mode= AUTO_MODE;
      once_is_on=0;
   }
   if (message=="changemode2")
   {
      buzz();
      mode= MAN_MODE;
      once_is_on=0;
   }
   if (message=="changemode3")
   {
      buzz();
      mode= TEST_MODE;
      once_is_on=0;
   }
   
    water_plant(ss,lcd,mode,once_is_on);
   
    // send a reply to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(itoa(give_stats(),buffer,__DECIMAL_DIG__));//??
    Udp.endPacket();
    Udp.flush();
  }
  water_plant(ss,lcd,mode,1);
    counter++;
    delay(1000);
    if (counter==5)//update mode
    {
      water_plant(ss,lcd,mode,0);
      counter=0;
    }
}