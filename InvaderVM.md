InvaderVM
===

## Registers

### 0: REG_ERRN

Stores the error code of functions.

### 1: REG_FUNC

Specifies the function address for the __VM_CALL__ instruction.

### 2: REG_RETV

Stores the return value of functions.

### 3: REG_HCUR

Specifies the heap address for heap operations.

### 4: REG_HEAP

This register is synchronized with the 32bit value of the heap specified by the __REG_HCUR__ register.

### 5-15: REG_ARG0-A

Specifies the function arguments. It is also used as the workspace.


## Instructions

### VM_END

Stops execution.

### VM_NOP 

Does nothing.

### VM_SET, VM_SETC, VM_SETI
*(char pos, long val)*

*(char pos, char val)*

*(char pos, int val)*

Loads __val__ into the __pos__ register.

### VM_CLR
*(char pos)*

Loads 0 into the __pos__ register.

### VM_MOV
*(char dst, char src)*

Assigns the value of __src__ register to __dst__ register.

### VM_XCHG
*(char dst, char src)*

Swaps __dst__ register and __src__ register.

### VM_CALL

Calls the function specified in __REG_FUNC__ register.

### VM_JMP
*(char offset)*

Increases the program counter by __offset__ bytes.

### VM_JMPIF
*(char pos, char offset)*

Increases the program counter by __offset__ bytes if the value of __pos__ register is not 0.

### VM_JMPNOT
*(char pos, char offset)*

Increases the program counter by __offset__ bytes if the value of __pos__ register is 0.

### VM_INC
*(char pos)*

Increases the value of the __pos__ register.

### VM_DEC
*(char pos)*

Decreases the value of the __pos__ register.

### VM_NEG
*(char pos)*

Negates the sign of the __pos__ register.

### VM_ADD 
*(char dst, char src)*

Adds the value of __src__ register to that of __dst__ register.

### VM_SUB
*(char dst, char src)*

Subtracts the value of __src__ register from that of __dst__ register.

### VM_MUL
*(char dst, char src)*

Assigns the product of __src__ register and __dst__ register to __dst__ register.

### VM_DIV
*(char qdst, char rdst, char src)*

Devides __qdst__ register by __src__ register and assigns the quotient to __qdst__ register and the remainder to __rdst__ register.

### VM_AND
*(char dst, char src)*

Assigns the result of a bitwise AND operation of __dst__ register and __src__ register to the __dst__ register.

### VM_OR
*(char dst, char src)*

Assigns the result of a bitwise OR operation of __dst__ register and __src__ register to the __dst__ register.

### VM_XOR
*(char dst, char src)*

Assigns the result of a bitwise XOR operation of __dst__ register and __src__ register to the __dst__ register.

### VM_NOT 
*(char dst)*

Inverts the bits of the __dst__ register.

### VM_SHR
*(char dst, char src)*

Shifts the __dst__ register right by the value of __src__ register.

### VM_SHL
*(char dst, char src)*

Shifts the __dst__ register left by the value of __src__ register.

### VM_EQ 
*(char dst, char lhs, char rhs)*

If the value of __lhs__ register is equal to that of __rhs__ register, assigns 1 to __dst__ regiser.
Otherwise, assigns 0 to __dst__ regiser.

### VM_LT 
*(char dst, char lhs, char rhs)*

If the value of __lhs__ register is less than that of __rhs__ register, assigns 1 to __dst__ regiser.
Otherwise, assigns 0 to __dst__ regiser.

### VM_LE
*(char dst, char lhs, char rhs)*

If the value of __lhs__ register is less than or equal to that of __rhs__ register, assigns 1 to __dst__ regiser.
Otherwise, assigns 0 to __dst__ regiser.

### VM_GT 
*(char dst, char lhs, char rhs)*
 
If the value of __lhs__ register is more than that of __rhs__ register, assigns 1 to __dst__ regiser.
Otherwise, assigns 0 to __dst__ regiser.

### VM_GE
*(char dst, char lhs, char rhs)*

If the value of __lhs__ register is more than or equal to that of __rhs__ register, assigns 1 to __dst__ regiser.
Otherwise, assigns 0 to __dst__ regiser.

### VM_WAIT
*(char src)*
 
