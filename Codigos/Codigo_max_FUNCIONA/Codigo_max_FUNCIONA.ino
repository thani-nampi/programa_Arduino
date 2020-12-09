#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int analogPin = 0;
int val = 0;
int rojo[101];
int infra[101];
float Rmayor=0;
float Imayor=0;
int val1=0;
float Rmenor=0;
float Imenor=0;
float RR=0;
float II=0;
float R=0;
float SpO2=0;

void setup()
{
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  
  lcd.begin(16, 2);
  
  lcd.setCursor(2, 0);
  lcd.print("PULSIOXIMETRO");
  
  lcd.setCursor(2, 1);
  lcd.print("Thani nampi");
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  delay(500);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Spo2");
  lcd.setCursor(12, 1);
  lcd.print("%");
   
}
void loop()
{
  digitalWrite(8, HIGH);
  delay(3);
  for (int i=0; i<101; i++)
  {
    val = analogRead(analogPin);
    rojo[i]=val;
  } 
  digitalWrite(8, LOW);
  
  for (int i=0; i<101; i++)
  {
    if (Rmayor < rojo[i])
    {
      Rmayor = rojo[i];
    }
  }
  for (int i=0; i<101; i++)
  {
    if (Rmenor > rojo[i])
    {
      Rmenor = rojo[i];
    }
  }
  digitalWrite(9, HIGH);
  delay(3);
  for (int i=0; i<101; i++)
  {
    val1 = analogRead(analogPin);
    infra[i]=val1;
  }
  digitalWrite(9, LOW);
  
  for (int i=0; i<101; i++)
  {
    if (Imayor < infra[i]);
    {
      Imayor = infra[i];
    }
  }
  for (int i=0; i<101; i++)
  {
    if (Imenor> infra[i])
    {
      Imenor = infra[i];
    }
  }
  
  // SE CALCULA R
  RR=Rmayor-Rmenor;
  II=Imayor-Imenor;
  
  R= RR/II;
  
  // SE CALCULA SPO2 A PARTIR DE LA FUNCION PARA LA CURVA EMPIRICA
  SpO2= -33.156*R;
  
  SpO2= SpO2+113.21;
  
  //se imprime SPO2 en la pantalla
  lcd.setCursor(5, 1);
  lcd.print("      ");
  
  lcd.setCursor(5, 1);
  lcd.print(SpO2);
  
  Rmayor=0;
  Rmenor=0;
  Imayor=0;
  Imenor=0;
  SpO2=0;
  R=0;
  RR=0;
  II=0;
  delay(1000);
  
}
