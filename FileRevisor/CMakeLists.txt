project(FileRevisor)
include(${CMAKE_SOURCE_DIR}/CMakeMacros.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE Sources *.h *.cpp)
source_group("" FILES ${Sources})

add_executable(FileRevisor ${Sources})
if(UNIX)
   set_target_properties(FileRevisor PROPERTIES OUTPUT_NAME filerevisor)
endif()

target_link_libraries(FileRevisor libFileRevisor)
if(UNIX AND NOT APPLE)
   target_link_libraries(FileRevisor stdc++fs)
endif()
if(MSVC)
   replace(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Ob1" "/Ob2")
   append(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Oi /Ot /GS- /arch:AVX2 /fp:fast")
   replace(CMAKE_EXE_LINKER_FLAGS_DEBUG "/debug" "/DEBUG:FASTLINK")
   replace(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "/debug" "/DEBUG:FASTLINK")
endif()

install(TARGETS FileRevisor DESTINATION "bin")

if(UNIX AND NOT APPLE)
   if(LCovCodeCoverageMode)
     target_link_libraries(FileRevisor CodeCovLibraryInterface)
   endif()
endif()
