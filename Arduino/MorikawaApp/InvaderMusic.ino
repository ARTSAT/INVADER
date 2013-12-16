void InvaderMusic_setup(void)
{
  return;
}

void InvaderMusic_loop(void)
{
  unsigned char raw;
  double freq;
  int duration; 

  // set tempo
  duration = 1000;

  // group A : Gyro and Magnet
  Morikawa.getTelemetryGyro(GYRO_X, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  Morikawa.getTelemetryGyro(GYRO_Y, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  Morikawa.getTelemetryGyro(GYRO_Z, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  Morikawa.getTelemetryMagnet(MAGNET_X, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  Morikawa.getTelemetryMagnet(MAGNET_Y, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  Morikawa.getTelemetryMagnet(MAGNET_Z, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  // Interval
  Morikawa.playFrequency(FREQUENCY_REST, duration*4.0);

  // group B : Current
  Morikawa.getTelemetryCurrent(CURRENT_BUS, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  Morikawa.getTelemetryCurrent(CURRENT_BATTERY, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  Morikawa.getTelemetryCurrent(CURRENT_SOLAR, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  Morikawa.getTelemetryCurrent(CURRENT_POWERCPU, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  Morikawa.getTelemetryCurrent(CURRENT_MAINCPU, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  Morikawa.getTelemetryCurrent(CURRENT_MISSIONCPU, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  // Interval
  Morikawa.playFrequency(FREQUENCY_REST, duration*4.0);

  // group C : Tempature

  Morikawa.getTelemetryTemperature(TEMPERATURE_BATTERY1, &raw);
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration/3.0);
  Morikawa.getTelemetryTemperature(TEMPERATURE_BATTERY2, &raw);
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration/3.0);
  Morikawa.getTelemetryTemperature(TEMPERATURE_BATTERY3, &raw);
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration/3.0);

  Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_PX, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_MX, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration);

  Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_PY1, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration/2.0);
  Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_PY2, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration/2.0);

  Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_MY1, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration/2.0);
  Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_MY2, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration/2.0);

  Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_PZ1, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration/2.0);
  Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_PZ2, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration/2.0);

  Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_MZ1, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration/2.0);
  Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_MZ2, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration/2.0);


  Morikawa.getTelemetryTemperature(TEMPERATURE_MAINCPU, &raw);        
  freq = raw2freq(raw);
  Morikawa.playFrequency(freq, duration*3.0);

  return;
}

float raw2freq(int raw) {

  float freq, base, range;

  base = 110.0;
  range = 64.0;

  freq = base * pow(2.0, (float)raw/range);
  return freq;

}

