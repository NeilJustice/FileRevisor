#pragma once
#include "libFileRevisor/Compiler/IfWindowsIgnoreTheseWarningsGlobally.h"

#include <array>
#include <execution>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#if defined __linux__ || defined __APPLE__
   #include <cstddef>
   #include <dirent.h>
   #include <ios>
   #include <memory>
   #include <mutex>
   #include <string.h>
   #include <thread>
   #include <unistd.h>
#endif
namespace fs = std::filesystem;
using namespace std;

#if defined _WIN32
   #define WIN32_LEAN_AND_MEAN
   #define NOGDI
   #define NOMINMAX
   #include "Windows.h"
#endif

// libFileRevisor Constants
#include "libFileRevisor/Constants/ErrnoValue.h"

// libFileRevisor Enums
#include "libFileRevisor/Enums/FileExceptionType.h"
#include "libFileRevisor/Enums/ProgramMode.h"

// libFileRevisor StaticUtilities
#include "libFileRevisor/StaticUtilities/ENUM_STRING_ARRAY.h"
#include "libFileRevisor/StaticUtilities/ReleaseAssert.h"
#include "libFileRevisor/StaticUtilities/StringUtil.h"
#include "libFileRevisor/StaticUtilities/Vector.h"
#include "libFileRevisor/StaticUtilities/Type.h"

// libFileRevisor UtilityComponents
#include "libFileRevisor/UtilityComponents/Console/Console.h"

// libFileRevisor ValueTypes
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"
#include "libFileRevisor/ValueTypes/RenameResult.h"
