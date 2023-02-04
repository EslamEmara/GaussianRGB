/*======= Configuration Parameters =======*/

#define BREATHE_IN_TIME         (2000)             /*Time taken in ms*/
#define BREATHE_OUT_TIME        (2000)             /*Time taken in ms*/

#define PAUSE_AT_MAX_TIME     (1000)
#define PAUSE_AT_MIN_TIME     (1000)

#define MAX_BRIGHTNESS  (255.0)          /*Max analog output (PWM) = 255*/

#define RED_PIN     3
#define GREEN_PIN   5
#define BLUE_PIN    6

#define COLOR     ((unsigned long int)(0x800000))

/*======= End of Configuration =======*/

float rgb[3] = {COLOR>>16&0xFF,COLOR>>8&0xFF,COLOR&0xFF};
float rgbMod[3] = {0};

float gamma = 0.14; // affects the width of peak (more or less darkness)
float beta = 0.5; // shifts the gaussian to be symmetric

void adjustRGBarr(){
  float max_n = rgb[0];    
  for(int i = 0; i < 3; i++) {                /*finding max element*/
      if((int)rgb[i] > (int)max_n)    
          max_n = rgb[i];    
  }
  if (max_n !=0)
  {
      for(int i=0;i<3;i++){           /*normalizing rgb for all elements to be less than 1*/
          rgb[i] = (float)rgb[i] / (float)max_n;
      }
  }
  else{
        for(int i=0;i<3;i++){           /*normalizing rgb for all elements to be less than 1*/
          rgb[i] = 1.0;
      }
  }
}
void writetoRGB(float pwm_val){  
  for(int i = 0;i<3;i++){
    rgbMod[i] = rgb[i]*pwm_val;
  }
  analogWrite(RED_PIN,(int) rgbMod[0]);
  analogWrite(GREEN_PIN,(int) rgbMod[1]);
  analogWrite(BLUE_PIN,(int) rgbMod[2]);
}
void setup() {
    Serial.begin(9600);
    pinMode(RED_PIN,OUTPUT);
    pinMode(GREEN_PIN,OUTPUT);
    pinMode(BLUE_PIN,OUTPUT);
    adjustRGBarr();
}

void loop() {
  float pwm_val = 0;
  float start = millis();
  for (int i=0;i<(BREATHE_IN_TIME/5);i++){
    pwm_val = (float)MAX_BRIGHTNESS*(exp(-(pow(((i/((float)((BREATHE_IN_TIME*2)/5.0)))-beta)/gamma,2.0))/2.0));
    writetoRGB(pwm_val);
    delay(5);
  }
  

  for (int i=0;i<(PAUSE_AT_MAX_TIME/5);i++){
    writetoRGB(pwm_val);

    delay(5);
  }
  for (int i=((BREATHE_OUT_TIME)/5);i<((BREATHE_OUT_TIME*2)/5);i++){
    pwm_val = (float)MAX_BRIGHTNESS*(exp(-(pow(((i/((float)((BREATHE_OUT_TIME*2)/5.0)))-beta)/gamma,2.0))/2.0));
    writetoRGB(pwm_val);

    delay(5);
  }
  for (int i=0;i<(PAUSE_AT_MIN_TIME/5);i++){
    writetoRGB(pwm_val);

    delay(5);
  }

}
