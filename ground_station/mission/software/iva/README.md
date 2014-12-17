iva
===

InvaderVM Assembler

## example

```asm
# load string into the heap
CLR,   HCUR
TXT,   11,   <NUM VAL=3>

# initialize registers
SETC,  FUNC, speakPhrase

# speakPhrase() argument
SETC,  ARG0, 11

# counter
SETC,  ARG1, 3

# 1000 millisec
SETI,  ARG2, 1000

# while (ARG1 > 0)
begin:
CLR,   HCUR
CALL

WAIT,  ARG2

SETC,  HCUR, 9
DEC,   ARG1
DEC,   HEAP
JMPIF, ARG1, begin:
# end while

CLR,   HCUR
TXT,   18,   konnnichi'wa uchu-

SETC,  ARG0, 18
CALL

END
```

```hex
00000000  05 03 1f 0b 00 00 00 3c  4e 55 4d 20 56 41 4c 3d  |.......<NUM VAL=|
00000010  33 3e 03 01 4a 03 05 0b  03 06 03 04 07 e8 03 05  |3>..J...........|
00000020  03 08 1e 07 03 03 09 0d  06 0d 04 0a 06 f1 05 03  |................|
00000030  1f 12 00 00 00 6b 6f 6e  6e 6e 69 63 68 69 27 77  |.....konnnichi'w|
00000040  61 20 75 63 68 75 2d 03  05 12 08 00              |a uchu-.....|
```
