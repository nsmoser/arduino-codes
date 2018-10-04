//This program can determine the temperature by detecting 
//the amount of resistance a thermistor provides
//The circuit requires one thermistor, one 10kOhm resistor, two 180Ohm resistors, and two LEDs
//The leds are connected to digital pins 8 and 9
//Pin 8 is activated when the temperature is within operational range (above 0 degrees celsius)
//Pin 9 is activated when the temperature is outside of operational range (below 0 degrees celsius)
//The program also prints temperature and resistance values to the serial monitor
    // which analog pin to connect
    #define THERMISTORPIN A0         
    // resistance at 25 degrees C
    #define THERMISTORNOMINAL 10000      
    // temp. for nominal resistance (almost always 25 C)
    #define TEMPERATURENOMINAL 25   
    // how many samples to take and average, more takes longer
    // but is more 'smooth'
    #define NUMSAMPLES 5
    // The beta coefficient of the thermistor (usually 3000-4000)
    #define BCOEFFICIENT 3950
    // the value of the 'other' resistor
    #define SERIESRESISTOR 10000    
     
    uint16_t samples[NUMSAMPLES];
     
    void setup(void) {
      Serial.begin(9600);
      analogReference(EXTERNAL);
      pinMode(8, OUTPUT);
      pinMode(9, OUTPUT);
    }
     
    void loop(void) {
      uint8_t i;
      float average;
     
      // take N samples in a row, with a slight delay
      for (i=0; i< NUMSAMPLES; i++) {
       samples[i] = analogRead(THERMISTORPIN);
       delay(10);
      }
     
      // average all the samples out
      average = 0;
      for (i=0; i< NUMSAMPLES; i++) {
         average += samples[i];
      }
      average /= NUMSAMPLES;
     
      Serial.print("Average analog reading "); 
      Serial.println(average);
     
      // convert the value to resistance
      average = 1023 / average - 1;
      average = SERIESRESISTOR / average;
      Serial.print("Thermistor resistance "); 
      Serial.println(average);
     
      float tempc;
      tempc = average / THERMISTORNOMINAL;     // (R/Ro)
      tempc = log(tempc);                  // ln(R/Ro)
      tempc /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
      tempc += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
      tempc = 1.0 / tempc;                 // Invert
      tempc -= 273.15;                         // convert to C
      float tempf;
      tempf = tempc * 1.8;                //convert to F
      tempf = tempf + 32;

      if (tempf < 32)
      {
      Serial.println("Its too cold");
      Serial.print("Temperature "); 
      Serial.print(tempc);
      Serial.println(" C");
      Serial.print("Temperature ");
      Serial.print(tempf);
      Serial.println(" F");
      digitalWrite(9, HIGH);
      digitalWrite(8, LOW);
      }
      else
      {
      Serial.println("its warm enough");
      Serial.print("Temperature "); 
      Serial.print(tempc);
      Serial.println(" C");
      Serial.print("Temperature ");
      Serial.print(tempf);
      Serial.println(" F");
      digitalWrite(8, HIGH);
      digitalWrite(9, LOW);
      }
     
     
     
      delay(5000);
    }
