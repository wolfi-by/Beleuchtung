//-------------------------------------------------------------------
#ifndef __gartenmaster_main_H__
#define __gartenmaster_main_H__
//-------------------------------------------------------------------
 
#include <arduino.h>
#include <SPI.h>
#include <w5100.h>
#include <socket.h>
#include <util.h>
#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Ethernet.h>
#include <Dns.h>
#include <Dhcp.h>
#include <SoftSPI.h>
#include <DigitalPin.h>
#include <SdVolume.h>
#include <SdStream.h>
#include <SdSpi.h>
#include <SdInfo.h>
#include <SdFile.h>
#include <SdFatUtil.h>
#include <SdFatStructs.h>
#include <SdFatmainpage.h>
#include <SdFatConfig.h>
#include <SdFat.h>
#include <SdBaseFile.h>
#include <Sd2Card.h>
#include <ostream.h>
#include <MinimumSerial.h>
#include <istream.h>
#include <iostream.h>
#include <ios.h>
#include <bufstream.h>
#include <ArduinoStream.h>
#include <pixeltypes.h>
#include <lib8tion.h>
//#include <hsv2rgb.h>
#include <FastSPI_LED2.h>
#include <fastspi_dma.h>
#include <fastspi_bitbang.h>
#include <fastspi_avr.h>
//#include <fastspi_arm.h>
#include <fastspi.h>
#include <fastpin.h>
//#include <dmx.h>
#include <delay.h>
#include <controller.h>
#include <clockless.h>
#include <chipsets.h>
#include <RS485_protocol.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
 
//-------------------------------------------------------------------
 
//-------------------------------------------------------------------
//Set Number of Channels of all slaves
#define Channelcount 8
 
enum MyTypeLight{
	dark = 0,
	light = 1
};
 
// size of buffer used to capture HTTP requests
#define REQ_BUF_SZ   40
 
// MAC address from Ethernet shield sticker under board
byte mac[] = {
0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 178, 50); // IP address, may need to change depending on network
EthernetServer server(80);  // create a server at port 80
SdFat sd;
//SdFile webFile;
char HTTP_req[REQ_BUF_SZ] = {
0}; // buffered HTTP request stored as null terminated string
char req_index = 0;              // index into HTTP_req buffer
 
 
//variables for Light
int sensorPinLight = 1;
int PercentLight;
int minimumLight = 1023;
int maximumLight = 0;
int sensorValueLight;
//boolean dimmingInProgress=false;
//boolean LightOn=false;
//boolean SendData=false;
int OldPercent;
boolean AllLightsON=false;
boolean WriteBackSettings=false;
byte ColorArray[3];
MyTypeLight lightState=light;
 
//Variables for Temperature
int sensorPinTemp = 0;
int PercentTemp;
int minimumTemp = 1023;
int maximumTemp = 0;
int sensorValueTemp;
 
 
//RS485 definition
const byte ENABLE_PIN = 3;
SoftwareSerial RS485 (2, 5);  // receive pin, transmit pin
 
//-------------------------------------------------------------------
 
//===================================================================
// -> DO NOT WRITE ANYTHING BETWEEN HERE...
// 		This section is reserved for automated code generation
// 		This process tries to detect all user-created
// 		functions in main_sketch.cpp, and inject their
// 		declarations into this file.
// 		If you do not want to use this automated process,
//		simply delete the lines below, with "&MM_DECLA" text
//===================================================================
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_BEG@---------------------
void loop();
void setup();
void RS485_Send(byte * msg);
void freeRam ();
void readSettingsFromSD();
void writeBackSettingsToSD();
void StrClear(char *str, char length);
void SetColorValues(byte Light[4]);
void GetColorValues(EthernetClient cl);
void SwitchLightOFF();
void SwitchLightON();
void SetLightSwitchState(EthernetClient cl);
void GetLightSwitchState(EthernetClient cl);
void GetAnalogSwitchState(EthernetClient cl);
int fRead ();
int fAvailable ();
void fWrite (const byte what);
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_END@---------------------
// -> ...AND HERE. This space is reserved for automated code generation!
//===================================================================
 
 
//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
 
 
 
 
 
 
 
 
