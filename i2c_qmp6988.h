#ifndef i2c_qmp6988_h
#define i2c_qmp6988_h

#include <M5Unified.h>
#include <M5UnitUnified.h>
#include <M5UnitUnifiedENV.h>

#include "NmeaXDR.h"
#include "Nmea0183Msg.h"

m5::unit::UnitSHT30 i2c_qmp6988_sensor;
bool i2c_qmp6988_found = false;

void i2c_qmp6988_report() {
  if (i2c_qmp6988_sensor.updated()) {
    gen_nmea0183_xdr("$BBXDR,C,%.2f,C,TEMP_QMP6988", i2c_qmp6988_sensor.temperature());    // C
    gen_nmea0183_xdr("$BBXDR,P,%.2f,P,PRES_QMP6988", i2c_qmp6988_sensor.pressure());       // Pa
  }
}

void i2c_qmp6988_try_init() {
  for (int i = 0; i < 3; i++) {
    i2c_qmp6988_found = i2c_qmp6988_sensor.begin(&Wire, QMP6988_SLAVE_ADDRESS_L, G2, G1, 100000U);
    if (i2c_qmp6988_found) {
      break;
    }
    delay(20);
  }
  if (i2c_qmp6988_found) {
    gen_nmea0183_msg("$BBTXT,01,01,01,ENVIRONMENT found qmp6988 sensor at address=0x%s", String(QMP6988_SLAVE_ADDRESS_L, HEX).c_str());
    app.onRepeat(5000, []() {
      i2c_qmp6988_report();
    });
  }
}

#endif