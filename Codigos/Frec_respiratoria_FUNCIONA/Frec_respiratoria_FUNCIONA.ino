#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int IRpin = A0;               // IR photodiode on analog pin A0
int IRemitter = 2;            // IR emitter LED on digital pin 2
//long ambientIR;                // variable to store the IR coming from the ambient
float obstacleIR;               // variable to store the IR coming from the object
float value[10];                // variable to store the IR values
float distance;                 // variable that will tell if there is an obstacle or not
int ciclos = 0;                  // almacena el numero de ciclos por minuto

void setup(){
  Serial.begin(9600);         // initializing Serial monitor
  pinMode(IRemitter,OUTPUT);  // IR emitter LED on digital pin 2
  digitalWrite(IRemitter,LOW);// setup IR LED as off

  lcd.begin(16, 2);
}

void loop(){
  //digitalWrite(IRemitter,LOW);     //turning the IR LEDs off to read the IR coming from the ambient
  //delay(1000);                      // minimum delay necessary to read values
  //ambientIR = analogRead(IRpin);   // storing IR coming from the ambient
  //delay(500);                      // minimum delay necessary to read values
  //distance = readIR(5.0);       // calling the function that will read the distance and passing the "accuracy" to it
  //Serial.println(distance);   // writing the read value on Serial monitor
  ciclos = contarCiclos ();   // Llama a la funcion que contara el total de ciclos por minuto
  
  if (ciclos < 12 || ciclos > 22){
  lcd.setCursor(10, 0);
  lcd.print("RIESGO");
  delay(500);
  lcd.setCursor(10, 0);
  lcd.print("      ");
  }
  
  Serial.print("Num de ciclos: ");
  Serial.println(ciclos);   // writing the read value on Serial monitor
  
  lcd.setCursor(5, 1);
  lcd.print("      ");

  lcd.setCursor(5, 1);
  lcd.print(ciclos);
  
  delay(600000);

}

float readIR(float times){
  int n = int(times);
  for(int x=0;x<times;x++){
    digitalWrite(IRemitter,HIGH);    //turning the IR LEDs on to read the IR coming from the obstacle
    delay(60);                      // minimum delay necessary to read values
    obstacleIR = analogRead(IRpin);  // storing IR coming from the obstacle
    value[x] = obstacleIR; // calculating changes in IR values and storing it for future average
  }
  
  for(int x=0;x<times;x++){          // calculating the average based on the "accuracy"
    distance+=value[x];
  }
  return(distance/times);            // return the final value
}



int contarCiclos(){
  unsigned long tiempoInicial = 0; // almacena el valor del tiempo inicial
  unsigned long tiempoActual = 0;         // almacenara el tiempo actual
  unsigned long tiempoTranscurrido = 0;   // almacenara la diferencia el tiempo necesario para controlar la topa de ciclos
  
  int numPico = -1;
  float senal[]= {0.0,0.0,0.0};
  float varAnt=0.0;
  float var=0.0;
  float distSum = 0.0;
  float distSumAnt = 0.0;

  while (tiempoTranscurrido<= 30000){
    //var = readIR(5.0);
    distSum = readIR(5.0); // almacena el promedio de cada nueva lectura del sensor
    var = distSum - distSumAnt; // arreglo para evitar la suma de valores
    Serial.println(var);
    if (var != varAnt){
        senal[0] = senal [1]; senal[1]=senal[2];
        senal[2] = var;
    }
    if (senal[1]>senal[0]&&senal[1]>senal[2]){
        numPico = numPico+1;
        Serial.println(numPico);
    }
    if (numPico==0){
      tiempoInicial = millis();
    }
  distSumAnt = distSum; //
    varAnt=var;
    
    tiempoActual=millis();          // Captura el tiempo transcurrido hasta este momento
    tiempoTranscurrido = tiempoActual - tiempoInicial;
    }
  return (numPico);
}
