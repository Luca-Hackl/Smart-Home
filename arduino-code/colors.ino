//-------------------------------------------------------------------------.----------.
//                                                                         | Websites |
//-------------------------------------------------------------------------.-------------.
//                                                                         | LED Website |
//-------------------------------------------------------------------------'-------------'

void LEDOff() {
  for (int i=0; i<NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
    pixels.show();   //Turns the LED off and returns that value to the void "handle_Sensor"      
  }
}

void sunset(){
  for (int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(229, 83, 0)); 
      pixels.show();   
     }
}

void relax(){
  for (int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(69,179,224)); 
      pixels.show();     
      }
}

void evening(){
  for (int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(155,0,0)); 
      pixels.show();     
      }
}

void pink(){
  for (int i=0; i<NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(255,182,193)); 
    pixels.show();     
    }
}

void pulsating(int pulsebrightness){

    int color = pulsebrightness + 35;
    for (int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(color,color-35,0)); 
      pixels.show();
    }
 
}
