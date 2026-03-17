##======================================================================================================================
## Common compiler options
##======================================================================================================================
add_library(onodrim_opts INTERFACE)

target_compile_features ( onodrim_opts INTERFACE  cxx_std_20 )

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
      target_compile_options( onodrim_opts INTERFACE /W3 /WX /EHsc )
  else()
    target_compile_options( onodrim_opts INTERFACE -Wshadow -Werror -Wall -Wextra -Wunused-variable -Wdocumentation)
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options( onodrim_opts INTERFACE /W3 /WX /EHsc /Zc:preprocessor)
else()
  target_compile_options( onodrim_opts INTERFACE -Wshadow -Werror -Wall -Wextra -Wunused-variable)
endif()

##======================================================================================================================
## Compiler options for main program
##======================================================================================================================
add_library(kumi_main INTERFACE)

# Inherit the standard options
target_link_libraries(kumi_main INTERFACE kumi_opts)

# Include directories for your main program
target_include_directories(kumi_main INTERFACE
                            ${PROJECT_SOURCE_DIR}/include
                          )

# Example: define the executable and link the interface
add_executable(main_exec main.cpp)
target_link_libraries(main_exec PRIVATE kumi_main)
