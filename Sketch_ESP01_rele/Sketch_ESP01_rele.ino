// Inclusao de biblioteca
#include "config.h"
//Instanciando Objetos
AdafruitIO_Feed *bomba = io.feed("bombaIO");
//Declaracao de constantes
#define GPIO_RELE 0
//Declaracao de variavel
unsigned int statusBomba = 0;

void setup() {
  Serial.begin(9600);
  pinMode(GPIO_RELE, OUTPUT);  
  io.connect();
  bomba->onMessage(mensagemRecebidaBomba);//se esta recendo dados do portal Adafruit
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500); 
  }  
  Serial.println("Conectado");
  bomba->get();
}
void loop() { 
  io.run(); 
}
//Implemento de funcao
void mensagemRecebidaBomba(AdafruitIO_Data *data) {
  statusBomba = data->toInt();
  digitalWrite(GPIO_RELE, statusBomba); 
}
