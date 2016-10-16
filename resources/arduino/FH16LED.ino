#include <Adafruit_NeoPixel.h>
#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 146 // in the spare strip it is 91
//#define PIXEL_COUNT 91

/*
  byte scaledR_orange=180;
  byte scaledG_orange=65;
  byte scaledB_orange=10;
  
  byte scaledR_yel=120;
  byte scaledG_yel=120;
  byte scaledB_yel=15;
  */
  
// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

/* Frame description:
 *  Byte 0: Header byte 0
 *  Byte 1: Header byte 1
 *  Byte 2: focus (pixel index)
 *  Byte 3: section size
 *  Byte 4: fade
 *  Byte 5: R value
 *  Byte 6: G value
 *  Byte 7: B value
 *  Byte 8: Checksum
 */
#define FRAME_LENGTH 9
byte FRAME[9] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0'};
byte HEADER[2] = {0xFE,0xDE};
byte target=PIXEL_COUNT/2+1;
byte focus=PIXEL_COUNT/2+1;
bool serial=false;
bool header=false;
bool sanity=false;
byte prevR=255, prevG=255, prevB=255;
byte R=prevR, G=prevG, B=prevB;
float temp=0;
byte section_size=5;
byte fade=0;

//byte pixel=PIXEL_COUNT/2;
//bool sign=true;

void setup() {
//  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(230);
  Serial.begin(115200);
}

void loop() {
  showDirectionofMovement();
  //swipeSpeedTest(count++);
  //pulsingGreen();
  
/*
 * for Knight Rider demo
  if(sign)
  {
    pixel++;
    if(pixel>=PIXEL_COUNT*2/3-2)
    {
      sign=false;
      pixel=PIXEL_COUNT*2/3-2;
    }
  }
  else
  {
    pixel--;
    if(pixel<=0+PIXEL_COUNT/3)
    {
      sign=true;
      pixel=0+PIXEL_COUNT/3;
    }
  }
  
  knightRider(pixel);
*/
}

void knightRider(byte pixel)
{
  // start from a blank LED strip
  resetLEDBuffer();

  byte scaledR=255;
  byte scaledG=0;
  byte scaledB=0;
  
  // set the focus point to full color
  strip.setPixelColor(pixel,strip.Color(scaledR,scaledG,scaledB));
  
  for(int i=1; i<7; ++i)
  {
    // start fading when reaching the last 1/10 of the section to be lighted
    if(i>=(5))
    {
      scaledR/=10;
    }
    
    // set the pixels in the section to more dimmed colors the more they are distant from the center
    strip.setPixelColor(pixel+i,strip.Color(scaledR,scaledG,scaledB));
    strip.setPixelColor(pixel-i,strip.Color(scaledR,scaledG,scaledB));
  }
  
  // activate the LED strip
  strip.show();
  delay(10);
}

void swipeSpeedTest(int counter)
{
  // start from a blank LED strip
  resetLEDBuffer();

  byte scaledR=0;
  byte scaledG=255;
  byte scaledB=0;
  
  byte pixel=counter%PIXEL_COUNT;
  
  // set the focus point to full color
  strip.setPixelColor(pixel,strip.Color(scaledR,scaledG,scaledB));
  
  for(int i=1; i<20; ++i)
  {
    // start fading when reaching the last 1/fade of the section to be lighted
    if(i>=(20-6))
    {
      scaledR=scaledR/6;
      scaledG=scaledG/6;
      scaledB=scaledB/6;
    }
    
    // set the pixels in the section to more dimmed colors the more they are distant from the center
    strip.setPixelColor(pixel+i,strip.Color(scaledR,scaledG,scaledB));
    //Serial.print("powering pixel ");Serial.print(pixel+i);Serial.print(" with value ");Serial.println(scaledR);
    //Serial.print("powering pixel ");Serial.print(pixel+i);Serial.print(" with value ");Serial.println(scaledG);
    //Serial.print("powering pixel ");Serial.print(pixel+i);Serial.print(" with value ");Serial.println(scaledB);Serial.println("");
    strip.setPixelColor(pixel-i,strip.Color(scaledR,scaledG,scaledB));
    //Serial.println("");
  }
  
  // activate the LED strip
  strip.show();
  // run at 50 Hz
  delay(10);
}

