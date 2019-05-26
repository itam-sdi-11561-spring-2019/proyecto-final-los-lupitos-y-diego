#include <ros.h>
#include <PID_v1.h>
#include <std_msgs/String.h>
#include <SoftwareSerial.h>
#include <geometry_msgs/Pose2D.h>
#include <std_msgs/Int32.h>

#define E1 13  // Enable Pin for motor 2
#define I1 12     // Control pin 1 for motor 2
#define I2 11     // Control pin 2 for motor 2

#define E2 10  // Enable Pin for motor 1
#define I3 9    // Control pin 1 for motor 1
#define I4 8     // Control pin 2 for motor 1

unsigned long tiempo;
int contM1=0;
int contM2=0;
int valueAntM1 = 0;
int valueAntM2 = 0;
int valueM1=0;
int valueM2=0;

// Direccion (1 adelante, -1 atras)
int dirM1= 1;
int dirM2 = 1;

double velM1=0;
double velM2=0;

//Motor derecho
double kpM2 = 0.65;
double kiM2 = 0.7;
double kdM2 = 0.15;

//Motor izquierdo
double kpM1 = .4;
double kiM1 = .9;
double kdM1 = 0.1;

//ErrorM2
double errM2 = 0;
double sumErrorM2= 0;
double errorAntM2 = 0;

//ErrorM1
double errM1 = 0;
double sumErrorM1 = 0;
double errorAntM1 = 0;

double pidM2 = 0;
double pidM1 = 0;

//correccion vel motores
int corrM2 = 0;
int corrM1 = 0;

void callback(geometry_msgs::Pose2D &vel){
  velM2 = abs(vel.x);
  velM1 = abs(vel.y);
}

std_msgs::Int32 ok_msg;
//std_msgs::Int32 vel1;
//std_msgs::Int32 vel2;

ros::NodeHandle nh;
ros::Subscriber<geometry_msgs::Pose2D> sub("/pose_dif", &callback);
ros::Publisher pubt("/ok_msg", &ok_msg);


void setup() {
  //Inicializamos
        for (int i =8 ; i<14 ; i++)                     
          pinMode(i, OUTPUT);

        //Activamos los motores
        digitalWrite(E1, HIGH);
        digitalWrite(E2, HIGH);

        pinMode(A0, INPUT);//Inicializamos la lectura de Motor1
        pinMode(A1, INPUT);//Inicializamos la lectura de Motor2
        
        Serial.begin(9600);
        tiempo = millis();

        nh.initNode();
        nh.subscribe(sub);
        nh.advertise(pubt);
        
}



void loop()
   {   
  
      valueM1= digitalRead(A0);  
      valueM2= digitalRead(A1);
      

      if(valueM1==1 && valueAntM1==0){
        contM1= contM1+1;
      }
      valueAntM1=valueM1;

      if(valueM2==1 && valueAntM2==0){
        contM2= contM2+1;
      }
      valueAntM2=valueM2;

      
    if(millis() >= tiempo+250){
    //PID
    
    // Error es velocidad deseada - velocidad detectada en orificios/segundo
    errM2 = velM2 - 4*contM2*dirM2;
    errM1 = velM1 - 4*contM1*dirM1;

    // Error integral
    sumErrorM2 += errM2;
    sumErrorM1 += errM1;

    // PID completo
    pidM2 = errM2*kpM2 + (errM2 - errorAntM2)*kdM2 + sumErrorM2*kiM2;
    pidM1 = errM1*kpM1 + (errM1 - errorAntM1)*kdM1 + sumErrorM1*kiM1;

    // Actualizar error previo
    errorAntM2= errM2;
    errorAntM1= errM1;

    // Ajustar salida
    corrM2 = min(abs(pidM2), 255);
    corrM1= min(abs(pidM1), 255);
    
    if (pidM2 >= 0){
      analogWrite(I1, corrM2);
      analogWrite(I2, 0);
      dirM2 = 1;
    }else{
      analogWrite(I1, 0);
      analogWrite(I2, corrM2);
      dirM2 = -1;
    }
    
    if (pidM1 >= 0){
      analogWrite(I4, corrM1);
      analogWrite(I3, 0);
      dirM1 = 1;
    }else{
      analogWrite(I4, 0);
      analogWrite(I3, corrM1);
      dirM1 = -1;
    }

      contM1=0;
      contM2=0;
      tiempo= millis();
    }

      ok_msg.data = contM1;
      pubt.publish(&ok_msg);

      nh.spinOnce();
}


