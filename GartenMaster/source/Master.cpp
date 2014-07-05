 
#include "GartenMaster_main.h"
 
// callback routines
void fWrite (const byte what)
{
	RS485.write (what);
}
 
int fAvailable ()
{
	return RS485.available ();
}
 
int fRead ()
{
	return RS485.read ();
}
 
//Rising-Falling-Edge Detection for Light Min/max
boolean Limit(int min, int max,int actual, MyTypeLight *state)
{
	MyTypeLight lastState = *state;
	if(actual > max)
	{
		lastState = light;
	}
	else if(actual < min)
	{
		lastState = dark;
	}
	if(lastState != *state)
	{
		*state = lastState;
		return true;
	}
	else
	{
		return false;
	}
}
 
 
 
// send the state of the switch to the web browser
void GetAnalogSwitchState(EthernetClient cl)
{
	//Send Analogvalues to HTML-Page
	 
	cl.print(F("<p>Percent Temperature: "));
	cl.print(PercentTemp);
	cl.println(F("</p>"));
	cl.print(F("<p>Percent Light: "));
	cl.print(PercentLight);
	cl.println(F("</p>"));
	 
}
 
void GetLightSwitchState(EthernetClient cl)
{
	//Send Analogvalues to HTML-Page
	 
	if (AllLightsON)
	{
		Serial.println(F("Webpage: Light on"));
		cl.println(F("Beleuchtung ist AN"));
	}
	else
	{
		Serial.println(F("Webpage: Light off"));
		cl.println(F("Beleuchtung ist AUS"));
	}
}
 
void SetLightSwitchState(EthernetClient cl)
{
	//Send Analogvalues to HTML-Page
	 
	if (AllLightsON)
	{
		Serial.println(F("Webpage: Light on"));
		cl.println(F("Beleuchtung ist AN"));
		SwitchLightON();
	}
	else
	{
		Serial.println(F("Webpage: Light off"));
		cl.println(F("Beleuchtung ist AUS"));
		SwitchLightOFF();
	}
}
 
 
void SwitchLightON(){
	Serial.println(F("RS485 Switching Light ON"));
	byte msg [] = {
		1,    // device 1
		1,    //Switch Channel
		1,    // CH1 ON
		1,     //CH 2 ON
		1,
		1
	};
	// send to slave
	//RS485_Send(msg);
	digitalWrite (ENABLE_PIN, HIGH);  // enable sending
	sendMsg (fWrite, msg, sizeof msg);
	digitalWrite (ENABLE_PIN, LOW);  // disable sending
}
 
void SwitchLightOFF(){
	Serial.println(F("RS485 switching Light OFF"));
	byte msg [] = {
		1,    // device 1
		1,    //Switch Channel
		0,    // CH1 OFF
		0     //CH 2 OFF
	};
	// send to slave
	//RS485_Send(msg);
	digitalWrite (ENABLE_PIN, HIGH);  // enable sending
	sendMsg (fWrite, msg, sizeof msg);
	digitalWrite (ENABLE_PIN, LOW);  // disable sending
}
 
void GetColorValues(EthernetClient cl){
	cl.println(F("Aktuelle Farbwerte:"));
	cl.print(F("<table border=\"1\">"));
	cl.print(F("<tr><th>Kanal</th><th>Rot</th><th>Gelb</th><th>Blau</th></tr>"));
	//Write a table with all colorvalues
	for (byte i=0;i<Channelcount;i++){
		cl.print(F("<tr><td>"));
		cl.print(EEPROM.read(i*4));
		cl.print(F("</td><td>"));
		cl.print(EEPROM.read(i*4+1));
		cl.print(F("</td><td>"));
		cl.print(EEPROM.read(i*4+2));
		cl.print(F("</td><td>"));
		cl.print(EEPROM.read(i*4+3));
		cl.print(F("</td></tr>"));
	}
	cl.println(F("</table>"));
}
 
 
void SetColorValues(byte Light[4]){
	EEPROM.write(Light[0]*4,Light[0]);
	EEPROM.write(Light[0]*4+1,Light[1]);
	EEPROM.write(Light[0]*4+2,Light[2]);
	EEPROM.write(Light[0]*4+3,Light[3]);
	 
	Serial.println(F("Saved Lightvalues:"));
	Serial.print(F("Channel: "));
	Serial.println(Light[0]);
	Serial.print(F("Red: "));
	Serial.println(Light[1]);
	Serial.print(F("Green: "));
	Serial.println(Light[2]);
	Serial.print(F("Bluie: "));
	Serial.println(Light[3]);
	 
}
// sets every element of str to 0 (clears array)
void StrClear(char *str, char length)
{
	for (int i = 0; i < length; i++) {
		str[i] = 0;
	}
}
 
// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
char StrContains(char *str, char *sfind)
{
	char found = 0;
	char index = 0;
	char len;
	 
	len = strlen(str);
	 
	if (strlen(sfind) > len) {
		return 0;
	}
	while (index < len) {
		if (str[index] == sfind[found]) {
			found++;
			if (strlen(sfind) == found) {
				return 1;
			}
		}
		else {
			found = 0;
		}
		index++;
	}
	 
	return 0;
}
 
void writeBackSettingsToSD()
{
	/*SdFile webFile;
	 
	if(  webFile.open("settings.txt", O_WRITE | O_CREAT | O_TRUNC))
	 
	{*/
		byte HI,LO;
		LO=minimumTemp;
		HI=minimumTemp >> 8;
		EEPROM.write(100,LO);
		EEPROM.write(101,HI);
		LO=maximumTemp;
		HI=maximumTemp >> 8;
		EEPROM.write(102,LO);
		EEPROM.write(103,HI);
		LO=minimumLight;
		HI=minimumLight >> 8;
		EEPROM.write(104,LO);
		EEPROM.write(105,HI);
		LO=maximumLight;
		HI=maximumLight >> 8;
		EEPROM.write(106,LO);
		EEPROM.write(107,HI);
		/*webFile.println(minimumTemp);
		webFile.println(maximumTemp);
		webFile.println(minimumLight);
		webFile.println(maximumLight);
		webFile.close();
	}//SDopen
		*/
	} //writeBack
