void lcd_template_output() {
  lcd.setCursor(0, 1);
  lcd.print(F("T:"));

  lcd.setCursor(10, 1);
  lcd.print(F("Rh:"));

  lcd.setCursor(0, 2);
  lcd.print(F("CO2:"));

  lcd.setCursor(10, 2);
  lcd.print(F("Pm2:"));

  lcd.setCursor(0, 3);
  lcd.print(F("Hg:"));

  lcd.setCursor(10, 3);
  lcd.print(F("Rain:"));

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
  lcd.print(F("        "));
  lcd.setCursor(2, 1);
  lcd.print(air_aht_temperature.temperature, 1);
  lcd.print((char)223);
  lcd.print(F("C"));
  if (yellow_banner_temperature) {
    lcd.print(alert);
  }

  lcd.setCursor(13, 1);
  lcd.print(F("       "));
  lcd.setCursor(13, 1);
  lcd.print(air_aht_humidity.relative_humidity, 1);
  lcd.print(F("%"));
  if (yellow_banner_humidity) {
    lcd.print(alert);
  }

  lcd.setCursor(4, 2);
  lcd.print(F("      "));
  lcd.setCursor(4, 2);
  lcd.print(air_scd_co2);
  if (yellow_banner_co2 || red_banner_co2) {
    lcd.print(alert);
  }

  lcd.setCursor(14, 2);
  lcd.print(F("      "));
  lcd.setCursor(14, 2);
  lcd.print(air_pm2);
  if (yellow_banner_pm2 || red_banner_pm2) {
    lcd.print(alert);
  }

  lcd.setCursor(3, 3);
  lcd.print(F("       "));
  lcd.setCursor(3, 3);
  lcd.print(air_bme_pressure_mm);
  lcd.print(F("mm"));
  if (rain_measures[RAIN_MEASURES - 1] < rain_measures[RAIN_MEASURES - 2]) {
    lcd.write(1);
  } else if (rain_measures[RAIN_MEASURES - 1] > rain_measures[RAIN_MEASURES - 2]) {
    lcd.write(0);
  } else {
    lcd.print(F(" "));
  }

  lcd.setCursor(15, 3);
  lcd.print(F("     "));
  lcd.setCursor(15, 3);
  switch (weather_forecast) {
    case 0:  // Без изменений
      lcd.print(F("Curr"));
      break;
    case 1:  // Без осадков
      lcd.print(F("No"));
      break;
    case 2:  // Низкая вероятность
      lcd.print(F("Low"));
      break;
    case 3:  // Средняя вероятность
      lcd.print(F("Mid"));
      break;
    case 4:  // Высокая вероятность
      lcd.print(F("High"));
      break;
    default:
      lcd.print(F("Curr"));
  }

  return;
}
