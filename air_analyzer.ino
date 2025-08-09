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
#define I2C_SPEED 10000

#define OLED_ADDRESS 0x3c
#define OLED_TIMEOUT_MS 180000

#define TMR1_INTERVAL_MS 60000
#define TMR2_INTERVAL_MS 300000

#define NEED_SET_RTC false

#define TEMP_GREEN_MIN 18
#define TEMP_GREEN_MAX 30

#define HUMIDITY_GREEN_MIN 30
#define HUMIDITY_GREEN_MAX 60

#define CO2_GREEN_MAX 800
#define CO2_YELLOW_MAX 1000

#define PM2_GREEN_MAX 115
#define PM2_YELLOW_MAX 150

#define RAIN_MEASURES 12
#define STATION_ALT_METERS 125


//Создаем объекты устройств
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled(OLED_ADDRESS);  //OLED экран
iarduino_RTC rtc_watch(RTC_DS3231);                            //Часы реального времени
SensirionI2cScd4x co2_sensor;                                  //Датчик CO2
Adafruit_AHTX0 aht10;                                          //Датчик температуры и влажности
BME280I2C bme280;                                              //Датчик атмосферного давления


//Глобальные переменные проекта
bool oled_active = true;
uint32_t cur_millis = 0;

bool button_pressed = false;

uint32_t last_action = 0;
uint32_t tmr1_prev_millis = 0;
uint32_t tmr2_prev_millis = 0;

String now_date = "";

bool co2_dataReady = false;
int16_t co2_dataStatus = 0;
uint16_t air_scd_c02 = 0;
float air_scd_temperature = 0.0;
float air_scd_humidity = 0.0;

uint16_t air_pm2 = 0;

float air_bme_temperature = 0;
float air_bme_humidity = 0;
float air_bme_pressure = 0;

uint32_t rain_measures[RAIN_MEASURES];

sensors_event_t air_aht_temperature;
sensors_event_t air_aht_humidity;

bool yellow_banner_temperature = false;

bool red_banner_humidity = false;

bool yellow_banner_co2 = false;
bool red_banner_co2 = false;

bool yellow_banner_pm2 = false;
bool red_banner_pm2 = false;


void setup() {

  //Подготавливаем пины светофора и подаем на них логический ноль
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  //Подготавливаем пин кнопки и включаем подтяжку к логической единице
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //Подготавливаем пины для датчика пыли и подаем логический ноль на пин датчика
  pinMode(PM2_LED_PIN, OUTPUT);
  digitalWrite(PM2_LED_PIN, LOW);
  pinMode(PM2_AOUT_PIN, INPUT);

  //Пауза для стабилизации схемы
  delay(1000);

  //Стартуем последовательный порт обмена
  Serial.begin(SERIAL_PORT_SPEED);
  while (!Serial) {
    delay(100);
  }
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
  oled.print("Device Loading:");

  //Подключаем модуль точного времени
  oled.setCursor(2, 2);
  oled.print("- RTC Watch ...");
  Serial.print("### RTC Watch wakeUp ...");
  rtc_watch.begin(&Wire);
  delay(100);
  now_date = rtc_watch.gettime("d M Y | H:i");
  Serial.print(" OK! ");
  Serial.println((String) "(" + now_date + ")");
  oled.print(" OK!");

  //Подключаем датчик CO2
  oled.setCursor(2, 3);
  oled.print("- CO2 Sensor ...");
  Serial.print("### CO2 Sensor wakeUp ...");
  co2_sensor.begin(Wire, SCD41_I2C_ADDR_62);
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
  oled.setCursor(2, 4);
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
  oled.setCursor(2, 5);
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
  oled.setCursor(0, 7);
  oled.print("Load Complete!");
  delay(2000);
  oled.clear();

  Serial.println("### UNIX_TIME; TEMPERATURE; HUMIDITY; CO2; PM2; PRESSURE");
}


void loop() {
  static bool tmr1_first = true;
  static bool tmr2_first = true;

  cur_millis = millis();
  delay(10);

  //Программный таймер для обновления данных атмосферного давления
  if ((cur_millis - tmr2_prev_millis >= TMR2_INTERVAL_MS) || tmr2_first) {
    tmr2_prev_millis = cur_millis;

    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    bme280.read(air_bme_pressure, air_bme_temperature, air_bme_humidity, tempUnit, presUnit);
    co2_sensor.setAmbientPressure(air_bme_pressure);

    if (tmr2_first) {
      tmr2_first = false;
      for (uint8_t i = 0; i < RAIN_MEASURES; i++) {
        rain_measures[i] = air_bme_pressure;
      }
    } else {
      for (uint8_t i = 0; i < (RAIN_MEASURES - 1); i++) {
        rain_measures[i] = rain_measures[i + 1];
      }
      rain_measures[RAIN_MEASURES - 1] = air_bme_pressure;
    }
  }

  //Программный таймер для обновления данных сенсоров
  if (((cur_millis - tmr1_prev_millis >= TMR1_INTERVAL_MS) || tmr1_first) && !button_pressed) {
    tmr1_prev_millis = cur_millis;
    if (tmr1_first) {
      tmr1_first = false;
    }

    //Получаем данные температуры и влажности
    if (!aht10.getEvent(&air_aht_humidity, &air_aht_temperature)) {
      Serial.println("### Wrong data from AHT sensor !!!");
      air_aht_temperature.temperature = 0.0;
      air_aht_humidity.relative_humidity = 0.0;
    }

    //Получаем данные CO2
    co2_dataStatus = co2_sensor.getDataReadyStatus(co2_dataReady);
    if (co2_dataStatus != 0 or !co2_dataReady) {
      Serial.println("### Wrong readyStatus from CO2 sensor !!!");
      air_scd_c02 = 8888;
      air_scd_temperature = 0;
      air_scd_humidity = 0;
    } else {
      delay(100);
      co2_dataStatus = co2_sensor.readMeasurement(air_scd_c02, air_scd_temperature, air_scd_humidity);
      if (co2_dataStatus != 0) {
        Serial.println("### Wrong data from CO2 sensor !!!");
        air_scd_c02 = 9999;
        air_scd_temperature = 0;
        air_scd_humidity = 0;
      }
    }

    //Получаем данные о пыли
    air_pm2 = getDustDensity();

    //Получаем текущее время с часов
    now_date = rtc_watch.gettime("d M Y | H:i");

    //Выводим обновленные данные в последовательный порт
    serial_output();

    //Зажигаем светофор
    set_led_color();

    //Выводим обновленные данные на OLED-экран
    oled_values_output();
  }

  //Таймаут для отключения OLED
  if ((millis() - last_action >= OLED_TIMEOUT_MS) && oled_active) {
    Serial.println("### Activity timeout. Set OLED off...");
    oled.setPower(false);
    oled_active = false;
  }

  //Кнопка отпущена
  if (digitalRead(BUTTON_PIN) && button_pressed) {
    button_pressed = false;
    oled_values_output();
  }

  //Кнопка нажата
  if (!digitalRead(BUTTON_PIN) && !button_pressed) {
    button_pressed = true;
    last_action = millis();
    if (oled_active) {
      oled_rain_output();
    } else {
      Serial.println("### Activity detected. Set OLED on...");
      oled_active = true;
      oled.setPower(true);
      oled_values_output();
    }
  }
}
