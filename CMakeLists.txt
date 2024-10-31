cmake_minimum_required(VERSION 3.10)
project(BashCompiler VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

# Check if running in Termux and set custom install prefix
if(DEFINED ENV{TERMUX_VERSION})
    set(CMAKE_INSTALL_PREFIX "/data/data/com.termux/files/usr")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/data/data/com.termux/files/usr")
endif()

add_executable(bashcompiler src/bashcompiler.cpp)

if(MSVC)
    target_compile_options(bashcompiler PRIVATE /W4)
else()
    target_compile_options(bashcompiler PRIVATE -Wall -Wextra -pedantic)
endif()

install(TARGETS bashcompiler
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES 
    README.md 
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)

set(CPACK_PACKAGE_NAME "bashcompiler")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A compiler tool for Bash scripts")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Taylo")
set(CPACK_PACKAGE_CONTACT "https://github.com/i-Taylo")

# Use the detected system processor for package architecture
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${CMAKE_SYSTEM_PROCESSOR}")

# Define dependencies
set(CPACK_DEBIAN_PACKAGE_DEPENDS "clang")

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

set(CPACK_GENERATOR "DEB")

include(CPack)
