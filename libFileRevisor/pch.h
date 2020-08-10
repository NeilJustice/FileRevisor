#pragma once
#include "libFileRevisor/Utilities/MSVCIgnoredWallWarnings.h"

#if defined __linux__
   #include <cstddef>
   #include <dirent.h>
   #include <experimental/filesystem>
   namespace fs = std::experimental::filesystem;
   #include <ios>
   #include <memory>
   #include <mutex>
   #if defined(__GNUG__) && !defined(__clang__)
   #include <parallel/algorithm>
   #endif
   #include <string.h>
   #include <unistd.h>
#elif defined _WIN32 || defined __APPLE__
   #include <execution>
   #include <filesystem>
   namespace fs = std::filesystem;
#endif

#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <sstream>
using namespace std;

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMINMAX
#include "Windows.h"
#endif

#include "libFileRevisor/Utilities/ReleaseAssert.h"
#include "libFileRevisor/Components/Strings/StringUtil.h"
#include "libFileRevisor/Constants/ErrnoValue.h"
#include "libFileRevisor/Components/Docopt/DocoptParser.h"
