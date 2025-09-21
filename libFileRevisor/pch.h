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
#if defined __linux__
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
#include <io.h> // _isatty()
#include "Windows.h"
#endif

// libFileRevisor Constants
#include "libFileRevisor/Constants/ErrnoValue.h"
#include "libFileRevisor/Enums/Color.h"
#include "libFileRevisor/Enums/FileExceptionType.h"
#include "libFileRevisor/Enums/ProgramMode.h"
#include "libFileRevisor/Enums/WindowsColor.h"
// libFileRevisor Components
#include "libFileRevisor/Components/Console/Console.h"
// libFileRevisor StaticUtilities
#include "libFileRevisor/StaticUtilities/ENUM_STRING_ARRAY.h"
#include "libFileRevisor/StaticUtilities/ReleaseAssert.h"
#include "libFileRevisor/StaticUtilities/StringUtil.h"
#include "libFileRevisor/StaticUtilities/Vector.h"
#include "libFileRevisor/StaticUtilities/Type.h"
// libFileRevisor ValueTypes
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"
#include "libFileRevisor/ValueTypes/RenameResult.h"
