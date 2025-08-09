int pm2RunningValue(int m) {
  const int _buff_max = 10;
  static bool flag_first = true;
  static int _buff[_buff_max], sum = 0;
  int i;

  if (flag_first) {
    flag_first = false;
    for (i = 0; i < _buff_max; i++) {
      _buff[i] = m;
      sum += m;
    }
    return m;
  } else {
    sum -= _buff[0];
    for (i = 0; i < (_buff_max - 1); i++) {
      _buff[i] = _buff[i + 1];
    }
    _buff[_buff_max - 1] = m;
    sum += _buff[_buff_max - 1];

    i = sum / _buff_max;
    return i;
  }
}


float getDustDensity() {
  uint16_t sensorADC = 0;
  float sensorVDC = 0.0;
  float sensorDustDensity = 5.0;

  digitalWrite(PM2_LED_PIN, HIGH);
  delayMicroseconds(280);
  sensorADC = analogRead(PM2_AOUT_PIN);
  delayMicroseconds(20);
  digitalWrite(PM2_LED_PIN, LOW);

  sensorADC = pm2RunningValue(sensorADC);
  sensorVDC = (5000 / 1024) * sensorADC * 11;

  if (sensorVDC > 400) {
    sensorVDC -= 400;
    sensorDustDensity = sensorVDC * 0.2;
  }

  return sensorDustDensity;
}
