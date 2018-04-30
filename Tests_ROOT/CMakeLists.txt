FIND_PACKAGE( ROOT REQUIRED)




file(GLOB src_files  "src/*.cpp" "src/*.cc" "src/*.cxx")


include_directories(../Include ./inc )
include_directories(include ${ROOT_INCLUDE_DIR})

add_definitions(-DUSE_ROOT)




set(Make_Dict_name MakeDict)
set(TEST_ROOT_HEADERS C:/ArggsComputerEcke/ArggLib/Tests_ROOT/inc/signal_slot_test.hh)
set(LINKDEF_FILE      C:/ArggsComputerEcke/ArggLib/Tests_ROOT/inc/signal_slot_test_LinkDef.hh)
set(SCT_DICTIONARY root_dict.cxx)
add_custom_target(${Make_Dict_name}
   ${ROOT_CINT_EXECUTABLE} -f ${SCT_DICTIONARY} -c -p  ${TEST_ROOT_HEADERS} ${LINKDEF_FILE} 
)

add_executable(bin_tests_ROOT  ${src_files} ${SCT_DICTIONARY} )
add_dependencies(bin_tests_ROOT ${Make_Dict_name})


TARGET_LINK_LIBRARIES(bin_tests_ROOT ${ROOT_LIBRARIES}  ${ADDITIONAL_LIBRARIES})

INSTALL(TARGETS bin_tests_ROOT  
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)