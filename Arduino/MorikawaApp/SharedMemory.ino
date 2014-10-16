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
**      SharedMemory.ino
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

#include <utility/I2Cm.h>

#if defined(TARGET_INVADER_EM1) || defined(TARGET_INVADER_FM1)
#define ADDRESS_BOOT        (0x7FF80UL)
#define ADDRESS_TEXT_X      (0x7FE80UL)
#define ADDRESS_TEXT_Y      (0x7FE00UL)
#define ADDRESS_TEXT_Z      (0x7FD80UL)
#define ADDRESS_TEXT_DEBUG  (0x7FD00UL)
#define ADDRESS_NOTE        (0x7FC00UL)
#define ADDRESS_DIGITALKER  (0x7FB80UL)
#define ADDRESS_MORSE       (0x7FB40UL)
#define ADDRESS_CAMERA      (0x7FB00UL)
#define SECTOR_LIMIT        (8)
#elif defined(TARGET_DESPATCH_FM1)
#define ADDRESS_BOOT        (0x3FF80UL)
#define ADDRESS_TEXT_X      (0x3FE80UL)
#define ADDRESS_TEXT_Y      (0x3FE00UL)
#define ADDRESS_TEXT_Z      (0x3FD80UL)
#define ADDRESS_TEXT_DEBUG  (0x3FD00UL)
#define ADDRESS_NOTE        (0x3FC00UL)
#define ADDRESS_DIGITALKER  (0x3FB80UL)
#define ADDRESS_MORSE       (0x3FB40UL)
#define ADDRESS_CAMERA      (0x3FB00UL)
#define SECTOR_LIMIT        (4)
#else
#define ADDRESS_BOOT        (0)
#define ADDRESS_TEXT_X      (0)
#define ADDRESS_TEXT_Y      (0)
#define ADDRESS_TEXT_Z      (0)
#define ADDRESS_TEXT_DEBUG  (0)
#define ADDRESS_NOTE        (0)
#define ADDRESS_DIGITALKER  (0)
#define ADDRESS_MORSE       (0)
#define ADDRESS_CAMERA      (0)
#define SECTOR_LIMIT        (0)
#endif

static void dump_help(void);
static void dump_param(void);
static void dump_memory(long addr, unsigned char size, char const* text, void const* data);
static void writeSharedMemory(unsigned long address, void const* data, unsigned char size, bool pascal);

static char sharedmemory_buffer[280];
static unsigned int sharedmemory_index;
static unsigned long sharedmemory_addr;
static unsigned long sharedmemory_next;

void SharedMemory_setup(void)
{
    Serial.println(F(""));
    dump_param();
    Serial.println(F(""));
    
    Serial.println(F("--- General purpose ---"));
    sharedmemory_index = 0;
    sharedmemory_addr = 0;
    sharedmemory_next = 0;
    dump_help();
    Serial.print(F("> "));
    return;
}

