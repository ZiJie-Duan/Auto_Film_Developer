#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define PWM_OUT 15
#define PWM_direction 14

#define Button_1 34
#define Button_2 36
#define Button_3 39

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int pwm_on=0;
int pwm_off=20;
int direction = 1;

int state = 0;

void change_direction(int direction);

void speed_change(int speed);
void Button_trigger(char button);
int Button_debouncer(char button);
void Button_1_remapped() {Button_trigger('a');}
void Button_2_remapped() {Button_trigger('b');}
void Button_3_remapped() {Button_trigger('c');}
void Update_Display(void);
//5,10,25,30,35,40,45,50,55,60 

void setup(){

    pinMode(Button_1, INPUT);
    pinMode(Button_2, INPUT);
    pinMode(Button_3, INPUT);
    pinMode(PWM_OUT, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(Button_1), Button_1_remapped, RISING);
    attachInterrupt(digitalPinToInterrupt(Button_2), Button_2_remapped, RISING);
    attachInterrupt(digitalPinToInterrupt(Button_3), Button_3_remapped, RISING);

    Serial.begin(9600);
    
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.display();
    delay(2000); // Pause for 2 seconds
    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println("Auto Film Developer");
    display.display();
}

void loop() {
  // put your main code here, to run repeatedly:
  Update_Display();
  if (state==1){
    digitalWrite(PWM_OUT, 1);
    delay(pwm_on);
    digitalWrite(PWM_OUT, 0);
    delay(pwm_off);
  }
}

void Update_Display(void){
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println("Auto Film Developer");
  display.setCursor(0,16);             // Start at top-left corner
  display.print("Speed: ");
  display.print(pwm_on);
  if (state==1){
    display.setCursor(80,16);
    display.println("[ON]");
  } else {
    display.setCursor(80,16);
    display.println("[OFF]");
  }
  display.display();
}

int Button_debouncer(char button){
  static unsigned long time_now = millis();
  static unsigned long button1_delay = time_now;
  static unsigned long button2_delay = time_now;
  static unsigned long button3_delay = time_now;

  time_now = millis();
  if (button=='a'){
    if (time_now-button1_delay>50){
      button1_delay=time_now;
      return 1;
    }
  } else if (button=='b'){
    if (time_now-button2_delay>50){
      button2_delay=time_now;
      return 1;
    }
  } else if (button=='c'){
    if (time_now-button3_delay>50){
      button3_delay=time_now;
      return 1;
    }
  }
  return 0;
}

void Button_trigger(char button){
  if (button=='a'){
    if (Button_debouncer('a')){
      if (state==1){
        state=0;
      }
      else{
        state=1;
      }
    }
  } else if (button=='b'){
    if (Button_debouncer('b')){
      speed_change(1);
    }
  } else if (button=='c'){
    if (Button_debouncer('c')){
      speed_change(-1);
    }
  }
}

void speed_change(int speed){
  if (speed >= 0){
    pwm_on += 2;
    pwm_off -= 2;
  } else {
    pwm_on -= 2;
    pwm_off += 2;
  }
  if (pwm_on > 0){
    change_direction(1);
  } else {
    change_direction(-1);
  }
}

void change_direction(int direction){
  if (direction==1){
    digitalWrite(PWM_direction, 1);
  } else {
    digitalWrite(PWM_direction, 0);
  }
}
// pinMode(pin, OUTPUT);
// }
// void loop()
// {
// analogWrite(pin, 128);
// delay(500);