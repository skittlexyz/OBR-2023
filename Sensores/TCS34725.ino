#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Inicializa com os valores padrÃµes(int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
  Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

void setup() 
{
  Serial.begin(9600);

  if (tcs.begin())//Se conseguirmos iniciar o sensor significa que ele estÃ¡ conectado 
  {
    Serial.println("Sensor encontrado");
  } 
  else//caso contrÃ¡rio ele nao estÃ¡ conectado 
  {
    Serial.println("Sensor nÃ£o encontrado, cheque suas conecÃ§Ãµes.");
    while (1);
  }

}

void loop() 
{
  uint16_t r, g, b, c;

  tcs.getRawData(&r, &g, &b, &c);//Pega os valores "crus" do sensor referentes ao Vermelho(r), Verde(g), Azul(b) e da Claridade(c).

  //Agora vamos printar Os valores referentes a cada cor.
  Serial.print("Vermelho : "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("Verde    : "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("Azul     : "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("Claridade: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
}
