cmake_minimum_required(VERSION 3.10)
project(BashCompiler VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

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

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.29), libstdc++6 (>= 9)")

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

set(CPACK_GENERATOR "DEB")

include(CPack)
