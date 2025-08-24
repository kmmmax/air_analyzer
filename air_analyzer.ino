//Подключение библиотек
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
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
#define I2C_SPEED 1000

#define LCD_ADDRESS 0x27
#define LCD_SIZE_X 20
#define LCD_SIZE_Y 4

#define TMR1_INTERVAL_MS 30000
#define TMR2_INTERVAL_MS 300000
#define TMR3_INTERVAL_MS 1000


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

#define TEMPERATURE_OFFSET 0.922f
#define HUMIDITY_OFFSET 1.13f


//Создаем объекты устройств
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_SIZE_X, LCD_SIZE_Y);  //LCD экран
iarduino_RTC rtc_watch(RTC_DS3231);                          //Часы реального времени
SensirionI2cScd4x co2_sensor;                                //Датчик CO2
Adafruit_AHTX0 aht10;                                        //Датчик температуры и влажности
BME280I2C bme280;                                            //Датчик атмосферного давления


//Глобальные переменные проекта
uint32_t cur_millis = 0;

uint32_t tmr1_prev_millis = 0;
uint32_t tmr2_prev_millis = 0;
uint32_t tmr3_prev_millis = 0;

bool co2_dataReady = false;
int16_t co2_dataStatus = 0;
uint16_t air_scd_co2 = 0;
float air_scd_temperature = 0.0;
float air_scd_humidity = 0.0;

uint16_t air_pm2 = 0;

float air_bme_temperature = 0;
float air_bme_humidity = 0;
float air_bme_pressure_pa = 0;
uint16_t air_bme_pressure_mm = 0;

uint32_t rain_measures[RAIN_MEASURES];

sensors_event_t air_aht_temperature;
sensors_event_t air_aht_humidity;

bool yellow_banner_temperature = false;
bool red_banner_humidity = false;

bool yellow_banner_co2 = false;
bool red_banner_co2 = false;

bool yellow_banner_pm2 = false;
bool red_banner_pm2 = false;

uint8_t uparrow[8] = { 0x4, 0xe, 0x15, 0x4, 0x4, 0x4, 0x0 };
uint8_t downarrow[8] = { 0x0, 0x4, 0x4, 0x4, 0x15, 0xe, 0x4 };

const char okMessage[4] = "Ok!";
const char alert[2] = "!";


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
  Serial.println(F("###"));
  Serial.println(F("### Air Analyzer"));

  //Стартуем шину I2C со скоростью I2C_SPEED
  Wire.begin();
  delay(100);
  Wire.setClock(I2C_SPEED);
  delay(100);

  //Инициализируем LCD-экран и выводим первую строчку
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, uparrow);
  lcd.createChar(1, downarrow);
  lcd.clear();
  lcd.home();
  lcd.print(F("Device Loading:"));
  lcd.cursor_off();
  lcd.blink_on();

  //Подключаем модуль точного времени
  lcd.setCursor(0, 2);
  lcd.print(F("                    "));
  lcd.setCursor(1, 2);
  lcd.print(F("RTC Watch: "));
  Serial.print(F("### RTC Watch wakeUp ... "));
  rtc_watch.begin(&Wire);
  delay(100);
  rtc_watch.gettime();
  Serial.println(okMessage);
  lcd.print(okMessage);
  delay(1000);

  //Подключаем датчик CO2
  lcd.setCursor(0, 2);
  lcd.print(F("                    "));
  lcd.setCursor(1, 2);
  lcd.print(F("CO2 Sensor: "));
  Serial.print(F("### CO2 Sensor wakeUp ... "));
  co2_sensor.begin(Wire, SCD41_I2C_ADDR_62);
  delay(100);
  while (co2_sensor.wakeUp() != 0) {
    delay(1000);
  }
  Serial.println(okMessage);

  Serial.print(F("### CO2 sensor stopPeriodicMeasurement ... "));
  delay(100);
  while (co2_sensor.stopPeriodicMeasurement() != 0) {
    delay(1000);
  }
  Serial.println(okMessage);

  Serial.print(F("### CO2 sensor reInit ... "));
  delay(100);
  while (co2_sensor.reinit() != 0) {
    delay(1000);
  }
  Serial.println(okMessage);

  Serial.print(F("### CO2 sensor startPeriodicMeasurement ... "));
  delay(100);
  while (co2_sensor.startPeriodicMeasurement() != 0) {
    delay(1000);
  }
  Serial.println(okMessage);

  Serial.print(F("### CO2 sensor getDataReadyStatus ... "));
  delay(100);
  while (co2_sensor.getDataReadyStatus(co2_dataReady) != 0) {
    delay(1000);
  }
  Serial.println(okMessage);

  Serial.print(F("### CO2 sensor dataReady ... "));
  delay(100);
  while (!co2_dataReady) {
    delay(1000);
    co2_sensor.getDataReadyStatus(co2_dataReady);
  }
  Serial.println(okMessage);
  lcd.print(okMessage);
  delay(1000);

  //Подключаем датчик AHT (температура и влажность)
  lcd.setCursor(0, 2);
  lcd.print(F("                    "));
  lcd.setCursor(1, 2);
  lcd.print(F("AHT Sensor: "));
  Serial.print(F("### AHT Sensor ... "));
  delay(100);
  while (!aht10.begin()) {
    delay(1000);
  }
  Serial.println(okMessage);
  lcd.print(okMessage);
  delay(1000);

  //Подключаем датчик атмосферного давления
  lcd.setCursor(0, 2);
  lcd.print(F("                    "));
  lcd.setCursor(1, 2);
  lcd.print(F("BME Sensor: "));
  Serial.print(F("### BME sensor wakeUp ... "));
  delay(100);
  while (!bme280.begin()) {
    delay(1000);
  }
  Serial.println(okMessage);
  lcd.print(okMessage);
  delay(1000);

  lcd.setCursor(0, 2);
  lcd.print(F("                    "));
  lcd.setCursor(1, 2);
  lcd.print(F("Device ready!"));
  lcd.cursor_off();
  lcd.blink_off();

  //Посветим светофором
  red_led_on();
  delay(1000);
  yellow_led_on();
  delay(1000);
  green_led_on();
  delay(1000);
  all_leds_off();
  delay(1000);

  lcd.clear();
  lcd_datetime_output();
  lcd_template_output();
}


