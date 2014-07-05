//-------------------------------------------------------------------
#ifndef __GartenFeld1_main_H__
#define __GartenFeld1_main_H__
//-------------------------------------------------------------------

#include <arduino.h>
#include <pixeltypes.h>
#include <lib8tion.h>
#include <hsv2rgb.h>
#include <FastSPI_LED2.h>
#include <fastspi_dma.h>
#include <fastspi_bitbang.h>
#include <fastspi_avr.h>
#include <fastspi_arm.h>
#include <fastspi.h>
#include <fastpin.h>
#include <dmx.h>
#include <delay.h>
#include <controller.h>
#include <clockless.h>
#include <chipsets.h>
#include <SoftwareSerial.h>
#include <RS485_protocol.h>
#include <types.h>
//-------------------------------------------------------------------
 
//-------------------------------------------------------------------
 
// Put yout declarations here
 
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
void SetLight();
int fRead ();
int fAvailable ();
void fWrite (const byte what);
void RS485();
void DisplayRGB(int r,int g, int b);
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_END@---------------------
// -> ...AND HERE. This space is reserved for automated code generation!
//===================================================================
 
 
//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
 
 
 
 
 
 
