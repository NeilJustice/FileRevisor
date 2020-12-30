#pragma once
#include "libFileRevisor/Compiler/IfWindowsIgnoreTheseWarningsGlobally.h"

#include <assert.h>
#include <fstream>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <unordered_set>
namespace fs = std::filesystem;
using namespace std;

#if defined __linux__
   #include <cstddef>
   #include <dirent.h>
   #include <ios>
   #include <memory>
   #include <mutex>
   #if defined(__GNUG__) && !defined(__clang__)
   #include <parallel/algorithm>
   #endif
   #include <string.h>
   #include <unistd.h>
#elif defined _WIN32 || defined __APPLE__
   #pragma warning(push)
   #pragma warning(disable: 4365) // signed/unsigned mismatch
   #include <execution>
   #pragma warning(pop)
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMINMAX
#include "Windows.h"
#endif

// libFileRevisor Constants
#include "libFileRevisor/Constants/ErrnoValue.h"

// libFileRevisor Docopt
#include "libFileRevisor/Components/Docopt/docopt.h"

// libFileRevisor StaticUtilities
#include "libFileRevisor/StaticUtilities/ReleaseAssert.h"
#include "libFileRevisor/StaticUtilities/StringUtil.h"
#include "libFileRevisor/StaticUtilities/Map.h"
#include "libFileRevisor/StaticUtilities/STRING_ENUM_MAP.h"
#include "libFileRevisor/StaticUtilities/Vector.h"
#include "libFileRevisor/StaticUtilities/Type.h"

// libFileRevisor Enums
#include "libFileRevisor/Enums/FileExceptionType.h"
#include "libFileRevisor/Enums/ProgramMode.h"

// libFileRevisor ValueTypes
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"
#include "libFileRevisor/ValueTypes/RenameResult.h"