void loop() {
  static bool tmr1_first = true;
  static bool tmr2_first = true;
  static bool tmr3_first = true;

  delay(1);
  cur_millis = millis();

  //Программный таймер для обновления данных часов
  if ((cur_millis - tmr3_prev_millis >= TMR3_INTERVAL_MS) || tmr3_first) {
    tmr3_prev_millis = cur_millis;
    if (tmr3_first) {
      tmr3_first = false;
    }
    lcd_datetime_output();
  }

  //Программный таймер для обновления данных атмосферного давления
  if ((cur_millis - tmr2_prev_millis >= TMR2_INTERVAL_MS) || tmr2_first) {
    tmr2_prev_millis = cur_millis;

    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    bme280.read(air_bme_pressure_pa, air_bme_temperature, air_bme_humidity, tempUnit, presUnit);
    air_bme_pressure_mm = air_bme_pressure_pa / 133.3f;

    co2_sensor.setAmbientPressure(air_bme_pressure_pa);

    if (tmr2_first) {
      tmr2_first = false;
      for (uint8_t i = 0; i < RAIN_MEASURES; i++) {
        rain_measures[i] = air_bme_pressure_pa;
      }
    } else {
      for (uint8_t i = 0; i < (RAIN_MEASURES - 1); i++) {
        rain_measures[i] = rain_measures[i + 1];
      }
      rain_measures[RAIN_MEASURES - 1] = air_bme_pressure_pa;
    }
  }

  //Программный таймер для обновления данных сенсоров
  if ((cur_millis - tmr1_prev_millis >= TMR1_INTERVAL_MS) || tmr1_first) {
    tmr1_prev_millis = cur_millis;
    if (tmr1_first) {
      tmr1_first = false;
    }

    //Получаем данные температуры и влажности и делаем поправку на погрешность датчика
    if (!aht10.getEvent(&air_aht_humidity, &air_aht_temperature)) {
      Serial.println(F("### Wrong data from AHT sensor !!!"));
      air_aht_temperature.temperature = 0.0;
      air_aht_humidity.relative_humidity = 0.0;
    } else {
      air_aht_temperature.temperature *= TEMPERATURE_OFFSET;
      air_aht_humidity.relative_humidity *= HUMIDITY_OFFSET;
    }

    //Получаем данные CO2
    co2_dataStatus = co2_sensor.getDataReadyStatus(co2_dataReady);
    if (co2_dataStatus != 0 or !co2_dataReady) {
      Serial.println(F("### Wrong readyStatus from CO2 sensor !!!"));
      air_scd_co2 = 8888;
      air_scd_temperature = 0;
      air_scd_humidity = 0;
    } else {
      delay(100);
      co2_dataStatus = co2_sensor.readMeasurement(air_scd_co2, air_scd_temperature, air_scd_humidity);
      if (co2_dataStatus != 0) {
        Serial.println(F("### Wrong data from CO2 sensor !!!"));
        air_scd_co2 = 9999;
        air_scd_temperature = 0;
        air_scd_humidity = 0;
      }
    }

    //Получаем данные о пыли
    air_pm2 = getDustDensity();

    //Выводим обновленные данные в последовательный порт
    serial_output();

    //Зажигаем светофор
    set_led_color();

    //Выводим обновленные данные на LCD-экран
    lcd_values_output();
  }
}
