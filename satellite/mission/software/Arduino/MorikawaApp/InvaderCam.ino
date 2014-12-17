/*
**      ARTSAT Morikawa Application
**
**      Original Copyright (C) 2013 - 2014 HORIGUCHI Junshi.
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
**      InvaderCam.ino
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

static unsigned long invadercam_seek;
static CameraType invadercam_type;
static bool invadercam_archive;

void InvaderCam_setup(void)
{
    String parse;
    String temp;
    CameraParam param;
    int b;
    int e;
    TSTError error(TSTERROR_OK);
    
    invadercam_seek = 0;
    invadercam_type = CAMERA_LIMIT;
    invadercam_archive = false;
    error = __debug__(Morikawa.writeSharedMemory(0, &invadercam_seek, sizeof(invadercam_seek)), 1);
    if (error == TSTERROR_OK) {
        error = __debug__(Morikawa.setFlashROMEraseMode(true), 2);
        if (error == TSTERROR_OK) {
            error = __debug__(Morikawa.getParamCamera(&param), 3);
            if (error == TSTERROR_OK) {
                if (param.size < lengthof(param.data)) {
                    param.data[param.size] = '\0';
                    parse = reinterpret_cast<char const*>(param.data);
                    parse.trim();
                    parse.toLowerCase();
                    parse = " " + parse + " ";
                    b = parse.indexOf(" ");
                    while (b != -1) {
                        e = parse.indexOf(" ", b + 1);
                        if (e != -1) {
                            temp = parse.substring(b + 1, e);
                            if (temp.compareTo("raw-vga") == 0) {
                                invadercam_type = CAMERA_RAW_VGA;
                            }
                            else if (temp.compareTo("raw-qvga") == 0) {
                                invadercam_type = CAMERA_RAW_QVGA;
                            }
                            else if (temp.compareTo("raw-qcif") == 0) {
                                invadercam_type = CAMERA_RAW_QCIF;
                            }
                            else if (temp.compareTo("rgb565-qvga") == 0) {
                                invadercam_type = CAMERA_RGB565_QVGA;
                            }
                            else if (temp.compareTo("rgb565-qqvga") == 0) {
                                invadercam_type = CAMERA_RGB565_QQVGA;
                            }
                            else if (temp.compareTo("rgb565-qqcif") == 0) {
                                invadercam_type = CAMERA_RGB565_QQCIF;
                            }
                            else if (temp.compareTo("yuv422-qvga") == 0) {
                                invadercam_type = CAMERA_YUV422_QVGA;
                            }
                            else if (temp.compareTo("yuv422-qqvga") == 0) {
                                invadercam_type = CAMERA_YUV422_QQVGA;
                            }
                            else if (temp.compareTo("yuv422-qqcif") == 0) {
                                invadercam_type = CAMERA_YUV422_QQCIF;
                            }
                            else if (temp.compareTo("archive") == 0) {
                                invadercam_archive = true;
                            }
                        }
                        b = e;
                    }
                    if (error == TSTERROR_OK) {
                        if (invadercam_type == CAMERA_LIMIT) {
                            error = __debug__(TSTERROR_INVALID_PARAM, 4);
                        }
                    }
                }
                else {
                    error = __debug__(TSTERROR_INVALID_PARAM, 5);
                }
            }
        }
    }
    if (error != TSTERROR_OK) {
        Morikawa.shutdown();
    }
    return;
}

void InvaderCam_loop(void)
{
    static TSTError (TSTMorikawa::*const s_read[2])(unsigned long, void*, unsigned int, unsigned int*) = {
        &TSTMorikawa::readFlashROM, &TSTMorikawa::readFRAM
    };
    unsigned long size;
    unsigned long output;
    unsigned long work;
    unsigned char temp[128];
    unsigned int skip;
    unsigned int actual;
    unsigned long i;
    unsigned long j;
    TSTError error(TSTERROR_OK);
    
    error = __debug__(Morikawa.snapshotCamera(invadercam_type, STORAGE_FLASHROM, 0, Morikawa.getSizeFlashROM(), &size), 101);
    if (error == TSTERROR_OK) {
        if (invadercam_archive) {
            error = __debug__(Morikawa.freezeFastLZ(STORAGE_FLASHROM, 0, size, STORAGE_NONE, 0, 0, STORAGE_NONE, 0, 0, &output), 102);
            if (error == TSTERROR_OK) {
                error = __debug__(Morikawa.freezeFastLZ(STORAGE_FLASHROM, 0, size, STORAGE_FRAM, 0, output, STORAGE_NONE, 0, 0, &work), 103);
                if (error == TSTERROR_OK) {
                    error = __debug__(Morikawa.freezeFastLZ(STORAGE_FLASHROM, 0, size, STORAGE_FRAM, 0, output, STORAGE_FRAM, output, work, &size), 104);
                }
            }
        }
        if (error == TSTERROR_OK) {
            skip = sizeof(invadercam_seek);
            for (i = 0, j = 0; i < size && error == TSTERROR_OK; ) {
                memset(temp, 0, sizeof(temp));
                error = __debug__((Morikawa.*s_read[invadercam_archive])(i, &temp[skip], min(size - i, sizeof(temp) - skip), &actual), 105);
                if (error == TSTERROR_OK) {
                    i += actual;
                    error = __debug__(Morikawa.writeSharedMemory(j, temp, sizeof(temp), &actual), 106);
                    if (error == TSTERROR_OK) {
                        j += actual;
                        skip = 0;
                    }
                }
            }
            if (error == TSTERROR_OK) {
                invadercam_seek += size;
                error = __debug__(Morikawa.writeSharedMemory(0, &invadercam_seek, sizeof(invadercam_seek)), 107);
            }
        }
    }
    Morikawa.shutdown();
    return;
}
