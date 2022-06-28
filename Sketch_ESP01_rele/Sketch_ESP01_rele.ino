// Inclusão de biblioteca
#include "config.h"
//Instanciando Objetos
AdafruitIO_Feed *bomba = io.feed("bombaIO");
//Declaração de constantes
#define PIN_RELE 0
//Declaração de variável
unsigned int statusBomba = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_RELE, OUTPUT);  
  io.connect();
  bomba->onMessage(mensagemRecebidaBomba);//se está recendo dados do portal Adafruit
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
//Implemento de funções
void mensagemRecebidaBomba(AdafruitIO_Data *data) {
  statusBomba = data->toInt();
  digitalWrite(PIN_RELE, statusBomba); 
}
