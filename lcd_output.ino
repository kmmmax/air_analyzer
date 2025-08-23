void lcd_template_output() {
  lcd.setCursor(0, 1);
  lcd.print("T:");

  lcd.setCursor(10, 1);
  lcd.print("Rh:");

  lcd.setCursor(0, 2);
  lcd.print("CO2:");

  lcd.setCursor(10, 2);
  lcd.print("Pm2:");

  lcd.setCursor(0, 3);
  lcd.print("Hg:");

  lcd.setCursor(10, 3);
  lcd.print("Rain:");

  return;
}

void lcd_datetime_output() {
  static bool sep = true;

  lcd.home();
  lcd.print(rtc_watch.gettime("d M Y"));
  lcd.print(" | ");
  lcd.print(rtc_watch.gettime("H"));
  if (sep) {
    lcd.print(":");
  } else {
    lcd.print(" ");
  }
  lcd.print(rtc_watch.gettime("i"));

  sep = !sep;

  return;
}


void lcd_values_output() {

  lcd.setCursor(2, 1);
  lcd.print("        ");
  lcd.setCursor(2, 1);
  lcd.print(air_aht_temperature.temperature, 1);
  lcd.print((char)223);
  lcd.print("C");
  if (yellow_banner_temperature) {
    lcd.print("!");
  }

  lcd.setCursor(13, 1);
  lcd.print("       ");
  lcd.setCursor(13, 1);
  lcd.print(air_aht_humidity.relative_humidity, 1);
  lcd.print("%");
  if (red_banner_humidity) {
    lcd.print("!");
  }

  lcd.setCursor(4, 2);
  lcd.print("      ");
  lcd.setCursor(4, 2);
  lcd.print(air_scd_co2);
  if (yellow_banner_co2 || red_banner_co2) {
    lcd.print("!");
  }

  lcd.setCursor(14, 2);
  lcd.print("      ");
  lcd.setCursor(14, 2);
  lcd.print(air_pm2);
  if (yellow_banner_pm2 || red_banner_pm2) {
    lcd.print("!");
  }

  lcd.setCursor(3, 3);
  lcd.print("       ");
  lcd.setCursor(3, 3);
  lcd.print(air_bme_pressure_hg);
  lcd.print("mm");
  if (rain_measures[RAIN_MEASURES - 1] < rain_measures[RAIN_MEASURES - 2]) {
    lcd.write(1);
  } else if (rain_measures[RAIN_MEASURES - 1] > rain_measures[RAIN_MEASURES - 2]) {
    lcd.write(0);
  } else {}

  lcd.setCursor(15, 3);
  lcd.print("     ");
  lcd.setCursor(15, 3);
  lcd.print(estimateRain());
  lcd.print("%");

  return;
}
