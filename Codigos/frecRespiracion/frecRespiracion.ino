// Simple Proximity Sensor using Infrared
// Description: Measure the distance to an obstacle using infrared light emitted by IR LED and 
//   read the value with a IR photodiode. The accuracy is not perfect, but works great 
//   with minor projects.
// Author: Ricardo Ouvina
// Date: 01/10/2012
// Version: 1.0

int IRpin = A0;               // IR photodiode on analog pin A0
int IRemitter = 2;            // IR emitter LED on digital pin 2
float ambientIR;                // variable to store the IR coming from the ambient
float obstacleIR;               // variable to store the IR coming from the object
float value[10];                // variable to store the IR values
float distance;                 // distancia promedio calculada
unsigned long tiempoInicial = 0; // almacena el valor del tiempo inicial
int ciclos = 0;                  // almacena el numero de ciclos por minuto

void setup(){
  Serial.begin(9600);         // initializing Serial monitor
  pinMode(IRemitter,OUTPUT);  // IR emitter LED on digital pin 2
  digitalWrite(IRemitter,LOW);// setup IR LED as off
  pinMode(11,OUTPUT);         // buzzer in digital pin 11
}

void loop(){                  // Definir flag. Sale en 0, volver a 1 cuando imprima el valor.
  tiempoInicial = millis();   // Captura el tiempo transcurrido hasta este momento
  ciclos = contarCiclos ();   // Llama a la funcion que contara el total de ciclos por minuto
  //Serial.println(ciclos);   // writing the read value on Serial monitor
}

float readIR(float times){           // se define times como float para que pueda dividir distances += value [x] 
  int veces = (int) times;           // se convierte a entero times para que puede ser usado como flag sin problemas
  for(int x=0;x<veces;x++){
    digitalWrite(IRemitter,LOW);     //turning the IR LEDs off to read the IR coming from the ambient
    delay(1);                        // minimum delay necessary to read values
    ambientIR = analogRead(IRpin);   // storing IR coming from the ambient
    digitalWrite(IRemitter,HIGH);    //turning the IR LEDs on to read the IR coming from the obstacle
    delay(1);                        // minimum delay necessary to read values
    obstacleIR = analogRead(IRpin);  // storing IR coming from the obstacle
    value[x] = ambientIR-obstacleIR; // calculating changes in IR values and storing it for future average
  }
  
  for(int x=0;x<veces;x++){          // calculating the average based on the "accuracy"
    distance+=value[x];
  }
  return(distance/times);            // return the final value = distancia promedio
}

int contarCiclos (){
  unsigned long tiempoActual = 0;         // almacenara el tiempo actual
  unsigned long tiempoTranscurrido = 0;   // almacenara la diferencia el tiempo necesario para controlar la topa de ciclos
  float distancia;                        // almacenara la distancia calculada
  float distanciaInicial;
  float distanciaAnterior;
  float variacionInicial;           
  float variacion;

  int flag = 1;
  while (tiempoTranscurrido<= 60000){
    distancia = readIR(5.0);        // calling the function that will read the distance and passing the "accuracy" to it
    if (tiempoTranscurrido==0) {
      distanciaInicial= distancia;
      distanciaAnterior = distancia;
      }
    else{
        variacion=distancia-distanciaAnterior;
        distanciaAnterior = distancia;
        if (flag==1){
            variacionInicial = variacion;
            flag = 0;
        }
        else{
            if (distancia == distanciaInicial){
                if (variacion*variacionInicial>0){
                    ciclos = ciclos +1;
                }
            }
        }
    }
    tiempoActual=millis();          // Captura el tiempo transcurrido hasta este momento
    tiempoTranscurrido = tiempoActual - tiempoInicial;
  }

  flag = 1;
  return ciclos;
}