/*
void InitSettingsToSD(){
	SdFile webFile;
	 
	if(  webFile.open("settings.txt", O_WRITE | O_CREAT | O_TRUNC))
	 
	{
		webFile.println(1022);
		webFile.println(1);
		webFile.println(1022);
		webFile.println(1);
		webFile.close();
		minimumTemp=1022;
		maximumTemp=1;
		minimumLight=1022;
		maximumLight=1;
	}//SDopen
} //End Init
	*/
	void readSettingsFromSD()
	{
/*	SdFile webFile;
	char netBuffer[20];
	if (webFile.open("settings.txt", O_READ))
	{
		int chPos = 0;
		int lineNo = 0;
		 
		Serial.println(F("- Settings erfolgreich geladen"));
		 
		while (webFile.available())
		{
			char ch = webFile.read();
			 
			if(ch == '\n') // '\r' oder '\n'
			{
				chPos = 0;
				 
				switch(lineNo)
				{
					case 0: //MAC-Adresse
					sscanf(netBuffer,"%d",&minimumTemp);
					break;
					case 1: //LocalIP
					sscanf(netBuffer,"%d",&maximumTemp);
					break;
					case 2: //Netmask
					sscanf(netBuffer,"%d",&minimumLight);
					break;
					case 3: //Gateway
					sscanf(netBuffer,"%d",&maximumLight);
					break;
				} // switch
				lineNo++;
			} // if(ch == '\n') // '\r' oder '\n'
			else if(ch == '\r') {
				// do nothing
			}
			else if(chPos < 100)
			{
				netBuffer[chPos] = ch;
				chPos++;
				netBuffer[chPos] = 0;
			}
		} // while (myFile.available())
		 
		webFile.close();
	} // if (myFile.isOpen())
	else
	{
		Serial.println(F("- Fehler beim ermitteln der Settings!\n"));
		sd.initErrorHalt();
	}*/
	minimumTemp= (EEPROM.read(101)<<8) | EEPROM.read(100);
	maximumTemp= (EEPROM.read(103)<<8) | EEPROM.read(102);
	minimumLight=(EEPROM.read(105)<<8) | EEPROM.read(104);
	maximumLight=(EEPROM.read(107)<<8) | EEPROM.read(106);
	 
}
 
 
//Show free RAM
void freeRam ()
{
	int free;
	extern int __heap_start, *__brkval;
	int v;
	free= (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
	Serial.print(F("Free System-RAM: "));
	Serial.println(free);
};
 
void RS485_Send(byte * msg){
	digitalWrite (ENABLE_PIN, HIGH);  // enable sending
	sendMsg (fWrite, msg, sizeof msg);
	digitalWrite (ENABLE_PIN, LOW);  // disable sending
}
 
void setup()
{
	//Setup Debug Serial
	delay(1000);
	Serial.begin(9600);       // for debugging
	delay(1000);
	Serial.println(F("\r\n"));
	Serial.println(F("Starting Master..."));
	freeRam();
	delay(1000);
	 
	// disable Ethernet chip
	pinMode(10, OUTPUT);
	digitalWrite(10, HIGH);
	delay(1000);
	 
	Serial.println(F("Starting RS485..."));
	pinMode(ENABLE_PIN, OUTPUT);
	RS485.begin(9600);
	Serial.println(F("RS485 init ok..."));
	delay(1000);
	 
	// initialize SD card
	Serial.println(F("Initializing SD card..."));
	delay(1000);
	if (!sd.begin(4, SPI_HALF_SPEED)) sd.initErrorHalt();
	delay(1000);
	Serial.println(F("SUCCESS - SD card initialized."));
	// check for index.htm file
	SdFile webFile;
	if (!webFile.open("index.htm", O_READ)) {
		sd.errorHalt("opening test.txt for read failed");
	}
	webFile.close();
	Serial.println(F("SUCCESS - Found index.htm file."));
	delay(1000);
	 
	 
	 
	//Read Settings from SD-Card
	Serial.print(F("Try to read settings..."));
	readSettingsFromSD();
	Serial.print(F("Min Temp: "));
	Serial.println(minimumTemp);
	Serial.print(F("Max Temp: "));
	Serial.println(maximumTemp);
	Serial.print(F("Min Light: "));
	Serial.println(minimumLight);
	Serial.print(F("Max Light: "));
	Serial.println(maximumLight);
	delay(1000);
	 
	Serial.print(F("Starting Ethernet... "));
	Ethernet.begin(mac, ip);  // initialize Ethernet device
	server.begin();           // start to listen for clients
	Serial.println(F("OK"));
	delay(1000);
	 
	 
	//Setting up colors for all Slaves
	byte device;
	byte channel;
	for (int i=0;i<Channelcount;i=i++){
		if (i==0) //Slave 1, Channel 0
		{device=1;channel=0;}
		if (i==1) //Slave 1, Channel 1
		{device=1;channel=1;}
		if (i==2) //Slave 2, Channel 0
		{device=2;channel=0;}
		if (i==3) //Slave 2, Channel 1
		{device=2;channel=1;}
		if (i==4) //Slave 3, Channel 0
		{device=3;channel=0;}
		if (i==5) //Slave 3, channel 1
		{device=3;channel=1;}
		if (i==6) //Slave 4, channel 0
		{device=4;channel=0;}
		if (i==7) //Slave 4, channel 1
		{device=4;channel=1;}
		//read Colorvalues for Slaves and channels
		byte red=EEPROM.read(i*4+1);
		byte green=EEPROM.read(i*4+2);
		byte blue=EEPROM.read(i*4+3);
		//debug output
		Serial.print(F("Init Slave "));
		Serial.print(device);
		Serial.print(F(" Channel: "));
		Serial.print(channel);
		Serial.print(F(" "));
		Serial.print(red);
		Serial.print(F(" "));
		Serial.print(green);
		Serial.print(F(" "));
		Serial.print(blue);
		Serial.print(F("... "));
		//create Message for Slave device
		byte msg [] = {
			device,    		//devicenumber
			2,    			//set color command
			red,    		//red color value
			green,     		//green color value
			blue,    		//blue color value
			channel      	//channel
			 
		};//end Message
		//Send Message to Slave via RS485
		//RS485_Send(msg);
		digitalWrite (ENABLE_PIN, HIGH);  // enable sending
		sendMsg (fWrite, msg, sizeof msg);
		digitalWrite (ENABLE_PIN, LOW);  // disable sending
		Serial.println("OK");
		delay(500);
	 /*
	byte msg [] = {
		1,    		//devicenumber
		2,    			//set color command
		140,    		//red color value
		40,     		//green color value
		0,    		//blue color value
		0      	//channel
	};
	digitalWrite (ENABLE_PIN, HIGH);  // enable sending
	sendMsg (fWrite, msg, sizeof msg);
	digitalWrite (ENABLE_PIN, LOW);  // disable sending
	delay(100);
	byte msg2 [] = {
		1,    		//devicenumber
		2,    			//set color command
		140,    		//red color value
		40,     		//green color value
		0,    		//blue color value
		1      	//channel
	};
	digitalWrite (ENABLE_PIN, HIGH);  // enable sending
	sendMsg (fWrite, msg2, sizeof msg2);
	digitalWrite (ENABLE_PIN, LOW);  // disable sending
		delay(1000);*/
	}
	Serial.println(F("Start loop..."));
}
 
void loop(){
	 
	//Get analog values
	sensorValueTemp=analogRead(sensorPinTemp);
	if (sensorValueTemp<minimumTemp)
	{
		minimumTemp=sensorValueTemp;
		WriteBackSettings=true;
		 
	}
	if (sensorValueTemp>maximumTemp)
	{
		maximumTemp=sensorValueTemp;
		WriteBackSettings=true;
	}
	PercentTemp = map(sensorValueTemp, minimumTemp,maximumTemp,0,100);
	 
	sensorValueLight=analogRead(sensorPinLight);
	if (sensorValueLight<minimumLight)
	{
		minimumLight=sensorValueLight;
		WriteBackSettings=true;
	}
	if (sensorValueLight>maximumLight)
	{
		maximumLight=sensorValueLight;
		WriteBackSettings=true;
	}
	if (WriteBackSettings){
		writeBackSettingsToSD();
		WriteBackSettings=false;
	}
	PercentLight = map(sensorValueLight, minimumLight,maximumLight,0,100);
	Serial.println(PercentLight);
	 
	//Werte berechnen aus
	 
	 
	 
	EthernetClient client = server.available();  // try to get client
	if (client) {  // got client?
		boolean currentLineIsBlank = true;
		while (client.connected()) {
			if (client.available()) {   // client data available to read
				char c = client.read(); // read 1 byte (character) from client
				// buffer first part of HTTP request in HTTP_req array (string)
				// leave last element in array as 0 to null terminate string (REQ_BUF_SZ - 1)
				if (req_index < (REQ_BUF_SZ - 1)) {
					HTTP_req[req_index] = c;          // save HTTP request character
					req_index++;
				}
				// last line of client request is blank and ends with \n
				// respond to client only after last line received
				 
				if (c == '\n' && currentLineIsBlank) {
					// send a standard http response header
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println("Connection: keep-alive");
					client.println();
					// Ajax request
					if (StrContains(HTTP_req, "analog_values")) {
						// read switch state and send appropriate paragraph text
						GetAnalogSwitchState(client);
					}
					/*else if (StrContains(HTTP_req, "LightState")) {
						// read switch state and send appropriate paragraph text
						GetLightSwitchState(client);
				}*/
				 
				 
				 
				else {  // web page request
					// send web page
					SdFile webFile;
					if (StrContains(HTTP_req, "GET / ")
					|| StrContains(HTTP_req, "GET /index.htm")) {//Open start-site
						if (!webFile.open("index.htm", O_READ)) {
							sd.errorHalt("opening index.htm for read failed");
						}
						Serial.println(F("Send Index.htm to Client"));
						 
					}
					else if (StrContains(HTTP_req, "GET /setcolor.htm")){
						if (!webFile.open("setcolor.htm", O_READ)) {
							sd.errorHalt("opening setcolor.htm for read failed");
						}
						Serial.println(F("Send setcolor.htm to Client"));
					}
					else if (StrContains(HTTP_req, "GET /GetColorState")){
						GetColorValues(client);
					}
					else if (StrContains(HTTP_req, "GET /SetColorState")){
						Serial.println(F("Set color state"));
						Serial.println(HTTP_req);
						byte Light[4];
						 
						char * pch;
						pch = strtok (HTTP_req,"=");
						pch = strtok (NULL, "=");
						int i=0;
						while (pch != NULL)
						{
							Light[i]=atoi (pch);
							i++;
							Serial.println(pch);
							Serial.println(Light[i]);
							pch = strtok (NULL, "=");
						}
						 
						Serial.println(F("Found values"));
						Serial.println(Light[0]);
						Serial.println(Light[1]);
						Serial.println(Light[2]);
						Serial.println(Light[3]);
						SetColorValues(Light);
						
						byte device,channel;
						if (Light[0]==0) {device=1;channel=0;}
						if (Light[0]==1) {device=1;channel=1;}
						if (Light[0]==2) {device=2;channel=0;}
						if (Light[0]==3) {device=2;channel=1;}
						if (Light[0]==4) {device=3;channel=0;}
						if (Light[0]==5) {device=3;channel=1;}
						if (Light[0]==6) {device=4;channel=0;}
						if (Light[0]==7) {device=4;channel=1;}
						byte msg [] = {
			device,    		//devicenumber
			2,    			//set color command
			Light[1],    		//red color value
			Light[2],     		//green color value
			Light[3],    		//blue color value
			channel      	//channel
			 
		};//end Message
		//Send Message to Slave via RS485
		//RS485_Send(msg);
		digitalWrite (ENABLE_PIN, HIGH);  // enable sending
		sendMsg (fWrite, msg, sizeof msg);
		digitalWrite (ENABLE_PIN, LOW);  // disable sending
						 
						if (!webFile.open("setcolor.htm", O_READ)) {
							sd.errorHalt("opening setcolor.htm for read failed");
						}
						Serial.println(F("Send setcolor.htm to Client"));
					}
					else if (StrContains(HTTP_req, "GET /SwitchLight")){
						if (AllLightsON)
						{AllLightsON=false;
							Serial.println(F("Switch all Lights OFF"));
						}
						else
						{AllLightsON=true;
							Serial.println(F("Switch all Lights ON"));
						}
						SetLightSwitchState(client);
					}
					else if (StrContains(HTTP_req, "GET /LightState")){
						//Status der Beleuchtung anzeigen
						GetLightSwitchState(client);
					}
					else if (StrContains(HTTP_req, "GET /switch.htm")) {//Open switch-site
						if (!webFile.open("switch.htm", O_READ)) {
							sd.errorHalt("opening switch.htm for read failed");
						}
						Serial.println(F("Send switch.htm to Client because of request"));
					}
					else if (StrContains(HTTP_req, "GET /ResetSettings")){
						//InitSettingsToSD();
						Serial.println(F("Reinit"));
						client.println(F("Reinitialisiert"));
						EEPROM.write(100,0xFF);
						EEPROM.write(101,0x03);
						EEPROM.write(102,0x00);
						EEPROM.write(103,0x00);
						EEPROM.write(104,0xFF);
						EEPROM.write(105,0x03);
						EEPROM.write(106,0x00);
						EEPROM.write(107,0x00);
						minimumTemp=1022;
						maximumTemp=2;
						minimumLight=1022;
						maximumLight=2;
					}
					else {
						 
						SdFile webFile;
						if (!webFile.open("index.htm", O_READ)) {
							sd.errorHalt("opening test.txt for read failed");
						}
					}
					Serial.println(F("Send opened Webpage to Client"));
					 
					// read from the file until there's nothing else in it:
					int data;
					while ((data = webFile.read()) >= 0) client.write(data);
					// close the file:
					webFile.close();
				}
				// display received HTTP request on serial port
				Serial.println(HTTP_req);
				// reset buffer index and all buffer elements to 0
				req_index = 0;
				StrClear(HTTP_req, REQ_BUF_SZ);
				break;
			}
			// every line of text received from the client ends with \r\n
			if (c == '\n') {
				// last character on line of received text
				// starting new line with next character read
				currentLineIsBlank = true;
			}
			else if (c != '\r') {
				// a text character was received from client
				currentLineIsBlank = false;
			}
		} // end if (client.available())
	} // end while (client.connected())
	delay(1);      // give the web browser time to receive the data
	client.stop(); // close the connection
} // end if (client)
 
 
if(Limit(30,70,PercentLight,&lightState) )
{
	Serial.print(F("Light percent value: "));
	Serial.println(PercentLight);
	switch(lightState)
	{
		case dark:
		{
			AllLightsON=true;
			SwitchLightON();
			break;
		}
		 
		case light:
		{
			AllLightsON=false;
			SwitchLightOFF();
			break;
		}
	}
}
 
//Receive RS485 Data
byte buf [20];
byte received = recvMsg (fAvailable, fRead, buf, sizeof (buf) - 1);
if (received){
	if (buf [0] == 0){//MyAddress
		Serial.println(F("Daten empfangen"));
		if (buf[1]==2){
			if ( buf[2]==1){
				//Dimmen von Slave 1 wurde beendet
				Serial.println(F("Slave 1 dimmen beendet"));
			}
		}
	}
}
 
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
