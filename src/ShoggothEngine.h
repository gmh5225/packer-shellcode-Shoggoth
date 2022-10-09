#pragma once
#include <Windows.h>
#include <time.h>
#include "asmjit/asmjit.h"
#include "Structs.h"

using namespace asmjit;

#define BLOCK_SIZE 8

class ShoggothPolyEngine
{
public:
    ShoggothPolyEngine(bool shellcodeMode);

    PBYTE StartPolymorphicEncrypt(PBYTE payload, int payloadSize, int& encryptedSize);
    PBYTE AddReflectiveLoader(PBYTE payload, int payloadSize, int& newPayloadSize);
private:
    bool shellcodeMode;

    CodeHolder asmjitCodeHolder;

    JitRuntime asmjitRuntime;

    x86::Assembler* asmjitAssembler;

    int startOffset = 0;
    int endOffset = 0;

    x86::Gp allRegs[16];

    x86::Gp generalPurposeRegs[14];

    ENCRYPT_TYPE *encryptListForBlocks;

    uint64_t numberOfBlocks;

    x86::Gp addressHolderForSecondEncryption;

    // -----
    void MixupArrayRegs(x86::Reg* registerArr, WORD size);

    PBYTE PushAllRegisters(int& codeSize);
    PBYTE PopAllRegisters(int& codeSize);

    x86::Gp GetRandomRegister();
    x86::Gp GetRandomGeneralPurposeRegister();

    void DebugBuffer(PBYTE buffer, int bufferSize);

    void StartAsmjit();
    void ResetAsmjit();

    void GenerateJumpOverRandomData();

    PBYTE GenerateRandomGarbage(int& garbageSize);

    void GenerateGarbageInstructions();

    void GenerateGarbageFunction();
    void GenerateSafeInstruction();
    void GenerateReversedInstructions();
    void GenerateJumpedInstructions();
    
    PBYTE AssembleCodeHolder(int& codeSize);

    void RandomUnsafeGarbage();
    
    PBYTE FirstEncryption(PBYTE plainPayload, int payloadSize, PBYTE key, int keySize);
    PBYTE GenerateRC4Decryptor(PBYTE payload, int payloadSize, RC4STATE* statePtr, int& firstEncryptionStubSize);
    void InitRC4State(RC4STATE* state, uint8_t* key, size_t len);
    void EncryptRC4(RC4STATE* state, uint8_t* msg, size_t len);
    PBYTE FirstDecryptor(PBYTE cipheredPayload, int cipheredPayloadSize, PBYTE key, int keySize, int& firstDecryptorSize);
    

    PBYTE GetPopInstructionAfterPayload(int& popSize);

    PBYTE GetCallInstructionOverPayload(int payloadSize, int& callSize);
    PBYTE GeneratePopWithGarbage(x86::Gp popReg, int& popStubSize);

    PBYTE SecondDecryptor(PBYTE encryptedPayload, int payloadSize, int& secondDecryptorBlockSize);
    PBYTE SecondEncryption(PBYTE plainPayload, int payloadSize, int& newPayloadSize);
    void GetRandomSecondEncryption(ENCRYPT_TYPE* encryptTypeHolder);
    PBYTE GenerateSecondDecryptorStub(int& decryptorStubSize, int offsetToEncryptedPayload);
    void ApplyRandomSecondEncryption(uint64_t* blockCursor, ENCRYPT_TYPE* encryptTypeHolder);

    
};