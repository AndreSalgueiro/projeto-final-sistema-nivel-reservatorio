//Inclusao de biblioteca
#include "config.h"
#include <NewPing.h>
//declarando de constantes
#define GPIO_TRIG 0
#define GPIO_ECHO 2
#define MAX_DISTANCIA 150
#define DELAY_LEITURA 5000
#define DIMENSAO_EFETIVA 65.00//cm de agua para reservatorio cheio
#define NIVEL_CHEIO 100//% 
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
  //apos conectar, obtem imediatamente o ultimo valor gravado deste feed e aciona a funcao mensagemRecebida...parar obter o ultimo valor publicado
  controleAutomatico->get();
}
void loop() {
  io.run();//mantem-se conectado
  if ( millis() - ultimaLeitura > DELAY_LEITURA) { 
    int medidaNivel = calculaNivel();
    //calcula porcentagem disponivel no reservatorio
    float aux = (100 / DIMENSAO_EFETIVA);
    float aux2 = aux * (medidaNivel - 35);//o valor 35 e o desconto do sensor ate a parte mais cheia do reservatorio
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
//Implemento de funcoes
void mensagemRecebidaCtrlAutomatico(AdafruitIO_Data *data) {
  statusAutomatico = data->toInt(); //obtem dados do topico controle automatico bomba
}
int calculaNivel(){
  int resultado = 0, contador = 0;   
  while (resultado == 0 && contador < 30){
    delay(10);    
    resultado = sonar.ping_cm(); 
    contador++;
  } 
  return  resultado;
}