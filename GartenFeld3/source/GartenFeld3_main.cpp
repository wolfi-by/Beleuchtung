#include "gartenfeld3_main.h"
 
//**************************************
//            PIN-BELEGUNG
//**************************************
/*

 1 - frei
 2 -
 3 - Grün PWM
 4 -
 5 - Steine (Blau PWM)
 6 - S�ule (Rot PWM)
 7 -
 8 - EN-Pin RS485
 9 - TX-Pin RS485
 10 - RX-Pin RS485
 11 -
 12 - DataPin FastSPI
 ***************************
 RS485 Busadresse: 4
 ***************************
*/
 
//Eigene Addresse
#define MyAddress 4
 
//defines für WS2811-Controller
#define NUM_LEDS 15 //- Anzahl
#define DATA_PIN 12 //- Pin
//LED-Array definieren
CRGB leds[NUM_LEDS];
 
//RGB-Werte
Color SetTargetValue[5];
Color TargetValue[5];
Color ActualValue[5];
 
//Pins für LED-Streifen PWM
int ColumnPin=6; //Rot
int RelayPin=5; //Blau
 
 
//Dimmen
boolean ValueReached[5];
byte actualChannel;
 
//RS485 definieren
SoftwareSerial rs485 (10, 9);  // receive pin, transmit pin
const byte ENABLE_PIN = 8;
 
//PWM RGB-Streifen berechnen
void DisplayRGB(int r,int g, int b){
	//Streifen ist nur einfarbig
	analogWrite(ColumnPin,r);
	 
	//Gr�ner Kanal ist frei
	 
	//Blauer Kanal schaltet Relais
	if (TargetValue[1].b)
	{
		digitalWrite(RelayPin,HIGH);
	}
	else
	{
		digitalWrite(RelayPin,LOW);
	}
	 
}
 
//RS485 Kommunikationsroutine
void RS485(){
	 
	byte ReceivedMessage[20];
	byte received = recvMsg (fAvailable, fRead, ReceivedMessage, sizeof (ReceivedMessage) - 1);
	 
	if (received)
	{
		delay(5);
		Serial.println("Data received...");
		 
		if (ReceivedMessage[0] == MyAddress)//MyAddress
		{
			 
/*
		Serial.println("Received Data: ");
		for (int i=0;i<sizeof (ReceivedMessage)-1;i++)
		{
Serial.print(ReceivedMessage[i]);
Serial.print(" ");
		}*/
		switch(ReceivedMessage[1])
		{
			case 1://Switchstate:
			{
				if (ReceivedMessage[2])//Kanal 1 einschalten
				{
					TargetValue[0]=SetTargetValue[0];
				}
				else//Kanal ausschalten
				{
					TargetValue[0].r=0;
					TargetValue[0].g=0;
					TargetValue[0].b=0;
				}
				if (ReceivedMessage[3])//Kanal 2 einschalten
				{
					TargetValue[1]=SetTargetValue[1];
				}
				else//Kanal ausschalten
				{
					TargetValue[1].r=0;
					TargetValue[1].g=0;
					TargetValue[1].b=0;
				}
				if (ReceivedMessage[4])//Kanal 3 einschalten
				{
					TargetValue[2]=SetTargetValue[2];
				}
				else//Kanal ausschalten
				{
					TargetValue[2].r=0;
					TargetValue[2].g=0;
					TargetValue[2].b=0;
				}
				if (ReceivedMessage[5])//Kanal 4 einschalten
				{
					TargetValue[3]=SetTargetValue[3];
				}
				else//Kanal ausschalten
				{
					TargetValue[3].r=0;
					TargetValue[3].g=0;
					TargetValue[3].b=0;
				}
				if (ReceivedMessage[6])//Kanal 5 einschalten
				{
					TargetValue[4]=SetTargetValue[4];
				}
				else//Kanal ausschalten
				{
					TargetValue[4].r=0;
					TargetValue[4].g=0;
					TargetValue[4].b=0;
				}
			}
			break;
			case	2://SetColorValue:
			{
				if ((ReceivedMessage[5]>=0) && (ReceivedMessage[5]<=5))
				{
					SetTargetValue[ReceivedMessage[5]].r=ReceivedMessage[2];//Red-Value
					SetTargetValue[ReceivedMessage[5]].g=ReceivedMessage[3];//Green-Value
					SetTargetValue[ReceivedMessage[5]].b=ReceivedMessage[4];//Blue-Value
					TargetValue[ReceivedMessage[5]]=SetTargetValue[ReceivedMessage[5]];//Take new value as targe
				}
			}
			break;
			case	3://SetFlicker:
			{
				Serial.println("No flicker programmed");
			}
			break;
			case 4://	SendState:
			{
				 
			}
			break;
			case	5://SendColorValues:
			{
				 
			}
			break;
			case 6://	SetDimmingtime:
			{
				 
			}
			break;
		}
/*       OLD ROUTINE
if (buf [1] == 2)//This Command sets Light to a value
      {
        Serial.println("Switch light");

        if (buf[2] == 1){
          Serial.println("Light off");
         dimmPWM=true;
         dimmSPI=true;
          SetLight(0,0,0);
        }
       if (buf[3]==1){
        Serial.println("Nur SPI aus");
        dimmPWM=false;
        dimmSPI=true;
      }
      if (buf[4]==1){
        Serial.println("Nur PWM aus");
        dimmPWM=true;
        dimmSPI=false;
      }
       
       
       
        if (buf[2] == 2){
          Serial.println("Light on");
          dimmPWM=true;
          dimmSPI=true;
          SetLight(157,50,1);
        }
      if (buf[3]==2){
        Serial.println("Nur SPI an");
        dimmPWM=false;
        dimmSPI=true;
      }
      if (buf[4]==2){
        Serial.println("Nur PWM an");
        dimmPWM=true;
        dimmSPI=false;
		}
		//Antwortnachricht
		byte msg [] = {
			0,  // device 0 (master)
			2,  // turn light on command received
			1   //Nothing is done... is set later
		};
		 
		delay (1);  // give the master a moment to prepare to receive
		digitalWrite (ENABLE_PIN, HIGH);  // enable sending
		sendMsg (fWrite, msg, sizeof msg);
		digitalWrite (ENABLE_PIN, LOW);  // disable sending
		*/
	}
}
}  // end if something received
 
 
 
 
//RS485 Schreibroutine
void fWrite (const byte what)
{
	rs485.write (what);
}
 
