void serial_output() {
  Serial.print(F("{ "));

  Serial.print(F("\"time\":")); Serial.print(rtc_watch.Unix); Serial.print(F(", "));

  Serial.print(F("\"temperature\":{"));
  Serial.print(F("\"aht\":\"")); Serial.print(air_aht_temperature.temperature); Serial.print(F("\","));
  Serial.print(F("\"scd\":\"")); Serial.print(air_scd_temperature); Serial.print(F("\","));
  Serial.print(F("\"bme\":\"")); Serial.print(air_bme_temperature); Serial.print(F("\""));
  Serial.print(F("}, "));
  
  Serial.print(F("\"humidity\":{"));
  Serial.print(F("\"aht\":\"")); Serial.print(air_aht_humidity.relative_humidity); Serial.print(F("\","));
  Serial.print(F("\"scd\":\"")); Serial.print(air_scd_humidity); Serial.print(F("\","));
  Serial.print(F("\"bme\":\"")); Serial.print(air_bme_humidity); Serial.print(F("\""));
  Serial.print(F("}, "));

  Serial.print(F("\"co2\":")); Serial.print(air_scd_co2); Serial.print(F(", "));

  Serial.print(F("\"pm2\":")); Serial.print(air_pm2); Serial.print(F(", "));

  Serial.print(F("\"pressure\":{"));
  Serial.print(F("\"pa\":\"")); Serial.print(air_bme_pressure_pa); Serial.print(F("\","));
  Serial.print(F("\"mm\":")); Serial.print(air_bme_pressure_mm);
  Serial.print(F("}"));

  Serial.println(F(" }"));

  return;
}