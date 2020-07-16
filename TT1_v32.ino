#include <Boards.h>
#include <Firmata.h>
#include <FirmataConstants.h>
#include <FirmataDefines.h>
#include <FirmataMarshaller.h>
#include <FirmataParser.h>

 /////////Humo -- Gas -- Contaminacion -- SGP30 //////////
#include <Wire.h>
#include "Adafruit_SGP30.h"
Adafruit_SGP30 sgp;
uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}

/////////humeda y temperatura --BME280 ///////////////////
#include <BME280I2C.h>
#include <Wire.h>
BME280I2C bme;
///// interrupciones //////////7
//#include <RTClock.h>
#include <EEPROM.h>
///////////// Puente H ///////
int led1=7, motor1f=9, motor1a=10;
int led2=8, motor2f=5, motor2a=6;

///////////// multiplexor /////
int multA=11, multB=12, multC=13, line=A6, total=0;

//////////// control PID  //////
 float kp=16, ki=0.0, kd=5.2,dt=0.015;// KI 0.1;kd=5.2 kp=16
 int ref=8, e_new=0, e_old=0,e_acum=0;
 int vel_robot=130;
    /// Multirramal
    int bandera=0;/// 0=izquierda 1=derecha;
 ///////// calibracion sensores linea ///////////
 int rango8=150, rango7=150,rango6=150, rango5=150,rango4=150, rango3=150,rango2=150, rango1=150; 
///////// microfono ///////////
int micro=A7;
///////// PIRS //////////
int pirder=2, pirizq=4, activos=0;
//////// CAMARA nocturna /////
int cam_on=A2;
//////// raspberry /////
int raspberry=A4;
/////// BUZZER ///////
int buzzer_on=A3;
//////// recargar ////
int refill=0;
//////// BLUETOTH
int on_line=0, dato1=0;// por default modo manual
int c_bme=0,c_asg=0;

void setup() {
  Serial.begin(9600);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(motor1f, OUTPUT);
  pinMode(motor1a, OUTPUT);
  pinMode(motor2f, OUTPUT);
  pinMode(motor2a, OUTPUT);
  pinMode(multA, OUTPUT);
  pinMode(multB, OUTPUT);
  pinMode(multC, OUTPUT);
  pinMode(line, INPUT);
  pinMode(A7, INPUT);// microfono
  pinMode(A3, OUTPUT);// buzzer
  pinMode(A2, OUTPUT);// CAM INFRARROJA
  pinMode(pirizq, INPUT);
  pinMode(pirder, INPUT);
  motores(2,0,0);//motor, sentido, vel
  motores(1,0,0);

  if (! sgp.begin()){
    //Serial.println("Sensor not found :(");
    while (1);
  }
  Wire.begin();
  while(!bme.begin())
  { Serial.println("Error BME280 sensor!");
    delay(1000);}
  digitalWrite(buzzer_on,HIGH);
  digitalWrite(cam_on,LOW);
motores(2,0,0);
motores(1,0,0);
  buzzer();
  delay(3000);
//  rt.attachSecondsInterrupt(segundo);
}
//checar UART serie, control manual,envio alertas
void loop() {
  int b=0;
if(raspberry==1){
  Serial.print("A1");
}
PIR();
sonido();
bt();
for(b=0;b<1000;b++){//1500
  if(on_line==1){
  PID_linea();}
  }
}
void segundo(){
  bateria();
  GasHumo();
  TempHum(&Serial);
}
void bt(){
  if (Serial.available())
   {  //digitalWrite(cam_on,!digitalRead(cam_on));
      char data = Serial.read();
      if (data == 'R')// RECARGA
      {refill=1;}
      if (data == 'C')// ENCENDER CAMARA
      { digitalWrite(cam_on,!digitalRead(cam_on));}
      if (data == 'M')// MODO MANUAL
      { Stop();
        on_line=0;}
      if (data == 'O')// MODO SEGUIDOR
      { on_line=1;}
      if (data == 'T')//
      { if(on_line==0){ adel();}}
      if (data == 'B')//
      { if(on_line==0){ atras();}}
      if (data == 'D')// 
      { if(on_line==0){ der();}}
      if (data == 'I')// 
      { if(on_line==0){ izq();}}
      if (data == 'S')// 
      { if(on_line==0){ Stop();}}
   }
}
void adel(){
  motores(2,0,100);
  motores(1,0,100);}
