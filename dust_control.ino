//функция чтения внутреннего опорного напряжения, универсальная (для всех ардуин)
long readVcc() {
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2);             // Wait for Vref to settle
  ADCSRA |= _BV(ADSC);  // Start conversion
  while (bit_is_set(ADCSRA, ADSC))
    ;                   // measuring
  uint8_t low = ADCL;   // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH;  // unlocks both
  long result = (high << 8) | low;

  result = 1.080f * 1023 * 1000 / result;  // расчёт реального VCC
  return result;                           // возвращает VCC
}


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
  float sensorVDC = 0.0f;
  float sensorDustDensity = 5.0f;

  digitalWrite(PM2_LED_PIN, HIGH);
  delayMicroseconds(280);
  sensorADC = analogRead(PM2_AOUT_PIN);
  delayMicroseconds(20);
  digitalWrite(PM2_LED_PIN, LOW);

  sensorADC = pm2RunningValue(sensorADC);
  sensorVDC = (readVcc() / 1024) * sensorADC * 11;

  if (sensorVDC > 400) {
    sensorVDC -= 400;
    sensorDustDensity = sensorVDC * 0.2f;
  }

  return sensorDustDensity;
}
