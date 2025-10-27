/*
 * ===============================================================
 *            Read ADC with timmer and Serial print 
 * ===============================================================
 *
 * Author: Danilo Molina by Estudios MA
 * Last Modification: 26/10/2025
 * Version: 1.0
 * 
 * Description:
 * This program signals in ADC channel with interrupt timmer and
 * send data using serial protocol.
 *
 * Dependencies:
 * ma_filter_smooth.h 
 *
 * Board:
 * - Arduino MEGA
 *
 * Development Tool:
 * - Arduino IDE 2.3.2
 * 
 * Usage Instructions:
 * - Set FS
 *
 * Version History:
 * - Version 1.0 (26/10/2025): Initial implementation.
 *
 * Connections:
 * Board |  External device
 * A0       Analog signal
 *
 */

#include "ma_filter_smooth.h"

#define LED_USER  13 // LED ArduinoMEGA

int fs = 300; // Hz
uint8_t SIGNAL_ANALOG = A0; // configure you ADC channel HERE!!!

int flag = 0;
int signal_raw = 0;
uint8_t led_state = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(LED_USER, OUTPUT);
  digitalWrite(LED_USER, LOW);

  ma_init_filter(fs);
}

void loop() {
  if (flag) {
    //Serial.print(signal_raw);
    //Serial.print("\t");
    Serial.print(signal_filtered);
    Serial.print("\t");
    //Serial.print(signal_abs);
    //Serial.print("\t");
    Serial.println(signal_ma*10.0); // ajust amplitude!!!
    flag =  0;
  }
}

/**
 * @brief Interrupcion de TIMER
 * @return none
 * @author Molina-Vidal, D.A. by MA
 */
ISR(TIMER1_COMPA_vect) {
  signal_raw = analogRead(SIGNAL_ANALOG);

  float x0 = signal_raw;
  float y0 = b0 * x0 + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;

  x2 = x1;
  x1 = x0;
  y2 = y1;
  y1 = y0;

  signal_notch = y0;

  float x0b = signal_notch;
  float y0b = b0b*x0b + b1b*x1b + b2b*x2b + b3b*x3b + b4b*x4b
            - a1b*y1b - a2b*y2b - a3b*y3b - a4b*y4b;

  x4b = x3b; x3b = x2b; x2b = x1b; x1b = x0b;
  y4b = y3b; y3b = y2b; y2b = y1b; y1b = y0b;

  signal_filtered = y0b;

  signal_abs = signal_filtered;
  if (signal_filtered < 0.0){
    signal_abs = signal_filtered*(-1);
  }
  signal_abs = signal_abs;
  signal_abs = signal_abs*signal_abs;

  // --- Média móvel ---
  ma_buffer[ma_index] = signal_abs;       
  ma_index = (ma_index + 1) % MA_SIZE;   

  float sum = 0;
  for (int i = 0; i < MA_SIZE; i++) {
    sum += ma_buffer[i];
  }
  signal_ma = sum / MA_SIZE;
  signal_ma = signal_ma;

  led_state = !led_state;
  digitalWrite(LED_USER, led_state);  
  flag = 1;
}
