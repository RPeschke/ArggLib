FIND_PACKAGE( ROOT REQUIRED)
file(GLOB src_files  "src/*.cpp" "src/*.cc" "src/*.cxx")


include_directories(../Include ./inc )
include_directories(include ${ROOT_INCLUDE_DIR})

add_definitions(-DUSE_ROOT)

add_executable(bin_tests_ROOT  ${src_files} )
TARGET_LINK_LIBRARIES(bin_tests_ROOT ${ROOT_LIBRARIES}  ${ADDITIONAL_LIBRARIES})

INSTALL(TARGETS bin_tests_ROOT  
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)