#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 m_p_u;

#define inGiu -9
#define gradiErrore 2

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI);
  //uint8_t w, uint8_t h, SPIClass *spi_ptr,
    //                               int8_t dc_pin, int8_t rst_pin, int8_t cs_pin,
      //                             uint32_t bitrate)

//reference state
int ref_eye_height = 40;
int ref_eye_width = 40;
int ref_space_between_eye = 10;
int ref_corner_radius = 10;
//current state of the eyes
int left_eye_height = ref_eye_height;
int left_eye_width = ref_eye_width;
int left_eye_x = 32;
int left_eye_y = 32;
int right_eye_x = 32+ref_eye_width+ref_space_between_eye;
int right_eye_y = 32;
int right_eye_height = ref_eye_height;
int right_eye_width = ref_eye_width;
int anim=0;


void draw_eyes(bool update=true){
    display.clearDisplay();        
    //draw from center
    int x = int(left_eye_x-left_eye_width/2);
    int y = int(left_eye_y-left_eye_height/2);
    display.fillRoundRect(x,y,left_eye_width,left_eye_height,ref_corner_radius,SSD1306_WHITE);
    x = int(right_eye_x-right_eye_width/2);
    y = int(right_eye_y-right_eye_height/2);
    display.fillRoundRect(x,y,right_eye_width,right_eye_height,ref_corner_radius,SSD1306_WHITE);    
    if(update)
    {
      display.display();
    }
    
}

void center_eyes(bool update=true){
  //move eyes to the center of the display, defined by SCREEN_WIDTH, SCREEN_HEIGHT
  left_eye_height = ref_eye_height;
  left_eye_width = ref_eye_width;
  right_eye_height = ref_eye_height;
  right_eye_width = ref_eye_width;
  
  left_eye_x = SCREEN_WIDTH/2-ref_eye_width/2-ref_space_between_eye/2;
  left_eye_y = SCREEN_HEIGHT/2;
  right_eye_x = SCREEN_WIDTH/2+ref_eye_width/2+ref_space_between_eye/2;
  right_eye_y = SCREEN_HEIGHT/2;
  
  draw_eyes(update);
}

void blink(int speed=12){
  draw_eyes();
  
  
  for(int i=0;i<3;i++)
  {
    left_eye_height = left_eye_height-speed;
    right_eye_height = right_eye_height-speed;    
    draw_eyes();
    delay(1);
  }
  for(int i=0;i<3;i++)
  {
    left_eye_height = left_eye_height+speed;
    right_eye_height = right_eye_height+speed;
    
    draw_eyes();
    delay(1);
  }
}

void sleep(){
  left_eye_height = 2;
  right_eye_height = 2;
  draw_eyes(true);  
}
void wakeup(){
  
  sleep();
  
  for(int h=0; h <= ref_eye_height; h+=2)
  {
    left_eye_height = h;
    right_eye_height = h;
    draw_eyes(true);
  }
  

}

void happy_eye(){
  center_eyes(false);
  //draw inverted triangle over eye lower part
  int offset = ref_eye_height/2;
  for(int i=0;i<10;i++)
  {
    display.fillTriangle(left_eye_x-left_eye_width/2-1, left_eye_y+offset, left_eye_x+left_eye_width/2+1, left_eye_y+5+offset, left_eye_x-left_eye_width/2-1,left_eye_y+left_eye_height+offset,SSD1306_BLACK);
    //display.fillRect(left_eye_x-left_eye_width/2-1, left_eye_y+5, left_eye_width+1, 20,SSD1306_BLACK);

    display.fillTriangle(right_eye_x+right_eye_width/2+1, right_eye_y+offset, right_eye_x-left_eye_width/2-1, right_eye_y+5+offset, right_eye_x+right_eye_width/2+1,right_eye_y+right_eye_height+offset,SSD1306_BLACK);
    //display.fillRect(right_eye_x-right_eye_width/2-1, right_eye_y+5, right_eye_width+1, 20,SSD1306_BLACK);  
    offset -= 2;
    display.display();
    delay(1);
  }
  
  
  display.display();
  delay(1000);
}

void sad_eye(){
  center_eyes(false);
  // Disegna il triangolo sopra l'occhio
  int offset = ref_eye_height/2;
  for(int i=0;i<10;i++)
  {
    // Sposta leggermente il triangolo in alto e allungalo leggermente per evitare la sovrapposizione
    display.fillTriangle(left_eye_x - left_eye_width / 2 - 3, left_eye_y - offset, left_eye_x + left_eye_width / 2 + 3, left_eye_y - 7 - offset, left_eye_x - left_eye_width / 2 - 3, left_eye_y - left_eye_height - offset, SSD1306_BLACK);
    display.fillTriangle(right_eye_x + right_eye_width / 2 + 3, right_eye_y - offset, right_eye_x - left_eye_width / 2 - 3, right_eye_y - 7 - offset, right_eye_x + right_eye_width / 2 + 3, right_eye_y - right_eye_height - offset, SSD1306_BLACK);
    offset -= 2;
    display.display();
    delay(1000);
  }
}

