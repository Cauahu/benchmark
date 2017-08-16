###################################
#if or not using c++11 new compiler
###################################
option (FMT_DEBUG "if using debug mode" ON)

###################################
#enable dynamic lib
###################################
option (enable_dynamic "if using debug mode" ON)

##########################################
#install path
#header ~/include
#libs   ~/lib
#exe     ~/bin
##########################################

#set(CMAKE_INSTALL_PREFIX /root/install)

message(STATUS "program will be install to " ${CMAKE_INSTALL_PREFIX})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(CMAKE_VERBOSE_MAKEFILE OFF)

######################################
#for test
option(test "enable test on" ON)
#########################################

##########################################
#do not modify belows
##########################################

set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo)

if(FMT_DEBUG) 
	set(CMAKE_BUILD_TYPE Debug)
else () 
	set(CMAKE_BUILD_TYPE Release) 
endif() 
message(STATUS "program build type " ${CMAKE_BUILD_TYPE}) 
