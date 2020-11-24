
int IFD = A0;               // IR Fotodiodo
int IR = 2;            // Infrarojo
int ambienteIR;                // luz ambiente
int obstaculoIR;               // si hay algún obstaculo
int valor[10];                // variable para guardar los valores
int distancia;
int ciclo;
int readIR;
void setup(){
  Serial.begin(9600);      
  pinMode(IR,OUTPUT);  
  digitalWrite(IR,LOW);
}

void loop(){
int readIR(int ciclo);
{
  for(int x=0;x<ciclo;x++){
    digitalWrite(IR,LOW);     
    delay(1);                        
    ambienteIR = analogRead(IFD);   
    digitalWrite(IR,HIGH);    
    delay(1);                        
    obstaculoIR = analogRead(IFD);  
    valor[x] = ambienteIR-obstaculoIR; 
  }
  
  for(int x=0;x<ciclo;x++){          
    distancia+=valor[x];
  }
  return(distancia/ciclo);           
distancia = readIR(5);       
  Serial.println(distancia);
}
}
