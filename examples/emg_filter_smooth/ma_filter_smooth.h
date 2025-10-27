/*
 * ===============================================================
 *            Moving Average Filter Library Header
 * ===============================================================
 *
 * Author: Molina-Vidal, D.A. by Estudios MA
 * Last Modification: 26/10/2025
 * Version: 1.0
 * 
 * Description:
 * Header file for MA (Moving Average) filter implementation.
 * Provides function to initialize the filter and all variables
 * and constants required for filter operation.
 *
 * Dependencies:
 * - Arduino core
 *
 * Board:
 * - Arduino MEGA (tested)
 *
 * Development Tool:
 * - Arduino IDE 2.3.2
 * 
 * Usage Instructions:
 * - Call ma_init_filter(unsigned int f) to initialize filter
 *   with desired window size f.
 * - Sampling frequency fs = 300 Hz
 *
 * Version History:
 * - Version 1.0 (26/10/2025): Initial implementation.
 *
 * Function Prototypes:
 * void ma_init_filter(unsigned int f);
 *
 */
#ifndef MA_FILTER_SMOOTH_H
#define MA_FILTER_SMOOTH_H

// ===================== Defines =====================
#define MA_SIZE 150  // FIlter Smoothing


// ============= Global Variables ====================
float signal_notch = 0.0;
float signal_filtered = 0.0;
float signal_abs = 0.0;
float signal_ma = 0.0;
float ma_buffer[MA_SIZE] = { 0 };
int ma_index = 0;


// ===================== Constans =====================
// Filter state variables
float x1 = 0, x2 = 0;
float y1 = 0, y2 = 0;

// Notch filter coefficients (fs = 300 Hz)
const float b0 = 0.95977357;
const float b1 = -0.59317269;
const float b2 = 0.95977357;
const float a1 = -0.59317269;
const float a2 = 0.91954714;

// Low-pass filter (fs = 300 Hz)
float x1l = 0, x2l = 0, y1l = 0, y2l = 0;
const float b0l = 0.46515308;
const float b1l = 0.93030615;
const float b2l = 0.46515308;
const float a1l = 0.6202041;
const float a2l = 0.24040821;

// Band-pass filter (fs = 300 Hz, Butterworth 2nd order)
float x1b = 0, x2b = 0, x3b = 0, x4b = 0;
float y1b = 0, y2b = 0, y3b = 0, y4b = 0;

const float b0b = 0.39133577;
const float b1b = 0.0;
const float b2b = -0.78267155;
const float b3b = 0.0;
const float b4b = 0.39133577;

const float a1b = -1.12825723;
const float a2b = 0.02613144;
const float a3b = -0.0152956;
const float a4b = 0.19581571;

/**
 * @brief Initialize the Moving Average filter.
 * @param f Window size of the filter (number of samples)
 */
void ma_init_filter(unsigned int f) {
  cli();

  TCCR1A = 0;            
  TCCR1B = 0;          
  TCCR1B |= (1 << WGM12); 
  TCCR1B |= (1 << CS11);   

  switch (f) {
    case 240:
      OCR1A = 8332;  // 240 Hz -> OK
      break;
    case 300:
      OCR1A = 6665;  // 300 Hz -> OK
      break;
    case 900:
      OCR1A = 2221;  // 900 Hz -> OK
      break;
    default:
      OCR1A = 6665;  // 300 Hz -> OK
  }

  TIMSK1 |= (1 << OCIE1A);
  TCNT1 = 0;
  sei();
}

#endif // MA_FILTER_SMOOTH_H