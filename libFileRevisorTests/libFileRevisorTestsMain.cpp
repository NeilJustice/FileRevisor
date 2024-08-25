#include "pch.h"

//#include <iostream>
//#include <regex>
//
//int main()
//{
//   //const string input = R"(abc C:\folder\file.txt def)";
//   //const string regexPattern = R"(C:\folder\file.txt)";
//
//   const string input = R"(abc . def .)";
//   const string regexPattern = R"(.)";
//
//   const string escapedRegexPattern = EscapeRegex(regexPattern);
//   const regex escapedRegexPatternRegex(escapedRegexPattern);
//
//   const string replacedInput = regex_replace(input, escapedRegexPatternRegex, R"(banana)");
//   cout << replacedInput << '\n';
//
//   return 0;
//}

int main(int argc, char* argv[])
{
   const int exitCode = ZenUnit::RunTests(argc, argv);
   return exitCode;
}
