FIND_PACKAGE( ROOT REQUIRED)




file(GLOB src_files  "src/*.cpp" "src/*.cc" "src/*.cxx")


include_directories(../Include ./inc )
include_directories(include ${ROOT_INCLUDE_DIR})

add_definitions(-DUSE_ROOT)




set(Make_Dict_name MakeDict)
set(TEST_ROOT_HEADERS  ${CMAKE_CURRENT_SOURCE_DIR}/inc/signal_slot_test.hh)
set(LINKDEF_FILE       ${CMAKE_CURRENT_SOURCE_DIR}/inc/signal_slot_test_LinkDef.hh)
set(MY_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/Include )
set(SCT_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/root_dict.cxx)
file(WRITE ${SCT_DICTIONARY} "" )
add_custom_target(${Make_Dict_name}
   ${ROOT_CINT_EXECUTABLE} -f ${SCT_DICTIONARY} -c -p -I${MY_INCLUDE_DIRECTORIES} ${TEST_ROOT_HEADERS} ${LINKDEF_FILE} 
)

add_executable(bin_tests_ROOT  ${src_files} ${SCT_DICTIONARY} )
add_dependencies(bin_tests_ROOT ${Make_Dict_name})


TARGET_LINK_LIBRARIES(bin_tests_ROOT ${ROOT_LIBRARIES}  ${ADDITIONAL_LIBRARIES})

INSTALL(TARGETS bin_tests_ROOT  
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)