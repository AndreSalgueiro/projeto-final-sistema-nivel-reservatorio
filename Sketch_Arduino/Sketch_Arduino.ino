//Inclusão de biblioteca
#include <NewPing.h>

//Definição de constantes
#define PIN_TRIG 7
#define PIN_ECHO 6

#define MAX_DISTANCIA 100
#define DELAY_LEITURA 5000
#define VOLUME_REAL 35.00

//Instanciando objetos
NewPing sonar(PIN_TRIG, PIN_ECHO, MAX_DISTANCIA);

//Declaração de variáveis
unsigned long ultimaLeitura = 0;

void setup() {
  Serial.begin(9600);
}
void loop() {
  if ( millis() - ultimaLeitura > DELAY_LEITURA) {   
    int distanciaAtual = sonar.ping_cm();
    //calcula porcentagem disponível no reservatório
    float aux = (100 / VOLUME_REAL);
    float aux2 = aux * (distanciaAtual - 25);
    int porcentagemNivel = 100 - aux2;
  //acerto para não ocorrer valores > 100 e < 0
  if(porcentagemNivel > 100){
    porcentagemNivel = 100;
  }else if(porcentagemNivel < 0){
    porcentagemNivel = 0;
  }    
    Serial.println(porcentagemNivel);
    ultimaLeitura = millis();
  }
}