//RS485 Prüfroutine
int fAvailable ()
{
	return rs485.available ();
}
 
//RS485 Leseroutine
int fRead ()
{
	return rs485.read ();
}
 
void SetLight()
{
	 
	boolean RedUP=false;
	boolean RedDown=false;
	boolean GreenUP=false;
	boolean GreenDown=false;
	boolean BlueUP=false;
	boolean BlueDown=false;
	 
	if (ActualValue[actualChannel].r < TargetValue[actualChannel].r){RedUP=true;Serial.println("Red up");}
	if (ActualValue[actualChannel].r > TargetValue[actualChannel].r){RedDown=true;Serial.println("Red down");}
	if (ActualValue[actualChannel].g < TargetValue[actualChannel].g){GreenUP=true;Serial.println("Green up");}
	if (ActualValue[actualChannel].g > TargetValue[actualChannel].g){GreenDown=true;Serial.println("Green down");}
	if (ActualValue[actualChannel].b < TargetValue[actualChannel].b){BlueUP=true;Serial.println("Blue up");}
	if (ActualValue[actualChannel].b > TargetValue[actualChannel].b){BlueDown=true;Serial.println("Blue down");}
	 
	Serial.print("Channel: ");
	Serial.println(actualChannel);
	Serial.println(ActualValue[actualChannel].r);
	Serial.println(TargetValue[actualChannel].r);
	Serial.println("---");
	 
	if(RedUP){ ActualValue[actualChannel].r++;Serial.println("Red add");}
	if(RedDown){ ActualValue[actualChannel].r--;Serial.println("Red sub");}
	if(GreenUP){ ActualValue[actualChannel].g++;Serial.println("Green add");}
	if(GreenDown){ ActualValue[actualChannel].g--;Serial.println("Green sub");}
	if(BlueUP){ ActualValue[actualChannel].b++;Serial.println("Blue add");}
	if(BlueDown){ ActualValue[actualChannel].b--;Serial.println("Blue sub");}
	 
	if (actualChannel==0) //LED-Streifen Schalten
	{
		 
		DisplayRGB(ActualValue[actualChannel].r,ActualValue[actualChannel].g,ActualValue[actualChannel].b); //PWM ausgeben
	}
	if (actualChannel==1)
	{
		//Fast-SPI selben Wert wie LED-Streifen zuweisen
		for(int a=0;a<NUM_LEDS;a++){
			leds[a].red = ActualValue[actualChannel].r;
			leds[a].green= ActualValue[actualChannel].g;
			leds[a].blue= ActualValue[actualChannel].b;
			FastLED.show(); //Fast-SPI LED Wert anzeigen
		}
		 
	}
	 
	//test if all values are reached.
	if (
	(ActualValue[actualChannel].r == TargetValue[actualChannel].r) &
	(ActualValue[actualChannel].g == TargetValue[actualChannel].g) &
	(ActualValue[actualChannel].b == TargetValue[actualChannel].b)
	) ValueReached[actualChannel]=true;
	 
}
 
//*******************************************************
//               SETUP
//*******************************************************
void setup(){
  //Fast-SPI initialisieren
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

  //Zuerst PWM-Treiber disable
  Serial.println("Set PWM off");
  pinMode(RelayPin,OUTPUT);
  digitalWrite(RelayPin,LOW);
  analogWrite(ColumnPin,0);
  DisplayRGB(0,0,0);//LED-Streifen auf Schwarz
  
for(int a=0;a<NUM_LEDS;a++){
			leds[a].red = 0;
			leds[a].green= 0;
			leds[a].blue= 0;
			FastLED.show(); //Fast-SPI LED Wert anzeigen
		}

Serial.begin(9600);
  Serial.println("Starting Slave...");
  delay(1000);
  

  Serial.println("Set SPI off");
  //Alle Fast-SPI-Lampen aus
  for (int a=0;a<NUM_LEDS;a++){
    leds[a]=CRGB::Black;
  }
  Serial.println("Init RS485");
  //RS485 Initialisieren
  rs485.begin (9600);
  pinMode (ENABLE_PIN, OUTPUT);  // driver output enable
  Serial.println("Start loop...");
  
  
  //*************Init ohne RS485 ***************
  for (int i=0;i<5;i++)
  {
	TargetValue[i].r=136;
	TargetValue[i].g=40;
	TargetValue[i].b=0;
	}
	//***************** INIT END ****************
}

void loop(){

  RS485();
  SetLight();
  actualChannel++;
  if (actualChannel>4) actualChannel=0;
  
}










