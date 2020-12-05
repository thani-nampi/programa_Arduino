# 1 "C:\\Users\\User\\Desktop\\1\\Autodesk_Inventor_Professional_2021\\Autodesk_Inventor_Professional_2021.ino"
# 2 "C:\\Users\\User\\Desktop\\1\\Autodesk_Inventor_Professional_2021\\Autodesk_Inventor_Professional_2021.ino" 2
# 3 "C:\\Users\\User\\Desktop\\1\\Autodesk_Inventor_Professional_2021\\Autodesk_Inventor_Professional_2021.ino" 2





SoftwareSerial BT(0,1); // BLuetooth puertos
int state;


void setup()
{
    // Bluetooth
    Serial.begin(9600);
    Serial.println("listo");
    BT.begin(9600);
    // declaraciones del sensor de temperatura
    Wire.begin();
    Serial.begin(9600);
    Wire.beginTransmission(0x40 /*temperatura*/);
    Wire.endTransmission();

}
void loop()
{
  unsigned int data[2]; // aca empieza lo de temperatura
  Wire.beginTransmission(0x40 /*temperatura*/);
  Wire.write(0xF5);
  Wire.endTransmission();

  Wire.requestFrom(0x40 /*temperatura*/, 2);
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  float humedad = ((data[0] * 256.0) + data[1]);
  humedad = ((125 * humedad) / 65536.0) - 6;

  Wire.beginTransmission(0x40 /*temperatura*/);
  Wire.write(0xF3);
  Wire.endTransmission();

  Wire.requestFrom(0x40 /*temperatura*/, 2);
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  float temp = ((data[0] * 256.0) + data[1]); // CON ESTO SE CONVIERTE LA DATA
  float cTemp = ((175.72 * temp) / 65536.0) - 46.85;
  float TempC = (0.04615 * sin(cTemp - 3.1415926535897932384626433832795) - 0.0006727 * pow(cTemp - 10, 2) + 36.97); // convertir a temperatura corporal

  // SALIDAS
  Serial.write("Temperatura Corporal : ");
  Serial.print(TempC);
  Serial.println(" °");


    if(Serial.available()) // Si llega un dato por el monitor serial se envía al puerto BT
    state = Serial.read();

  {
     BT.write(Serial.read());
  }


    if(BT.available()) // Si llega un dato por el puerto BT se envía al monitor serial
  {
    Serial.write(BT.read());
  }



}
