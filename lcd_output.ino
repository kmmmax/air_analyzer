void lcd_values_output() {
  lcd.clear();
  lcd.home();
  lcd.print(now_date);

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(air_aht_temperature.temperature, 1);
  lcd.print((char)223); lcd.print("C");
  if (yellow_banner_temperature) {
    lcd.print("!");
  }

  lcd.setCursor(10, 1);
  lcd.print("Rh:");
  lcd.print(air_aht_humidity.relative_humidity, 1);
  lcd.print("%");
  if (red_banner_humidity) {
    lcd.print("!");
  }

  lcd.setCursor(0, 2);
  lcd.print("CO2:");
  lcd.print(air_scd_c02);
  if (yellow_banner_co2 || red_banner_co2) {
    lcd.print("!");
  }
  
  lcd.setCursor(10, 2);
  lcd.print("Pm2:");
  lcd.print(air_pm2);
  if (yellow_banner_pm2 || red_banner_pm2) {
    lcd.print("!");
  }

  lcd.setCursor(0, 3);
  lcd.print("Hg:");
  lcd.print(air_bme_pressure_hg);
  lcd.print("mm");
  if (air_bme_pressure_hg < rain_measures[RAIN_MEASURES - 1]) {
  }

  lcd.setCursor(10, 3);
  lcd.print("Rain:");
  lcd.print(estimateRain());
  lcd.print("%");

  return;
}