void showDirectionofMovement()
{
  // check for serial connection
  if(Serial.available() <= 0){
    Serial.println("Serial connection not available. Aborting.");
    serial=false;
  }
  else
  {
    serial=true;
  }
  
  // start from a blank LED strip
  resetLEDBuffer();

  // store an entire frame (9 bytes)
  for(int i = 0; i < FRAME_LENGTH; i++){
    FRAME[i] = Serial.read();
  }
  
  // sanity check on the header
  if( ! (FRAME[0] == HEADER[0] && FRAME[1] == HEADER[1])){
    Serial.println("Can't find start sequence. Aborting.");
    header=false;
  }
  else
  {
    header=true;
  }

  byte checksum=FRAME[2]^FRAME[3]^FRAME[4]^FRAME[5]^FRAME[6]^FRAME[7];

  if(FRAME[FRAME_LENGTH-1] != checksum){
    Serial.println("Frame corrupted. Aborting.");
    sanity=false;
  }
  else
  {
    sanity=true;
  }

  // update the values only if a valid packet is received
  if(serial && header && sanity)
  {
    temp=(float) FRAME[2] /100.0*PIXEL_COUNT;
    target=(uint8_t)temp;Serial.print("new target");Serial.println(target);
    section_size=FRAME[3];
    fade=FRAME[4];
    R=FRAME[5];
    G=FRAME[6];
    B=FRAME[7];
  }

  // move the focus towards the target
  int8_t displacement=focus-target;
  if(displacement>0){Serial.println("Displacement>0");
    focus--;}
  else if(displacement<0){Serial.println("Displacement<0");
    focus++;}
  else return; // we are where we're supposed to be
  Serial.print("focus ");Serial.println(focus);
  byte scaledR=R;
  byte scaledG=G;
  byte scaledB=B;
  
  // make the section odd in the number of pixels
  // and set the scaling to half of the section
  if(section_size%2==0)
    section_size=section_size+1;
  
  // set the focus point to full color
  if(focus >=0 && focus<PIXEL_COUNT)
    strip.setPixelColor(focus,strip.Color(scaledR,scaledG,scaledB));
  
  for(int i=1; i<(section_size-1)/2; ++i)
  {
    // start fading when reaching the last 1/fade of the section to be lighted
    if(i>=(section_size-1)/2-fade)
    {
      scaledR=scaledR/fade;
      scaledG=scaledG/fade;
      scaledB=scaledB/fade;
    }
    
    // set the pixels in the section to more dimmed colors the more they are distant from the center
    if(focus >0 && focus<PIXEL_COUNT)
    {
      /*
      Serial.print("powering pixel ");Serial.print(pixel+i);Serial.print(" with value ");Serial.println(scaledR);
      Serial.print("powering pixel ");Serial.print(pixel+i);Serial.print(" with value ");Serial.println(scaledG);
      Serial.print("powering pixel ");Serial.print(pixel+i);Serial.print(" with value ");Serial.println(scaledB);Serial.println("");
      Serial.println("");
      */
        strip.setPixelColor(focus+i,strip.Color(scaledR,scaledG,scaledB));
        strip.setPixelColor(focus-i,strip.Color(scaledR,scaledG,scaledB));
      
    }
  }
  
  // activate the LED strip
  strip.show();
  delay(10);
}

void pulsingGreen()
{ 
  for(int i = PIXEL_COUNT/2; i < PIXEL_COUNT; i++){
    strip.setPixelColor(i,strip.Color(0,255,0));
    strip.setPixelColor(PIXEL_COUNT-i-1,strip.Color(0,255,0));
    strip.show();
    delay(20);
  }
   for(int i = 0; i < PIXEL_COUNT/2; i++){
    strip.setPixelColor(i,strip.Color(0,0,0));
    strip.setPixelColor(PIXEL_COUNT-i-1,strip.Color(0,0,0));
    strip.show();
    delay(20);
  }
}

void resetLEDBuffer()
{
  for(int i = 0; i < PIXEL_COUNT; i++)
  {
    strip.setPixelColor(i,strip.Color(0,0,0));
    //strip.setBrightness(255);
  }
}

//Theatre-style crawling lights.
void LeftChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 100 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
//Theatre-style crawling lights.
void RightChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 100 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i-q, c);    //turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i-q, 0);        //turn every third pixel off
      }
    }
  }
}