void atras(){
  motores(2,1,100);
  motores(1,1,100);}
void izq(){
  motores(2,0,100);
  motores(1,0,0);
  delay(300);
  motores(2,0,100);
  motores(1,0,100);}
void der(){
  motores(2,0,0);
  motores(1,0,100);
  delay(300);
  motores(2,0,100);
  motores(1,0,100);}
void enchufe(){
  Stop();
  delay(500);
  motores(2,1,120);//110 350
  motores(1,0,0);
  delay(300);
  atras();
  delay(785);
  Stop();
  on_line=0;
  refill=0;
}
void buzzer(){
  digitalWrite(buzzer_on,LOW);
  delay(200);
  digitalWrite(buzzer_on,HIGH);
  delay(100);
  digitalWrite(buzzer_on,LOW);
  delay(500);
  digitalWrite(buzzer_on,HIGH);
  delay(100);
}
void motores(byte motor,byte direccion,int vel){
  //motor 1 o 2, direccion 1 o 0, vel pwm;
  vel=255-vel;
   if(motor==1){
   if(direccion==HIGH){
    analogWrite(motor1a,255);
    delay(2);
    analogWrite(motor1f,vel);   }
   else{    
    analogWrite(motor1f,255);
    delay(2);
    analogWrite(motor1a,vel);}
   }

   if(motor==2){
   if(direccion==HIGH){
    analogWrite(motor2a,255);
    delay(2);
    analogWrite(motor2f,vel);}
   else{
    analogWrite(motor2f,255);
    delay(2);
    analogWrite(motor2a,vel);}
   }
}
void leds(){
  digitalWrite(7,HIGH);
  digitalWrite(8,HIGH);
  delay(100);
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
  delay(100);
  
}
void calibracion(){
  int s1=0,s2=0,s3=0,s4=0,s5=0,s6=0,s7=0,s8=0,retardo=100;
  
  digitalWrite(multC,0),digitalWrite(multB,1),digitalWrite(multA,1);
  s8=analogRead(A6), delay(retardo);
  
  digitalWrite(multC,0),digitalWrite(multB,0),digitalWrite(multA,0);
  s7=analogRead(A6), delay(retardo);
  
  digitalWrite(multC,0),digitalWrite(multB,0),digitalWrite(multA,1);
  s6=analogRead(A6), delay(retardo);
  
  digitalWrite(multC,0),digitalWrite(multB,1),digitalWrite(multA,0);
  s5=analogRead(A6), delay(retardo);

  digitalWrite(multC,1),digitalWrite(multB,0),digitalWrite(multA,0);
  s4=analogRead(A6), delay(retardo);

  digitalWrite(multC,1),digitalWrite(multB,1),digitalWrite(multA,1);
  s3=analogRead(A6), delay(retardo);
  
  digitalWrite(multC,1),digitalWrite(multB,1),digitalWrite(multA,0);
  s2=analogRead(A6), delay(retardo);
  
  digitalWrite(multC,1),digitalWrite(multB,0),digitalWrite(multA,1);
  s1=analogRead(A6), delay(retardo);

  Serial.println(s8);
  Serial.println(s7);
  Serial.println(s6);
  Serial.println(s5);
  Serial.println(s4);
  Serial.println(s3);
  Serial.println(s2);
  Serial.println(s1);
  //adelante atras
}
int s_linea(){
  
  int s1=0,s2=0,s3=0,s4=0,s5=0,s6=0,s7=0,s8=0,retardo=100,inter=0;
  
  digitalWrite(multC,0),digitalWrite(multB,1),digitalWrite(multA,1);
  s8=analogRead(A6), delayMicroseconds(retardo);
  //Serial.println(s8);
  if(s8>rango8){s8=1;}
  else{s8=0;}
  
  digitalWrite(multC,0),digitalWrite(multB,0),digitalWrite(multA,0);
  s7=analogRead(A6), delayMicroseconds(retardo);
  //Serial.println(s7);
  if(s7>rango7){s7=1;}
  else{s7=0;}
  
  digitalWrite(multC,0),digitalWrite(multB,0),digitalWrite(multA,1);
  s6=analogRead(A6), delayMicroseconds(retardo);
  //Serial.println(s6);
  if(s6>rango6){s6=1;}
  else{s6=0;}
  
  digitalWrite(multC,0),digitalWrite(multB,1),digitalWrite(multA,0);
  s5=analogRead(A6), delayMicroseconds(retardo);
  //Serial.println(s5);
  if(s5>rango5){s5=1;}
  else{s5=0;}

  digitalWrite(multC,1),digitalWrite(multB,0),digitalWrite(multA,0);
  s4=analogRead(A6), delayMicroseconds(retardo);
  //Serial.println(s4);
  if(s4>rango4){s4=1;}
  else{s4=0;}
  
  digitalWrite(multC,1),digitalWrite(multB,1),digitalWrite(multA,1);
  s3=analogRead(A6), delayMicroseconds(retardo);
  //Serial.println(s3);
  if(s3>rango3){s3=1;}
  else{s3=0;}
  
  digitalWrite(multC,1),digitalWrite(multB,1),digitalWrite(multA,0);
  s2=analogRead(A6),  delayMicroseconds(retardo);
  //Serial.println(s2);
  if(s2>rango2){s2=1;}
  else{s2=0;}
  
  digitalWrite(multC,1),digitalWrite(multB,0),digitalWrite(multA,1);
  s1=analogRead(A6), delayMicroseconds(retardo);
  //Serial.println(s1);
  if(s1>=rango1){s1=1;}
  else{s1=0;}

//  Serial.println(s8);
//  Serial.println(s7);
//  Serial.println(s6);
//  Serial.println(s5);
//  Serial.println(s4);
//  Serial.println(s3);
//  Serial.println(s2);
//  Serial.println(s1);
//  delay(3000);
  if((s8==1)&&(s7==0)&&(s6==0)&&(s5==0)&&(s4==0)&&(s3==0)&&(s2==0)&&(s1==0)){total=1;}
  if((s8==1)&&(s7==1)&&(s6==0)&&(s5==0)&&(s4==0)&&(s3==0)&&(s2==0)&&(s1==0)){total=2;}
  if((s8==0)&&(s7==1)&&(s6==0)&&(s5==0)&&(s4==0)&&(s3==0)&&(s2==0)&&(s1==0)){total=3;}
  if((s8==0)&&(s7==1)&&(s6==1)&&(s5==0)&&(s4==0)&&(s3==0)&&(s2==0)&&(s1==0)){total=4;}
  if((s8==0)&&(s7==0)&&(s6==1)&&(s5==0)&&(s4==0)&&(s3==0)&&(s2==0)&&(s1==0)){total=5;}
  if((s8==0)&&(s7==0)&&(s6==1)&&(s5==1)&&(s4==0)&&(s3==0)&&(s2==0)&&(s1==0)){total=6;}
  if((s8==0)&&(s7==0)&&(s6==0)&&(s5==1)&&(s4==0)&&(s3==0)&&(s2==0)&&(s1==0)){total=7;}
  if((s8==0)&&(s7==0)&&(s6==0)&&(s5==1)&&(s4==1)&&(s3==0)&&(s2==0)&&(s1==0)){total=8;}
  if((s8==0)&&(s7==0)&&(s6==0)&&(s5==0)&&(s4==1)&&(s3==0)&&(s2==0)&&(s1==0)){total=9;}
  if((s8==0)&&(s7==0)&&(s6==0)&&(s5==0)&&(s4==1)&&(s3==1)&&(s2==0)&&(s1==0)){total=10;}
  if((s8==0)&&(s7==0)&&(s6==0)&&(s5==0)&&(s4==0)&&(s3==1)&&(s2==0)&&(s1==0)){total=11;}
  if((s8==0)&&(s7==0)&&(s6==0)&&(s5==0)&&(s4==0)&&(s3==1)&&(s2==1)&&(s1==0)){total=12;}
  if((s8==0)&&(s7==0)&&(s6==0)&&(s5==0)&&(s4==0)&&(s3==0)&&(s2==1)&&(s1==0)){total=13;}
  if((s8==0)&&(s7==0)&&(s6==0)&&(s5==0)&&(s4==0)&&(s3==0)&&(s2==1)&&(s1==1)){total=14;}
  if((s8==0)&&(s7==0)&&(s6==0)&&(s5==0)&&(s4==0)&&(s3==0)&&(s2==0)&&(s1==1)){total=15;}
  if((s8==1)&&(s7==1)&&(s6==0)&&(s3==0)&&(s2==0)&&(s1==1)){if(refill==1){enchufe();total=5;}}
  
  if((s6==1)&&(s5==1)&&(s4==1)&&(s3==1)){  intersecciones();inter=1;  }
  if((s7==1)&&(s6==1)&&(s5==1)&&(s4==1)&&(s3==1)){ if(inter==0){ intersecciones(); } }
  if((s6==1)&&(s5==1)&&(s4==1)&&(s3==1)&&(s2==1)){ if(inter==0){ intersecciones(); }  }
  if((s8==0)&&(s7==0)&&(s6==0)&&(s5==1)&&(s4==1)&&(s3==1)){if(inter==0){ intersecciones(); }}//derecha
  if((s6==1)&&(s5==1)&&(s4==1)&&(s3==0)&&(s2==0)&&(s1==0)){if(inter==0){ intersecciones(); }}
  if((s8==0)&&(s7==0)&&(s6==0)&&(s5==0)&&(s4==0)&&(s3==0)&&(s2==0)&&(s1==0)){digitalWrite(7,HIGH);}
  else{digitalWrite(7,LOW);}
  return total;
}
void intersecciones(){
    switch(bandera){
    case 0:
      Izquierda();
      delay(100);
      break;
    case 1:
      Derecha();
      break;
    case 2:
      Derecha();
      break;
    case 3:
      Izquierda();
      delay(100);
      break;
    case 4:
      Izquierda();
      bandera=0;
      break;
  }
  bandera++;
}
void Derecha(){/// izquierda
  Stop();
  delay(200);
  motores(2,0,150);
  motores(1,0,0);
  delay(56);//55
  motores(1,1,100);
  delay(327);
}
void Izquierda(){
  Stop();
  delay(200);
  motores(1,0,130);
  motores(2,0,0);
  delay(10);//10
  motores(2,1,100);
  delay(200);
}
void Stop(){
  motores(1,0,0);
  motores(2,0,0);
  //delay(5000);
}
void PID_linea(){
 float p=0,i=0,d=0;
 int PID=0,s_in=0,out2=0,out1=0;
 s_in=s_linea();
 //Serial.println(s_in);
 e_new=s_in-ref;
 
 p=kp*e_new;
 //i=ki*(e_acum+e_new*dt);
 d=kd*((e_new-e_old)/dt);
 PID=p+d;
  
 e_old=e_new;
 e_acum=e_acum+e_new*dt;
out2=vel_robot-PID;
out1=vel_robot+PID+10;
 
if(out1>254){out1=250;}
if(out2>254){out2=250;}
if(out1<25){out1=25;}
if(out2<25){out2=25;}

 motores(2,0,out1);//motor, sentido, vel
 motores(1,0,out2);//motor, sentido, vel
 delay(13);
}
void TempHum(   Stream* client){
   float temp(NAN), hum(NAN), pres(NAN);
   float maxtemp=25, maxhum=40;
  int t1,t2,t3,h1,h2,h3;
   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

   bme.read(pres, temp, hum, tempUnit, presUnit);
 
  c_bme++;
  EEPROM.put(c_bme+9, temp); 
  EEPROM.put(c_bme+6, hum); 
  if(c_bme==2){
    c_bme=0;
   if(dato1==1){
    EEPROM.get(9,t1);
    EEPROM.get(10,t2);
    EEPROM.get(11,t3);
    EEPROM.get(6,h1);
    EEPROM.get(7,h2);
    EEPROM.get(8,h3);
    temp=t1+t2+t3/3;
    hum=h1+h2+h3/3;
    //client->print("Temp: ");
    client->print("S1");
    client->print(temp);
    //client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
    //client->print("\t\tHumidity: ");
    client->print("S2");
    client->print(hum);
    //client->print("% RH");
    dato1=0;
   }
  }
  temp=temp-t1;
  hum=hum-h1;
  if(temp>2){  if(temp){Serial.println("A4");}}
  if(hum>3){   if(hum>maxhum){Serial.println("A3");}}
}
void GasHumo() {
  int maxTVOC=10, maxECO2=408,TVOC=0, ECO2=0;
  long maxrawh2=14000,maxrawethanol=17700,rawethanol=0,rawh2=0;
  int t1,t2,t3,h1,h2,h3;
    if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }
  TVOC=sgp.TVOC;
  ECO2=sgp.eCO2;
  c_asg++;
  EEPROM.put(c_asg+3, TVOC); 
  EEPROM.put(c_asg, ECO2); 
  if(c_asg==2){
  if(dato1==0){
    EEPROM.get(3,t1);
    EEPROM.get(4,t2);
    EEPROM.get(5,t3);
    EEPROM.get(0,h1);
    EEPROM.get(1,h2);
    EEPROM.get(2,h3);
    TVOC=t1+t2+t3/3;
    ECO2=h1+h2+h3/3;
  Serial.print("S3"); 
  Serial.print(TVOC);
  //Serial.print(",");
  //Serial.print(" ppb\t");
  Serial.print("S4 "); 
  Serial.println(ECO2);
  //Serial.println(" ppm");}
  dato1=1;
  }
  if (! sgp.IAQmeasureRaw()) {
    Serial.println("Raw Measurement failed");
    return;  }

  //rawh2=sgp.rawH2;
  //rawethanol=sgp.rawEthanol;
  //Serial.print("Raw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
  //Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");

 if(TVOC>maxTVOC){
  Serial.println("A5"); }
 if(ECO2>maxECO2){
  Serial.println("A5"); }
 // if(rawh2>maxrawh2){
 // Serial.println("Alerta rawh2"); }
 // if(rawethanol>rawethanol){
 // Serial.println("Alerta rawethanol"); }
}
}
void bateria(){
  int voltage=0,minvoltage=348;
  voltage=analogRead(A0);
  
  if(voltage<minvoltage){
    refill=1;
  }
  if(c_bme==2){
    if(dato1==1){
    Serial.println("S5");
    Serial.print((voltage*7.33)/345,2);
    
    }
  }
}

void sonido(){
  int nivel=0,minnivel=800;
  nivel=analogRead(A7);
  Serial.println(nivel);
  if(nivel<minnivel){
    Serial.println("A2");
  }
  //delay(100);
}

void PIR(){
  int pir1=0,pir2=0;
  pir1=digitalRead(pirizq);
  pir2=digitalRead(pirder);
  if(((activos==1)&&(pir1==HIGH))||((activos==1)&&(pir2==HIGH))){ 
  }
  else{
    if(pir1==HIGH){
    Serial.println("A1");//digitalWrite(led1, HIGH); 
   }
    else{activos=0;}
    
    if(pir2==HIGH){
    Serial.println("1");//digitalWrite(led2, HIGH);
   }
    else{ activos=0;}
  }
  if((pir1==HIGH)||(pir2==HIGH)){
     activos=1; 
  }
}
