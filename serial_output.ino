void serial_output() {
  Serial.print(rtc_watch.Unix); Serial.print(";");
  Serial.print(air_aht_temperature.temperature); Serial.print(";");
  Serial.print(air_aht_humidity.relative_humidity); Serial.print(";");
  Serial.print(air_scd_c02); Serial.print(";");
  Serial.print(air_pm2); Serial.print(";");
  Serial.println(air_bme_pressure);

  return;
}