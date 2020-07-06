#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>


U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

int btn1 = 0;
int btn2 = 0;
int btn3 = 0;
int btn4 = 0;

int button1 = 6;
int button2 = 9;
int button3 = 10;
int button4 = 11;

int periodo = 1000;
unsigned long TiempoAhora=0;

String S1 = "TEMP: 18C" ;
String S2 = "25% RH";
String S3 = "TVOC: 2ppb";
String S4 = "eCO2: 410ppm";
String S5 = "BAT: 8.21v";


int xPin = A1;
int yPin = A0;
int buttonPin = 2;

int xPosition = 0;
int yPosition = 0;
int buttonState = 0;


String BT_input;                          // to store input character received via BT.
int LED = 12;                              // device to control

void setup()  
{  
  Serial.begin(9600);                      //default baud rate of module
  Serial.setTimeout(50);                  // sets the maximum milliseconds to wait for serial data. It defaults to 1000 milliseconds.
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(LED, OUTPUT);

    //activar resistencia pull-up en el pin pulsador 
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(button1, INPUT_PULLUP); 
  pinMode(button2, INPUT_PULLUP); 
  pinMode(button3, INPUT_PULLUP); 
  pinMode(button4, INPUT_PULLUP); 
  // Para las versiones anteriores a 1.0.1 Arduino 
  // pinMode (buttonPin, INPUT); 
  // digitalWrite (buttonPin, HIGH);
  
   u8g2.begin();
   
  while (!Serial) 
  {
     // wait for serial port to connect. Needed for native USB port only
  }
}


//================================= MAIN ======================================== //
void loop() 
{
//   //========SEND DATA TO OLED====== //
//   u8g2.clearBuffer();          // clear the internal memory
//   u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall   u8g2_font_logisoso28_tr
//   u8g2.drawStr(8,29,"MAIN"); // write something to the internal memory
//   u8g2.sendBuffer();         // transfer internal memory to the display
//   // ============================== //
   //delay(1000);
   //Serial.print("Serial OK");
   oled_init();
   //check_serial();
   //ReadJoystick();
}



