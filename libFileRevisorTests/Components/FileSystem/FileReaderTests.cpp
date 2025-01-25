#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileReader.h"

TESTS(FileReaderTests)
AFACT(DefaultConstructor_SetsFunctionPointers)
AFACT(ReadFirst256Bytes_ReadsFirst256Bytes_ReturnsPairOfNumberOfBytesReadAnd256ByteCharArray)
EVIDENCE

FileReader _fileReader;
// Function Pointers
#if defined __linux__
METALMOCK_NONVOID4_STATIC_OR_FREE(size_t, _call_fread, void*, size_t, size_t, FILE*)
#elif defined _WIN32
METALMOCK_NONVOID5_STATIC_OR_FREE(size_t, _call_fread_nolock_s, void*, size_t, size_t, size_t, FILE*)
#endif

STARTUP
{
   // Function Pointers
#if defined __linux__
   _fileReader._call_fread = BIND_4ARG_METALMOCK_OBJECT(_call_freadMock);
#elif defined _WIN32
   _fileReader._call_fread_nolock_s = BIND_5ARG_METALMOCK_OBJECT(_call_fread_nolock_sMock);
#endif
}

TEST(DefaultConstructor_SetsFunctionPointers)
{
   const FileReader fileReader;
   // Function Pointers
#if defined __linux__
   STD_FUNCTION_TARGETS(fread, fileReader._call_fread);
#elif defined _WIN32
   STD_FUNCTION_TARGETS(_fread_nolock_s, fileReader._call_fread_nolock_s);
#endif
}

#if defined __linux__

struct _fread_CallHistoryValues
{
   size_t numberOfCalls = 0;

   size_t returnValue = 0;
   array<char, 256> bufferReturnValue = {};

   size_t elementSize = 0;
   size_t elementCount = 0;
   FILE* rawFilePointer = nullptr;
} _fread_CallHistory;

size_t fread_CallInstead(void* buffer, size_t elementSize, size_t elementCount, FILE* rawFilePointer)
{
   ++_fread_CallHistory.numberOfCalls;

   const array<char, 256> expectedInitialBufferContents{};
   const char* bufferAsConstCharPointer = reinterpret_cast<const char*>(buffer);
   ARRAYS_ARE_EQUAL(expectedInitialBufferContents.data(), bufferAsConstCharPointer, 256);
   memcpy(buffer, _fread_CallHistory.bufferReturnValue.data(), _fread_CallHistory.bufferReturnValue.size());

   _fread_CallHistory.elementSize = elementSize;
   _fread_CallHistory.elementCount = elementCount;
   _fread_CallHistory.rawFilePointer = rawFilePointer;
   return _fread_CallHistory.returnValue;
}

void fread_AssertCalledOnceWith(size_t expectedElementSize, size_t expectedElementCount, FILE* expectedRawFilePointer) const
{
   ARE_EQUAL(1, _fread_CallHistory.numberOfCalls);
   ARE_EQUAL(expectedElementSize, _fread_CallHistory.elementSize);
   ARE_EQUAL(expectedElementCount, _fread_CallHistory.elementCount);
   ARE_EQUAL(expectedRawFilePointer, _fread_CallHistory.rawFilePointer);
}

TEST(ReadFirst256Bytes_ReadsFirst256Bytes_ReturnsPairOfNumberOfBytesReadAnd256ByteCharArray)
{
   const size_t numberOfBytesRead = ZenUnit::Random<size_t>();
   _fread_CallHistory.returnValue = numberOfBytesRead;

   const array<char, 256> bufferReturnValue = ZenUnit::RandomStdArray<char, 256>();
   _fread_CallHistory.bufferReturnValue = bufferReturnValue;

   _call_freadMock.CallInstead(std::bind(
      &FileReaderTests::fread_CallInstead, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));
   FILE* const rawFilePointer = tmpfile();
   //
   const pair<size_t, array<char, 256>> numberOfBytesReadAndFirst256FileBytes = _fileReader.ReadFirst256Bytes(rawFilePointer);
   //
   fread_AssertCalledOnceWith(1, 256, rawFilePointer);
   const pair<size_t, array<char, 256>> expectedNumberOfBytesReadAndFirst256FileBytes = { numberOfBytesRead, bufferReturnValue };
   PAIRS_ARE_EQUAL(expectedNumberOfBytesReadAndFirst256FileBytes, numberOfBytesReadAndFirst256FileBytes);
}

