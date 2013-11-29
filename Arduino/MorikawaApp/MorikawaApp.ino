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
**      MorikawaApp.ino
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

#include <MorikawaSDK.h>

#define TARGET_BUILD_DEBUG

#define REGISTER_APP(param)\
    g_setup = &param##_setup;\
    g_loop = &param##_loop;

enum {
    APP_SELF_TEST               = 0,
    APP_HELLO_SPACE,
    APP_SPEECH_TEXT,
    APP_CODE_TEXT,
    APP_PLAY_MELODY,
    APP_INVADER_BOT,
    APP_INVADER_MUSIC,
    APP_INVADER_CAM,
    APP_SECRET_ICON,
    APP_TELEMETRY_DUMP,
    MGR_ERASE_SELFTESTLOG       = 128,
    MGR_FORMAT_EEPROM,
    MGR_FORMAT_SHAREDMEMORY,
    MGR_FORMAT_FRAM,
    MGR_FORMAT_FLASHROM,
    APP_LIMIT
};

static void dump(TSTError error, int hint);

static void(*g_setup)(void)(NULL);
static void(*g_loop)(void)(NULL);
static TSTError g_error(TSTERROR_OK);

void setup(void)
{
    TSTError error;
    
#ifdef TARGET_BUILD_DEBUG
    Serial.begin(9600);
#endif
    error = __debug__(Morikawa.setup(), 0);
    if (error == TSTERROR_OK) {
#ifdef TARGET_BUILD_DEBUG
        SharedMemory_setup();
        SharedMemory_loop();
#endif
        switch (Morikawa.getBootMode()) {
            case APP_HELLO_SPACE:
                REGISTER_APP(HelloSpace)
                break;
            case APP_SPEECH_TEXT:
                REGISTER_APP(SpeechText)
                break;
            case APP_CODE_TEXT:
                REGISTER_APP(CodeText)
                break;
            case APP_PLAY_MELODY:
                REGISTER_APP(PlayMelody)
                break;
            case APP_INVADER_BOT:
                REGISTER_APP(InvaderBot)
                break;
            case APP_INVADER_MUSIC:
                REGISTER_APP(InvaderMusic)
                break;
            case APP_INVADER_CAM:
                REGISTER_APP(InvaderCam)
                break;
            case APP_SECRET_ICON:
                REGISTER_APP(SecretIcon)
                break;
            case APP_TELEMETRY_DUMP:
                REGISTER_APP(TelemetryDump)
                break;
            case MGR_ERASE_SELFTESTLOG:
                REGISTER_APP(EraseSelfTestLog)
                break;
            case MGR_FORMAT_EEPROM:
                REGISTER_APP(FormatEEPROM)
                break;
            case MGR_FORMAT_SHAREDMEMORY:
                REGISTER_APP(FormatSharedMemory)
                break;
            case MGR_FORMAT_FRAM:
                REGISTER_APP(FormatFRAM)
                break;
            case MGR_FORMAT_FLASHROM:
                REGISTER_APP(FormatFlashROM)
                break;
            case APP_SELF_TEST:
            default:
                REGISTER_APP(SelfTest)
                break;
        }
        (*g_setup)();
    }
    else {
        Morikawa.shutdown();
    }
    return;
}

void loop(void)
{
    Morikawa.loop();
    (*g_loop)();
    return;
}

void delay10mTimes(unsigned int times)
{
    for (; times > 0; --times) {
        delay(10);
        Morikawa.loop();
    }
    return;
}

TSTError __debug__(TSTError error, int hint)
{
    if (error != TSTERROR_OK) {
        if (g_error == TSTERROR_OK) {
            dump(error, hint);
            g_error = error;
        }
    }
    return error;
}

static void dump(TSTError error, int hint)
{
    static char s_text[16];
    
    snprintf(s_text, sizeof(s_text), "SOS E%02x H%03d", error, hint);
#ifdef TARGET_BUILD_DEBUG
    Serial.println(s_text);
#endif
    Morikawa.setText(TEXT_DEBUG, s_text);
    Morikawa.playMorse(NOTE_C6, s_text);
    return;
}
