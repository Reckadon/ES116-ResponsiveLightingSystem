#include "arduinoFFT.h"

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03
/*
These values can be changed in order to evaluate the functions
*/
const uint16_t samples = 128;            
const uint16_t samplingFrequency = 10000;
const uint16_t amplitude = 1024;
const int pinNumber = A0;
const int mic_baseline = 645;

float vReal[samples];
float vImag[samples];

int low_thresh = 400; // threshold freq value for low band
int mid_thresh = 3000; // threshold freq value for mid band
float low_amp;
float mid_amp;
float high_amp;

int led_thresh_low = 400; //threshold amp value for low brightness led
int led_thresh_mid = 1000; //threshold amp value for medium brightness led
int led_bright_low = 0;
int led_bright_mid = 10;
int led_bright_high = 255; 

const int low_band_led_pin = 11;
const int mid_band_led_pin = 10;
const int high_band_led_pin = 9;


/* Create FFT object */
ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency);


void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Ready");
}

void loop()
{
  /* Input raw data */
  for (uint16_t i = 0; i < samples; i++){
    int amp = analogRead(pinNumber) - mic_baseline;
    vReal[i] = amp;
    vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
  }
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	/* Weigh data */
  FFT.compute(FFTDirection::Forward); /* Compute FFT */
  FFT.complexToMagnitude(); /* Compute magnitudes */
  ControlLights(vReal, (samples >> 1), SCL_FREQUENCY);
  float x = FFT.majorPeak();
  Serial.print("f0=");
  Serial.print(x, 6);
  Serial.println("Hz");
  
  // while(1); /* Run Once */
  delay(100); /* Repeat after delay */
}

void ControlLights(float *vData, uint16_t bufferSize, uint8_t scaleType){
  //reset amp vals
  low_amp = 0;
  mid_amp = 0;
  high_amp = 0;

  for (uint16_t i = 0; i < bufferSize; i++){
    double abscissa;
    /* Print abscissa value */
    switch (scaleType){
      case SCL_INDEX:
      abscissa = (i * 1.0);
      break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
      break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
      break;
    }
 
    float amp = vData[i];
    if(scaleType==SCL_FREQUENCY){
      if(abscissa <= low_thresh) low_amp += amp;
      else if(abscissa <= mid_thresh) mid_amp += amp;
      else high_amp += amp;
    }
  }
  Serial.print("Low: ");
  Serial.print(low_amp);
  Serial.print(" Mid: ");
  Serial.print(mid_amp);
  Serial.print(" High: ");
  Serial.print(high_amp);
  Serial.println();
  
  //deciding brightness level for each led strip
  //low band led
  if(low_amp <= led_thresh_low) analogWrite(low_band_led_pin, led_bright_low);
  else if(low_amp <= led_thresh_mid) analogWrite(low_band_led_pin, led_bright_mid);
  else analogWrite(low_band_led_pin, led_bright_high);

  //mid band led
  if(mid_amp <= led_thresh_low) analogWrite(mid_band_led_pin, led_bright_low);
  else if(mid_amp <= led_thresh_mid) analogWrite(mid_band_led_pin, led_bright_mid);
  else analogWrite(mid_band_led_pin, led_bright_high);

  //high band led
  if(high_amp <= led_thresh_low) analogWrite(high_band_led_pin, led_bright_low);
  else if(high_amp <= led_thresh_mid) analogWrite(high_band_led_pin, led_bright_mid);
  else analogWrite(high_band_led_pin, led_bright_high);
}


