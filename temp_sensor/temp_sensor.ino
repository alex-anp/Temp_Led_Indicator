
/*
  Светодиодный индикатор температуры

  Разработан для отображения температуры двигателя мопеда на приборной панели с помощью 2х цветного светодиода.

  Отображает темперетуру вспышками диода или постоянным цветом диода
  
  Не горит - мотор холодный, ниже прогревочной тимпературы
  Горит зеленым - прогревочная температура
  Мигает красным - высокая темперетура, выше рабочей температуры
  Горит красным - перегрев, больше максимальной температыры


                lowTemp               normTemp              warnTemp             maxTemp
  tC --------------+---------------------+----------------------+-------------------+--------------
        led_off         green_blink            green_on              red_blink          red_on

*/

#include <microDS18B20.h>

#define SENSOR_PIN 8 // Дата пин термодатчика
#define SENSOR_GRD 6 // Минус пин термодатчика
#define SENSOR_VIN 7 // Плюс пин термодатчика

#define GRN_LED_PIN 4 // Пин геленого диода
#define RED_LED_PIN 5 // Пин красного диода
#define GRD_LED_PIN 3 // Пин земли диода, минус

#define maxTemp   125  // и больше - Предельная температура (диод горит красным)
#define warnTemp  100  // и больше - Опасная температура (диод мигает красным)
#define normTemp  40   // и больше - Рабочая температура (диод горит зеленым)
#define lowTemp   1    // и больше - Прогревочная температура (диод мигает зеленым)

MicroDS18B20 <SENSOR_PIN> sensor;

float temp = 0.00; // Переменная для хранения температуры

void setup() {
  Serial.begin(9600);

  // Инициализация пинов диода
  pinMode(GRN_LED_PIN, OUTPUT); 
  pinMode(RED_LED_PIN, OUTPUT); 
  pinMode(GRD_LED_PIN, OUTPUT); 
  digitalWrite(GRD_LED_PIN, LOW);

  // Инициализация пинов термодатчика
  pinMode(SENSOR_GRD, OUTPUT); 
  digitalWrite(SENSOR_GRD, LOW);
  pinMode(SENSOR_VIN, OUTPUT); 
  digitalWrite(SENSOR_VIN, HIGH);
  
  // ждем стабилизации чипа DS18B20
  delay(1000);
  
  led_blink(RED_LED_PIN, 1);
  delay(500);
  led_blink(GRN_LED_PIN, 2);
  
  delay(2000);
  
}

void loop() {
  // запрос температуры  
  sensor.requestTemp();
  temp = sensor.getTemp();
  
  Serial.println(temp);

  show_temp();
  
  delay(1000);  
}

// Функция отображения температуры 
void show_temp(){
  if (temp <= lowTemp){ led_off(); } 
  else if (temp <= normTemp){ led_off(); led_blink(GRN_LED_PIN, 2); }
  else if (temp <= warnTemp){ green_on(); delay(1000); }
  else if (temp <= maxTemp){ led_off(); led_blink(RED_LED_PIN, 2); }
  else { red_on(); delay(1000); }
}

// Функция мигания диодом
void led_blink(byte led_pin, byte times){
  for (byte i=0; i< times; i++){
    digitalWrite(led_pin, HIGH);
    delay (100);
    digitalWrite(led_pin, LOW);
    delay (200);
  }
}

// Включает красный
void red_on(){
  digitalWrite(GRN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);  
}

// Включает Зеленый
void green_on(){
  digitalWrite(GRN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);  
}

// Выключает диод
void led_off(){
  digitalWrite(GRN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
}