Pauses the program for the value of __src__ register in milliseconds.

### VM_TXT
*(long len, char[] data)*

Loads __len__ bytes of binary data to the heap.

### VM_EXC
*(long len)*

Loads __len__ bytes of binary data from the heap as a program and resets the program counter.

### VM_COMPRESSED

Treats following binary data as a FastLZ compressed program.


## Ported Functions

#### TSTError getMemorySpec(MemorySpec* result);

| REG_HCUR    | REG_ERRN |
| ----------- | -------- |
| MemorySpec* | TSTError |

#### TSTError getMemoryInfo(MemoryInfo* result);

| REG_HCUR    | REG_ERRN |
| ----------- | -------- |
| MemoryInfo* | TSTError |

#### TSTError getMemoryLog(MemoryInfo* result);

| REG_HCUR    | REG_ERRN |
| ----------- | -------- |
| MemoryInfo* | TSTError |

#### void saveMemoryLog(void);

| n/a | 
| --- |
| n/a |

#### TSTError getSelfTestLog(SelfTestLog* result);

| REG_HCUR     | REG_ERRN |
| ------------ | -------- |
| SelfTestLog* | TSTError |

#### void eraseSelfTestLog(void);

| n/a | 
| --- |
| n/a |

#### unsigned long getSizeEEPROM(void);

| REG_RETV      |
| ------------- |
| unsigned long |

#### unsigned long getSizeSharedMemory(void);

| REG_RETV      |
| ------------- |
| unsigned long |

#### unsigned long getSizeFRAM(void);

| REG_RETV      |
| ------------- |
| unsigned long |

#### unsigned long getSizeFlashROM(void);

| REG_RETV      |
| ------------- |
| unsigned long |

#### unsigned int getPageSizeEEPROM(void);

| REG_RETV      |
| ------------- |
| unsigned long |

#### unsigned int getPageSizeSharedMemory(void);

| REG_RETV      |
| ------------- |
| unsigned long |

#### unsigned int getPageSizeFRAM(void);

| REG_RETV      |
| ------------- |
| unsigned long |

#### unsigned int getPageSizeFlashROM(void);

| REG_RETV      |
| ------------- |
| unsigned long |

#### unsigned long getSectorSizeEEPROM(void);

| REG_RETV      |
| ------------- |
| unsigned long |

#### unsigned long getSectorSizeSharedMemory(void);

| REG_RETV      |
| ------------- |
| unsigned long |

#### unsigned long getSectorSizeFRAM(void);

| REG_RETV      |
| ------------- |
| unsigned long |

#### unsigned long getSectorSizeFlashROM(void);

| REG_RETV      |
| ------------- |
| unsigned long |

#### TSTError getCameraFormat(CameraType mode, CameraFormat* result);

| REG_ARG0   | REG_HCUR      | REG_ERRN |
| ---------- | ------------- | -------- |
| CameraType | CameraFormat* | TSTError |

#### unsigned long getBootTime(void);

| REG_RETV      |
| ------------- |
| unsigned long |

#### unsigned char getBootCount(void);

| REG_RETV      |
| ------------- |
| unsigned char |

#### unsigned char getBootMode(void);

| REG_RETV      |
| ------------- |
| unsigned char |

#### TSTError getParamNote(NoteParam* result);

| REG_HCUR   | REG_ERRN |
| ---------- | -------- |
| NoteParam* | TSTError |

#### TSTError getParamMorse(MorseParam* result);

| REG_HCUR    | REG_ERRN |
| ----------- | -------- |
| MorseParam* | TSTError |

#### TSTError getParamDigitalker(DigitalkerParam* result);

| REG_HCUR         | REG_ERRN |
| ---------------- | -------- |
| DigitalkerParam* | TSTError |

#### TSTError getParamCamera(CameraParam* result);

| REG_HCUR     | REG_ERRN |
| ------------ | -------- |
| CameraParam* | TSTError |

#### TSTError getTelemetryTime(TimeType type, unsigned long* result);

| REG_ARG0 | REG_RETV      | REG_ERRN |
| -------- | ------------- | -------- |
| TimeType | unsigned long | TSTError |

#### TSTError getTelemetryVoltage(VoltageType type, unsigned char* result);

