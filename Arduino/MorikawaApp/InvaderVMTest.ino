/*
**      ARTSAT Morikawa Application
 **
 **      Original Copyright (C) 2013 - 2013 Ron Hashimoto.
 **                                          http://h2so5.net/
 **                                          mail@h2so5.net
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
 **      InvaderVMTest.ino
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
 **      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.ß
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
struct VMState;

#ifdef TARGET_BUILD_DEBUG

void InvaderVM_dump()
{
  Serial.println("dump register:");
  for (int i = 0; i < INVADER_VM_REGISTER_SIZE; ++i) {
    Serial.println(vm_state.reg[i], DEC); 
  } 
}

#define VM_ASSERT_EQUAL(a, b) do { \
if (a != b) { \
  Serial.print("X"); \
  return false; \
} \
else { \
  Serial.print(".");\
} \
} while (0)

#define VM_DO_TEST(name) do { \ 
Serial.print("[" #name "] "); \
if (InvaderVMTest_##name()) { \ 
  Serial.println(" PASS"); \ 
  passed++; \
} \ 
else { \ 
  Serial.println(" FAIL"); \ 
  InvaderVM_dump(); \
  failed++; \
} \ 
} while (0)

void InvaderVM_run(VMState *state);
void InvaderVM_loadProgram(VMState *state);

bool InvaderVMTest_VM_SET(void)
{
  const char code[] = { 
    VM_SET,  REG_ERRN, 0x0F, 0, 0, 0,
    VM_SET,  REG_FUNC, 0x01, 0, 0, 0,
    VM_SET,  REG_RETV, 0x02, 0, 0, 0,
    VM_SET,  REG_HCUR, 0x03, 0, 0, 0,
    VM_SET,  REG_HEAP, 0x04, 0, 0, 0,
    VM_SET,  REG_ARG0, 0x05, 0, 0, 0,
    VM_SET,  REG_ARG1, 0xFF, 0xFF, 0xFF, 0xFF,
    VM_SETC, REG_ARG2, 0x07,
    VM_SET,  REG_ARG3, 0x08, 0, 0, 0,
    VM_SET,  REG_ARG4, 0x09, 0, 0, 0,
    VM_SET,  REG_ARG5, 0x0A, 0, 0, 0,
    VM_SET,  REG_ARG6, 0x0B, 0, 0, 0,
    VM_SETI, REG_ARG7, 0x0C, 0x0C,
    VM_SET,  REG_ARG8, 0x0D, 0, 0, 0,
    VM_SET,  REG_ARG9, 0x0E, 0, 0, 0,
    VM_SET,  REG_ARGA, 0x0F, 0, 0, 0,
    VM_CLR,  REG_ARG5,
    VM_CLR,  REG_ARG8,
    VM_END
  };
  
  memcpy(vm_state.code, code, sizeof(code));
  InvaderVM_run(&vm_state);
  
  VM_ASSERT_EQUAL(vm_state.reg[REG_ERRN], 0x0F);
  VM_ASSERT_EQUAL(vm_state.reg[REG_FUNC], 0x01);
  VM_ASSERT_EQUAL(vm_state.reg[REG_RETV], 0x02);
  VM_ASSERT_EQUAL(vm_state.reg[REG_HCUR], 0x03);
  VM_ASSERT_EQUAL(vm_state.reg[REG_HEAP], 0x04);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG0], 0x05);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG1], 0xFFFFFFFF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG2], 0x07);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG3], 0x08);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG4], 0x09);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG5], 0x00);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG6], 0x0B);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG7], 0x0C0C);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG8], 0x00);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG9], 0x0E);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARGA], 0x0F);
  
  return true;
}

bool InvaderVMTest_VM_MOV(void)
{
  const char code[] = {
    VM_SET, REG_ARG0, 0xFF, 0, 0xFF, 0,
    VM_MOV, REG_ARG1, REG_ARG0,
    VM_MOV, REG_ARG2, REG_ARG1,
    VM_MOV, REG_ARG3, REG_ARG2,
    VM_MOV, REG_ARG4, REG_ARG3,
    VM_MOV, REG_ARG5, REG_ARG4,
    VM_MOV, REG_ARG6, REG_ARG5,
    VM_MOV, REG_ARG7, REG_ARG6,
    VM_MOV, REG_ARG8, REG_ARG7,
    VM_MOV, REG_ARG9, REG_ARG8,
    VM_MOV, REG_ARGA, REG_ARG9,
    VM_END
  };
  
  memcpy(vm_state.code, code, sizeof(code));
  InvaderVM_run(&vm_state);
  
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG0], 0xFF00FF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG1], 0xFF00FF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG2], 0xFF00FF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG3], 0xFF00FF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG4], 0xFF00FF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG5], 0xFF00FF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG6], 0xFF00FF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG7], 0xFF00FF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG8], 0xFF00FF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG9], 0xFF00FF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARGA], 0xFF00FF);
  
  return true;
}

bool InvaderVMTest_VM_XCHG(void)
{
  const char code[] = {
    VM_SETC, REG_ARG0, 0xFF,
    VM_XCHG, REG_ARG0, REG_ARG1,
    VM_SETC, REG_ARG2, 0xFF,
    VM_XCHG, REG_ARG2, REG_ARG3,
    VM_SETC, REG_ARG4, 0xFF,
    VM_XCHG, REG_ARG4, REG_ARG5,
    VM_SETC, REG_ARG6, 0xFF,
    VM_XCHG, REG_ARG6, REG_ARG7,
    VM_SETC, REG_ARG8, 0xFF,
    VM_XCHG, REG_ARG8, REG_ARG9,
    VM_SETC, REG_ARGA, 0xFF,
    VM_XCHG, REG_ARGA, REG_ARG0,
    VM_END
  };

  memcpy(vm_state.code, code, sizeof(code));
  InvaderVM_run(&vm_state);

  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG0], 0xFF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG1], 0xFF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG2], 0x00);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG3], 0xFF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG4], 0x00);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG5], 0xFF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG6], 0x00);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG7], 0xFF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG8], 0x00);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG9], 0xFF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARGA], 0x00);

  return true;
}
bool InvaderVMTest_Jump(void)
{
  const char code[] = {
    VM_SETC,   REG_ARG0, 0x01,
    VM_JMP,    2,
    VM_CLR,    REG_ARG0,
    VM_JMPIF,  REG_ARG0, 6,
    VM_SETC,   REG_ARG1, 0x05,
    VM_SETC,   REG_ARG2, 0x08,
    VM_JMPNOT, REG_ARG0, 3,
    VM_SETC,   REG_ARG2, 0x09,
    VM_END
  };
  
  memcpy(vm_state.code, code, sizeof(code));
  InvaderVM_run(&vm_state);
  
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG0], 0x01);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG1], 0x00);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG2], 0x09);
  
  return true;
}

bool InvaderVMTest_Heap(void)
{
  const char code[] = {
    VM_CLR,  REG_HCUR,
    VM_SET,  REG_HEAP, 0xFF, 0, 0, 0,
    VM_SETC, REG_HCUR, 0x01,
    VM_SET,  REG_HEAP, 0xFF, 0, 0, 0,
    VM_SETC, REG_HCUR, 0x02,
    VM_SET,  REG_HEAP, 0xFF, 0, 0, 0,
    VM_CLR,  REG_HCUR,
    VM_NOP,
    VM_END
  };
  
  memcpy(vm_state.code, code, sizeof(code));
  InvaderVM_run(&vm_state);
  
  VM_ASSERT_EQUAL(vm_state.reg[REG_HEAP], 0xFFFFFF);
  
  return true;
}

bool InvaderVMTest_Arithmetic(void)
{
  const char code[] = {
    VM_INC,  REG_ARG0,
    VM_INC,  REG_ARG0,
    VM_INC,  REG_ARG0,
    VM_DEC,  REG_ARG1,
    VM_DEC,  REG_ARG1,
    VM_MOV,  REG_ARG2, REG_ARG1,
    VM_NEG,  REG_ARG2,
    VM_ADD,  REG_ARG3, REG_ARG0,
    VM_ADD,  REG_ARG3, REG_ARG0,
    VM_ADD,  REG_ARG3, REG_ARG0,
    VM_SUB,  REG_ARG4, REG_ARG1,
    VM_SUB,  REG_ARG4, REG_ARG1,
    VM_MOV,  REG_ARG5, REG_ARG3,
    VM_MOV,  REG_ARG6, REG_ARG3,
    VM_MUL,  REG_ARG5, REG_ARG1,
    VM_DIV,  REG_ARG6, REG_ARG7, REG_ARG2,
    VM_END
  };
  
  memcpy(vm_state.code, code, sizeof(code));
  InvaderVM_run(&vm_state);
  
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG0],  3);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG1], -2);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG2],  2);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG3],  9);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG4],  4);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG5], -18);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG6],  4);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG7],  1);
  
  return true;
}

bool InvaderVMTest_Bitwise(void)
{
  const char code[] = {
    VM_SET, REG_ARG0, 0xFF, 0xFF, 0x00, 0x00,
    VM_SET, REG_ARG1, 0xAA, 0xAA, 0xAA, 0xAA,
    VM_SET, REG_ARG2, 0x55, 0x55, 0x55, 0x55,
    VM_MOV, REG_ARG3, REG_ARG2,
    VM_MOV, REG_ARG4, REG_ARG3,
    VM_MOV, REG_ARG5, REG_ARG4,
    VM_MOV, REG_ARG6, REG_ARG5,    
    VM_AND, REG_ARG3, REG_ARG0,
    VM_OR,  REG_ARG4, REG_ARG1,
    VM_XOR, REG_ARG5, REG_ARG0,
    VM_NOT, REG_ARG6,
    VM_END
  };
  
  memcpy(vm_state.code, code, sizeof(code));
  InvaderVM_run(&vm_state);
  
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG3], 0x5555);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG4], 0xFFFFFFFF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG5], 0x5555AAAA);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG6], 0xAAAAAAAA);
  
  return true;
}

bool InvaderVMTest_Shift(void)
{
  const char code[] = {
    VM_SET, REG_ARG0, 0xFF, 0xFF, 0x00, 0x00,
    VM_MOV, REG_ARG1, REG_ARG0,
    VM_SET, REG_ARG2, 0x03, 0x00, 0x00, 0x00,
    VM_MOV, REG_ARG3, REG_ARG2,
    VM_SHR, REG_ARG0, REG_ARG2,
    VM_SHL, REG_ARG1, REG_ARG3,
    VM_END
  };
  
  memcpy(vm_state.code, code, sizeof(code));
  InvaderVM_run(&vm_state);
  
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG0], 0x1FFF);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG1], 0x7FFF8);
  
  return true;
}

bool InvaderVMTest_Comparison1(void)
{
  const char code[] = {
    VM_SETC, REG_ARG0, 0x01,
    VM_SETC, REG_ARG1, 0x02,
    VM_SETC, REG_ARG2, 0x03,
    VM_SETC, REG_ARG3, 0x04,
    
    VM_EQ,  REG_ARG4, REG_ARG0, REG_ARG0,
    VM_EQ,  REG_ARG5, REG_ARG0, REG_ARG1,
    VM_LT,  REG_ARG6, REG_ARG0, REG_ARG1,
    VM_LT,  REG_ARG7, REG_ARG2, REG_ARG1,
    VM_LE,  REG_ARG8, REG_ARG2, REG_ARG2,
    
    VM_END
  };
  
  memcpy(vm_state.code, code, sizeof(code));
  InvaderVM_run(&vm_state);
  
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG4], 1);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG5], 0);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG6], 1);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG7], 0);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG8], 1);
  
  return true;
}

bool InvaderVMTest_Comparison2(void)
{
  const char code[] = {
    VM_SETC, REG_ARG0, 0x01,
    VM_SETC, REG_ARG1, 0x02,
    VM_SETC, REG_ARG2, 0x03,
    VM_SETC, REG_ARG3, 0x04,
    
    VM_LE,  REG_ARG4, REG_ARG0, REG_ARG3, 
    VM_GT,  REG_ARG5, REG_ARG3, REG_ARG2,
    VM_GT,  REG_ARG6, REG_ARG3, REG_ARG3,
    VM_GE,  REG_ARG7, REG_ARG2, REG_ARG3,
    VM_GE,  REG_ARG8, REG_ARG3, REG_ARG3,
    
    VM_END
  };
  
  memcpy(vm_state.code, code, sizeof(code));
  InvaderVM_run(&vm_state);
  
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG4], 1);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG5], 1);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG6], 0);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG7], 0);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG8], 1);
  
  return true;
}

bool InvaderVMTest_Meta(void)
{
  const char code[] = {
    VM_TXT, 0, 0, 0, 0, 13, 0, 0, 0,
    VM_SETC, REG_ARG0, 0x01,
    VM_SETC, REG_ARG1, 0x02,
    VM_SETC, REG_ARG2, 0x03,
    VM_SETC, REG_ARG3, 0x04,
    VM_END,
    VM_EXC, 0, 0, 0, 0, 13, 0, 0, 0,
    VM_END
  };
  
  memcpy(vm_state.code, code, sizeof(code));
  InvaderVM_run(&vm_state);

  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG0], 0x01);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG1], 0x02);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG2], 0x03);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG3], 0x04);
  
  return true;
}

bool InvaderVMTest_Text(void)
{
  const char code[] = {
    VM_TXT, 0, 0, 0, 0, 4, 0, 0, 0,
    'a', 'b', 'c', 'd',
    VM_SETC, REG_ARG0, TEXT_DEBUG,
    VM_CLR,  REG_ARG1,
    VM_SETC, REG_ARG2, 4,
    VM_SETC, REG_FUNC, VMFunc_setText,
    VM_CALL,
    VM_SETC, REG_ARG0, TEXT_DEBUG,
    VM_SETC, REG_ARG1, 4,
    VM_SETC, REG_ARG2, 5,
    VM_SETC, REG_FUNC, VMFunc_getText,
    VM_CALL,
    VM_CLR,  REG_HCUR,
    VM_MOV,  REG_ARG0, REG_HEAP,
    VM_SETC, REG_HCUR, 4,
    VM_EQ,   REG_ARG1, REG_ARG0, REG_HEAP,
    VM_END
  };
  
  memcpy(vm_state.code, code, sizeof(code));
  InvaderVM_run(&vm_state);

  VM_ASSERT_EQUAL(vm_state.reg[REG_RETV], 5);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG1], 1);
  
  return true;
}

bool InvaderVMTest_Compressed(void)
{
  TSTError error;
  
  const char code[] = {
    VM_INC, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_ADD, REG_ARG1, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_ADD, REG_ARG1, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_ADD, REG_ARG1, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_ADD, REG_ARG1, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_ADD, REG_ARG1, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_ADD, REG_ARG1, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_ADD, REG_ARG1, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_ADD, REG_ARG1, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_ADD, REG_ARG1, REG_ARG0,
    VM_INC, REG_ARG0,
    VM_ADD, REG_ARG1, REG_ARG0,
    VM_END
  };
  
  char tmp[128] = { VM_COMPRESSED };
  unsigned int r = 0;
  unsigned long o = 0, w = 0;
  error = Morikawa.writeSharedMemory(0, code, sizeof(code), &r);
  error = Morikawa.freezeFastLZ(STORAGE_SHAREDMEMORY, 0, r, STORAGE_NONE, 0, 0, STORAGE_NONE, 0, 0, &o);
  error = Morikawa.freezeFastLZ(STORAGE_SHAREDMEMORY, 0, r, STORAGE_FRAM, 0, o, STORAGE_NONE, 0, 0, &w);
  error = Morikawa.freezeFastLZ(STORAGE_SHAREDMEMORY, 0, r, STORAGE_FRAM, 0, o, STORAGE_FRAM, o, w, &o);
  Morikawa.readFRAM(0, tmp + 1, o, &r);
  error = Morikawa.setText(TEXT_Z, tmp, o + 1);
  
  InvaderVM_loadProgram(&vm_state);
  InvaderVM_run(&vm_state);

  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG0], 18);
  VM_ASSERT_EQUAL(vm_state.reg[REG_ARG1], 135);
  
  return true;
}

void InvaderVM_runTests(void)
{
  int passed = 0, failed = 0;
  Serial.println("\n**** Start InvaderVM Tests ****\n");
  
  VM_DO_TEST(VM_SET);
  VM_DO_TEST(VM_MOV);
  VM_DO_TEST(VM_XCHG);
  VM_DO_TEST(Jump);
  VM_DO_TEST(Heap);
  VM_DO_TEST(Arithmetic);
  VM_DO_TEST(Bitwise);
  VM_DO_TEST(Shift);
  VM_DO_TEST(Comparison1);
  VM_DO_TEST(Comparison2);
  VM_DO_TEST(Meta);
  VM_DO_TEST(Text);
  VM_DO_TEST(Compressed);
  
  Serial.println("\n**** Result ****");
  char result[128];
  snprintf(result, sizeof(result), "Passed: %d\nFailed: %d", passed, failed);
  Serial.println(result);
  if (failed == 0) {
    Serial.println("\nAll Tests Passed :-)\n");
  }
  else {
    Serial.println("\nSome Tests Failed :-(\n");
  }
  
  Morikawa.shutdown();
}

#endif