void SharedMemory_loop(void)
{
    char rx;
    bool dump;
    unsigned char data[128];
    char temp[32];
    unsigned int size;
    int pos;
    int type;
    int hex;
    int i;
    int j;
    
    while (true) {
        if (Serial.available()) {
            rx = Serial.read();
            Serial.print(rx);
            if (sharedmemory_index < lengthof(sharedmemory_buffer)) {
                sharedmemory_buffer[sharedmemory_index] = rx;
                ++sharedmemory_index;
                if (sharedmemory_index >= 2) {
                    if (strncmp_P(&sharedmemory_buffer[sharedmemory_index - 2], PSTR("\r\n"), 2) == 0) {
                        sharedmemory_buffer[sharedmemory_index - 2] = '\0';
                        sharedmemory_index -= 2;
                        if (strcmp_P(sharedmemory_buffer, PSTR("boot")) == 0) {
                            Serial.println(F("boot application..."));
                            break;
                        }
                        else if (strcmp_P(sharedmemory_buffer, PSTR("help")) == 0) {
                            dump_help();
                        }
                        else if (strcmp_P(sharedmemory_buffer, PSTR("param")) == 0) {
                            dump_param();
                        }
                        else if (strcmp_P(sharedmemory_buffer, PSTR("size")) == 0) {
                            Serial.print(F("size : "));
                            snprintf(temp, sizeof(temp), "0x%05lX", Morikawa.getSizeSharedMemory());
                            Serial.print(temp);
                            Serial.println(F(" bytes"));
                        }
                        else if (strcmp_P(sharedmemory_buffer, PSTR("format")) == 0) {
                            Serial.print(F("format start, wait..."));
                            Morikawa.formatSharedMemory();
                            Serial.println(F("done."));
                        }
                        else if (strcmp_P(sharedmemory_buffer, PSTR("erase")) == 0) {
                            Serial.print(F("erase "));
                            Serial.print(sizeof(data));
                            Serial.println(F(" bytes."));
                            memset(data, 0xFF, sizeof(data));
                            switch (Morikawa.writeSharedMemory(sharedmemory_addr, data, sizeof(data), &size)) {
                                case TSTERROR_OK:
                                    dump_memory(sharedmemory_addr, size, NULL, NULL);
                                    sharedmemory_next = sharedmemory_addr;
                                    sharedmemory_addr += size;
                                    break;
                                case TSTERROR_INVALID_FORMAT:
                                    Serial.println(F("end of memory."));
                                    break;
                                default:
                                    Serial.println(F("erasing failed."));
                                    break;
                            }
                        }
                        else if (strstr_P(sharedmemory_buffer, PSTR("write")) == sharedmemory_buffer) {
                            pos = 5;
                            type = -1;
                            if (strstr_P(&sharedmemory_buffer[pos], PSTR(" ")) == &sharedmemory_buffer[pos]) {
                                pos += 1;
                                type = 0;
                            }
                            else if (strstr_P(&sharedmemory_buffer[pos], PSTR("-count ")) == &sharedmemory_buffer[pos]) {
                                pos += 7;
                                type = 1;
                            }
                            else if (strstr_P(&sharedmemory_buffer[pos], PSTR("-mode ")) == &sharedmemory_buffer[pos]) {
                                pos += 6;
                                type = 2;
                            }
                            else if (strstr_P(&sharedmemory_buffer[pos], PSTR("-note ")) == &sharedmemory_buffer[pos]) {
                                pos += 6;
                                type = 3;
                            }
                            else if (strstr_P(&sharedmemory_buffer[pos], PSTR("-morse ")) == &sharedmemory_buffer[pos]) {
                                pos += 7;
                                type = 4;
                            }
                            else if (strstr_P(&sharedmemory_buffer[pos], PSTR("-digi ")) == &sharedmemory_buffer[pos]) {
                                pos += 6;
                                type = 5;
                            }
                            else if (strstr_P(&sharedmemory_buffer[pos], PSTR("-cam ")) == &sharedmemory_buffer[pos]) {
                                pos += 5;
                                type = 6;
                            }
                            else if (strstr_P(&sharedmemory_buffer[pos], PSTR("-text_x ")) == &sharedmemory_buffer[pos]) {
                                pos += 8;
                                type = 7;
                            }
                            else if (strstr_P(&sharedmemory_buffer[pos], PSTR("-text_y ")) == &sharedmemory_buffer[pos]) {
                                pos += 8;
                                type = 8;
                            }
                            else if (strstr_P(&sharedmemory_buffer[pos], PSTR("-text_z ")) == &sharedmemory_buffer[pos]) {
                                pos += 8;
                                type = 9;
                            }
                            else if (strstr_P(&sharedmemory_buffer[pos], PSTR("-text_d ")) == &sharedmemory_buffer[pos]) {
                                pos += 8;
                                type = 10;
                            }
                            if (type >= 0) {
                                dump = false;
                                Serial.print(F("write : "));
                                Serial.println(&sharedmemory_buffer[pos]);
                                if (strstr_P(&sharedmemory_buffer[pos], PSTR("0x")) == &sharedmemory_buffer[pos]) {
                                    for (i = pos + 2; i < sharedmemory_index; ++i) {
                                        if (!(('0' <= sharedmemory_buffer[i] && sharedmemory_buffer[i] <= '9') ||
                                              ('a' <= sharedmemory_buffer[i] && sharedmemory_buffer[i] <= 'f') ||
                                              ('A' <= sharedmemory_buffer[i] && sharedmemory_buffer[i] <= 'F'))) {
                                            break;
                                        }
                                    }
                                    if (i >= sharedmemory_index) {
                                        if ((sharedmemory_index - pos - 2) % 2 == 0) {
                                            for (i = pos + 2, j = pos; i < sharedmemory_index; i += 2, ++j) {
                                                if (sscanf(&sharedmemory_buffer[i], "%02X", &hex) == 1) {
                                                    sharedmemory_buffer[j] = hex;
                                                }
                                                else {
                                                    break;
                                                }
                                            }
                                            if (i >= sharedmemory_index) {
                                                sharedmemory_index = j;
                                                dump = true;
                                            }
                                        }
                                    }
                                }
                                else {
                                    dump = true;
                                }
                                if (dump) {
                                    switch (type) {
                                        case 0:
                                            switch (Morikawa.writeSharedMemory(sharedmemory_addr, &sharedmemory_buffer[pos], sharedmemory_index - pos, &size)) {
                                                case TSTERROR_OK:
                                                    dump_memory(sharedmemory_addr, size, NULL, NULL);
                                                    sharedmemory_next = sharedmemory_addr;
                                                    sharedmemory_addr += size;
                                                    break;
                                                case TSTERROR_INVALID_FORMAT:
                                                    Serial.println(F("end of memory."));
                                                    break;
                                                default:
                                                    Serial.println(F("writing failed."));
                                                    break;
                                            }
                                            break;
                                        case 1:
                                            if (sharedmemory_index - pos == 1) {
                                                writeSharedMemory(ADDRESS_BOOT +  0 + 4, &sharedmemory_buffer[pos], sharedmemory_index - pos, false);
                                                writeSharedMemory(ADDRESS_BOOT + 38 + 4, &sharedmemory_buffer[pos], sharedmemory_index - pos, false);
                                                writeSharedMemory(ADDRESS_BOOT + 76 + 4, &sharedmemory_buffer[pos], sharedmemory_index - pos, false);
                                                dump_memory(ADDRESS_BOOT, sharedmemory_index - pos, NULL, NULL);
                                            }
                                            else {
                                                Serial.println(F("invalid format (must be size == 1)."));
                                            }
                                            break;
                                        case 2:
                                            if (sharedmemory_index - pos == 1) {
                                                writeSharedMemory(ADDRESS_BOOT +  0 + 5, &sharedmemory_buffer[pos], sharedmemory_index - pos, false);
                                                writeSharedMemory(ADDRESS_BOOT + 38 + 5, &sharedmemory_buffer[pos], sharedmemory_index - pos, false);
                                                writeSharedMemory(ADDRESS_BOOT + 76 + 5, &sharedmemory_buffer[pos], sharedmemory_index - pos, false);
                                                dump_memory(ADDRESS_BOOT, sharedmemory_index - pos, NULL, NULL);
                                            }
                                            else {
                                                Serial.println(F("invalid format (must be size == 1)."));
                                            }
                                            break;
                                        case 3:
                                            if (sharedmemory_index - pos < 256) {
                                                writeSharedMemory(ADDRESS_NOTE, &sharedmemory_buffer[pos], sharedmemory_index - pos, true);
                                                dump_memory(ADDRESS_NOTE, sharedmemory_index - pos, NULL, NULL);
                                            }
                                            else {
                                                Serial.println(F("invalid format (must be size < 256)."));
                                            }
                                            break;
                                        case 4:
                                            if (sharedmemory_index - pos < 64) {
                                                writeSharedMemory(ADDRESS_MORSE, &sharedmemory_buffer[pos], sharedmemory_index - pos, true);
                                                dump_memory(ADDRESS_MORSE, sharedmemory_index - pos, NULL, NULL);
                                            }
                                            else {
                                                Serial.println(F("invalid format (must be size < 64)."));
                                            }
                                            break;
                                        case 5:
                                            if (sharedmemory_index - pos < 128) {
                                                writeSharedMemory(ADDRESS_DIGITALKER, &sharedmemory_buffer[pos], sharedmemory_index - pos, true);
                                                dump_memory(ADDRESS_DIGITALKER, sharedmemory_index - pos, NULL, NULL);
                                            }
                                            else {
                                                Serial.println(F("invalid format (must be size < 128)."));
                                            }
                                            break;
                                        case 6:
                                            if (sharedmemory_index - pos < 64) {
                                                writeSharedMemory(ADDRESS_CAMERA, &sharedmemory_buffer[pos], sharedmemory_index - pos, true);
                                                dump_memory(ADDRESS_CAMERA, sharedmemory_index - pos, NULL, NULL);
                                            }
                                            else {
                                                Serial.println(F("invalid format (must be size < 64)."));
                                            }
                                            break;
                                        case 7:
                                            if (sharedmemory_index - pos < 128) {
                                                writeSharedMemory(ADDRESS_TEXT_X, &sharedmemory_buffer[pos], sharedmemory_index - pos, true);
                                                dump_memory(ADDRESS_TEXT_X, sharedmemory_index - pos, NULL, NULL);
                                            }
                                            else {
                                                Serial.println(F("invalid format (must be size < 128)."));
                                            }
                                            break;
                                        case 8:
                                            if (sharedmemory_index - pos < 128) {
                                                writeSharedMemory(ADDRESS_TEXT_Y, &sharedmemory_buffer[pos], sharedmemory_index - pos, true);
                                                dump_memory(ADDRESS_TEXT_Y, sharedmemory_index - pos, NULL, NULL);
                                            }
                                            else {
                                                Serial.println(F("invalid format (must be size < 128)."));
                                            }
                                            break;
                                        case 9:
                                            if (sharedmemory_index - pos < 128) {
                                                writeSharedMemory(ADDRESS_TEXT_Z, &sharedmemory_buffer[pos], sharedmemory_index - pos, true);
                                                dump_memory(ADDRESS_TEXT_Z, sharedmemory_index - pos, NULL, NULL);
                                            }
                                            else {
                                                Serial.println(F("invalid format (must be size < 128)."));
                                            }
                                            break;
                                        case 10:
                                            if (sharedmemory_index - pos < 128) {
                                                writeSharedMemory(ADDRESS_TEXT_DEBUG, &sharedmemory_buffer[pos], sharedmemory_index - pos, true);
                                                dump_memory(ADDRESS_TEXT_DEBUG, sharedmemory_index - pos, NULL, NULL);
                                            }
                                            else {
                                                Serial.println(F("invalid format (must be size < 128)."));
                                            }
                                            break;
                                        default:
                                            break;
                                    }
                                }
                                else {
                                    Serial.println(F("invalid format."));
                                }
                            }
                        }
                        else {
                            dump = false;
                            if (sharedmemory_index > 0) {
                                for (i = 0; i < sharedmemory_index; ++i) {
                                    if (!(('0' <= sharedmemory_buffer[i] && sharedmemory_buffer[i] <= '9') ||
                                          ('a' <= sharedmemory_buffer[i] && sharedmemory_buffer[i] <= 'f') ||
                                          ('A' <= sharedmemory_buffer[i] && sharedmemory_buffer[i] <= 'F'))) {
                                        break;
                                    }
                                }
                                if (i >= sharedmemory_index) {
                                    if (sscanf(sharedmemory_buffer, "%lX", &sharedmemory_addr) == 1) {
                                        if (sharedmemory_addr > Morikawa.getSizeSharedMemory()) {
                                            sharedmemory_addr = Morikawa.getSizeSharedMemory();
                                        }
                                        dump = true;
                                    }
                                }
                            }
                            else {
                                sharedmemory_addr = sharedmemory_next;
                                dump = true;
                            }
                            if (dump) {
                                switch (Morikawa.readSharedMemory(sharedmemory_addr, data, sizeof(data), &size)) {
                                    case TSTERROR_OK:
                                        dump_memory(sharedmemory_addr, size, NULL, data);
                                        sharedmemory_next = sharedmemory_addr + size;
                                        break;
                                    case TSTERROR_INVALID_FORMAT:
                                        Serial.println(F("end of memory."));
                                        break;
                                    default:
                                        Serial.println(F("reading failed."));
                                        break;
                                }
                            }
                        }
                        sharedmemory_index = 0;
                        Serial.print(F("> "));
                    }
                }
            }
            else {
                sharedmemory_index = 0;
                Serial.print(F("> "));
            }
        }
    }
    return;
}

