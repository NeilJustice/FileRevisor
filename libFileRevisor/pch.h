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
#include <regex>
#include <sstream>
using namespace std;

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMINMAX
#include "Windows.h"
#endif

#include "libFileRevisor/Components/Console/Console.h"
#include "libFileRevisor/Components/DataStructures/Vector.h"
#include "libFileRevisor/Components/Docopt/DocoptParser.h"
#include "libFileRevisor/Components/Exceptions/Exception.h"
#include "libFileRevisor/Components/Exceptions/TryCatchCaller.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/FunctionCallers/Member/NonVoidOneArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/FunctionCallers/Member/NonVoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/FunctionCallers/NonMember/VoidFourArgFunctionCaller.h"
#include "libFileRevisor/Components/Iteration/Counter/PredicateCounter.h"
#include "libFileRevisor/Components/Iteration/ForEach/OneExtraArgMemberForEacher.h"
#include "libFileRevisor/Components/Iteration/ForEach/ParallelOneExtraArgMemberForEacher.h"
#include "libFileRevisor/Components/Iteration/Math/OneExtraArgMemberFunctionSumator.h"
#include "libFileRevisor/Components/Iteration/Transform/OneExtraArgMemberFunctionTransformer.h"
#include "libFileRevisor/Components/Strings/Pluralizer.h"
#include "libFileRevisor/Components/Strings/StringUtil.h"
#include "libFileRevisor/Components/Time/Stopwatch.h"
#include "libFileRevisor/Constants/ErrnoValue.h"
#include "libFileRevisor/Utilities/ReleaseAssert.h"
