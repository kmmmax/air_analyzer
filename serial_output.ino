void serial_output() {

  Serial.print("{ ");

  Serial.print((String)"\"timestamp\":" + rtc_watch.Unix + ",");

  Serial.print((String)"\"aht_temp\":" + "\"" + air_aht_temperature.temperature + "\"" + ",");
  Serial.print((String)"\"bme_temp\":" + "\"" + air_bme_temperature + "\"" + ",");
  Serial.print((String)"\"scd_temp\":" + "\"" + air_scd_temperature + "\"" + ",");

  Serial.print((String)"\"aht_hum\":" + "\"" + air_aht_humidity.relative_humidity + "\"" + ",");
  Serial.print((String)"\"bme_hum\":" + "\"" + air_bme_humidity + "\"" + ",");
  Serial.print((String)"\"scd_hum\":" + "\"" + air_scd_humidity + "\"" + ",");

  Serial.print((String)"\"co2\":" + air_scd_co2 + ",");

  Serial.print((String)"\"pm2\":" + air_pm2 + ",");

  Serial.println(" }");

/*
  Serial.print(air_bme_pressure_pa); Serial.print(";");
  Serial.println(air_bme_pressure_hg);
*/

  return;
}