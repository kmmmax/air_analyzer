void red_led_on() {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  return;
}

void yellow_led_on() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  return;
}

void green_led_on() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  return;
}

void all_leds_off() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  return;
}



//Управление светофором
void set_led_color() {

  yellow_banner_temperature = false;

  red_banner_humidity = false;

  yellow_banner_co2 = false;
  red_banner_co2 = false;

  yellow_banner_pm2 = false;
  red_banner_pm2 = false;

  if (air_aht_temperature.temperature < TEMP_GREEN_MIN or air_aht_temperature.temperature > TEMP_GREEN_MAX) {
    yellow_banner_temperature = true;
  }

  if (air_aht_humidity.relative_humidity < HUMIDITY_GREEN_MIN or air_aht_humidity.relative_humidity > HUMIDITY_GREEN_MAX) {
    red_banner_humidity = true;
  }

  if (air_scd_co2 > CO2_GREEN_MAX) {
    yellow_banner_co2 = true;
  }
  if (air_scd_co2 > CO2_YELLOW_MAX) {
    red_banner_co2 = true;
  }

  if (air_pm2 > PM2_GREEN_MAX) {
    yellow_banner_pm2 = true;
  }
  if (air_pm2 > PM2_YELLOW_MAX) {
    red_banner_pm2 = true;
  }

  if (red_banner_humidity || red_banner_co2 || red_banner_pm2) {
    red_led_on();
    return;
  }

  if (yellow_banner_temperature || yellow_banner_co2 || yellow_banner_pm2) {
    yellow_led_on();
    return;
  }

  green_led_on();

  return;
}
