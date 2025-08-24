uint8_t estimateRain() {

  // Приводим давление к высоте на уровне моря
  uint32_t oldPressure = rain_measures[0] + (STATION_ALTITUDE_METERS * 12);
  uint32_t curPressure = rain_measures[RAIN_MEASURES - 1] + (STATION_ALTITUDE_METERS * 12);

  uint8_t pressureStatus = 0;
  if (curPressure < 100000) {
    pressureStatus = 0;  // Очень низкое давление
  } else if (curPressure < 101300) {
    pressureStatus = 1;  // Низкое давление
  } else if (curPressure < 102000) {
    pressureStatus = 2;  // Нормальное давление
  } else {
    pressureStatus = 3;  // Высокое давление
  }

  int32_t deltaPressure = curPressure - oldPressure;
  uint8_t trendStrength = 0;
  if (deltaPressure <= -250) {
    trendStrength = 0;  // Резкое падение
  } else if (deltaPressure <= -100) {
    trendStrength = 1;  // Умеренное падение
  } else if (deltaPressure <= -30) {
    trendStrength = 2;  // Слабое падение
  } else if (deltaPressure < 30) {
    trendStrength = 3;  // Стабильно
  } else if (deltaPressure < 100) {
    trendStrength = 4;  // Слабый рост
  } else if (deltaPressure < 250) {
    trendStrength = 5;  // Умеренный рост
  } else {
    trendStrength = 6;  // Резкий рост
  }

  uint8_t forecastValue = 0;
  switch (pressureStatus) {

    case 0:                      // Очень низкое давление
      if (trendStrength <= 3) {  // Давление стабильное или понижается
        forecastValue = 4;       // Высокая вероятность осадков
      } else {                   // Давление повышается
        forecastValue = 3;       // Средняя вероятность осадков
      }
      break;

    case 1:                             // Низкое давление
      if (trendStrength <= 2) {         // Давление падает
        forecastValue = 4;              // Высокая вероятность осадков
      } else if (trendStrength <= 3) {  // Давление стабильное
        forecastValue = 3;              // Средняя вероятность осадков
      } else {                          // Давление повышается
        forecastValue = 2;              // Низкая вероятность осадков
      }
      break;

    case 2:                             // Нормальное давление
      if (trendStrength == 0) {         // Резкое падение
        forecastValue = 4;              // Высокая вероятность
      } else if (trendStrength <= 2) {  // Слабое или умеренное падение
        forecastValue = 3;              // Средняя вероятность
      } else if (trendStrength == 3) {  // Стабильное давление
        forecastValue = 0;              // Осадки без изменений
      } else {                          // Давление растет
        forecastValue = 1;              // Без осадков
      }
      break;

    case 3:                             // Высокое давление
      if (trendStrength <= 1) {         // Умеренное или резкое падение
        forecastValue = 3;              // Средняя вероятность
      } else if (trendStrength <= 2) {  // Слабое падение
        forecastValue = 2;              // Низкая вероятность
      } else {                          // Давление стабильно или растет
        forecastValue = 1;              // Без осадков
      }
      break;

    default:
      forecastValue = 0;  // Осадки без изменений
  }

  return forecastValue;
}