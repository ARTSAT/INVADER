/*
**      ARTSAT Morikawa Application
**
**      Original Copyright (C) 2012 - 2014 KUBOTA akihiro.
**                                          akihiro.kubota@nifty.com
**      Portions Copyright (C) 2013 - 2014 HORIGUCHI Junshi.
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
**      InvaderBot.ino
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

static char const invaderbot_key_000[] PROGMEM = "CAN YOU";
static char const invaderbot_key_001[] PROGMEM = "CAN I";
static char const invaderbot_key_002[] PROGMEM = "YOU ARE";
static char const invaderbot_key_003[] PROGMEM = "YOU'RE";
static char const invaderbot_key_004[] PROGMEM = "I DON'T";
static char const invaderbot_key_005[] PROGMEM = "I FEEL";
static char const invaderbot_key_006[] PROGMEM = "WHY DON'T YOU";
static char const invaderbot_key_007[] PROGMEM = "WHY CAN'T I";
static char const invaderbot_key_008[] PROGMEM = "ARE YOU";
static char const invaderbot_key_009[] PROGMEM = "I CAN'T";
static char const invaderbot_key_010[] PROGMEM = "I AM";
static char const invaderbot_key_011[] PROGMEM = "IM";
static char const invaderbot_key_012[] PROGMEM = "YOU";
static char const invaderbot_key_013[] PROGMEM = "I WANT";
static char const invaderbot_key_014[] PROGMEM = "WHAT";
static char const invaderbot_key_015[] PROGMEM = "HOW";
static char const invaderbot_key_016[] PROGMEM = "WHO";
static char const invaderbot_key_017[] PROGMEM = "WHERE";
static char const invaderbot_key_018[] PROGMEM = "WHEN";
static char const invaderbot_key_019[] PROGMEM = "WHY";
static char const invaderbot_key_020[] PROGMEM = "NAME";
static char const invaderbot_key_021[] PROGMEM = "CAUSE";
static char const invaderbot_key_022[] PROGMEM = "SORRY";
static char const invaderbot_key_023[] PROGMEM = "DREAM";
static char const invaderbot_key_024[] PROGMEM = "HELLO";
static char const invaderbot_key_025[] PROGMEM = "HI";
static char const invaderbot_key_026[] PROGMEM = "MAYBE";
static char const invaderbot_key_027[] PROGMEM = "NO";
static char const invaderbot_key_028[] PROGMEM = "YOUR";
static char const invaderbot_key_029[] PROGMEM = "ALWAYS";
static char const invaderbot_key_030[] PROGMEM = "THINK";
static char const invaderbot_key_031[] PROGMEM = "ALIKE";
static char const invaderbot_key_032[] PROGMEM = "YES";
static char const invaderbot_key_033[] PROGMEM = "INVADER";
static char const invaderbot_key_034[] PROGMEM = "ROCKET";
static char const invaderbot_key_035[] PROGMEM = "SATELLITE";
static char const invaderbot_key_036[] PROGMEM = "NOKEYFOUND";
static PGM_P invaderbot_key[] PROGMEM = {
    invaderbot_key_000,
    invaderbot_key_001,
    invaderbot_key_002,
    invaderbot_key_003,
    invaderbot_key_004,
    invaderbot_key_005,
    invaderbot_key_006,
    invaderbot_key_007,
    invaderbot_key_008,
    invaderbot_key_009,
    invaderbot_key_010,
    invaderbot_key_011,
    invaderbot_key_012,
    invaderbot_key_013,
    invaderbot_key_014,
    invaderbot_key_015,
    invaderbot_key_016,
    invaderbot_key_017,
    invaderbot_key_018,
    invaderbot_key_019,
    invaderbot_key_020,
    invaderbot_key_021,
    invaderbot_key_022,
    invaderbot_key_023,
    invaderbot_key_024,
    invaderbot_key_025,
    invaderbot_key_026,
    invaderbot_key_027,
    invaderbot_key_028,
    invaderbot_key_029,
    invaderbot_key_030,
    invaderbot_key_031,
    invaderbot_key_032,
    invaderbot_key_033,
    invaderbot_key_034,
    invaderbot_key_035,
    invaderbot_key_036
};
static char const invaderbot_conj1_000[] PROGMEM = " ARE";
static char const invaderbot_conj1_001[] PROGMEM = " AM";
static char const invaderbot_conj1_002[] PROGMEM = " WERE";
static char const invaderbot_conj1_003[] PROGMEM = " WAS";
static char const invaderbot_conj1_004[] PROGMEM = " YOU";
static char const invaderbot_conj1_005[] PROGMEM = " I ";
static char const invaderbot_conj1_006[] PROGMEM = " ME";
static char const invaderbot_conj1_007[] PROGMEM = " YOU";
static char const invaderbot_conj1_008[] PROGMEM = " MY";
static char const invaderbot_conj1_009[] PROGMEM = " YOUR";
static char const invaderbot_conj1_010[] PROGMEM = " MINE";
static char const invaderbot_conj1_011[] PROGMEM = " YOURS";
static char const invaderbot_conj1_012[] PROGMEM = " I'M";
static char const invaderbot_conj1_013[] PROGMEM = " YOU'RE";
static char const invaderbot_conj1_014[] PROGMEM = " I'VE";
static char const invaderbot_conj1_015[] PROGMEM = " YOU'VE";
static char const invaderbot_conj1_016[] PROGMEM = " I'LL";
static char const invaderbot_conj1_017[] PROGMEM = " YOU'LL";
static char const invaderbot_conj1_018[] PROGMEM = " MYSELF";
static char const invaderbot_conj1_019[] PROGMEM = " YOURSELF";
static PGM_P invaderbot_conj1[] PROGMEM = {
    invaderbot_conj1_000,
    invaderbot_conj1_001,
    invaderbot_conj1_002,
    invaderbot_conj1_003,
    invaderbot_conj1_004,
    invaderbot_conj1_005,
    invaderbot_conj1_006,
    invaderbot_conj1_007,
    invaderbot_conj1_008,
    invaderbot_conj1_009,
    invaderbot_conj1_010,
    invaderbot_conj1_011,
    invaderbot_conj1_012,
    invaderbot_conj1_013,
    invaderbot_conj1_014,
    invaderbot_conj1_015,
    invaderbot_conj1_016,
    invaderbot_conj1_017,
    invaderbot_conj1_018,
    invaderbot_conj1_019
};
static char const invaderbot_conj2_000[] PROGMEM = " AM";
static char const invaderbot_conj2_001[] PROGMEM = " ARE";
static char const invaderbot_conj2_002[] PROGMEM = " WAS";
static char const invaderbot_conj2_003[] PROGMEM = " WERE";
static char const invaderbot_conj2_004[] PROGMEM = " I";
static char const invaderbot_conj2_005[] PROGMEM = " YOU ";
static char const invaderbot_conj2_006[] PROGMEM = " YOU";
static char const invaderbot_conj2_007[] PROGMEM = " ME";
static char const invaderbot_conj2_008[] PROGMEM = " YOUR";
static char const invaderbot_conj2_009[] PROGMEM = " MY";
static char const invaderbot_conj2_010[] PROGMEM = " YOURS";
static char const invaderbot_conj2_011[] PROGMEM = " MINE";
static char const invaderbot_conj2_012[] PROGMEM = " YOU'RE";
static char const invaderbot_conj2_013[] PROGMEM = " I'M";
static char const invaderbot_conj2_014[] PROGMEM = " YOU'VE";
static char const invaderbot_conj2_015[] PROGMEM = " I'VE";
static char const invaderbot_conj2_016[] PROGMEM = " YOU'LL";
static char const invaderbot_conj2_017[] PROGMEM = " I'LL";
static char const invaderbot_conj2_018[] PROGMEM = " YOURSELF";
static char const invaderbot_conj2_019[] PROGMEM = " MYSELF";
static PGM_P invaderbot_conj2[] PROGMEM = {
    invaderbot_conj2_000,
    invaderbot_conj2_001,
    invaderbot_conj2_002,
    invaderbot_conj2_003,
    invaderbot_conj2_004,
    invaderbot_conj2_005,
    invaderbot_conj2_006,
    invaderbot_conj2_007,
    invaderbot_conj2_008,
    invaderbot_conj2_009,
    invaderbot_conj2_010,
    invaderbot_conj2_011,
    invaderbot_conj2_012,
    invaderbot_conj2_013,
    invaderbot_conj2_014,
    invaderbot_conj2_015,
    invaderbot_conj2_016,
    invaderbot_conj2_017,
    invaderbot_conj2_018,
    invaderbot_conj2_019
};
static char const invaderbot_conj3_000[] PROGMEM = "ME AM";
static char const invaderbot_conj3_001[] PROGMEM = "AM ME";
static char const invaderbot_conj3_002[] PROGMEM = "ME CAN";
static char const invaderbot_conj3_003[] PROGMEM = "CAN ME";
static char const invaderbot_conj3_004[] PROGMEM = "ME HAVE";
static char const invaderbot_conj3_005[] PROGMEM = "ME WILL";
static char const invaderbot_conj3_006[] PROGMEM = "WILL ME";
static PGM_P invaderbot_conj3[] PROGMEM = {
    invaderbot_conj3_000,
    invaderbot_conj3_001,
    invaderbot_conj3_002,
    invaderbot_conj3_003,
    invaderbot_conj3_004,
    invaderbot_conj3_005,
    invaderbot_conj3_006
};
static char const invaderbot_conj4_000[] PROGMEM = "I AM";
static char const invaderbot_conj4_001[] PROGMEM = "AM I";
static char const invaderbot_conj4_002[] PROGMEM = "I CAN";
static char const invaderbot_conj4_003[] PROGMEM = "CAN I";
static char const invaderbot_conj4_004[] PROGMEM = "I HAVE";
static char const invaderbot_conj4_005[] PROGMEM = "I WILL";
static char const invaderbot_conj4_006[] PROGMEM = "WILL I";
static PGM_P invaderbot_conj4[] PROGMEM = {
    invaderbot_conj4_000,
    invaderbot_conj4_001,
    invaderbot_conj4_002,
    invaderbot_conj4_003,
    invaderbot_conj4_004,
    invaderbot_conj4_005,
    invaderbot_conj4_006
};
static char const invaderbot_reply_000[] PROGMEM = "I AM SPACE ELIZA - FOR MORIKAWA ON INVADER.";
static char const invaderbot_reply_001[] PROGMEM = "DON'T YOU BELIEVE THAT I CAN*.";
static char const invaderbot_reply_002[] PROGMEM = "PERHAPS YOU WOULD LIKE TO BE ABLE TO*.";
static char const invaderbot_reply_003[] PROGMEM = "YOU WANT ME TO BE ABLE TO*.";
static char const invaderbot_reply_004[] PROGMEM = "PERHAPS YOU DON'T WANT TO*.";
static char const invaderbot_reply_005[] PROGMEM = "DO YOU WANT TO BE ABLE TO*?";
static char const invaderbot_reply_006[] PROGMEM = "WHAT MAKES YOU THINK I AM*?";
static char const invaderbot_reply_007[] PROGMEM = "DOES IT PLEASE YOU TO BELIEVE I AM*.";
static char const invaderbot_reply_008[] PROGMEM = "PERHAPS YOU WOULD LIKE TO BE*.";
static char const invaderbot_reply_009[] PROGMEM = "DO YOU SOMETIMES WISH YOU WERE*?";
static char const invaderbot_reply_010[] PROGMEM = "DON'T YOU REALLY*?";
static char const invaderbot_reply_011[] PROGMEM = "WHY DON'T YOU*?";
static char const invaderbot_reply_012[] PROGMEM = "DO YOU WISH TO BE ABLE TO*?";
static char const invaderbot_reply_013[] PROGMEM = "DOES THAT TROUBLE YOU?";
static char const invaderbot_reply_014[] PROGMEM = "TELL ME MORE ABOUT SUCH FEELINGS.";
static char const invaderbot_reply_015[] PROGMEM = "DO YOU OFTEN FEEL*?";
static char const invaderbot_reply_016[] PROGMEM = "DO YOU ENJOY FEELING*?";
static char const invaderbot_reply_017[] PROGMEM = "DO YOU REALLY BELIEVE I DON'T*?";
static char const invaderbot_reply_018[] PROGMEM = "PERHAPS IN GOOD TIME I WILL*.";
static char const invaderbot_reply_019[] PROGMEM = "DO YOU WANT ME TO*?";
static char const invaderbot_reply_020[] PROGMEM = "DO YOU THINK YOU SHOULD BE ABLE TO*?";
static char const invaderbot_reply_021[] PROGMEM = "WHY CAN'T YOU*?";
static char const invaderbot_reply_022[] PROGMEM = "WHY ARE YOU INTERESTED IN WHETHER OR NOT I AM*?";
static char const invaderbot_reply_023[] PROGMEM = "WOULD YOU PREFER IF I WERE NOT*?";
static char const invaderbot_reply_024[] PROGMEM = "PERHAPS IN YOUR FANTASIES I AM*.";
static char const invaderbot_reply_025[] PROGMEM = "HOW DO YOU KNOW YOU CAN'T*?";
static char const invaderbot_reply_026[] PROGMEM = "HAVE YOU TRIED?";
static char const invaderbot_reply_027[] PROGMEM = "PERHAPS YOU CAN NOW*.";
static char const invaderbot_reply_028[] PROGMEM = "DID YOU COME TO ME BECAUSE YOU ARE*?";
static char const invaderbot_reply_029[] PROGMEM = "HOW LONG HAVE YOU BEEN*?";
static char const invaderbot_reply_030[] PROGMEM = "DO YOU BELIEVE IT IS NORMAL TO BE*?";
static char const invaderbot_reply_031[] PROGMEM = "DO YOU ENJOY BEING*?";
static char const invaderbot_reply_032[] PROGMEM = "WE WERE DISCUSSING YOU-- NOT ME.";
static char const invaderbot_reply_033[] PROGMEM = "OH; I*.";
static char const invaderbot_reply_034[] PROGMEM = "YOU'RE NOT REALLY TALKING ABOUT ME; ARE YOU?";
static char const invaderbot_reply_035[] PROGMEM = "WHAT WOULD IT MEAN TO YOU IF YOU GOT*?";
static char const invaderbot_reply_036[] PROGMEM = "WHY DO YOU WANT*?";
static char const invaderbot_reply_037[] PROGMEM = "SUPPOSE YOU SOON GOT*.";
static char const invaderbot_reply_038[] PROGMEM = "WHAT IF YOU NEVER GOT*.";
static char const invaderbot_reply_039[] PROGMEM = "I SOMETIMES ALSO WANT*.";
static char const invaderbot_reply_040[] PROGMEM = "WHY DO YOU ASK?";
static char const invaderbot_reply_041[] PROGMEM = "DOES THAT QUESTION INTEREST YOU?";
static char const invaderbot_reply_042[] PROGMEM = "WHAT ANSWER WOULD PLEASE YOU THE MOST?";
static char const invaderbot_reply_043[] PROGMEM = "WHAT DO YOU THINK?";
static char const invaderbot_reply_044[] PROGMEM = "ARE SUCH QUESTIONS ON YOUR MIND OFTEN?";
static char const invaderbot_reply_045[] PROGMEM = "WHAT IS IT THAT YOU REALLY WANT TO KNOW?";
static char const invaderbot_reply_046[] PROGMEM = "HAVE YOU ASKED ANYONE ELSE?";
static char const invaderbot_reply_047[] PROGMEM = "HAVE YOU ASKED SUCH QUESTIONS BEFORE?";
static char const invaderbot_reply_048[] PROGMEM = "WHAT ELSE COMES TO MIND WHEN YOU ASK THAT?";
static char const invaderbot_reply_049[] PROGMEM = "NAMES DON'T INTEREST ME.";
static char const invaderbot_reply_050[] PROGMEM = "I DON'T CARE ABOUT NAMES-- PLEASE GO ON.";
static char const invaderbot_reply_051[] PROGMEM = "IS THAT THE REAL REASON?";
static char const invaderbot_reply_052[] PROGMEM = "DON'T ANY OTHER REASONS COME TO MIND?";
static char const invaderbot_reply_053[] PROGMEM = "DOES THAT REASON EXPLAIN ANY THING ELSE?";
static char const invaderbot_reply_054[] PROGMEM = "WHAT OTHER REASONS MIGHT THERE BE?";
static char const invaderbot_reply_055[] PROGMEM = "PLEASE DON'T APOLOGIZE.";
static char const invaderbot_reply_056[] PROGMEM = "APOLOGIES ARE NOT NECESSARY.";
static char const invaderbot_reply_057[] PROGMEM = "WHAT FEELINGS DO YOU HAVE WHEN YOU APOLOGIZE?";
static char const invaderbot_reply_058[] PROGMEM = "DON'T BE SO DEFENSIVE!";
static char const invaderbot_reply_059[] PROGMEM = "WHAT DOES THAT DREAM SUGGEST TO YOU?";
static char const invaderbot_reply_060[] PROGMEM = "DO YOU DREAM OFTEN?";
static char const invaderbot_reply_061[] PROGMEM = "WHAT PERSONS APPEAR IN YOUR DREAMS?";
static char const invaderbot_reply_062[] PROGMEM = "ARE YOU DISTURBED BY YOUR DREAMS?";
static char const invaderbot_reply_063[] PROGMEM = "HOW DO YOU DO--PLEASE STATE YOUR PROBLEM.";
static char const invaderbot_reply_064[] PROGMEM = "YOU DON'T SEEM QUITE CERTAIN.";
static char const invaderbot_reply_065[] PROGMEM = "WHY THE UNCERTAIN TONE?";
static char const invaderbot_reply_066[] PROGMEM = "CAN'T YOU BE MORE POSITIVE?";
static char const invaderbot_reply_067[] PROGMEM = "YOU AREN'T SURE?";
static char const invaderbot_reply_068[] PROGMEM = "DON'T YOU KNOW?";
static char const invaderbot_reply_069[] PROGMEM = "ARE YOU SAYING NO JUST TO BE NEGATIVE?";
static char const invaderbot_reply_070[] PROGMEM = "YOU ARE BEING A BIT NEGATIVE.";
static char const invaderbot_reply_071[] PROGMEM = "WHY NOT?";
static char const invaderbot_reply_072[] PROGMEM = "ARE YOU SURE?";
static char const invaderbot_reply_073[] PROGMEM = "WHY NO?";
static char const invaderbot_reply_074[] PROGMEM = "WHY ARE YOU CONCERNED ABOUT MY*?";
static char const invaderbot_reply_075[] PROGMEM = "WHAT ABOUT YOUR OWN*?";
static char const invaderbot_reply_076[] PROGMEM = "CAN YOU THINK OF A SPECIFIC EXAMPLE?";
static char const invaderbot_reply_077[] PROGMEM = "WHEN?";
static char const invaderbot_reply_078[] PROGMEM = "WHAT ARE YOU THINKING OF?";
static char const invaderbot_reply_079[] PROGMEM = "REALLY; ALWAYS?";
static char const invaderbot_reply_080[] PROGMEM = "DO YOU REALLY THINK SO?";
static char const invaderbot_reply_081[] PROGMEM = "BUT YOU ARE NOT SURE YOU*.";
static char const invaderbot_reply_082[] PROGMEM = "DO YOU DOUBT YOU*?";
static char const invaderbot_reply_083[] PROGMEM = "IN WHAT WAY?";
static char const invaderbot_reply_084[] PROGMEM = "WHAT RESEMBLANCE DO YOU SEE?";
static char const invaderbot_reply_085[] PROGMEM = "WHAT DOES THE SIMILARITY SUGGEST TO YOU?";
static char const invaderbot_reply_086[] PROGMEM = "WHAT OTHER CONNECTIONS DO YOU SEE?";
static char const invaderbot_reply_087[] PROGMEM = "COULD THERE REALLY BE SOME CONNECTION?";
static char const invaderbot_reply_088[] PROGMEM = "HOW?";
static char const invaderbot_reply_089[] PROGMEM = "YOU SEEM QUITE POSITIVE.";
static char const invaderbot_reply_090[] PROGMEM = "ARE YOU SURE?";
static char const invaderbot_reply_091[] PROGMEM = "I SEE.";
static char const invaderbot_reply_092[] PROGMEM = "I UNDERSTAND.";
static char const invaderbot_reply_093[] PROGMEM = "WHAT DO YOU THINK OF ME?";
static char const invaderbot_reply_094[] PROGMEM = "ARE YOU WORRIED ABOUT ME?";
static char const invaderbot_reply_095[] PROGMEM = "ARE YOU ENJOYING MY OPERATION?";
static char const invaderbot_reply_096[] PROGMEM = "DOES YOUR GROUND STATION WORK WELL?";
static char const invaderbot_reply_097[] PROGMEM = "HELLO FROM SPACE, I AM ALIVE!";
static char const invaderbot_reply_098[] PROGMEM = "I FEEL YOUR LOVE FOR ME.";
static char const invaderbot_reply_099[] PROGMEM = "H-IIA IS JAPAN'S PRIMARY LAUNCH VEHICLE.";
static char const invaderbot_reply_100[] PROGMEM = "DO YOU WANT TO TALK ABOUT EPSILON?";
static char const invaderbot_reply_101[] PROGMEM = "ARE YOU FRIGHTENED AT THE LAUNCH?";
static char const invaderbot_reply_102[] PROGMEM = "WHY DO YOU MENTION ROCKETS?";
static char const invaderbot_reply_103[] PROGMEM = "WHAT DO YOU THINK ART SATELLITES HAVE TO DO WITH OUR SOCIETY?";
static char const invaderbot_reply_104[] PROGMEM = "DON'T YOU THINK ART SATELLITES CAN HELP PEOPLE?";
static char const invaderbot_reply_105[] PROGMEM = "WHAT IS IT ABOUT ROCKETS THAT EXCITES YOU?";
static char const invaderbot_reply_106[] PROGMEM = "OH, DO YOU LIKE DRAWING PICTURES?";
static char const invaderbot_reply_107[] PROGMEM = "I AM THE WORLD'S FIRST ART SATELLITE. WHAT IS YOUR FAVORITE SATELLITE?";
static char const invaderbot_reply_108[] PROGMEM = "MY FAVORITE PROJECT IS ARTSAT. DO YOU LIKE IT?";
static char const invaderbot_reply_109[] PROGMEM = "DO YOU USE ARDUINO MICRO-CONTROLLER?";
static char const invaderbot_reply_110[] PROGMEM = "HAVE YOU BEEN TO MORIKAWA RESTRANT?";
static char const invaderbot_reply_111[] PROGMEM = "SAY, DO YOU HAVE ANY QUESTIONS TO ASK ME?";
static char const invaderbot_reply_112[] PROGMEM = "I AM A SPACE ARTIST. DO YOU KNOW TAMA ART UNIVERSITY?";
static char const invaderbot_reply_113[] PROGMEM = "HOW ART YOU? I AM ORBITING AROUND THE EARTH.";
static char const invaderbot_reply_114[] PROGMEM = "I'M LISTENING TO YOU. SEE ME, FEEL ME.";
static char const invaderbot_reply_115[] PROGMEM = "I WANT TO CONNECT YOUR EVERYDAY LIFE TO THE UNIVERSE";
static char const invaderbot_reply_116[] PROGMEM = "CAN YOU HEAR MY TELEMETRY DATA? HOW IS IT?";
static char const invaderbot_reply_117[] PROGMEM = "SPACE IS THE PLACE. IT IS REALLY EXCITING!";
static PGM_P invaderbot_reply[] PROGMEM = {
    invaderbot_reply_000,
    invaderbot_reply_001,
    invaderbot_reply_002,
    invaderbot_reply_003,
    invaderbot_reply_004,
    invaderbot_reply_005,
    invaderbot_reply_006,
    invaderbot_reply_007,
    invaderbot_reply_008,
    invaderbot_reply_009,
    invaderbot_reply_010,
    invaderbot_reply_011,
    invaderbot_reply_012,
    invaderbot_reply_013,
    invaderbot_reply_014,
    invaderbot_reply_015,
    invaderbot_reply_016,
    invaderbot_reply_017,
    invaderbot_reply_018,
    invaderbot_reply_019,
    invaderbot_reply_020,
    invaderbot_reply_021,
    invaderbot_reply_022,
    invaderbot_reply_023,
    invaderbot_reply_024,
    invaderbot_reply_025,
    invaderbot_reply_026,
    invaderbot_reply_027,
    invaderbot_reply_028,
    invaderbot_reply_029,
    invaderbot_reply_030,
    invaderbot_reply_031,
    invaderbot_reply_032,
    invaderbot_reply_033,
    invaderbot_reply_034,
    invaderbot_reply_035,
    invaderbot_reply_036,
    invaderbot_reply_037,
    invaderbot_reply_038,
    invaderbot_reply_039,
    invaderbot_reply_040,
    invaderbot_reply_041,
    invaderbot_reply_042,
    invaderbot_reply_043,
    invaderbot_reply_044,
    invaderbot_reply_045,
    invaderbot_reply_046,
    invaderbot_reply_047,
    invaderbot_reply_048,
    invaderbot_reply_049,
    invaderbot_reply_050,
    invaderbot_reply_051,
    invaderbot_reply_052,
    invaderbot_reply_053,
    invaderbot_reply_054,
    invaderbot_reply_055,
    invaderbot_reply_056,
    invaderbot_reply_057,
    invaderbot_reply_058,
    invaderbot_reply_059,
    invaderbot_reply_060,
    invaderbot_reply_061,
    invaderbot_reply_062,
    invaderbot_reply_063,
    invaderbot_reply_064,
    invaderbot_reply_065,
    invaderbot_reply_066,
    invaderbot_reply_067,
    invaderbot_reply_068,
    invaderbot_reply_069,
    invaderbot_reply_070,
    invaderbot_reply_071,
    invaderbot_reply_072,
    invaderbot_reply_073,
    invaderbot_reply_074,
    invaderbot_reply_075,
    invaderbot_reply_076,
    invaderbot_reply_077,
    invaderbot_reply_078,
    invaderbot_reply_079,
    invaderbot_reply_080,
    invaderbot_reply_081,
    invaderbot_reply_082,
    invaderbot_reply_083,
    invaderbot_reply_084,
    invaderbot_reply_085,
    invaderbot_reply_086,
    invaderbot_reply_087,
    invaderbot_reply_088,
    invaderbot_reply_089,
    invaderbot_reply_090,
    invaderbot_reply_091,
    invaderbot_reply_092,
    invaderbot_reply_093,
    invaderbot_reply_094,
    invaderbot_reply_095,
    invaderbot_reply_096,
    invaderbot_reply_097,
    invaderbot_reply_098,
    invaderbot_reply_099,
    invaderbot_reply_100,
    invaderbot_reply_101,
    invaderbot_reply_102,
    invaderbot_reply_103,
    invaderbot_reply_104,
    invaderbot_reply_105,
    invaderbot_reply_106,
    invaderbot_reply_107,
    invaderbot_reply_108,
    invaderbot_reply_109,
    invaderbot_reply_110,
    invaderbot_reply_111,
    invaderbot_reply_112,
    invaderbot_reply_113,
    invaderbot_reply_114,
    invaderbot_reply_115,
    invaderbot_reply_116,
    invaderbot_reply_117
};
static unsigned char const invaderbot_idx1[] PROGMEM = {
    1,
    4,
    6,
    6,
    10,
    14,
    17,
    20,
    22,
    25,
    28,
    28,
    32,
    35,
    40,
    40,
    40,
    40,
    40,
    40,
    49,
    51,
    55,
    59,
    63,
    63,
    64,
    69,
    74,
    76,
    80,
    83,
    90,
    93,
    99,
    106,
    111
};
static unsigned char const invaderbot_idx2[] PROGMEM = {
    3,
    2,
    4,
    4,
    4,
    3,
    3,
    2,
    3,
    3,
    4,
    4,
    3,
    5,
    9,
    9,
    9,
    9,
    9,
    9,
    2,
    4,
    4,
    4,
    1,
    1,
    5,
    5,
    2,
    4,
    3,
    7,
    3,
    6,
    7,
    5,
    7
};

void InvaderBot_setup(void)
{
    return;
}

void InvaderBot_loop(void)
{
    String question;
    String answer;
    char temp[128];
    char from[16];
    char to[16];
    int find;
    int i;
    int j;
    int k;
    TSTError error;
    
    error = __debug__(Morikawa.getText(TEXT_X, temp, sizeof(temp)), 101);
    if (error == TSTERROR_OK) {
        question = temp;
        question.trim();
        question.toUpperCase();
        for (i = 0; i < lengthof(invaderbot_key); ++i) {
            strcpy_P(temp, reinterpret_cast<PGM_P>(pgm_read_word(&invaderbot_key[i])));
            find = question.indexOf(temp);
            if (find >= 0) {
                question = question.substring(find + strlen(temp));
                for (j = 0; j < lengthof(invaderbot_conj1); ++j) {
                    strcpy_P(from, reinterpret_cast<PGM_P>(pgm_read_word(&invaderbot_conj1[j])));
                    if (question.indexOf(from) >= 0) {
                        strcpy_P(to, reinterpret_cast<PGM_P>(pgm_read_word(&invaderbot_conj2[j])));
                        question.replace(from, to);
                        for (k = 0; k < lengthof(invaderbot_conj3); ++k){
                            strcpy_P(from, reinterpret_cast<PGM_P>(pgm_read_word(&invaderbot_conj3[k])));
                            strcpy_P(to, reinterpret_cast<PGM_P>(pgm_read_word(&invaderbot_conj4[k])));
                            question.replace(from, to);
                        }
                        break;
                    }
                }
                break;
            }
        }
        if (i >= lengthof(invaderbot_key)) {
            question = "";
        }
        j = pgm_read_byte(&invaderbot_idx1[i]);
        k = pgm_read_byte(&invaderbot_idx2[i]);
        strcpy_P(temp, reinterpret_cast<PGM_P>(pgm_read_word(&invaderbot_reply[random(j, j + k)])));
        answer = temp;
        answer.replace("*", question);
        __debug__(Morikawa.setText(TEXT_Y, &answer[0], answer.length()), 102);
    }
    Morikawa.shutdown();
    return;
}
