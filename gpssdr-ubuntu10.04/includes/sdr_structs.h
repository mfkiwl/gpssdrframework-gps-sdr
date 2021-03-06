/*----------------------------------------------------------------------------------------------*/
/*! \file sdr-structs.h
//
// FILENAME: sdr-structs.h
//
// DESCRIPTION: Additional SDR structures
//
// DEVELOPERS: Gregory W. Heckler (2003-2009)
//
// LICENSE TERMS: Copyright (c) Gregory W. Heckler 2009
//
// This file is part of the GPS Software Defined Radio (GPS-SDR)
//
// The GPS-SDR is free software; you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version. The GPS-SDR is distributed in the hope that
// it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// Note:  Comments within this file follow a syntax that is compatible with
//        DOXYGEN and are utilized for automated document extraction
//
// Reference:
 */
/*----------------------------------------------------------------------------------------------*/

#ifndef SDR_STRUCTS_H_
#define SDR_STRUCTS_H_

/*----------------------------------------------------------------------------------------------*/
/*! @ingroup STRUCTS
 *  @brief Define the CPX structure, used in the Fine_Acquisition FFT */
typedef struct CPX
{
  int16_t i;  //!< Real value
  int16_t q;  //!< Imaginary value
} CPX;

/*! \ingroup STRUCTS
 *	@brief Format of complex accumulations */
typedef struct CPX_ACCUM {

  int32_t i;  //!< Inphase (real)
  int32_t q;  //!< Quadrature (imaginary)

} CPX_ACCUM;


/*! \ingroup STRUCTS
 *
 */
typedef struct MIX {

  int16_t i;  //!< Inphase (real)
  int16_t nq; //!< Quadrature (imaginary)
  int16_t q;  //!< Quadrature (imaginary)
  int16_t ni; //!< Inphase (real)

} MIX;
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*! \ingroup STRUCTS
 * @brief Options parsed from command line to start the receiver */
typedef struct _Options_S
{

  int32_t verbose;    //!< Do a lot of extra printing
  int32_t   log_channel;  //!< Log low-level tracking loop info
  int32_t mode;     //!< Run in a  2 antenna mode with 2 DBS-RXs, Run in a  2 antenna mode, board A L1, board B L2
  int32_t decimate;
  int32_t realtime;
  int32_t tlm_type;   //!< Telemetry type (named pipe/serial)
  int32_t source;     //!< GPS data source type
  int32_t bb2file;    //!< record baseband data to a file
	double	f_lo_a;			//!< LO freq for board A
	double	f_ddc_a;		//!< DDC freq for board A
	double	f_lo_b;			//!< LO freq for board B
	double	f_ddc_b;		//!< DDC freq for board B
	double 	bandwidth;		//!< DBS-RX filter bandwidth
	double 	gi;				//!< IF gain
	double 	gr;				//!< RF gain
	double	f_sample;		//!< Sample rate (depending on the clock)
	char	*filename;		//!< File to read samples from

} Options_S;
/*----------------------------------------------------------------------------------------------*/


//!< FIFO structure for linked list?
/*----------------------------------------------------------------------------------------------*/
/*! \ingroup STRUCTS
 *  @brief linked list structure for circular FIFO buffer */
typedef struct ms_packet {

	ms_packet *next;
  int32_t count;          //!< Number of packets
	CPX data[SAMPS_MS];				//!< Payload size

} ms_packet;
/*----------------------------------------------------------------------------------------------*/


//!< Correlator and channel structs
/*----------------------------------------------------------------------------------------------*/
/*! \ingroup STRUCTS
 * @brief Feedback applied from channel object to correlator object */
typedef struct _NCO_Command
{

	double carrier_nco;	//!< New carr_freq
	double code_nco;	//!< New code_freq
  uint32_t kill;      //!< Stop or start the channel
  uint32_t reset_1ms; //!< Reset the 1ms counter
  uint32_t reset_20ms;  //!< Reset the 20ms counter
  uint32_t set_z_count; //!< Set the z count
  uint32_t z_count;   //!< Actual value
  uint32_t length;    //!< Integrate for this many ms
  uint32_t navigate;    //!< Use this correlator to navigate

} NCO_Command_S;


/*! \ingroup STRUCTS
 * @brief Raw correlation values, early, prompt, and late */
typedef struct _Correlation_S
{

  int32_t I[3];
  int32_t Q[3];

} Correlation_S;


/*! \ingroup STRUCTS
 * @brief Hold state information of the correlator */
typedef struct _Correlator_State_S
{

	double 	code_phase; 		//!< Code phase (chips)
	double 	carrier_phase;		//!< Carrier phase (cycles)
	double  carrier_phase_prev;	//!< Used for phase correction to correlations
	double 	code_phase_mod;		//!< Code phase (chips), mod 1023
	double 	carrier_phase_mod;	//!< Carrier phsae (cycles), mod 1
	double 	code_nco;			//!< Code NCO
	double 	carrier_nco;		//!< Carrier NCO

  uint32_t  chan;
  uint32_t  sv;
  uint32_t  navigate;     //!< Is this correlator sending out valid measurements
  uint32_t  active;       //!< Active flag
  uint32_t  count;        //!< How long has this been active (ms)
  uint32_t   scount;      //!< Number of samples in current accumulation
  uint32_t  _1ms_epoch;     //!< _1ms_epoch
  uint32_t  _20ms_epoch;    //!< _20ms_epoch
  uint32_t  _z_count;     //!< Keep track of the z count
  uint32_t  rollover;     //!< rollover point of C/A code in next ms packet
  uint32_t  cbin[3];      //!< Code bins
  uint32_t  sbin;       //!< Carriers bins
	MIX		*pcode[3];			//!< pointer to early-prompt-late codes
	CPX		*psine;				//!< pointer to Doppler removal vector
	MIX		*code_rows[2*CODE_BINS+1];	//!< Row pointers to presampled code table

} Correlator_State_S;


/*! \ingroup STRUCTS
 * @brief State of phase lock loop */
typedef struct _Phase_lock_loop {

	float PLLBW;				//!< PLL Bandwidth (Hz)
	float FLLBW;				//!< FLL Bandwidth (Hz)
	float a3;
	float b3;
	float w0p;
	float w0p2;
	float w0p3;
	float a2;
	float w0f;
	float w0f2;
	float gain;
	float w;					//!< Acceleration accumulator
	float x;					//!< Velocity accumulator
	float z;					//!< P feedback
	float pll_lock;				//!< PLL Lock Indicator
	float fll_lock;				//!< FLL lock indicator
	float t;					//!< Integration length (ms)
  int32_t fll_lock_ticks;   //!< Is the fll locked?

} Phase_lock_loop;


/*! \ingroup STRUCTS
 * @brief State of delay lock loop */
typedef struct _Delay_lock_loop
{

	float DLLBW;				//!< Bandwidth
	float x;					//!< Velocity accumulator
	float z;					//!< Proportional feedback
	float a;					//!< Acceleration accumulator
	float w0;					//!< w0
	float w02;					//!< w0^2
	float t;					//!< Integration length

} Delay_lock_loop;
/*----------------------------------------------------------------------------------------------*/

#endif


