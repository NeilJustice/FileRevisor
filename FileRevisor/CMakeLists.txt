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
if(UNIX)
   target_link_libraries(FileRevisor stdc++fs)
endif()

install(TARGETS FileRevisor DESTINATION "bin")

if(UNIX)
   if(CodeCovMode)
     target_link_libraries(FileRevisor CodeCovLibraryInterface)
   endif()
endif()