| REG_ARG0    | REG_RETV      | REG_ERRN |
| ----------- | ------------- | -------- |
| VoltageType | unsigned char | TSTError |

#### TSTError getTelemetryCurrent(CurrentType type, unsigned char* result);

| REG_ARG0    | REG_RETV      | REG_ERRN |
| ----------- | ------------- | -------- |
| CurrentType | unsigned char | TSTError |

#### TSTError getTelemetryTemperature(TemperatureType type, unsigned char* result);

| REG_ARG0        | REG_RETV      | REG_ERRN |
| --------------- | ------------- | -------- |
| TemperatureType | unsigned char | TSTError |

#### TSTError getTelemetryGyro(GyroType type, unsigned char* result);

| REG_ARG0 | REG_RETV      | REG_ERRN |
| -------- | ------------- | -------- |
| GyroType | unsigned char | TSTError |

#### TSTError getTelemetryMagnet(MagnetType type, unsigned char* result);

| REG_ARG0   | REG_RETV      | REG_ERRN |
| ---------- | ------------- | -------- |
| MagnetType | unsigned char | TSTError |

#### TSTError setFlashROMEraseMode(bool param);

| REG_ARG0 | REG_ERRN |
| -------- | -------- |
| bool     | TSTError |

#### bool getFlashROMEraseMode(void);

| REG_RETV |
| -------- |
| bool     |

#### TSTError setText(TextType index, char const* text, int length = -1);

| REG_ARG0 | REG_HCUR | REG_ARG1 | REG_ERRN |
| -------- | -------- | -------- | -------- |
| TextType | char*    | int      | TSTError |

#### TSTError getText(TextType index, char* text, unsigned int length, int* result = NULL);

| REG_ARG0 | REG_HCUR | REG_ARG1     | REG_RETV | REG_ERRN |
| -------- | -------- | ------------ | -------- | -------- |
| TextType | char*    | unsigned int | int      | TSTError |

#### TSTError setLED(LEDType index, unsigned char pwm);

| REG_ARG0 | REG_ARG1      | REG_ERRN |
| -------- | ------------- | -------- |
| LEDType  | unsigned char | TSTError |

#### unsigned char getLED(LEDType index);

| REG_ARG0 | REG_RETV      |
| -------- | ------------- |
| LEDType  | unsigned char |

#### TSTError setNoteBPM(int param = -1);

| REG_ARG0 | REG_ERRN |
| -------- | -------- |
| int      | TSTError |

#### int getNoteBPM(void);

| REG_RETV |
| -------- |
| int      |

#### TSTError setMorseWPM(int param = -1);

| REG_ARG0 | REG_ERRN |
| -------- | -------- |
| int      | TSTError |

#### int getMorseWPM(void);

| REG_RETV |
| -------- |
| int      |

#### TSTError setSpeakAsyncMode(bool param);

| REG_ARG0 | REG_ERRN |
| -------- | -------- |
| boot     | TSTError |

#### bool getSpeakAsyncMode(void);

| REG_RETV |
| -------- |
| bool     |

#### bool isValid(void);

| REG_RETV |
| -------- |
| bool     |

#### bool isValidSharedMemory(void);

| REG_RETV |
| -------- |
| bool     |

##### bool isValidFRAM(void);

| REG_RETV |
| -------- |
| bool     |

#### bool isValidFlashROM(void);

| REG_RETV |
| -------- |
| bool     |

#### bool isValidLED(void);

| REG_RETV |
| -------- |
| bool     |

#### bool isValidTone(void);

| REG_RETV |
| -------- |
| bool     |

#### bool isValidDigitalker(void);

| REG_RETV |
| -------- |
| bool     |

#### bool isValidCamera(void);

| REG_RETV |
| -------- |
| bool     |

#### TSTError isBusyDigitalker(bool* result);

| REG_RETV | REG_ERRN |
| -------- | -------- |
| bool     | TSTError |

#### bool hasTelemetryUpdate(void);

| REG_RETV |
| -------- |
| bool     |

#### bool hasAbnormalShutdown(void);

| REG_RETV |
| -------- |
| bool     |

#### void shutdown(void);

| n/a |
| --- |
| n/a |

#### TSTError writeEEPROM(unsigned long address, void const* data, unsigned int size, unsigned int* result = NULL);