static void dump_help(void)
{
    Serial.println(F("Usage : "));
    Serial.println(F("  boot\\n : continue booting"));
    Serial.println(F("  help\\n : print this information"));
    Serial.println(F("  param\\n : dump param blocks"));
    Serial.println(F("  <hex address>\\n : dump 128 bytes from inputted address"));
    Serial.println(F("  \\n : dump next 128 bytes"));
    Serial.println(F("  size\\n : print memory size"));
    Serial.println(F("  format\\n : format all"));
    Serial.println(F("  erase\\n : erase 128 bytes from current address"));
    Serial.println(F("  write <string | 0x<hex value>>\\n : write inputted string into current address"));
    Serial.println(F("  write-count <string | 0x<hex value>>\\n : write inputted string into BootCount param"));
    Serial.println(F("  write-mode <string | 0x<hex value>>\\n : write inputted string into BootMode param"));
    Serial.println(F("  write-note <string | 0x<hex value>>\\n : write inputted string into NoteParam"));
    Serial.println(F("  write-morse <string | 0x<hex value>>\\n : write inputted string into MorseParam"));
    Serial.println(F("  write-digi <string | 0x<hex value>>\\n : write inputted string into DigitalkerParam"));
    Serial.println(F("  write-cam <string | 0x<hex value>>\\n : write inputted string into CameraParam"));
    Serial.println(F("  write-text_x <string | 0x<hex value>>\\n : write inputted string into TEXT_X"));
    Serial.println(F("  write-text_y <string | 0x<hex value>>\\n : write inputted string into TEXT_Y"));
    Serial.println(F("  write-text_z <string | 0x<hex value>>\\n : write inputted string into TEXT_Z"));
    Serial.println(F("  write-text_d <string | 0x<hex value>>\\n : write inputted string into TEXT_DEBUG"));
    return;
}

