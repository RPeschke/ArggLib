file(GLOB src_files  "src/*.cpp" "src/*.cc" "src/*.cxx")


add_executable(bin_tests  ${src_files} )
target_link_libraries(bin_tests PUBLIC ArggLib)


INSTALL(TARGETS bin_tests  
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)