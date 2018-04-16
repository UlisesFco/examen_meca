#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <Arduino.h>


#include<Servo.h>
#include<Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 24
#define DHTTYPE DHT11
 
DHT dht(DHTPIN, DHTTYPE);
Servo servo;  

int pos = 0; 
int l = 0;

ros::NodeHandle nh;

std_msgs::Float32MultiArray msg_sensor;
ros::Publisher pub_sensor("sensor",&msg_sensor);


void setup() {
  pinMode(8,OUTPUT);
  pinMode(A11, INPUT);
  
  servo.attach(8); 
  servo.write(0); 
  Serial.begin(9600);
  
  dht.begin();
  
  nh.initNode();
  
  //msg_sensor.layout.dim.push_back(std_msgs::MultiArrayDimension *);
  msg_sensor.layout.dim[0].size = 3;
  msg_sensor.layout.dim[0].stride = 1;
  msg_sensor.layout.dim[0].label = "datos_sensor";
  msg_sensor.layout.dim_length = 1;
  msg_sensor.data_length = 3;

  nh.advertise(pub_sensor);
}
int fotoRes(){
  l = 100000/analogRead(A11);
  return l;
}

float leerTemp(){
  float temp = dht.readTemperature();
  return temp;
}

float leerHumedad(){
  float hum = dht.readHumidity();
  return hum;
}

//bool criterio(float temp, float luz, float agua){
//  bool res = false;
//  if(luz < 150 && temp > 25){
//    res = true;
//  }
//  else if(luz < 150 && agua < 50){
//    res = true;
//  }
//  else if (temp > 25 && agua < 50){
//    res = true;
//  }
//  return res;
//}

/* Hay tres variales: calor, humedad y luz
si dos se cumplen el servo se mantiene estático, 
si no busca un lugar mejor*/
void loop() {
  delay(2500);
  float l = fotoRes();
  float hum = leerHumedad();
  float temp = leerTemp();

  float datos[3] ={hum,temp,l};

  Serial.print("hum ");
  Serial.print(hum);
  Serial.print(" luz ");
  Serial.print(l);
  Serial.print(" temp ");
  Serial.print(temp);
  Serial.println();
  
 if(pos < 360){
  if(criterio(temp,l,hum) == false){
      servo.write(pos);
      pos+=1;              
      delay(20);                       
  }
   else
      delay(200);
  }else{
    pos = 0;
    servo.write(pos);
    delay(20);
  }
msg_sensor.data[0] = datos[0];
msg_sensor.data[1] = datos[1];
msg_sensor.data[2] = datos[2];

pub_sensor.publish(&msg_sensor);

nh.spinOnce();
delay(100);
}
