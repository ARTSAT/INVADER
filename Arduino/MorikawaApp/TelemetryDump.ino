/*
**      ARTSAT Morikawa Application
**
**      Original Copyright (C) 2013 - 2013 HORIGUCHI Junshi.
**                                          http://iridium.jp/
**                                          zap00365@nifty.com
**      Portions Copyright (C) <year> <author>
**                                          <website>
**                                          <e-mail>
**      All rights reserved.
**
**      Version     Arduino
**      Website     http://artsat.jp/
**      E-mail      info@artsat.jp
**
**      This source code is for Arduino IDE.
**      Arduino 1.0.5
**
**      TelemetryDump.ino
**
**      ------------------------------------------------------------------------
**
**      GNU GENERAL PUBLIC LICENSE (GPLv3)
**
**      This program is free software: you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation, either version 3 of the License,
**      or (at your option) any later version.
**      This program is distributed in the hope that it will be useful,
**      but WITHOUT ANY WARRANTY; without even the implied warranty of
**      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
**      See the GNU General Public License for more details.
**      You should have received a copy of the GNU General Public License
**      along with this program. If not, see <http://www.gnu.org/licenses/>.
**
**      このプログラムはフリーソフトウェアです。あなたはこれをフリーソフトウェア財団によって発行された
**      GNU 一般公衆利用許諾書（バージョン 3 か、それ以降のバージョンのうちどれか）が定める条件の下で
**      再頒布または改変することができます。このプログラムは有用であることを願って頒布されますが、
**      *全くの無保証* です。商業可能性の保証や特定目的への適合性は、言外に示されたものも含め全く存在しません。
**      詳しくは GNU 一般公衆利用許諾書をご覧ください。
**      あなたはこのプログラムと共に GNU 一般公衆利用許諾書のコピーを一部受け取っているはずです。
**      もし受け取っていなければ <http://www.gnu.org/licenses/> をご覧ください。
*/

static unsigned long telemetrydump_seek;
static long telemetrydump_interval;
static long telemetrydump_limit;
static long telemetrydump_count;
static bool telemetrydump_voltage;
static bool telemetrydump_current;
static bool telemetrydump_temperature;
static bool telemetrydump_gyro;
static bool telemetrydump_magnet;

void TelemetryDump_setup(void)
{
    String parse;
    String temp;
    char param[128];
    int b;
    int e;
    TSTError error(TSTERROR_OK);
    
    telemetrydump_seek = 0;
    telemetrydump_interval = 0;
    telemetrydump_limit = 0;
    telemetrydump_count = 0;
    telemetrydump_voltage = false;
    telemetrydump_current = false;
    telemetrydump_temperature = false;
    telemetrydump_gyro = false;
    telemetrydump_magnet = false;
    error = __debug__(Morikawa.writeSharedMemory(0, &telemetrydump_seek, sizeof(telemetrydump_seek)), 1);
    if (error == TSTERROR_OK) {
        error = __debug__(Morikawa.getText(TEXT_X, param, sizeof(param)), 2);
        if (error == TSTERROR_OK) {
            parse = param;
            parse.trim();
            parse.toLowerCase();
            parse = " " + parse + " ";
            b = parse.indexOf(" ");
            while (b != -1) {
                e = parse.indexOf(" ", b + 1);
                if (e != -1) {
                    temp = parse.substring(b + 1, e);
                    if (temp.startsWith("interval=")) {
                        telemetrydump_interval = atol(&temp[9]);
                    }
                    else if (temp.startsWith("limit=")) {
                        telemetrydump_limit = atol(&temp[6]);
                    }
                    else if (temp.compareTo("voltage") == 0) {
                        telemetrydump_voltage = true;
                    }
                    else if (temp.compareTo("current") == 0) {
                        telemetrydump_current = true;
                    }
                    else if (temp.compareTo("temperature") == 0) {
                        telemetrydump_temperature = true;
                    }
                    else if (temp.compareTo("gyro") == 0) {
                        telemetrydump_gyro = true;
                    }
                    else if (temp.compareTo("magnet") == 0) {
                        telemetrydump_magnet = true;
                    }
                }
                b = e;
            }
            if (error == TSTERROR_OK) {
                if (telemetrydump_interval > 0 && telemetrydump_limit >= 0) {
                    if (!(telemetrydump_voltage || telemetrydump_current || telemetrydump_temperature || telemetrydump_gyro || telemetrydump_magnet)) {
                        telemetrydump_voltage = true;
                        telemetrydump_current = true;
                        telemetrydump_temperature = true;
                        telemetrydump_gyro = true;
                        telemetrydump_magnet = true;
                    }
                }
                else {
                    error = __debug__(TSTERROR_INVALID_PARAM, 3);
                }
            }
        }
    }
    if (error != TSTERROR_OK) {
        Morikawa.shutdown();
    }
    return;
}

