FIND_PACKAGE( ROOT REQUIRED)




file(GLOB src_files  "src/*.cpp" "src/*.cc" "src/*.cxx")





add_definitions(-DUSE_ROOT)






add_executable(bin_tests_ROOT  ${src_files} ${SCT_DICTIONARY} )
target_include_directories(bin_tests_ROOT PUBLIC  inc/)
TARGET_LINK_LIBRARIES(bin_tests_ROOT  PUBLIC ArggLib CERN_ROOT)
get_target_property(MyIncludes bin_tests_ROOT  INCLUDE_DIRECTORIES)
get_target_property(MyIncludes1 ArggLib  INTERFACE_INCLUDE_DIRECTORIES)
MESSAGE("${MyIncludes}")
MESSAGE("${MyIncludes1}")
LIST(APPEND MyIncludes ${MyIncludes1})
MESSAGE("${MyIncludes}")
ADD_GEN_ROOT_DICT_AUTO_LINK(bin_tests_ROOT   "${MyIncludes}")
#ADD_GEN_ROOT_DICT(bin_tests_ROOT   inc/signal_slot_test.hh  inc/signal_slot_test_LinkDef.hh -I${PROJECT_SOURCE_DIR}/Include)





INSTALL(TARGETS bin_tests_ROOT  
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)