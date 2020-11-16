#include "MAX30100_PulseOximeter.h"
#include <SoftwareSerial.h>
#include <Wire.h>

#define Addr 0x40 //temperatura
#define periodo_informacion_MS     1000 //max30100


PulseOximeter pox;
uint32_t reporte_final = 0;

SoftwareSerial miBT(10,11); // BLuetooth puertos

char dato_max30100 ;


void setup()
{
    // Bluetooth
    Serial.begin(9600);
    Serial.println("listo");
    miBT.begin(38400); 
    // declaraciones del sensor de temperatura
    Wire.begin();
    Serial.begin(9600);
    Wire.beginTransmission(Addr);
    Wire.endTransmission();
    delay(300);
    // declaraciones del max      
    Serial.begin(115200);
    Serial.print("INICIANDO MEDIDAS");
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
     
}
void loop()
{
  unsigned int data[2]; // aca empieza lo de temperatura
  Wire.beginTransmission(Addr);
  Wire.write(0xF5);
  Wire.endTransmission();
  delay(500);
    
  Wire.requestFrom(Addr, 2);
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  
  float humedad  = ((data[0] * 256.0) + data[1]);
  humedad = ((125 * humedad) / 65536.0) - 6;

  Wire.beginTransmission(Addr);
  Wire.write(0xF3);
  Wire.endTransmission();
  delay(500);
  
  Wire.requestFrom(Addr, 2);
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  float temp  = ((data[0] * 256.0) + data[1]); // CON ESTO SE CONVIERTE LA DATA
  float cTemp = ((175.72 * temp) / 65536.0) - 46.85;
  float fTemp = cTemp * 1.8 + 32;
   
  // SALIDAS
  Serial.print("Humeda Relativa : ");
  Serial.print(humedad);
  Serial.println(" % RH");
  Serial.print("Temperatura en Celsius : ");
  Serial.print(cTemp);
  Serial.println(" C");
  delay(500);
    
if (Serial.available())
  miBT.write(Serial.read()); // lee Arduino y envia a BT
  
   if (millis() - reporte_final > periodo_informacion_MS) {
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
 
        reporte_final = millis();
   if (Serial.available())
   miBT.write(Serial.read()); // lee Arduino y envia a 
    }
}
