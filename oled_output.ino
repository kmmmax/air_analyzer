void oled_rain_output() {
  oled.clear();

  oled.setCursor(7, 0);
  oled.print(now_date);

  oled.setCursorXY(35, 28);
  oled.print("mm.Hg: ");
  oled.print(air_bme_pressure * 0.0075f, 0);

  oled.setCursorXY(37, 42);
  oled.print("Rain: ");
  oled.print(estimateRain());
  oled.print("%");

  oled.fastLineH(16, 0, 127);
  oled.fastLineH(63, 0, 127);
  oled.fastLineV(0, 16, 63);
  oled.fastLineV(127, 16, 63);

  return;
}

void oled_values_output() {
  oled.clear();

  oled.setCursor(7, 0);
  oled.print(now_date);

  oled.setCursorXY(5, 24);
  oled.print("T:");
  oled.setCursorXY(18, 24);
  oled.print(air_aht_temperature.temperature, 1);
  if (yellow_banner_temperature) {
    oled.print(" !");
  }

  oled.setCursorXY(5, 47);
  oled.print("H:");
  oled.setCursorXY(18, 47);
  oled.print(air_aht_humidity.relative_humidity, 1);
  if (red_banner_humidity) {
    oled.print(" !");
  }

  oled.setCursorXY(63, 24);
  oled.print("CO2:");
  oled.setCursorXY(88, 24);
  oled.print(air_scd_c02);
  if (yellow_banner_co2 || red_banner_co2) {
    oled.print(" !");
  }
  

  oled.setCursorXY(63, 47);
  oled.print("PM2:");
  oled.setCursorXY(88, 47);
  oled.print(air_pm2);
  if (yellow_banner_pm2 || red_banner_pm2) {
    oled.print(" !");
  }

  oled.fastLineH(16, 0, 127);
  oled.fastLineH(39, 0, 127);
  oled.fastLineH(63, 0, 127);

  oled.fastLineV(0, 16, 63);
  oled.fastLineV(58, 16, 63);
  oled.fastLineV(127, 16, 63);

  return;
}
