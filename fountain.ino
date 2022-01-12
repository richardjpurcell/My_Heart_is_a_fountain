/*
 * File:      fountain04.ino
 * Author:    Richard Purcell
 * Date:      2022-12-10
 * Version:   1.0
 * Purpose:   My Heart is a Fountian haloween/nocturne costume
 * Usage:     automatic on arduion UNO
 * Required:  FastLED.h library
 * Notes:     The code does 4 things, reads a heart monitor,
 *            launches a light sequence based on heart rate,
 *            buzzes a buzzer based on heart rate,
 *            and runs a random light sequence.         
 */
#include <FastLED.h>

#define FOUNTAIN_LED_PIN 12
#define RANDOM_LED_PIN 7
#define NUM_LEDS    32
#define RANDOM_NUM_LEDS 36
#define AUDIO_OUTPUT_PIN 2

CRGB leds[NUM_LEDS];
CRGB random_leds[RANDOM_NUM_LEDS];

//  Variables
int PulseSensorPurplePin = 0; // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
int random_num_1 = 0;
int random_num_2 = 0;
int rand_val_1 = 0;
int rand_val_2 = 0;
// holds the incoming raw data. Signal value can range from 0-1024
int Signal;   
int Count = 0;  
// Determine which Signal to "count as a beat", and which to ingore.              
int Threshold = 600;          
bool Flow = 0;

// The SetUp Function:
void setup() {
  pinMode(AUDIO_OUTPUT_PIN, OUTPUT);
  FastLED.addLeds<WS2812, FOUNTAIN_LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, RANDOM_LED_PIN, GRB>(random_leds, RANDOM_NUM_LEDS);
  Serial.begin(9600);   
}

//Main Loop Function
void loop()
{
  //read from the heart monitor
  Signal = analogRead(PulseSensorPurplePin); 
  //output to the serial plotter
  Serial.println(Signal);     

  //if random led has been on for a while, turn it off
  if(rand_val_1 > 240) {
    rand_val_1 = 0;
    random_leds[random_num_1].setRGB(0, 0, 0);
    random_num_1 = random(36);
  }
  //if 2nd random led has been on for a while, turn it off
  if(rand_val_2 > 240){
    rand_val_2 =0;
    random_leds[random_num_2].setRGB(0, 0, 0);
    random_num_2 = random(36);
  }
  //set new random leds on
  random_leds[random_num_1].setRGB(0, 0, rand_val_1);
  random_leds[random_num_2].setRGB(0, rand_val_2, rand_val_2);

  rand_val_1 += 20;
  rand_val_2 += 30;
  
  //fire off fountain lights if above threshold
  if (Signal > Threshold)
  {
    //set buzzer to high
    digitalWrite(AUDIO_OUTPUT_PIN, HIGH);  
    //start led fountain chase sequnece
    if (Flow == 1 && Count < NUM_LEDS)
    {
      leds[Count - 1] = CRGB::Black;
      leds[Count].setRGB(0, 0, 15);
      leds[Count + 1].setRGB(0, 0, 25);
      leds[Count + 2].setRGB(0, 0, 50);
      leds[Count + 3].setRGB(50, 0, 50);
      leds[Count + 4].setRGB(100, 0, 50);
      leds[Count + 5].setRGB(150, 0, 50);
      leds[Count + 6].setRGB(225, 0, 0);

      Count = Count + 1;
    }
    else if (Flow == 1 && Count > NUM_LEDS)
    {
      leds[Count - 1] = CRGB::Black;
      Flow = 0;
      Count = Count + 1;
    }
    else if (Flow == 0)
    {
      Count = 0;
      Flow = 1;
      leds[0] = CRGB::Green;
    }
    //set buzzer to low
    digitalWrite(AUDIO_OUTPUT_PIN, LOW);
  }
  else
  {
    if (Flow && Count < NUM_LEDS)
    {
      leds[Count - 1] = CRGB::Black;
      leds[Count].setRGB(0, 0, 15);
      leds[Count + 1].setRGB(0, 0, 25);
      leds[Count + 2].setRGB(0, 0, 50);
      leds[Count + 3].setRGB(50, 0, 50);
      leds[Count + 4].setRGB(100, 0, 50);
      leds[Count + 5].setRGB(150, 0, 50);
      leds[Count + 6].setRGB(225, 0, 0);

      Count = Count + 1;
    }
    else
    {
      leds, NUM_LEDS, CRGB::Black); //turn off all led's
      leds[Count - 1] = CRGB::Black;
      Flow = 0;
      Count = Count + 1;
    }
  }

  FastLED.show();

  delay(30);
}