void angry_eye(){
  center_eyes(false);
  // Disegna il triangolo sopra l'occhio per creare l'effetto di occhi arrabbiati
  int offset = ref_eye_height / 2;
  for (int i = 0; i < 10; i++)
  {
    // Sposta leggermente il triangolo in alto e allungalo leggermente per evitare la sovrapposizione
    display.fillTriangle(right_eye_x - right_eye_width / 2 - 3, right_eye_y - offset, right_eye_x + right_eye_width / 2 + 3, right_eye_y - 7 - offset, right_eye_x - right_eye_width / 2 - 3, right_eye_y - right_eye_height - offset, SSD1306_BLACK);
    display.fillTriangle(left_eye_x + left_eye_width / 2 + 3, left_eye_y - offset, left_eye_x - right_eye_width / 2 - 3, left_eye_y - 7 - offset, left_eye_x + left_eye_width / 2 + 3, left_eye_y - left_eye_height - offset, SSD1306_BLACK);
    offset -= 2;
    display.display();
    delay(1000);
  }
}

void vibration_random_eye(int steps = 20, int max_offset = 4) {
  int max_distance = SCREEN_WIDTH / 4;
  for (int i = 0; i < steps; i++) {
    int left_distance = random(0, max_distance);
    int left_offset = random(-max_offset, max_offset);
    int right_distance = random(0, max_distance);
    int right_offset = random(-max_offset, max_offset);

    left_eye_x = SCREEN_WIDTH / 2 - ref_eye_width / 2 - ref_space_between_eye / 2 + left_offset;
    right_eye_x = SCREEN_WIDTH / 2 + ref_eye_width / 2 + ref_space_between_eye / 2 + right_offset;

    draw_eyes();
    delay(30);
  }

  center_eyes();
}

void vibration(int steps = 20, int max_offset = 4) {
  int max_distance = SCREEN_WIDTH / 4;
  for (int i = 0; i < steps; i++) {
    // Calcola una distanza casuale per lo spostamento laterale
    int distance = random(0, max_distance);

    // Calcola un offset casuale dalla posizione centrale
    int offset = random(-max_offset, max_offset);

    // Applica lo stesso movimento sia all'occhio sinistro che a quello destro
    left_eye_x = SCREEN_WIDTH / 2 - ref_eye_width / 2 - ref_space_between_eye / 2 + offset;
    right_eye_x = SCREEN_WIDTH / 2 + ref_eye_width / 2 + ref_space_between_eye / 2 + offset;

    draw_eyes();
    delay(30);
  }

  // Riporta gli occhi al centro
  center_eyes();
}

void saccade(int direction_x, int direction_y){
  //quick movement of the eye, no size change. stay at position after movement, will not move back,  call again with opposite direction
  //direction == -1 :  move left
  //direction == 1 :  move right
  
  int direction_x_movement_amplitude = 8;
  int direction_y_movement_amplitude = 6;
  int blink_amplitude = 8;

  for(int i=0;i<1;i++)
  {
    left_eye_x+=direction_x_movement_amplitude*direction_x;
    right_eye_x+=direction_x_movement_amplitude*direction_x;    
    left_eye_y+=direction_y_movement_amplitude*direction_y;
    right_eye_y+=direction_y_movement_amplitude*direction_y;    
    
    right_eye_height-=blink_amplitude;
    left_eye_height-=blink_amplitude;
    draw_eyes();
    delay(1);
  }
  
  for(int i=0;i<1;i++)
  {
    left_eye_x+=direction_x_movement_amplitude*direction_x;
    right_eye_x+=direction_x_movement_amplitude*direction_x;    
    left_eye_y+=direction_y_movement_amplitude*direction_y;
    right_eye_y+=direction_y_movement_amplitude*direction_y;

    right_eye_height+=blink_amplitude;
    left_eye_height+=blink_amplitude;

    draw_eyes();
    delay(1);
  }
  
}

