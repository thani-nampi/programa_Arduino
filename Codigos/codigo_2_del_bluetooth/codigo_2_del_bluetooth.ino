#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX
 
 
char c = ' ';
 
void setup() 
{
    Serial.begin(9600);
    Serial.println("Inserta comandos");
    
    BTserial.begin(9600);  
}
 
void loop() 
{
    BTserial.println("97%"); 
    Serial.println("97%"); 
    delay(1000); 
}
