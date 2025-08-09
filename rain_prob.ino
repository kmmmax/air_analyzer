int estimateRain() {

  uint32_t pressureSum = 0;
  for (uint8_t i = 0; i < RAIN_MEASURES; i++) {
    pressureSum += rain_measures[i];
  }

  float avgPressure = pressureSum / RAIN_MEASURES;
  float avgSeaPressure = avgPressure + (STATION_ALT_METERS * 12.01);

  float deltaPressure = rain_measures[RAIN_MEASURES - 1] - rain_measures[0];

  uint8_t probTrend = 0;
  if (deltaPressure < -300.0f) {
    probTrend = 80;
  } else if (deltaPressure < -150.0f) {
    probTrend = 60;
  } else if (deltaPressure < 50.0f) {
    probTrend = 40;
  } else if (deltaPressure < 100.0f) {
    probTrend = 30;
  } else {
    probTrend = 20;
  }

  uint8_t probPressure = 0;
  if (avgSeaPressure < 100000) {
    probPressure = 90;
  } else if (avgSeaPressure < 101000) {
    probPressure = 70;
  } else if (avgSeaPressure < 101500) {
    probPressure = 50;
  } else if (avgSeaPressure < 102000) {
    probPressure = 30;
  } else {
    probPressure = 20;
  }

  float combinedProb = 0.4f * probTrend + 0.6f * probPressure;
  if (combinedProb < 0.0f) {
    combinedProb = 0.0f;
  }
  if (combinedProb > 100.0f) {
    combinedProb = 100.0f;
  }

  return (int)(combinedProb + 0.5f);
}