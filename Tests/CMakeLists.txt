file(GLOB src_files  "src/*.cpp" "src/*.cc" "src/*.cxx")
include_directories(../include ./inc )

add_executable(bin_tests  ${src_files} )

INSTALL(TARGETS bin_tests  
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)