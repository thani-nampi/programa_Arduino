#include <Arduino.h>
#line 1 "C:\\Users\\User\\CODIGO_Q_FUNCA\\CODIGO_Q_FUNCA.ino"
#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial  
SoftwareSerial BT(0,1);    // Definimos los pines RX y TX del Arduino conectados al Bluetooth
int state = 0;
#line 4 "C:\\Users\\User\\CODIGO_Q_FUNCA\\CODIGO_Q_FUNCA.ino"
void setup();
#line 10 "C:\\Users\\User\\CODIGO_Q_FUNCA\\CODIGO_Q_FUNCA.ino"
void loop();
#line 4 "C:\\Users\\User\\CODIGO_Q_FUNCA\\CODIGO_Q_FUNCA.ino"
void setup()
{
  BT.begin(9600);       // Inicializamos el puerto serie BT (Para Modo AT 2)
  Serial.begin(9600);   // Inicializamos  el puerto serie  
}
 
void loop()
{ 
  Serial.print("E");
  if(Serial.available()>0)  // Si llega un dato por el monitor serial se envía al puerto BT

  if(BT.available()>0) // Si llega un dato por el puerto BT se envía al monitor serial
  state = Serial.read();
  {
    if(state == 'E')
    {
    Serial.write(BT.read());
    Serial.write("olamundo");
    }
  }
}

