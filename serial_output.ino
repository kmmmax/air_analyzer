void serial_output() {
  Serial.print("{ ");
  Serial.print((String)"\"time\":" + rtc_watch.Unix + ",");
  Serial.print((String)"\"temp\":" + "\"" + air_aht_temperature.temperature + "\"" + ",");
  Serial.print((String)"\"hum\":" + "\"" + air_aht_humidity.relative_humidity + "\"" + ",");
  Serial.print((String)"\"co2\":" + air_scd_co2 + ",");
  Serial.print((String)"\"pm2\":" + air_pm2 + ",");
  Serial.print((String)"\"pa\":" + "\"" + air_bme_pressure_pa + "\"");
  Serial.println(" }");
  return;
}