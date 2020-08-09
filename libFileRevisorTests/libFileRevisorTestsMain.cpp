#include "pch.h"

//pair<DWORD, string> GetWindowsLastErrorWithDescription()
//{
//   const DWORD lastError = GetLastError();
//   char lastErrorDescriptionChars[256]{};
//   string lastErrorDescription;
//   if (lastError != 0)
//   {
//      const DWORD numberOfMessageCharacters = FormatMessage(
//         FORMAT_MESSAGE_FROM_SYSTEM,
//         nullptr,
//         lastError,
//         0,
//         lastErrorDescriptionChars,
//         sizeof(lastErrorDescriptionChars),
//         nullptr);
//      lastErrorDescription.assign(
//         lastErrorDescriptionChars, lastErrorDescriptionChars + numberOfMessageCharacters);
//   }
//   const pair<DWORD, string> windowsLastErrorWithDescription(lastError, lastErrorDescription);
//   return windowsLastErrorWithDescription;
//}

//#include <direct.h>
//#include "libFileRevisor/Components/Exceptions/ErrorCodeTranslator.h"

int main(int argc, char* argv[])
{
   //const int rmdirReturnValue = _rmdir(R"(C:\Test\Test1)");
   //const int errnoValue = *_errno();
   //cout << "rmdirReturnValue: " << rmdirReturnValue << '\n';
   //cout << "      errnoValue: " << errnoValue << '\n';
   //ErrorCodeTranslator errorCodeTranslator;
   //const string description = errorCodeTranslator.GetErrnoDescription(41);
   //cout << description << '\n';

   //SetFileAttributesA(R"(C:\Test1\File.txt)", 0);
   //const pair<DWORD, string> windowsLastError = GetWindowsLastErrorWithDescription();
   //cout << windowsLastError.first << ": " << windowsLastError.second;

   const int exitCode = ZenUnit::RunTests(argc, argv);
   return exitCode;
}
