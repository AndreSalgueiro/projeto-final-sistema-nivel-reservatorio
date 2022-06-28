//Adafruit IO Config
#define IO_USERNAME  "<usuário Adafruit IO>"
#define IO_KEY       "<chave Adafruit IO>"

//WIFI
#include "AdafruitIO_WiFi.h"
#define WIFI_SSID   "<usuario Wi-fi>"
#define WIFI_PASS   "<senha Wi-fi>"

//Instanciando objetos
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);