void TelemetryDump_loop(void)
{
    unsigned long time;
    unsigned char dump;
    int i;
    TSTError error(TSTERROR_OK);
    
    if (telemetrydump_limit <= 0) {
        Morikawa.shutdown();
    }
    if (Morikawa.hasTelemetryUpdate()) {
        error = __debug__(Morikawa.getTelemetryTime(TIME_OBCTIME, &time), 101);
        if (error == TSTERROR_OK) {
            if (++telemetrydump_count >= telemetrydump_interval) {
                error = __debug__(Morikawa.writeSharedMemory(sizeof(telemetrydump_seek) + telemetrydump_seek, &time, sizeof(time)), 102);
                if (error == TSTERROR_OK) {
                    telemetrydump_seek += sizeof(time);
                    if (telemetrydump_voltage) {
                        for (i = 0; i < VOLTAGE_LIMIT && error == TSTERROR_OK; ++i) {
                            error = __debug__(Morikawa.getTelemetryVoltage(static_cast<VoltageType>(i), &dump), 103);
                            if (error == TSTERROR_OK) {
                                error = __debug__(Morikawa.writeSharedMemory(sizeof(telemetrydump_seek) + telemetrydump_seek, &dump, sizeof(dump)), 104);
                                if (error == TSTERROR_OK) {
                                    telemetrydump_seek += sizeof(dump);
                                }
                            }
                        }
                    }
                    if (telemetrydump_current) {
                        for (i = 0; i < CURRENT_LIMIT && error == TSTERROR_OK; ++i) {
                            error = __debug__(Morikawa.getTelemetryCurrent(static_cast<CurrentType>(i), &dump), 105);
                            if (error == TSTERROR_OK) {
                                error = __debug__(Morikawa.writeSharedMemory(sizeof(telemetrydump_seek) + telemetrydump_seek, &dump, sizeof(dump)), 106);
                                if (error == TSTERROR_OK) {
                                    telemetrydump_seek += sizeof(dump);
                                }
                            }
                        }
                    }
                    if (telemetrydump_temperature) {
                        for (i = 0; i < TEMPERATURE_LIMIT && error == TSTERROR_OK; ++i) {
                            error = __debug__(Morikawa.getTelemetryTemperature(static_cast<TemperatureType>(i), &dump), 107);
                            if (error == TSTERROR_OK) {
                                error = __debug__(Morikawa.writeSharedMemory(sizeof(telemetrydump_seek) + telemetrydump_seek, &dump, sizeof(dump)), 108);
                                if (error == TSTERROR_OK) {
                                    telemetrydump_seek += sizeof(dump);
                                }
                            }
                        }
                    }
                    if (telemetrydump_gyro) {
                        for (i = 0; i < GYRO_LIMIT && error == TSTERROR_OK; ++i) {
                            error = __debug__(Morikawa.getTelemetryGyro(static_cast<GyroType>(i), &dump), 109);
                            if (error == TSTERROR_OK) {
                                error = __debug__(Morikawa.writeSharedMemory(sizeof(telemetrydump_seek) + telemetrydump_seek, &dump, sizeof(dump)), 110);
                                if (error == TSTERROR_OK) {
                                    telemetrydump_seek += sizeof(dump);
                                }
                            }
                        }
                    }
                    if (telemetrydump_magnet) {
                        for (i = 0; i < MAGNET_LIMIT && error == TSTERROR_OK; ++i) {
                            error = __debug__(Morikawa.getTelemetryMagnet(static_cast<MagnetType>(i), &dump), 111);
                            if (error == TSTERROR_OK) {
                                error = __debug__(Morikawa.writeSharedMemory(sizeof(telemetrydump_seek) + telemetrydump_seek, &dump, sizeof(dump)), 112);
                                if (error == TSTERROR_OK) {
                                    telemetrydump_seek += sizeof(dump);
                                }
                            }
                        }
                    }
                }
                if (error == TSTERROR_OK) {
                    error = __debug__(Morikawa.writeSharedMemory(0, &telemetrydump_seek, sizeof(telemetrydump_seek)), 113);
                }
                --telemetrydump_limit;
                telemetrydump_count = 0;
            }
        }
    }
    if (error != TSTERROR_OK) {
        Morikawa.shutdown();
    }
    return;
}