#elif defined _WIN32

struct _fread_nolock_s_CallHistoryValues
{
   size_t numberOfCalls = 0;

   size_t returnValue = 0;
   array<char, 256> bufferReturnValue = {};

   size_t bufferSize = 0;
   size_t elementSize = 0;
   size_t elementCount = 0;
   FILE* rawFilePointer = nullptr;
} _fread_nolock_s_CallHistory;

size_t _fread_nolock_s_CallInstead(void* buffer, size_t bufferSize, size_t elementSize, size_t elementCount, FILE* rawFilePointer)
{
   ++_fread_nolock_s_CallHistory.numberOfCalls;

   const array<char, 256> expectedInitialBufferContents{};
   const char* bufferAsConstCharPointer = reinterpret_cast<const char*>(buffer);
   ARRAYS_ARE_EQUAL(expectedInitialBufferContents.data(), bufferAsConstCharPointer, 256);
   memcpy(buffer, _fread_nolock_s_CallHistory.bufferReturnValue.data(), _fread_nolock_s_CallHistory.bufferReturnValue.size());

   _fread_nolock_s_CallHistory.bufferSize = bufferSize;
   _fread_nolock_s_CallHistory.elementSize = elementSize;
   _fread_nolock_s_CallHistory.elementCount = elementCount;
   _fread_nolock_s_CallHistory.rawFilePointer = rawFilePointer;
   return _fread_nolock_s_CallHistory.returnValue;
}

void _fread_nolock_s_AssertCalledOnceWith(size_t expectedBufferSize, size_t expectedElementSize, size_t expectedElementCount, FILE* expectedRawFilePointer)
{
   ARE_EQUAL(1, _fread_nolock_s_CallHistory.numberOfCalls);
   ARE_EQUAL(expectedBufferSize, _fread_nolock_s_CallHistory.bufferSize);
   ARE_EQUAL(expectedElementSize, _fread_nolock_s_CallHistory.elementSize);
   ARE_EQUAL(expectedElementCount, _fread_nolock_s_CallHistory.elementCount);
   ARE_EQUAL(expectedRawFilePointer, _fread_nolock_s_CallHistory.rawFilePointer);
}

TEST(ReadFirst256Bytes_ReadsFirst256Bytes_ReturnsPairOfNumberOfBytesReadAnd256ByteCharArray)
{
   const size_t numberOfBytesRead = ZenUnit::Random<size_t>();
   _fread_nolock_s_CallHistory.returnValue = numberOfBytesRead;

   const array<char, 256> bufferReturnValue = ZenUnit::RandomStdArray<char, 256>();
   _fread_nolock_s_CallHistory.bufferReturnValue = bufferReturnValue;

   _call_fread_nolock_sMock.CallInstead(std::bind(
      &FileReaderTests::_fread_nolock_s_CallInstead, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5));
   FILE* const rawFilePointer = tmpfile();
   //
   const pair<size_t, array<char, 256>> numberOfBytesReadAndFirst256FileBytes = _fileReader.ReadFirst256Bytes(rawFilePointer);
   //
   _fread_nolock_s_AssertCalledOnceWith(256, 1, 256, rawFilePointer);
   const pair<size_t, array<char, 256>> expectedNumberOfBytesReadAndFirst256FileBytes = { numberOfBytesRead, bufferReturnValue };
   PAIRS_ARE_EQUAL(expectedNumberOfBytesReadAndFirst256FileBytes, numberOfBytesReadAndFirst256FileBytes);
}

#endif

RUN_TESTS(FileReaderTests)
