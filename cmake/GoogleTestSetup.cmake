####################################################################################
##
##     _____    ____ _       ____ _        __ _      __ _  __ _  ______ _   ___ _
##    / /| |]  |  __ \\     / ___ \\      / \ \\    |   \\/   |]|  _____|] / ___|]
##   / //| |]  | |] \ \\   | |]  \_|]    / //\ \\   | |\ / /| |]| |]___ _ ( ((_ _
##  / //_| |]_ | |]  ) ))  | |]  __ _   / _____ \\  | |]\_/ | |]|  _____|] \___ \\
## |_____   _|]| |]_/ //   | |]__/  |] / //    \ \\ | |]    | |]| |]___ _   ___) ))
##       |_|]  |_____//     \_____/|]]/_//      \_\\|_|]    |_|]|_______|] |____//
##       
##
####################################################################################
##
##
##  File    : CMakeLists.txt
##  Author  : Anthony Islas
##  Purpose : A set of useful functions to setup a Google Test framework
##  Group   : cmake
##  
##  NOTE    : It assumed the framework will be laid out as such 
##        
##            src/_
##                 |__module0/_
##                             |_Foo.hpp
##                             |_Foo.cpp
##                             |_Bar.hpp
##                             |_Bar.cpp
##                             |_gtest/_
##                                      |_Foo[Test Suffix].cpp 
##                                      |_Bar[Test Suffix].cpp 
##
##            And use of built-in vairables :
##              TEST_SOURCES
##              TEST_INCLUDES
##              TEST_RESOURCES
##
##  License : GNU GPL v3
##
####################################################################################


####################################################################################
#
# GTest Vars
#
####################################################################################
if ( THIRDPARTY )

  set ( GTEST_ROOT     
        ${THIRDPARTY}/googletest/googletest 
      )
  set ( GTEST_LIB      
        ${THIRDPARTY}/lib/libgtest.a 
      )
  set ( GTEST_LIB_MAIN 
        ${THIRDPARTY}/lib/libgtest_main.a
      )

else()

  message( AUTHOR "No thirdparty provided" )

endif()

####################################################################################
####################################################################################
#
# FUNCTIONS
#
####################################################################################
####################################################################################


####################################################################################
#
# \brief Takes in a string and capitalizes the first letter
#
# \param testStr is both the input and output string
#
# \return Will place the capitalized string in testStr
#
####################################################################################
function( capitalize VAR_STRING )

  string( SUBSTRING ${${VAR_STRING}} 0 1 FIRST_LETTER )

  string( TOUPPER ${FIRST_LETTER} FIRST_LETTER )

  string( REGEX REPLACE "^.(.*)" "${FIRST_LETTER}\\1" ${VAR_STRING} "${${VAR_STRING}}" )

  set( ${VAR_STRING} ${${VAR_STRING}} PARENT_SCOPE )

endfunction( capitalize )


####################################################################################
#
# \brief Automates the include of test folder and propogation of test variables 
#
####################################################################################
function( include_gtest_folder folder_name )

  

endfunction( include_gtest_folder )


####################################################################################
#
# \brief Allows the user to create resource files for certain tests to be placed
#        in the install path of the test target
#
# \param cur_target is the target which depends on these test resources
# \param res_files are the resource files to use
#
# \return Will take the resource file and copy them into the TEST_RESOURCE location
#         where the cur_target may use them. This ONLY copies them, thus they must 
#         exist. This only to simplify scoping of where support files are 
#
####################################################################################
function ( setup_resources cur_target res_files )

  if ( NOT EXISTS ${TEST_RESOURCES} )
    add_custom_command ( TARGET ${cur_target} 
                         POST_BUILD
                         COMMAND mkdir ${TEST_RESOURCES}
                         COMMAND echo "Built resource directory" 
                       )
  endif ()
  foreach ( res_file ${res_files} )
    string( REGEX MATCH "([a-zA-Z0-9]|_)+\\..*" simple_res ${res_file} )
    if ( NOT EXISTS ${new_res} )
      message( STATUS "Making resource ${simple_res}" ) 
      add_custom_command ( TARGET ${TARGET_NAME}
                           POST_BUILD
                           COMMAND cp ${res_file} ${TEST_RESOURCES}/
                           COMMAND echo "       Copied ${simple_res} to ${TEST_RESOURCES}"
                         )
    endif ()
  endforeach()
    
endfunction ()

####################################################################################
#
# \brief Takes a path and auto-generates a test name based on the path
#
# \param path is the path where the test files are
# \param test_name shall be set in the PARENT SCOPE 
#
# \return Will place the auto-generated test name in test_name using the following
#         format : ${name of folder above this one}Tests
#         e.g. src/module0/gtest/ will get Module0Tests
#
####################################################################################
function( auto_test_name PATH TEST_NAME )

  get_filename_component( ${TEST_NAME} 
                          ${PATH}
                          ABSOLUTE
                          )

  string( REGEX REPLACE ".*/([a-zA-Z]+)/gtest$" "\\1" ${TEST_NAME} ${${TEST_NAME}} )
  # string( REGEX MATCH "(\\w+)\\/gtest$" ${TEST_NAME} ${${TEST_NAME}} )

  capitalize( ${TEST_NAME} )

  set( ${TEST_NAME} ${${TEST_NAME}} PARENT_SCOPE )

endfunction( auto_test_name )


####################################################################################
#
# \brief Assigns a defined test name as a macro expansion in the file to automate
#        Test Case naming.
#
# \param test_files is the set of test files to assign this test case to
# \param test_suffix is the [Test Suffix] used to distinguish between src and test
# \param test_macro is what the C/C++ macro should be defined as ( e.g. TEST_CASE )
# \param test_name is the test name that these tests belong to
#  
# \return Will add to each test_file a CXX compile flag at the PARENT SCOPE in the 
#         form "-D${test_macro}=${test_name}${test file w/o suffix}" which 
#         should be used as the test case name for the entire file. From there
#         users should define each TEST() uniquely. E.g. TEST( TEST_CASE, FooTest )
#
####################################################################################
function( auto_test_case test_files test_suffix test_macro test_name )



endfunction( auto_test_case )


