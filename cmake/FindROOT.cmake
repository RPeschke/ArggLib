# - Find ROOT instalation
# This module tries to find the ROOT installation on your system.
# It tries to find the root-config script which gives you all the needed information.
# If the system variable ROOTSYS is set this is straight forward.
# If not the module uses the pathes given in ROOT_CONFIG_SEARCHPATH.
# If you need an other path you should add this path to this varaible.  
# The root-config script is then used to detect basically everything else.
# This module defines a number of key variables and macros.

# F.Uhlig@gsi.de (fairroot.gsi.de)


MESSAGE(STATUS "Looking for Root...")

SET(ROOT_CONFIG_SEARCHPATH
  ${SIMPATH}/tools/root/bin
  $ENV{ROOTSYS}/bin
  /opt/local/bin
  /root/bin
)

SET(ROOT_DEFINITIONS "")

SET(ROOT_INSTALLED_VERSION_TOO_OLD FALSE)

SET(ROOT_CONFIG_EXECUTABLE ROOT_CONFIG_EXECUTABLE-NOTFOUND)

FIND_PROGRAM(ROOT_CONFIG_EXECUTABLE NAMES root-config PATHS
   ${ROOT_CONFIG_SEARCHPATH}
   NO_DEFAULT_PATH)
    
IF (${ROOT_CONFIG_EXECUTABLE} MATCHES "ROOT_CONFIG_EXECUTABLE-NOTFOUND")
  MESSAGE( FATAL_ERROR "ROOT not installed in the searchpath and ROOTSYS is not set. Please
 set ROOTSYS or add the path to your ROOT installation in the Macro FindROOT.cmake in the
 subdirectory cmake/modules.")
ELSE (${ROOT_CONFIG_EXECUTABLE} MATCHES "ROOT_CONFIG_EXECUTABLE-NOTFOUND")
  STRING(REGEX REPLACE "(^.*)/bin/root-config" "\\1" test ${ROOT_CONFIG_EXECUTABLE}) 
  SET( ENV{ROOTSYS} ${test})
  set( ROOTSYS ${test})
ENDIF (${ROOT_CONFIG_EXECUTABLE} MATCHES "ROOT_CONFIG_EXECUTABLE-NOTFOUND")  

# root config is a bash script and not commonly executable under Windows
# make some static assumptions instead
IF (WIN32)
  SET(ROOT_FOUND FALSE)
  IF (ROOT_CONFIG_EXECUTABLE)
    SET(ROOT_FOUND TRUE)
    set(ROOT_INCLUDE_DIR ${ROOTSYS}/include)
    set(ROOT_LIBRARY_DIR ${ROOTSYS}/lib)
    SET(ROOT_BINARY_DIR ${ROOTSYS}/bin)
    set(ROOT_LIBRARIES ${ROOT_LIBRARY_DIR}/*.lib )
    FIND_PROGRAM(ROOT_CINT_EXECUTABLE
      NAMES rootcint
      PATHS ${ROOT_BINARY_DIR}
      NO_DEFAULT_PATH
      )
    MESSAGE(STATUS "Found ROOT: $ENV{ROOTSYS}/bin/root (WIN32/version not identified)")
  ENDIF (ROOT_CONFIG_EXECUTABLE)
  
ELSE(WIN32)

  IF (ROOT_CONFIG_EXECUTABLE)
    
    SET(ROOT_FOUND FALSE)

    EXEC_PROGRAM(${ROOT_CONFIG_EXECUTABLE} ARGS "--version" OUTPUT_VARIABLE ROOTVERSION)

    MESSAGE(STATUS "Found ROOT: $ENV{ROOTSYS}/bin/root (found version ${ROOTVERSION})")

    # we need at least version 5.00/00
    IF (NOT ROOT_MIN_VERSION)
      SET(ROOT_MIN_VERSION "5.00/00")
    ENDIF (NOT ROOT_MIN_VERSION)
    
    # now parse the parts of the user given version string into variables
    STRING(REGEX REPLACE "^([0-9]+)\\.[0-9][0-9]+\\/[0-9][0-9]+" "\\1" req_root_major_vers "${ROOT_MIN_VERSION}")
    STRING(REGEX REPLACE "^[0-9]+\\.([0-9][0-9])+\\/[0-9][0-9]+.*" "\\1" req_root_minor_vers "${ROOT_MIN_VERSION}")
    STRING(REGEX REPLACE "^[0-9]+\\.[0-9][0-9]+\\/([0-9][0-9]+)" "\\1" req_root_patch_vers "${ROOT_MIN_VERSION}")
    
    # and now the version string given by qmake
    STRING(REGEX REPLACE "^([0-9]+)\\.[0-9][0-9]+\\/[0-9][0-9]+.*" "\\1" found_root_major_vers "${ROOTVERSION}")
    STRING(REGEX REPLACE "^[0-9]+\\.([0-9][0-9])+\\/[0-9][0-9]+.*" "\\1" found_root_minor_vers "${ROOTVERSION}")
    STRING(REGEX REPLACE "^[0-9]+\\.[0-9][0-9]+\\/([0-9][0-9]+).*" "\\1" found_root_patch_vers "${ROOTVERSION}")

    IF (found_root_major_vers LESS 5)
      MESSAGE( FATAL_ERROR "Invalid ROOT version \"${ROOTERSION}\", at least major version 4 is required, e.g. \"5.00/00\"")
    ENDIF (found_root_major_vers LESS 5)

    # compute an overall version number which can be compared at once
    MATH(EXPR req_vers "${req_root_major_vers}*10000 + ${req_root_minor_vers}*100 + ${req_root_patch_vers}")
    MATH(EXPR found_vers "${found_root_major_vers}*10000 + ${found_root_minor_vers}*100 + ${found_root_patch_vers}")
    
    IF (found_vers LESS req_vers)
      SET(ROOT_FOUND FALSE)
      SET(ROOT_INSTALLED_VERSION_TOO_OLD TRUE)
    ELSE (found_vers LESS req_vers)
      SET(ROOT_FOUND TRUE)
    ENDIF (found_vers LESS req_vers)

  ENDIF (ROOT_CONFIG_EXECUTABLE)


  IF (ROOT_FOUND)

    # ask root-config for the library dir
    # Set ROOT_LIBRARY_DIR

    EXEC_PROGRAM( ${ROOT_CONFIG_EXECUTABLE}
      ARGS "--libdir"
      OUTPUT_VARIABLE ROOT_LIBRARY_DIR_TMP )

    IF(EXISTS "${ROOT_LIBRARY_DIR_TMP}")
      SET(ROOT_LIBRARY_DIR ${ROOT_LIBRARY_DIR_TMP} )
    ELSE(EXISTS "${ROOT_LIBRARY_DIR_TMP}")
      MESSAGE("Warning: ROOT_CONFIG_EXECUTABLE reported ${ROOT_LIBRARY_DIR_TMP} as library path,")
      MESSAGE("Warning: but ${ROOT_LIBRARY_DIR_TMP} does NOT exist, ROOT must NOT be installed correctly.")
    ENDIF(EXISTS "${ROOT_LIBRARY_DIR_TMP}")
    
    # ask root-config for the binary dir
    EXEC_PROGRAM(${ROOT_CONFIG_EXECUTABLE}
      ARGS "--bindir"
      OUTPUT_VARIABLE root_bins )
    SET(ROOT_BINARY_DIR ${root_bins})

    # ask root-config for the include dir
    EXEC_PROGRAM( ${ROOT_CONFIG_EXECUTABLE}
      ARGS "--incdir" 
      OUTPUT_VARIABLE root_headers )
    SET(ROOT_INCLUDE_DIR ${root_headers})
    # CACHE INTERNAL "")

    # ask root-config for the library varaibles
    EXEC_PROGRAM( ${ROOT_CONFIG_EXECUTABLE}
      #    ARGS "--noldflags --noauxlibs --libs" 
      ARGS "--glibs" 
      OUTPUT_VARIABLE root_flags )

    #  STRING(REGEX MATCHALL "([^ ])+"  root_libs_all ${root_flags})
    #  STRING(REGEX MATCHALL "-L([^ ])+"  root_library ${root_flags})
    #  REMOVE_FROM_LIST(root_flags "${root_libs_all}" "${root_library}")

    SET(ROOT_LIBRARIES "${root_flags} -lMinuit")

    # Make variables changeble to the advanced user
    MARK_AS_ADVANCED( ROOT_LIBRARY_DIR ROOT_INCLUDE_DIR ROOT_DEFINITIONS)

    # Set ROOT_INCLUDES
    SET( ROOT_INCLUDES ${ROOT_INCLUDE_DIR})

    SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${ROOT_LIBRARY_DIR})

    #######################################
    #
    #       Check the executables of ROOT 
    #          ( rootcint ) 
    #
    #######################################

    FIND_PROGRAM(ROOT_CINT_EXECUTABLE
      NAMES rootcint
      PATHS ${ROOT_BINARY_DIR}
      NO_DEFAULT_PATH
      )

  ENDIF (ROOT_FOUND)
ENDIF(WIN32)


IF(ROOT_FOUND AND NOT TARGET CERN_ROOT)
	add_library (CERN_ROOT INTERFACE)
	target_include_directories(CERN_ROOT INTERFACE  ${ROOT_INCLUDE_DIR})
	TARGET_LINK_LIBRARIES(CERN_ROOT INTERFACE ${ROOT_LIBRARIES} )
	target_compile_definitions(CERN_ROOT INTERFACE USE_ROOT)
ENDIF(ROOT_FOUND AND NOT TARGET CERN_ROOT)


  ###########################################
  #
  #       Macros for building ROOT dictionary
  #
  ###########################################

MACRO (ROOT_GENERATE_DICTIONARY_OLD )
 
   set(INFILES "")    

   foreach (_current_FILE ${ARGN})

     IF (${_current_FILE} MATCHES "^.*\\.h$")
       IF (${_current_FILE} MATCHES "^.*Link.*$")
         set(LINKDEF_FILE ${_current_FILE})
       ELSE (${_current_FILE} MATCHES "^.*Link.*$")
         set(INFILES ${INFILES} ${_current_FILE})
       ENDIF (${_current_FILE} MATCHES "^.*Link.*$")
     ELSEIF (${_current_FILE} MATCHES "^.*\\.hh$")
       IF (${_current_FILE} MATCHES "^.*Link.*$")
         set(LINKDEF_FILE ${_current_FILE})
       ELSE (${_current_FILE} MATCHES "^.*Link.*$")
         set(INFILES ${INFILES} ${_current_FILE})
       ENDIF (${_current_FILE} MATCHES "^.*Link.*$")
     ELSE (${_current_FILE} MATCHES "^.*\\.h$")
       IF (${_current_FILE} MATCHES "^.*\\.cxx$")
         set(OUTFILE ${_current_FILE})
       ELSE (${_current_FILE} MATCHES "^.*\\.cxx$")
         set(INCLUDE_DIRS ${INCLUDE_DIRS} -I${_current_FILE})   
       ENDIF (${_current_FILE} MATCHES "^.*\\.cxx$")
     ENDIF (${_current_FILE} MATCHES "^.*\\.h$")
     
   endforeach (_current_FILE ${ARGN})
   
#  MESSAGE("INFILES: ${INFILES}")
#  MESSAGE("OutFILE: ${OUTFILE}")
#  MESSAGE("LINKDEF_FILE: ${LINKDEF_FILE}")
#  MESSAGE("INCLUDE_DIRS: ${INCLUDE_DIRS}")

   STRING(REGEX REPLACE "(^.*).cxx" "\\1.h" bla "${OUTFILE}")
#   MESSAGE("BLA: ${bla}")
   SET (OUTFILES ${OUTFILE} ${bla})

   ADD_CUSTOM_COMMAND(OUTPUT ${OUTFILES}
      COMMAND ${ROOT_CINT_EXECUTABLE}
      ARGS -f ${OUTFILE} -c -p -I${INCLUDE_DIRS} ${INFILES} ${LINKDEF_FILE} )

#   MESSAGE("ROOT_CINT_EXECUTABLE has created the dictionary ${OUTFILE}")

ENDMACRO (ROOT_GENERATE_DICTIONARY_OLD)

  ###########################################
  #
  #       Macros for building ROOT dictionary
  #
  ###########################################

MACRO (ROOT_GENERATE_DICTIONARY INFILES LINKDEF_FILE OUTFILE INCLUDE_DIRS_IN)
 
  set(INCLUDE_DIRS)

  foreach (_current_FILE ${INCLUDE_DIRS_IN})
    set(INCLUDE_DIRS ${INCLUDE_DIRS} -I${_current_FILE})   
  endforeach (_current_FILE ${INCLUDE_DIRS_IN})
 

#  MESSAGE("INFILES: ${INFILES}")
#  MESSAGE("OutFILE: ${OUTFILE}")
#  MESSAGE("LINKDEF_FILE: ${LINKDEF_FILE}")
#  MESSAGE("INCLUDE_DIRS: ${INCLUDE_DIRS}")

  STRING(REGEX REPLACE "^(.*)\\.(.*)$" "\\1.h" bla "${OUTFILE}")
#  MESSAGE("BLA: ${bla}")
  SET (OUTFILES ${OUTFILE} ${bla})


  if (CMAKE_SYSTEM_NAME MATCHES Linux)
    ADD_CUSTOM_COMMAND(OUTPUT ${OUTFILES}
       COMMAND LD_LIBRARY_PATH=${ROOT_LIBRARY_DIR} ROOTSYS=${ROOTSYS} ${ROOT_CINT_EXECUTABLE}
       ARGS -f ${OUTFILE} -c -DHAVE_CONFIG_H ${INCLUDE_DIRS} ${INFILES} ${LINKDEF_FILE} DEPENDS ${INFILES} ${LINKDEF_FILE})
  else (CMAKE_SYSTEM_NAME MATCHES Linux)
    if (CMAKE_SYSTEM_NAME MATCHES Darwin)
      ADD_CUSTOM_COMMAND(OUTPUT ${OUTFILES}
       COMMAND DYLD_LIBRARY_PATH=${ROOT_LIBRARY_DIR} ROOTSYS=${ROOTSYS} ${ROOT_CINT_EXECUTABLE}
       ARGS -f ${OUTFILE} -c -DHAVE_CONFIG_H ${INCLUDE_DIRS} ${INFILES} ${LINKDEF_FILE} DEPENDS ${INFILES} ${LINKDEF_FILE})
    endif (CMAKE_SYSTEM_NAME MATCHES Darwin)
  endif (CMAKE_SYSTEM_NAME MATCHES Linux)

ENDMACRO (ROOT_GENERATE_DICTIONARY)

MACRO (GENERATE_ROOT_TEST_SCRIPT SCRIPT_FULL_NAME)

  get_filename_component(path_name ${SCRIPT_FULL_NAME} PATH)
  get_filename_component(file_extension ${SCRIPT_FULL_NAME} EXT)
  get_filename_component(file_name ${SCRIPT_FULL_NAME} NAME_WE)
  set(shell_script_name "${file_name}.sh")

  #MESSAGE("PATH: ${path_name}")
  #MESSAGE("Ext: ${file_extension}")
  #MESSAGE("Name: ${file_name}")
  #MESSAGE("Shell Name: ${shell_script_name}")

  string(REPLACE ${PROJECT_SOURCE_DIR} 
         ${PROJECT_BINARY_DIR} new_path ${path_name}
        )

  #MESSAGE("New PATH: ${new_path}")

  file(MAKE_DIRECTORY ${new_path}/data)

  CONVERT_LIST_TO_STRING(${LD_LIBRARY_PATH})
  set(MY_LD_LIBRARY_PATH ${output})
  set(my_script_name ${SCRIPT_FULL_NAME})

  if(CMAKE_SYSTEM MATCHES Darwin)
    configure_file(${PROJECT_SOURCE_DIR}/cmake/scripts/root_macro_macos.sh.in
                   ${new_path}/${shell_script_name}
                  )
  else(CMAKE_SYSTEM MATCHES Darwin)
    configure_file(${PROJECT_SOURCE_DIR}/cmake/scripts/root_macro.sh.in
                   ${new_path}/${shell_script_name}
                  )
  endif(CMAKE_SYSTEM MATCHES Darwin)

  EXEC_PROGRAM(/bin/chmod ARGS "u+x  ${new_path}/${shell_script_name}")

ENDMACRO (GENERATE_ROOT_TEST_SCRIPT)


FUNCTION(PREPEND var prefix)
   SET(listVar "")
   FOREACH(f ${ARGN})
      LIST(APPEND listVar "${prefix}/${f}")
   ENDFOREACH(f)
   SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION()


function(root_dict_counter outCounter)
	GET_PROPERTY(VAR GLOBAL PROPERTY root_dict_counter_var)
if (CMAKE_SYSTEM_NAME MATCHES Linux)
	set(VAR "1")
else()

	MATH(EXPR VAR "${VAR}+1")
endif()
	set(${outCounter}  ${VAR} PARENT_SCOPE)
	SET_PROPERTY(GLOBAL PROPERTY root_dict_counter_var ${VAR})
endfunction()

function(ADD_GEN_ROOT_DICT1 Make_Dict_name dict_file headers_input LINKDEF_FILE)
add_custom_target(${Make_Dict_name}
 ${ROOT_CINT_EXECUTABLE} -f ${dict_file} -c -p -I${ROOT_INCLUDE_DIR} ${ARGN}  ${headers_input} ${LINKDEF_FILE} 
)
set(incs ${ARGN})
message(${incs})

endfunction()


function(ADD_GEN_ROOT_DICT TargetName headers_input LINKDEF_FILE MY_INCLUDE_DIRECTORIES)
message("'${MY_INCLUDE_DIRECTORIES}'")
#PREPEND(MY_INCLUDE_DIRECTORIES " -I" ${MY_INCLUDE_DIRECTORIES})
root_dict_counter(c1)
set(Make_Dict_name "${TargetName}_dict_${c1}")


set(dict_file  ${CMAKE_CURRENT_BINARY_DIR}/${Make_Dict_name}.cxx)
file(WRITE ${dict_file} "" )
set(dict_file_pcm_short ${Make_Dict_name}_rdict.pcm)
set(dict_file_pcm  ${CMAKE_CURRENT_BINARY_DIR}/${dict_file_pcm_short})
file(WRITE ${dict_file_pcm} "" )

#set(incImpl "")
foreach(tarInc ${MY_INCLUDE_DIRECTORIES})
list(APPEND  incImpl "-I${tarInc}")
#set(incImpl "${incImpl} -I${tarInc}")
endforeach(tarInc)
message(${incImpl})

#add_custom_target(${Make_Dict_name}
# ${ROOT_CINT_EXECUTABLE} -f ${dict_file} -c -p -I${ROOT_INCLUDE_DIR} "${incImpl}" ${headers_input} ${LINKDEF_FILE} 
#)
ADD_GEN_ROOT_DICT1(${Make_Dict_name} ${dict_file}   ${headers_input} ${LINKDEF_FILE} ${incImpl})
add_dependencies(${TargetName} ${Make_Dict_name})
target_sources(${TargetName} PRIVATE  ${dict_file})
source_group(dictionary FILES  ${dict_file} )
#MESSAGE(STATUS "<PROJECT_SOURCE_DIR> ${PROJECT_SOURCE_DIR}</PROJECT_SOURCE_DIR>")

if (CMAKE_SYSTEM_NAME MATCHES Linux)
	add_custom_command(TARGET ${TargetName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${dict_file_pcm}" "${PROJECT_SOURCE_DIR}/lib/${dict_file_pcm_short}" 
	)
else (CMAKE_SYSTEM_NAME MATCHES Linux) #windows
	add_custom_command(TARGET ${TargetName} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${dict_file_pcm}" "${PROJECT_SOURCE_DIR}/bin/${dict_file_pcm_short}"
	)
endif (CMAKE_SYSTEM_NAME MATCHES Linux)
endfunction()



function(ADD_GEN_ROOT_DICT_PREPEND TargetName headers_input LINKDEF_FILE MY_INCLUDE_DIRECTORIES)
PREPEND(headers_input  "${CMAKE_CURRENT_SOURCE_DIR}/" ${headers_input})
PREPEND(LINKDEF_FILE   "${CMAKE_CURRENT_SOURCE_DIR}/" ${LINKDEF_FILE})

ADD_GEN_ROOT_DICT(${TargetName} "${headers_input}" ${LINKDEF_FILE} ${MY_INCLUDE_DIRECTORIES} )
endfunction()


function(ADD_GEN_ROOT_DICT_AUTO TargetName MY_INCLUDE_DIRECTORIES)
#PREPEND(MY_INCLUDE_DIRECTORIES " -I" ${MY_INCLUDE_DIRECTORIES})


file(GLOB_RECURSE headers_input "*.hpp")
file(GLOB_RECURSE LINKDEF_FILE "*LinkDef.hh")

ADD_GEN_ROOT_DICT(${TargetName} "${headers_input}" "${LINKDEF_FILE}" "${MY_INCLUDE_DIRECTORIES}")

endfunction()

FUNCTION(FIND_ROOT_OBJECTS header_input patter closingPattern outputVar)
set(linkDef_content "")
foreach(loop_var ${headers_input})
	FILE(READ "${loop_var}" contents)
	FOREACH(line ${contents})
		STRING(FIND "${line}" ${patter} matchres)
		If( NOT(${matchres}  EQUAL -1))
			string(SUBSTRING ${line} ${matchres} 600 substr)
			STRING(FIND "${substr}" " " matchres)
			string(SUBSTRING ${substr} ${matchres}  600 substr)
			STRING(FIND "${substr}" "${closingPattern}" matchres)
			string(SUBSTRING ${substr} 1  ${matchres} substr)
			STRING( REPLACE "${closingPattern}" " " substr ${substr})
			list(APPEND  linkDef_content ${substr})
		ENDIF()

	endforeach(line)
	
    
endforeach(loop_var)
set(${outputVar} ${linkDef_content} PARENT_SCOPE)
ENDFUNCTION()


function(ADD_GEN_ROOT_DICT_AUTO_LINK TargetName MY_INCLUDE_DIRECTORIES)
message("'${MY_INCLUDE_DIRECTORIES}'")
#PREPEND(MY_INCLUDE_DIRECTORIES " -I" ${MY_INCLUDE_DIRECTORIES})


file(GLOB_RECURSE headers_input "*.hpp")

FIND_ROOT_OBJECTS("${headers_input}" "ROOTCLASS" "{" ROOTCLASSES )

	


FIND_ROOT_OBJECTS("${headers_input}" "ROOTFUNCTION" "\(" ROOTFUNCTIONs )


FIND_ROOT_OBJECTS("${headers_input}" "ROOTFUNCTION operator" ";" ROOToperators1 )


FIND_ROOT_OBJECTS("${headers_input}" "ROOTFUNCTION  operator" ";" ROOToperators2 )

FIND_ROOT_OBJECTS("${headers_input}" "ROOTFUNCTION   operator" ";" ROOToperators3 )


set(LINKDEF_FILE "${CMAKE_CURRENT_BINARY_DIR}/autoLinkDef.hh")
file(WRITE ${LINKDEF_FILE} "#ifdef __CINT__ \n" )

foreach(VAR ${ROOTCLASSES})
	file(APPEND  ${LINKDEF_FILE} "#pragma link C++ class  ${VAR}; \n" )
endforeach(VAR)

foreach(VAR ${ROOTFUNCTIONs})
	STRING(FIND "${VAR}" "operator" matchres)
	if(${matchres} EQUAL -1)
		file(APPEND  ${LINKDEF_FILE} "#pragma link C++ function  ${VAR}; \n" )
	endif()
endforeach(VAR)

foreach(VAR ${ROOToperators1})
	file(APPEND  ${LINKDEF_FILE} "#pragma link C++ function  ${VAR}; \n" )
endforeach(VAR)
foreach(VAR ${ROOToperators2})
	file(APPEND  ${LINKDEF_FILE} "#pragma link C++ function  ${VAR}; \n" )
endforeach(VAR)
foreach(VAR ${ROOToperators3})
	file(APPEND  ${LINKDEF_FILE} "#pragma link C++ function  ${VAR}; \n" )
endforeach(VAR)

file(APPEND  ${LINKDEF_FILE} "#endif //__CINT__ \n" )
#file(GLOB_RECURSE LINKDEF_FILE "*LinkDef.hh")



ADD_GEN_ROOT_DICT(${TargetName} "${headers_input}" "${LINKDEF_FILE}" "${MY_INCLUDE_DIRECTORIES}" )

endfunction()