static void dump_param(void)
{
    unsigned long time;
    NoteParam note;
    MorseParam morse;
    DigitalkerParam digitalker;
    CameraParam camera;
    char temp[128];
    int size;
    
    Serial.println(F("--- BootParam ---"));
    time = Morikawa.getBootTime();
    snprintf(temp, sizeof(temp), " Time : %02X %02X %02X %02X",
             static_cast<unsigned char>(time >> 24),
             static_cast<unsigned char>(time >> 16),
             static_cast<unsigned char>(time >>  8),
             static_cast<unsigned char>(time >>  0)
             );
    Serial.println(temp);
    snprintf(temp, sizeof(temp), "Count : %02X", Morikawa.getBootCount());
    Serial.println(temp);
    snprintf(temp, sizeof(temp), " Mode : %02X", Morikawa.getBootMode());
    Serial.println(temp);
    Serial.println(F(""));
    
    Serial.println(F("--- NoteParam ---"));
    if (Morikawa.getParamNote(&note) == TSTERROR_OK) {
        dump_memory(-1, note.size, NULL, note.data);
    }
    else {
        Serial.println(F("NoteParam is broken."));
    }
    Serial.println(F(""));
    
    Serial.println(F("--- MorseParam ---"));
    if (Morikawa.getParamMorse(&morse) == TSTERROR_OK) {
        dump_memory(-1, morse.size, NULL, morse.data);
    }
    else {
        Serial.println(F("MorseParam is broken."));
    }
    Serial.println(F(""));
    
    Serial.println(F("--- DigitalkerParam ---"));
    if (Morikawa.getParamDigitalker(&digitalker) == TSTERROR_OK) {
        dump_memory(-1, digitalker.size, NULL, digitalker.data);
    }
    else {
        Serial.println(F("DigitalkerParam is broken."));
    }
    Serial.println(F(""));
    
    Serial.println(F("--- CameraParam ---"));
    if (Morikawa.getParamCamera(&camera) == TSTERROR_OK) {
        dump_memory(-1, camera.size, NULL, camera.data);
    }
    else {
        Serial.println(F("CameraParam is broken."));
    }
    Serial.println(F(""));
    
    Serial.println(F("--- TEXT_X ---"));
    if (Morikawa.getText(TEXT_X, temp, sizeof(temp), &size) == TSTERROR_OK) {
        dump_memory(-1, size - 1, temp, temp);
    }
    else {
        Serial.println(F("TEXT_X is broken."));
    }
    Serial.println(F(""));
    
    Serial.println(F("--- TEXT_Y ---"));
    if (Morikawa.getText(TEXT_Y, temp, sizeof(temp), &size) == TSTERROR_OK) {
        dump_memory(-1, size - 1, temp, temp);
    }
    else {
        Serial.println(F("TEXT_Y is broken."));
    }
    Serial.println(F(""));
    
    Serial.println(F("--- TEXT_Z ---"));
    if (Morikawa.getText(TEXT_Z, temp, sizeof(temp), &size) == TSTERROR_OK) {
        dump_memory(-1, size - 1, temp, temp);
    }
    else {
        Serial.println(F("TEXT_Z is broken."));
    }
    Serial.println(F(""));
    
    Serial.println(F("--- TEXT_DEBUG ---"));
    if (Morikawa.getText(TEXT_DEBUG, temp, sizeof(temp), &size) == TSTERROR_OK) {
        dump_memory(-1, size - 1, temp, temp);
    }
    else {
        Serial.println(F("TEXT_DEBUG is broken."));
    }
    return;
}

