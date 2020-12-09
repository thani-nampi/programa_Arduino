#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int val;

void setup() 
{
  Serial.begin(9600);
  pinMode(A0,INPUT);
  
  lcd.begin(16, 2);
  
  lcd.setCursor(2, 0);
  lcd.print("Temperatura");
  
  lcd.setCursor(2, 1);
  lcd.print("Thani_nampi");
  delay(2000);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Temp");
  lcd.setCursor(12, 1);
  lcd.print("°C");
}
void loop() 
{
  val = analogRead(A0);
  float mv = (val/1024.0)*5000;
  float temp = mv/10;
  
  if (temp < 36 || temp > 37){
  lcd.setCursor(10, 0);
  lcd.print("RIESGO");
  delay(500);
  lcd.setCursor(10, 0);
  lcd.print("      ");
  }
    
 
  lcd.setCursor(5, 1);
  lcd.print(temp);
 
 }
