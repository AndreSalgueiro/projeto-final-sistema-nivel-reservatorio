// Inclusão de biblioteca
#include "config.h"

//Instanciando objetos
AdafruitIO_Feed *nivel = io.feed("nivelIO");
AdafruitIO_Feed *controleAutomatico = io.feed("controleAutomaticoIO");
AdafruitIO_Feed *bomba = io.feed("bombaIO");

//declaracao de constantes

#define NIVEL_CHEIO 95//% 
#define NIVEL_VAZIO 40//%

//Declaração de variável
byte statusAutomatico = 0;

void setup() {
  Serial.begin(9600);
  io.connect();
  //se está recendo dados do Dashboard
  controleAutomatico->onMessage(mensagemRecebidaCtrlAutomatico);
  while (io.status() < AIO_CONNECTED) {//se conecta ao wifi e ao Dashboard da Adafruit
    Serial.print(".");
    delay(500);
  }
  Serial.println("Conectado");
  controleAutomatico->get();//após conectar, envia o último valor deste feed
}
void loop() {
  io.run();//mantem-se conectado
  if(Serial.available()){// se tem dados para ler  
    String valorRecebido = Serial.readString();//valor recebido do Arduino
    nivel->save(valorRecebido);// atualiza o dashboard da Adafruit
    if( valorRecebido.toInt() >= NIVEL_CHEIO ){//Se o reservatório está cheio
      bomba->save(LOW);
    }else if ( (valorRecebido.toInt() <= NIVEL_VAZIO ) && (statusAutomatico == HIGH ) ){               //se o reservatório está vazio e o modo automático está ligado
      bomba->save(HIGH);     
    }   
  }
}
// Implemento de funções
void mensagemRecebidaCtrlAutomatico(AdafruitIO_Data *data) {
  statusAutomatico = data->toInt();
}