static void dump_memory(long addr, unsigned char size, char const* text, void const* data)
{
    char temp[16];
    int i;
    
    Serial.print(F("["));
    if (addr >= 0) {
        Serial.print(F("address : "));
        snprintf(temp, sizeof(temp), "0x%05lX", addr);
        Serial.print(temp);
        Serial.print(F(", "));
    }
    Serial.print(size);
    Serial.println(F(" bytes]"));
    if (text != NULL) {
        Serial.println(text);
    }
    if (data != NULL) {
        for (i = 0; i < size; ++i) {
            snprintf(temp, sizeof(temp), "%02X%c", static_cast<unsigned char const*>(data)[i], (i % 16 != 15) ? (' ') : ('\n'));
            Serial.print(temp);
        }
        if (i % 16 != 0) {
            Serial.println(F(""));
        }
    }
    return;
}

static void writeSharedMemory(unsigned long address, void const* data, unsigned char size, bool pascal)
{
#if defined(TARGET_INVADER_EM1) || defined(TARGET_INVADER_FM1)
    static unsigned char const s_address[SECTOR_LIMIT] PROGMEM = {
        0x50,
        0x54,
        0x51,
        0x55,
        0x52,
        0x56,
        0x53,
        0x57
    };
    unsigned char i2c(pgm_read_byte(&s_address[(address >> 16) & 0x07]));
#elif defined(TARGET_DESPATCH_FM1)
    static unsigned char const s_address[SECTOR_LIMIT] PROGMEM = {
        0x50,
        0x54,
        0x51,
        0x55
    };
    unsigned char i2c(pgm_read_byte(&s_address[(address >> 16) & 0x03]));
#else
    unsigned char i2c(0);
#endif
    unsigned char const* temp(static_cast<unsigned char const*>(data));
    
    if (pascal) {
        while (true) {
            I2Cm.clear();
            if (I2Cm.send(i2c, true) == 0) {
                break;
            }
        }
        I2Cm.clear();
        I2Cm.write((address >> 8) & 0xFF);
        I2Cm.write((address >> 0) & 0xFF);
        I2Cm.write(size);
        I2Cm.send(i2c, true);
        ++address;
    }
    for (; size > 0; --size, ++temp) {
        while (true) {
            I2Cm.clear();
            if (I2Cm.send(i2c, true) == 0) {
                break;
            }
        }
        I2Cm.clear();
        I2Cm.write((address >> 8) & 0xFF);
        I2Cm.write((address >> 0) & 0xFF);
        I2Cm.write(*temp);
        I2Cm.send(i2c, true);
        ++address;
    }
    return;
}