void move_right_big_eye(){
  move_big_eye(1);
}
void move_left_big_eye(){
  move_big_eye(-1);
}
void move_big_eye(int direction){
  //direction == -1 :  move left
  //direction == 1 :  move right

  int direction_oversize = 1;
  int direction_movement_amplitude = 2;
  int blink_amplitude = 5;

  for(int i=0;i<3;i++)
  {
    left_eye_x+=direction_movement_amplitude*direction;
    right_eye_x+=direction_movement_amplitude*direction;    
    right_eye_height-=blink_amplitude;
    left_eye_height-=blink_amplitude;
    if(direction>0)
    {
      right_eye_height+=direction_oversize;
      right_eye_width+=direction_oversize;
    }else
    {
      left_eye_height+=direction_oversize;
      left_eye_width+=direction_oversize;
    }

    draw_eyes();
    delay(1);
  }
  for(int i=0;i<3;i++)
  {
    left_eye_x+=direction_movement_amplitude*direction;
    right_eye_x+=direction_movement_amplitude*direction;
    right_eye_height+=blink_amplitude;
    left_eye_height+=blink_amplitude;
    if(direction>0)
    {
      right_eye_height+=direction_oversize;
      right_eye_width+=direction_oversize;
    }else
    {
      left_eye_height+=direction_oversize;
      left_eye_width+=direction_oversize;
    }
    draw_eyes();
    delay(1);
  }

  delay(1000);

  for(int i=0;i<3;i++)
  {
    left_eye_x-=direction_movement_amplitude*direction;
    right_eye_x-=direction_movement_amplitude*direction;    
    right_eye_height-=blink_amplitude;
    left_eye_height-=blink_amplitude;
    if(direction>0)
    {
      right_eye_height-=direction_oversize;
      right_eye_width-=direction_oversize;
    }else
    {
      left_eye_height-=direction_oversize;
      left_eye_width-=direction_oversize;
    }
    draw_eyes();
    delay(1);
  }
  for(int i=0;i<3;i++)
  {
    left_eye_x-=direction_movement_amplitude*direction;
    right_eye_x-=direction_movement_amplitude*direction;    
    right_eye_height+=blink_amplitude;
    left_eye_height+=blink_amplitude;
    if(direction>0)
    {
      right_eye_height-=direction_oversize;
      right_eye_width-=direction_oversize;
    }else
    {
      left_eye_height-=direction_oversize;
      left_eye_width-=direction_oversize;
    }
    draw_eyes();
    delay(1);
  }


  center_eyes();
}

void setup() {
  // put your setup code here, to run once:
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  
  
  //for usb communication  
  Serial.begin(115200);
  
  

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  
  // Clear the buffer
  display.clearDisplay();
  
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("LA"));
  display.display();
  delay(300);
  sleep();
  

  // Draw a single pixel in white
  //display.drawPixel(10, 10, SSD1306_WHITE);
  delay(1500);
  wakeup();
  center_eyes(true);

  //accellerometro
  while(!Serial)
  delay(20);
  if(!m_p_u.begin()){

    while(1){

      delay(20);
    }
  }

  // Chiudi la comunicazione seriale quando non è più necessaria ----------------------------------
  //Serial.end();
}

void loop() {
  // put your main code here, to run repeatedly:

  sensors_event_t acc, gcc, temp;
  m_p_u.getEvent(&acc,&gcc,&temp);
  Serial.println("-------------------------------------------------------------------------------");
  Serial.println("Accellerazione x, y, z");
  Serial.println(acc.acceleration.x);
  Serial.println(acc.acceleration.y);
  Serial.println(acc.acceleration.z);
  Serial.println("Giroscopio in gradi x, y, z");
  Serial.println((gcc.gyro.x)*180/3.14);
  Serial.println((gcc.gyro.y)*180/3.14);
  Serial.println((gcc.gyro.z)*180/3.14);
  Serial.println("Temperatura");
  Serial.println(temp.temperature);

  delay(500 * random(6, 20));

  if (random(0, 3) != 2) {
    blink();
  } else {
    int dir_x, dir_y; // declare variables outside the switch block
    switch (random(0, 11)) {
      case 0:
        blink();
        blink();
        break;
      case 1:
        move_right_big_eye();
        break;
      case 2:
        move_left_big_eye();
        break;
      case 3:
        happy_eye();
        break;
      case 4:
        sleep();
        delay(500 * random(10, 30));
        wakeup();
        break;
      case 5:
        dir_x = random(-1, 2); // initialize variables inside the case
        dir_y = random(-1, 2);
        saccade(dir_x, dir_y);
        delay(300);
        saccade(-dir_x, -dir_y);
        delay(300);
        break;
      case 6:
        move_right_big_eye();
        move_left_big_eye();
        break;
      case 7:
        sad_eye();
        break;
      case 8:
        angry_eye();
        break;
      case 9:
        vibration_random_eye(random(20, 50));
        break;
      case 10:
        vibration(random(20, 50));
        break;
    }
  }
}
