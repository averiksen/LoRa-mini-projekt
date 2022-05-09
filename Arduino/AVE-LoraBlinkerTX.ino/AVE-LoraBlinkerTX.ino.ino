/*
 * Lora to Lora (P2P) using RN2483 and ESP8266
 * This sketch will transmit a lora message with payload "20" every 2 seconds. 
 * An LED will blink every time a package is sent. Payload and package ID is printed on serial monitor. 
 * 
 * Code based on JP Meijers code from 30 September 2016
 * MN Petersen, Aug 2020
 * 
 * CONNECTIONS:
 * RN2483 - ESP8266:
 * TX     - D5 
 * RX     - D6
 * 3V3    - 3V3
 * GND    - GND
 * 
 * D7 on ESP8266 is connected to anode (long leg) on LED. Insert resistor between cathode and GND on ESP8266.
 * 
 */

#include <SoftwareSerial.h>
//SoftwareSerial loraSerial(14, 12, false, 128); // (rxPin (D5), txPin (D6), inverse_logic, buffer size); 
SoftwareSerial loraSerial(14, 12); // (rxPin (D5), txPin (D6), inverse_logic, buffer size);



String str;

//AVE - For synchronization
unsigned long time_ref = 0; 
unsigned long time_interval = 2000;
unsigned long time_before = 0;
int sync_pin = 5;
int sync_LED = 4;



//AVE - sinusoidal signal
uint8_t i = 0;
uint8_t sinus[] = {127, 166, 202, 230, 248, 255, 248, 230, 202, 166, 127, 87, 51, 23, 5, 0, 5, 23, 51, 87};
unsigned long time1 = 0;
unsigned long time2 = 0;


void setup() {

  
  //output LED pin
  pinMode(13, OUTPUT);  // D7 on ESP8266
  led_off();
  
  // Open serial communications and wait for port to open:
  Serial.begin(115200);  // Serial communication to PC
  
  loraSerial.begin(9600);  // Serial communication to RN2483
  loraSerial.setTimeout(1000);
  lora_autobaud();
  
  led_on();
  delay(1000);
  led_off();

  Serial.println("Initing LoRa");
  
  loraSerial.listen();
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  loraSerial.println("sys get ver");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("mac pause");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
//  loraSerial.println("radio set bt 0.5");  // Uncomment if we want to use FSK 
//  wait_for_ok();
  
  loraSerial.println("radio set mod lora"); // Comment if we want to use FSK 
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set freq 869100000");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set pwr 14");  //max power 14 dBm
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set sf sf7");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set afcbw 41.7");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set rxbw 125");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
//  loraSerial.println("radio set bitrate 50000");
//  wait_for_ok();
  
//  loraSerial.println("radio set fdev 25000");
//  wait_for_ok();
  
  loraSerial.println("radio set prlen 8");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set crc on");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set iqi off");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set cr 4/5");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set wdt 60000"); //disable for continuous reception
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set sync 12");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set bw 125");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  Serial.println("starting loop");
}

void loop() {
    
    led_on();
    time_ref = millis();
    loraSerial.println("radio tx " + String(message(millis()), HEX));
    //str = loraSerial.readStringUntil('\n');
    //Serial.println(str);
    //str = loraSerial.readStringUntil('\n');
    //Serial.println(str);
    time1 = millis()-time_ref;
    led_off();

    Serial.println(time1);
    delay(100);
    
}

void lora_autobaud(){
  String response = "";
  while (response=="")
  {
    delay(1000);
    loraSerial.write((byte)0x00);
    loraSerial.write(0x55);
    loraSerial.println();
    loraSerial.println("sys get ver");
    response = loraSerial.readStringUntil('\n');
  }
}

/*
 * This function blocks until the word "ok\n" is received on the UART,
 * or until a timeout of 3*5 seconds.
 */
int wait_for_ok()
{
  str = loraSerial.readStringUntil('\n');
  if ( str.indexOf("ok") == 0 ) {
    return 1;
  }
  else return 0;
}

void led_on()
{
  digitalWrite(13, 1);
}

void led_off()
{
  digitalWrite(13, 0);
}

unsigned long message(unsigned long time2) {
  unsigned long message = 0x8000000; 
  message = message | (time2 << 8);
  message = message | sinus[i];
  //Serial.println(message, BIN);
  
  if (i >= 19) {
    i = 0;
  } else {
    i++;
  }
  return message;
}