| REG_ARG0      | REG_HCUR | REG_ARG1     | REG_RETV     | REG_ERRN |
| ------------- | -------- | ------------ | ------------ | -------- |
| unsigned long | char*    | unsigned int | unsigned int | TSTError |

#### TSTError readEEPROM(unsigned long address, void* data, unsigned int size, unsigned int* result = NULL);

| REG_ARG0      | REG_HCUR | REG_ARG1     | REG_RETV     | REG_ERRN |
| ------------- | -------- | ------------ | ------------ | -------- |
| unsigned long | char*    | unsigned int | unsigned int | TSTError |

#### TSTError formatEEPROM(void);

| REG_ERRN |
| -------- |
| TSTError |

#### TSTError writeSharedMemory(unsigned long address, void const* data, unsigned int size, unsigned int* result = NULL);

| REG_ARG0      | REG_HCUR | REG_ARG1     | REG_RETV     | REG_ERRN |
| ------------- | -------- | ------------ | ------------ | -------- |
| unsigned long | char*    | unsigned int | unsigned int | TSTError |

#### TSTError readSharedMemory(unsigned long address, void* data, unsigned int size, unsigned int* result = NULL);

| REG_ARG0      | REG_HCUR | REG_ARG1     | REG_RETV     | REG_ERRN |
| ------------- | -------- | ------------ | ------------ | -------- |
| unsigned long | char*    | unsigned int | unsigned int | TSTError |

#### TSTError formatSharedMemory(void);

| REG_ERRN |
| -------- |
| TSTError |

#### TSTError writeFRAM(unsigned long address, void const* data, unsigned int size, unsigned int* result = NULL);

| REG_ARG0      | REG_HCUR | REG_ARG1     | REG_RETV     | REG_ERRN |
| ------------- | -------- | ------------ | ------------ | -------- |
| unsigned long | char*    | unsigned int | unsigned int | TSTError |

#### TSTError readFRAM(unsigned long address, void* data, unsigned int size, unsigned int* result = NULL);

| REG_ARG0      | REG_HCUR | REG_ARG1     | REG_RETV     | REG_ERRN |
| ------------- | -------- | ------------ | ------------ | -------- |
| unsigned long | char*    | unsigned int | unsigned int | TSTError |

#### TSTError formatFRAM(void);

| REG_ERRN |
| -------- |
| TSTError |

#### TSTError writeFlashROM(unsigned long address, void const* data, unsigned int size, unsigned int* result = NULL);

| REG_ARG0      | REG_HCUR | REG_ARG1     | REG_RETV     | REG_ERRN |
| ------------- | -------- | ------------ | ------------ | -------- |
| unsigned long | char*    | unsigned int | unsigned int | TSTError |

#### TSTError readFlashROM(unsigned long address, void* data, unsigned int size, unsigned int* result = NULL);

| REG_ARG0      | REG_HCUR | REG_ARG1     | REG_RETV     | REG_ERRN |
| ------------- | -------- | ------------ | ------------ | -------- |
| unsigned long | char*    | unsigned int | unsigned int | TSTError |

#### TSTError formatFlashROM(void);

| REG_ERRN |
| -------- |
| TSTError |

#### TSTError playFrequency(FrequencyType frequency, unsigned long duration);

| REG_ARG0      | REG_ARG1      | REG_ERRN |
| ------------- | ------------- | -------- |
| FrequencyType | unsigned long | TSTError |

#### TSTError playFrequency2(FrequencySequence const* sequence, int length = -1);

| REG_HCUR           | REG_ARG0 | REG_ERRN |
| ------------------ | -------- | -------- |
| FrequencySequence* | int      | TSTError |

#### TSTError playNote(NoteType note, DurationType duration, QualifierType qualifier = QUALIFIER_NONE);

| REG_ARG0 | REG_ARG1     | REG_ARG2      | REG_ERRN |
| -------- | ------------ | ------------- | -------- |
| NoteType | DurationType | QualifierType | TSTError |

#### TSTError playNote2(NoteSequence const* sequence, int length = -1);

| REG_HCUR      | REG_ARG0 | REG_ERRN |
| ------------- | -------- | -------- |
| NoteSequence* | int      | TSTError |

