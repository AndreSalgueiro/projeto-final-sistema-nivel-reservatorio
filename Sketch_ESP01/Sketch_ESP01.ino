// Inclusao de biblioteca
#include "config.h"
#include <NewPing.h>
//declarando de constantes
#define GPIO_TRIG 0
#define GPIO_ECHO 2
#define MAX_DISTANCIA 100
#define DELAY_LEITURA 5000
#define VOLUME_REAL 35.00
#define NIVEL_CHEIO 95//% 
#define NIVEL_VAZIO 40//%
//Instanciando objetos
AdafruitIO_Feed *nivel = io.feed("nivelIO");
AdafruitIO_Feed *controleAutomatico = io.feed("controleAutomaticoIO");
AdafruitIO_Feed *bomba = io.feed("bombaIO");
NewPing sonar(GPIO_TRIG, GPIO_ECHO, MAX_DISTANCIA);
//Declarando de variavel
byte statusAutomatico = 0;
unsigned long ultimaLeitura = 0;

void setup() {
  Serial.begin(9600);
  io.connect();
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Conectado");
  //se escreve nesse topico vai executar a funcao assim que o Adafruito IO publicar novo dado
  controleAutomatico->onMessage(mensagemRecebidaCtrlAutomatico); 
  //apos conectar, obtem imediatamente o ultimo valor gravado deste feed e aciona a funcao mensagemRecebida...parar obter o ultomo valor publicado
  controleAutomatico->get();
}
void loop() {
  io.run();//mantem-se conectado
  if ( millis() - ultimaLeitura > DELAY_LEITURA) {   
    int distanciaAtual = sonar.ping_cm();
    //calcula porcentagem disponivel no reservatorio
    float aux = (100 / VOLUME_REAL);
    float aux2 = aux * (distanciaAtual - 25);
    int porcentagemNivel = 100 - aux2;
    //ajuste para nao ocorrer valores > 100 e < 0
    if(porcentagemNivel > 100){
      porcentagemNivel = 100;
    }else if(porcentagemNivel < 0){
      porcentagemNivel = 0;
    }  
    nivel->save(porcentagemNivel);//publica no topico de nivel
    if( porcentagemNivel >= NIVEL_CHEIO ){//Se o reservatorio esta cheio
      bomba->save(LOW);//publica no topico da bomba    
    }else if ((porcentagemNivel <= NIVEL_VAZIO) && (statusAutomatico == HIGH )){//se o reservatorio esta vazio e o modo automatico esta ligado
      bomba->save(HIGH);//publica no topico da bomba      
    } 
    ultimaLeitura = millis();
  }  
}
//Implemento de funcao
void mensagemRecebidaCtrlAutomatico(AdafruitIO_Data *data) {
  statusAutomatico = data->toInt(); //obtem dados do topico controle automatico bomba
}