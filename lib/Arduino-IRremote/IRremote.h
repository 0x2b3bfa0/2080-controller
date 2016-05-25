
//******************************************************************************
// IRremote
// Version 2.0.1 June, 2015
// Copyright 2009 Ken Shirriff
// For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
// Edited by Mitra to add new controller SANYO
//
// Interrupt code based on NECIRrcv by Joe Knapp
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
// Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
//
// JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
// LG added by Darryl Smith (based on the JVC protocol)
// Whynter A/C ARC-110WD added by Francesco Meschia
// Customized for this project by Helio Machado <crushedice200@gmail.com> (2016)
//******************************************************************************

#ifndef IRremote_h
#define IRremote_h

//------------------------------------------------------------------------------
// The ISR header contains several useful macros the user may wish to use
//
#include "IRremoteInt.h"
#include "ir_Velleman.h"

//------------------------------------------------------------------------------
// Supported IR protocols
// Each protocol you include costs memory and, during decode, costs time
// Disable (set to 0) all the protocols you do not need/want!
//

#define DECODE_VELLEMAN        1
#define SEND_VELLEMAN          1

//------------------------------------------------------------------------------
// When sending a Pronto code we request to send either the "once" code
//                                                   or the "repeat" code
// If the code requested does not exist we can request to fallback on the
// other code (the one we did not explicitly request)
//
// I would suggest that "fallback" will be the standard calling method
// The last paragraph on this page discusses the rationale of this idea:
//   http://www.remotecentral.com/features/irdisp2.htm
//
#define PRONTO_ONCE        false
#define PRONTO_REPEAT      true
#define PRONTO_FALLBACK    true
#define PRONTO_NOFALLBACK  false

//------------------------------------------------------------------------------
// An enumerated list of all supported formats
// You do NOT need to remove entries from this list when disabling protocols!
//
typedef
	enum {
		UNKNOWN      = -1,
		UNUSED       =  0,
		VELLEMAN
	}
decode_type_t;

//------------------------------------------------------------------------------
// Set DEBUG to 1 for lots of lovely debug output
//
#define DEBUG  0

//------------------------------------------------------------------------------
// Debug directives
//
#if DEBUG
#	define DBG_PRINT(...)    Serial.print(__VA_ARGS__)
#	define DBG_PRINTLN(...)  Serial.println(__VA_ARGS__)
#else
#	define DBG_PRINT(...)
#	define DBG_PRINTLN(...)
#endif

//------------------------------------------------------------------------------
// Mark & Space matching functions
//
int  MATCH       (int measured, int desired) ;
int  MATCH_MARK  (int measured_ticks, int desired_us) ;
int  MATCH_SPACE (int measured_ticks, int desired_us) ;

//------------------------------------------------------------------------------
// Results returned from the decoder
//
class decode_results
{
	public:
		decode_type_t          decode_type;  // UNKNOWN, NEC, SONY, RC5, ...
		unsigned int           address;      // Used by Panasonic & Sharp [16-bits]
		unsigned long          value;        // Decoded value [max 32-bits]
		char*                  name;         // The button name
		int                    bits;         // Number of bits in decoded value
		volatile unsigned int  *rawbuf;      // Raw intervals in 50uS ticks
		int                    rawlen;       // Number of records in rawbuf
		int                    overflow;     // true iff IR raw code too long
};

//------------------------------------------------------------------------------
// Decoded value for NEC when a repeat code is received
//
#define REPEAT 0xFFFFFFFF

//------------------------------------------------------------------------------
// Main class for receiving IR
//
class IRrecv
{
	public:
		IRrecv (int recvpin) ;
		IRrecv (int recvpin, int blinkpin);

		void  blink13    (int blinkflag) ;
		int   decode     (decode_results *results) ;
		void  enableIRIn ( ) ;
		bool  isIdle     ( ) ;
		void  resume     ( ) ;

	private:
		long  decodeHash (decode_results *results) ;
		int   compare    (unsigned int oldval, unsigned int newval) ;

		//......................................................................
#if DECODE_VELLEMAN
    static unsigned int rx_shift;
		bool  decodeVelleman(decode_results *results);
#endif
} ;

//------------------------------------------------------------------------------
// Main class for sending IR
//
class IRsend
{
	public:
		IRsend () { }

		void  custom_delay_usec (unsigned long uSecs);
		void  enableIROut 		(int khz) ;
		void  mark        		(unsigned int usec) ;
		void  space       		(unsigned int usec) ;
		void  sendRaw     		(const unsigned int buf[],  unsigned int len,  unsigned int hz) ;

		//......................................................................
  #if SEND_VELLEMAN
	  void  send(uint8_t data, uint8_t address = 2);
		void  print(char data, uint8_t address = 2, int delay_time = 750);
		void  print(char* data, uint8_t address = 2, int delay_time = 750);
		void  println(char data, uint8_t address = 2, int delay_time = 750);
		void  println(char* data, uint8_t address = 2, int delay_time = 750);
	private:
		static unsigned int shift;
		bool  setShift(uint8_t new_shift, uint8_t address = 2, int delay_time = 750);
  #endif
};

#endif
