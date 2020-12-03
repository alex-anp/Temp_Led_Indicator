/*
  Светодиодный индикатор температуры

  Разработан для отображения температуры двигателя мопеда на приборной панели с помощью светодиода.

  Отображает темперетуру вспышками диода
  1 всп. -> 110+С 
  2 всп. -> 120+С 
  3 всп. -> 130+С 
  4 всп. -> 140+С 
  5 всп. -> 150+С 
  Больше 160 горит постоянно (можно задать в настройках прошивки)
*/

#include "max6675.h"

#define LED_PIN LED_BUILTIN // встроенный светодиод, 13 пин

#define maxTemp   150  // Предельная температура (диод горит постоянно)
#define ofsetTemp 100  // Отступ по температуре (диапазон который нам не интересен)
#define stepTemp  10   // Размер шага (количество градусов на одну вспышку диода)

int temp = 0;

// Распиновка модуля
int thermoDO  = 2;  //он же SO
int thermoCS  = 3;
int thermoCLK = 4;  //он же SCK
int vccPin    = 5;  //пин для питания
int gndPin    = 6;  //пин для земли

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() {
  
  Serial.begin(9600);
  
  pinMode(LED_PIN, OUTPUT);

  //активируем питание и землю
  pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
  pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);
  Serial.println("MAX6675 - Start");
  
  //ждем стабилизации чипа MAX
  delay(1000);
  
  led_blink(LED_PIN, 1);
  delay(750);
  led_blink(LED_PIN, 3);
  
  delay(2000);
  
}

void loop() {

  Serial.print("C = ");
  Serial.println(thermocouple.readCelsius());

  temp = thermocouple.readCelsius();

  if (temp >= maxTemp){
    digitalWrite(LED_PIN,HIGH);
  } else {
    digitalWrite(LED_PIN,LOW);
    if (temp > ofsetTemp){
      led_blink(LED_PIN, (temp - ofsetTemp)/stepTemp);
    }
  }
  
  delay(2000);

}

// Функция мигания диодом
void led_blink(byte led_pin, byte times){
  Serial.println(times);
  for (byte i=0; i< times; i++){
    digitalWrite(led_pin,HIGH);
    delay (100);
    digitalWrite(led_pin,LOW);
    delay (200);
  }
}