#### TSTError playMorse(NoteType note, char const* sequence, int length = -1);

| REG_ARG0 | REG_HCUR | REG_ARG1 | REG_ERRN |
| -------- | -------- | -------- | -------- |
| NoteType | char*    | int      | TSTError |


#### TSTError speakPhrase(char const* phrase, int length = -1);

| REG_HCUR | REG_ARG0 | REG_ERRN |
| -------- | -------- | -------- |
| char*    | long     | TSTError |

#### TSTError waitPhrase(void);

| REG_ERRN |
| -------- |
| TSTError |

#### TSTError stopPhrase(void);

| REG_ERRN |
| -------- |
| TSTError |

#### TSTError snapshotCamera(CameraType mode, StorageType storage, unsigned long address, unsigned long size, unsigned long* result);

| REG_ARG0   | REG_ARG1    | REG_ARG2      | REG_ARG3      | REG_RETV      | REG_ERRN |
| ---------- | ----------- | ------------- | ------------- | ------------- | -------- |
| CameraType | StorageType | unsigned long | unsigned long | unsigned long | TSTError |

#### TSTError freezeFastLZ(StorageType istorage, unsigned long iaddress, unsigned long isize, StorageType ostorage, unsigned long oaddress, unsigned long osize, StorageType wstorage, unsigned long waddress, unsigned long wsize, unsigned long* result);

| REG_ARG0    | REG_ARG1      | REG_ARG2      | REG_ARG3    | REG_ARG4      | REG_ARG5      | REG_ARG6    | REG_ARG7      | REG_ARG8      | REG_RETV      | REG_ERRN |
| ----------- | ------------- | ------------- | ----------- | ------------- | ------------- | ----------- | ------------- | ------------- | ------------- | -------- |
| StorageType | unsigned long | unsigned long | StorageType | unsigned long | unsigned long | StorageType | unsigned long | unsigned long | unsigned long | TSTError |

#### TSTError meltFastLZ(StorageType istorage, unsigned long iaddress, unsigned long isize, StorageType ostorage, unsigned long oaddress, unsigned long osize, unsigned long* result);

| REG_ARG0    | REG_ARG1      | REG_ARG2      | REG_ARG3    | REG_ARG4      | REG_ARG5      | REG_RETV      | REG_ERRN |
| ----------- | ------------- | ------------- | ----------- | ------------- | ------------- | ------------- | -------- |
| StorageType | unsigned long | unsigned long | StorageType | unsigned long | unsigned long | unsigned long | TSTError |

#### TSTError enableAudioBus(void);

| REG_ERRN |
| -------- |
| TSTError |

#### void disableAudioBus(void);

| n/a |
| --- |
| n/a |


## Unported Functions

* TSTError getTelemetryVoltage(VoltageType type, double* result) const;
* TSTError getTelemetryCurrent(CurrentType type, double* result) const;
* TSTError getTelemetryTemperature(TemperatureType type, double* result) const;
* TSTError getTelemetryGyro(GyroType type, double* result) const;
* TSTError getTelemetryMagnet(MagnetType type, double* result) const;
* TSTError setTextPGM(TextType index, char const PROGMEM* text, int length = -1);
* TSTError setup(void);
* void cleanup(void);
* void loop(void);
* static TSTError writeEEPROMPGM(unsigned long address, void const PROGMEM* data, unsigned int size, unsigned int* result = NULL);
* TSTError writeSharedMemoryPGM(unsigned long address, void const PROGMEM* data, unsigned int size, unsigned int* result = NULL);
* TSTError writeFRAMPGM(unsigned long address, void const PROGMEM* data, unsigned int size, unsigned int* result = NULL);
* TSTError writeFlashROMPGM(unsigned long address, void const PROGMEM* data, unsigned int size, unsigned int* result = NULL);
* TSTError playFrequencyPGM(FrequencySequence const PROGMEM* sequence, int length = -1);
* TSTError playNotePGM(NoteSequence const PROGMEM* sequence, int length = -1);
* TSTError playMorse(NoteType note, char character);
* TSTError playMorsePGM(NoteType note, char const PROGMEM* sequence, int length = -1);
* TSTError speakPhrasePGM(char const PROGMEM* phrase, int length = -1);
