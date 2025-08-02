//Подключение библиотек
#include <Wire.h>
#include <GyverOLED.h>
#include <iarduino_RTC.h>
#include <SensirionI2cScd4x.h>
#include <Adafruit_AHTX0.h>
#include <BME280I2C.h>


//Задаем макросы для удобства
#define RED_LED 2
#define YELLOW_LED 3
#define GREEN_LED 4

#define BUTTON_PIN 9

#define PM2_LED_PIN A1
#define PM2_AOUT_PIN A0

#define SERIAL_PORT_SPEED 9600
#define I2C_SPEED 100000

#define OLED_ADDRESS 0x3c


//Создаем объекты устройств
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled(OLED_ADDRESS);   //OLED экран
iarduino_RTC rtc_watch(RTC_DS3231);                             //Часы реального времени
SensirionI2cScd4x co2_sensor;                                   //Датчик CO2
Adafruit_AHTX0 aht10;                                           //Датчик температуры и влажности
BME280I2C bme280;                                               //Датчик атмосферного давления



//Глобальные переменные проекта
String now_date = "";

bool co2_dataReady = false;
uint16_t air_scd_c02 = 0;

sensors_event_t air_aht_temperature;
sensors_event_t air_aht_humidity;


void setup() {

  //Подготавливаем пины светофора и подаем на них логический ноль
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  //Подготавливаем пин кнпоки и включаем подтяжку к логической единице
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //Подготавливаем пины для датчика пыли и подаем логический ноль на пин датчика
  pinMode(PM2_LED_PIN, OUTPUT);
  digitalWrite(PM2_LED_PIN, LOW);
  pinMode(PM2_AOUT_PIN, INPUT);

  //Пауза для стабилизации схемы
  delay(1000);

  //Стартуем последовательный порт обмена
  Serial.begin(SERIAL_PORT_SPEED);
  Serial.println("###");
  Serial.println("### Air Analyzer");

  //Стартуем шину I2C со скоростью I2C_SPEED
  Wire.begin();
  delay(100);
  Wire.setClock(I2C_SPEED);
  delay(100);

  //Инициализируем OLED-экран и выводим первую строчку
  oled.init();
  oled.clear();
  oled.home();
  oled.print("Air Analyzer Loading:");
  
  //Подключаем модуль точного времени
  oled.setCursor(0, 2);
  oled.print("- RTC Watch ...");
  Serial.print("### RTC Watch wakeUp ...");
  while (rtc_watch.begin(&Wire)) {
    delay(1000);
    Serial.print(".");
  }
  now_date = rtc_watch.gettime("d M Y | H:i");
  Serial.print(" OK! ");
  Serial.println((String) "(" + now_date + ")");
  oled.print(" OK!");

  //Подключаем датчик CO2
  oled.setCursor(0,3);
  oled.print("- CO2 Sensor ...");
  co2_sensor.begin(Wire, SCD41_I2C_ADDR_62);

  Serial.print("### CO2 Sensor wakeUp ...");
  delay(100);
  while (co2_sensor.wakeUp() != 0) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" OK!");

  Serial.print("### CO2 sensor stopPeriodicMeasurement ...");
  delay(100);
  while (co2_sensor.stopPeriodicMeasurement() != 0) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" OK!");

  Serial.print("### CO2 sensor reInit ...");
  delay(100);
  while (co2_sensor.reinit() != 0) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" OK!");

  Serial.print("### CO2 sensor startPeriodicMeasurement ...");
  delay(100);
  while (co2_sensor.startPeriodicMeasurement() != 0) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" OK!");

  Serial.print("### CO2 sensor getDataReadyStatus ...");
  delay(100);
  while (co2_sensor.getDataReadyStatus(co2_dataReady) != 0) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" OK!");

  Serial.print("### CO2 sensor dataReady ...");
  delay(100);
  while (!co2_dataReady) {
    delay(1000);
    co2_sensor.getDataReadyStatus(co2_dataReady);
    Serial.print(".");
  }
  Serial.println(" OK!");

  oled.print(" OK!");

  //Подключаем датчик AHT (температура и влажность)
  oled.setCursor(0,4);
  oled.print("- AHT Sensor ...");
  Serial.print("### AHT Sensor ...");
  delay(100);
  while (!aht10.begin()) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" OK!");
  oled.print(" OK!");

  //Подключаем датчик атмосферного давления
  oled.setCursor(0,5);
  oled.print("- BME Sensor ...");
  Serial.print("### BME sensor wakeUp ...");
  delay(100);
  while (!bme280.begin()) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" OK!");
  oled.print("OK!");

  //Посветим светофором
  red_led_on();
  delay(500);
  yellow_led_on();
  delay(500);
  green_led_on();
  delay(500);
  all_leds_off();

  //Заканчиваем подготовку
  oled.setCursor(0,7);
  oled.print("Load Complete!");
  delay(2000);
  oled.clear();
}


void loop() {

}