//=================================ALERT FUNCTION======================================== //
void alert(){
  
        if (BT_input=="A")                
        {
          //Serial.println(BT_input);
          digitalWrite(LED, HIGH);
          //Serial.println("ALERTA DETECTADA");
          //========SEND DATA TO OLED====== //
          u8g2.clearBuffer();          // clear the internal memory
          u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall   u8g2_font_logisoso28_tr
          u8g2.drawStr(5,29,"ALERTA"); // write something to the internal memory
          u8g2.sendBuffer();         // transfer internal memory to the display
          // ============================== //
          while(1){
            if (Serial.available())
            {
              BT_input = Serial.readString();   // read input string from bluetooth 
        
              if (BT_input=="A1")                
              {
                   //========SEND DATA TO OLED====== //
                    u8g2.clearBuffer();          // clear the internal memory
                    u8g2.setFont(u8g2_font_ncenB14_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall   u8g2_font_logisoso28_tr
                    u8g2.drawStr(8,20,"PRESENCIA"); // write something to the internal memory
                    u8g2.sendBuffer();         // transfer internal memory to the display
                    delay(2000);
                   // ============================== //
                   break;
              }
              if (BT_input=="A2")                
              {
                   //========SEND DATA TO OLED====== //
                    u8g2.clearBuffer();          // clear the internal memory
                    u8g2.setFont(u8g2_font_ncenB14_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall   u8g2_font_logisoso28_tr
                    u8g2.drawStr(8,20,"SONIDO"); // write something to the internal memory
                    u8g2.sendBuffer();         // transfer internal memory to the display
                    delay(2000);
                   // ============================== //
                   break;
              }
              if (BT_input=="A3")                
              {
                   //========SEND DATA TO OLED====== //
                    u8g2.clearBuffer();          // clear the internal memory
                    u8g2.setFont(u8g2_font_ncenB14_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall   u8g2_font_logisoso28_tr
                    u8g2.drawStr(8,20,"HUMEDAD"); // write something to the internal memory
                    u8g2.sendBuffer();         // transfer internal memory to the display
                    delay(2000);
                   // ============================== //
                   break;
              }
              if (BT_input=="A4")                
              {
                   //========SEND DATA TO OLED====== //
                    u8g2.clearBuffer();          // clear the internal memory
                    u8g2.setFont(u8g2_font_helvB12_tf);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall   u8g2_font_logisoso28_tr
                    u8g2.drawStr(0,20,"TEMPERATURA"); // write something to the internal memory
                    u8g2.sendBuffer();         // transfer internal memory to the display
                    delay(2000);
                   // ============================== //
                   break;
              }
              if (BT_input=="A5")                
              {
                   //========SEND DATA TO OLED====== //
                    u8g2.clearBuffer();          // clear the internal memory
                    u8g2.setFont(u8g2_font_ncenB14_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall   u8g2_font_logisoso28_tr
                    u8g2.drawStr(8,20,"GAS"); // write something to the internal memory
                    u8g2.sendBuffer();         // transfer internal memory to the display
                    delay(2000);
                   // ============================== //
                   break;
              }
              else
              {
              break;
              }
             

              
            }
          }
                
        }



       
        else if (BT_input=="B")
        {
          digitalWrite(LED, LOW);
          Serial.println(BT_input);
          Serial.println("LED is OFF");
        }
        else 
        {
          //Serial.println(BT_input);        
          //Serial.println("NO DATA COMING IN");
        }
    }


//=================================JOYSTICK FUNCTION======================================== //
void ReadJoystick(){
  
  xPosition = analogRead(xPin);
  yPosition = analogRead(yPin);
  buttonState = digitalRead(buttonPin);

  if(xPosition>550){
      Serial.print("IZQUIERDA");  //IZQUIERDA
      Serial.print("\n");
      delay(100);
  }
  
  if(xPosition<450){
      Serial.print("DERECHA");  //DERECHA
      Serial.print("\n");
      delay(100);
  }

  if(yPosition>550){
      Serial.print("ARRIBA"); //ARRIBA
      Serial.print("\n");
      delay(100);
  }
  
  if(yPosition<450){
      Serial.print("ABAJO");  //ABAJO
      Serial.print("\n");
      delay(100);
  }

  //Serial.print("\n");
   
}

//=================================SENSOR FUNCTION======================================== //
void sensores(){

        if (BT_input=="S")                
        {
          //Serial.println(BT_input);
          digitalWrite(LED, HIGH);
          //Serial.println("ALERTA DETECTADA");
//          //========SEND DATA TO OLED====== //
//          u8g2.clearBuffer();          // clear the internal memory
//          u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall   u8g2_font_logisoso28_tr
//          u8g2.drawStr(5,29,"SENSORES"); // write something to the internal memory
//          u8g2.sendBuffer();         // transfer internal memory to the display
//          // ============================== //
          while(1){
            if (Serial.available())
            {
              BT_input = Serial.readString();   // read input string from bluetooth 
        
              if (BT_input=="S1")                
              {
                   while(1){
                            if (Serial.available())
                            {
                            S1 = Serial.readString();   // read input string from bluetooth 
                            S1 = "TEMP: " + S1 + "C";
//                            Serial.print(S1);  //TEMPERATURA
//                            Serial.print("\n");
                            break;
                            }
                            }
              }
              if (BT_input=="S2")                
              {
                   while(1){
                            if (Serial.available())
                            {
                            S2 = Serial.readString();   // read input string from bluetooth 
                            S2 = S2 + "% RH";
//                            Serial.print(S2);  //HUMEDAD
//                            Serial.print("\n");
                            break;
                            }
                            }
              }
              if (BT_input=="S3")                
              {
                    while(1){
                            if (Serial.available())
                            {
                            S3 = Serial.readString();   // read input string from bluetooth 
                            S3 = "TVOC: " + S3 + "ppb";
//                            Serial.print(S3);  //HUMEDAD
//                            Serial.print("\n");
                            break;
                            }
                            }
              }
              if (BT_input=="S4")                
              {
                   while(1){
                            if (Serial.available())
                            {
                            S4 = Serial.readString();   // read input string from bluetooth 
                            S4 = "eCO2: " + S4 + "ppm";
//                            Serial.print(S4);  //HUMEDAD
//                            Serial.print("\n");
                            break;
                            }
                            }
              }
              if (BT_input=="S5")                
              {
                   while(1){
                            if (Serial.available())
                            {
                            S5 = Serial.readString();   // read input string from bluetooth 
                            S5 = "BAT: " + S5 + "v";
//                            Serial.print(S5);  //HUMEDAD
//                            Serial.print("\n");
                            break;
                            }
                            }
              }
              else
              {
                break;
              }
             

              
            }
          }
                
        }
        else 
        {
        }
}
 

//=================================SENSOR FUNCTION======================================== //
void oled_init(){
  
//========SEND DATA TO OLED====== //
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_fub14_tf);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall   u8g2_font_8x13B_tf
  u8g2.drawStr(0, 25, S1.c_str()); // write something to the internal memory S1.c_str()
  u8g2.sendBuffer();         // transfer internal memory to the display
  TiempoAhora = millis();
  while(millis() < TiempoAhora+periodo){
    check_serial();
    ReadJoystick();
    button_check();
  }
//============================== //

//========SEND DATA TO OLED====== //
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_courB18_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall   u8g2_font_logisoso28_tr u8g2_font_t0_22b_me 
  u8g2.drawStr(0, 25, S2.c_str()); // write something to the internal memory S1.c_str()
  u8g2.sendBuffer();         // transfer internal memory to the display
  TiempoAhora = millis();
  while(millis() < TiempoAhora+periodo){
    check_serial();
    ReadJoystick();
    button_check();
  }
//============================== //

//========SEND DATA TO OLED====== //
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_fub14_tf);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall   u8g2_font_logisoso28_tr
  u8g2.drawStr(0, 25, S3.c_str()); // write something to the internal memory S1.c_str()
  u8g2.sendBuffer();         // transfer internal memory to the display
  TiempoAhora = millis();
  while(millis() < TiempoAhora+periodo){
    check_serial();
    ReadJoystick();
    button_check();
  }
//============================== //

//========SEND DATA TO OLED====== //
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_crox4tb_tr  );  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall   u8g2_font_logisoso28_tr
  u8g2.drawStr(0, 25, S4.c_str()); // write something to the internal memory S1.c_str()
  u8g2.sendBuffer();         // transfer internal memory to the display
  TiempoAhora = millis();
  while(millis() < TiempoAhora+periodo){
    check_serial();
    ReadJoystick();
    button_check();
  }
//============================== //

//========SEND DATA TO OLED====== //
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_fub14_tf);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall   u8g2_font_logisoso28_tr
  u8g2.drawStr(0, 25, S5.c_str()); // write something to the internal memory S1.c_str()
  u8g2.sendBuffer();         // transfer internal memory to the display
  TiempoAhora = millis();
  while(millis() < TiempoAhora+periodo){
    check_serial();
    ReadJoystick();
    button_check();
  }
//============================== //

}

void check_serial(){ 
  if (Serial.available())
   {   
   BT_input = Serial.readString();   // read input string from bluetooth 
   sensores();
   alert();
   
   }
}

void button_check(){
  btn1 = 0;
  btn2 = 0;
  btn3 = 0;
  btn4 = 0;
  btn1=digitalRead(button1);
  btn2=digitalRead(button2);
  btn3=digitalRead(button3);
  btn4=digitalRead(button4);
  if (btn1 == 0){
    Serial.print("R");
    Serial.print("\n");
    delay(400);
  }
  if (btn2 == 0){
    Serial.print("C");
    Serial.print("\n");
    delay(400);
  }
  if (btn3 == 0){
    Serial.print("M");
    Serial.print("\n");
    delay(400);
  }
  if (btn4 == 0){
    Serial.print("O");
    Serial.print("\n");
    delay(400);
  }
}
