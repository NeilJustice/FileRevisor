project(libFileRevisor)
include(${CMAKE_SOURCE_DIR}/CMakeMacros.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_SOURCE_DIR})

folder_source_group(Compiler)
folder_source_group(Components)
folder_source_group_subfolder(Components ErrorHandling)
folder_source_group_subfolder(Components Exceptions)
folder_source_group_subfolder(Components FileRevisor)
folder_source_group_subfolder(Components FileSystem)
folder_source_group_subfolder(Components Math)
folder_source_group_subfolder(Components Process)
folder_source_group_subsubfolder(Components Process Windows)
folder_source_group_subfolder(Components Random)
folder_source_group_subfolder(Components SubPrograms)
folder_source_group_subfolder(Components Utility)
folder_source_group_subsubfolder(Components Utility Assertion)
folder_source_group_subsubfolder(Components Utility Console)
folder_source_group_subsubfolder(Components Utility DataStructures)
folder_source_group_subsubfolder(Components Utility Docopt)
folder_source_group_subsubfolder(Components Utility FunctionCallers)
folder_source_group_subsubsubfolder(Components Utility FunctionCallers Member)
folder_source_group_subsubsubfolder(Components Utility FunctionCallers NonMember)
folder_source_group_subsubsubfolder(Components Utility FunctionCallers TryCatchCallers)
folder_source_group_subsubfolder(Components Utility Iteration)
folder_source_group_subsubsubfolder(Components Utility Iteration Counter)
folder_source_group_subsubsubfolder(Components Utility Iteration ForEach)
folder_source_group_subsubsubfolder(Components Utility Iteration Math)
folder_source_group_subsubsubfolder(Components Utility Iteration Transform)
folder_source_group_subsubfolder(Components Utility Strings)
folder_source_group_subsubfolder(Components Utility Threading)
folder_source_group_subsubfolder(Components Utility Time)
folder_source_group_subfolder(Components Windows)
folder_source_group(Constants)
folder_source_group(Enums)
folder_source_group(Exceptions)
folder_source_group(StaticUtilities)
folder_source_group(ValueTypes)

file(GLOB_RECURSE Sources *.h *.cpp *.ui)
source_group("" FILES ${Sources})

add_library(libFileRevisor ${Sources})
if(UNIX)
   set_target_properties(libFileRevisor PROPERTIES DEBUG_OUTPUT_NAME "FileRevisorDebug")
   set_target_properties(libFileRevisor PROPERTIES RELWITHDEBINFO_OUTPUT_NAME "FileRevisorRelWithDebInfo")
elseif(MSVC)
   set_target_properties(libFileRevisor PROPERTIES DEBUG_OUTPUT_NAME "libFileRevisorDebug")
   set_target_properties(libFileRevisor PROPERTIES RELWITHDEBINFO_OUTPUT_NAME "libFileRevisorRelWithDebInfo")

   replace(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Ob1" "/Ob2")
   append(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Oi /Ot /GS- /arch:AVX2 /fp:fast")
endif()

IfMSVCEnablePrecompiledHeaders()

if(UNIX)
   if(LCovCodeCoverageMode)
     target_link_libraries(libFileRevisor CodeCovLibraryInterface)
   endif()
endif()
