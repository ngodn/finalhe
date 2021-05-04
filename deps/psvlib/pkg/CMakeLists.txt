set (CMAKE_CXX_STANDARD 11)

if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
  endif ()

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()
add_library(vitapkg
    pkg.c pkg.h
    pkg_out.c pkg_out.h
    pkg_utils.h
    pkg_zrif.c pkg_zrif.h)
target_include_directories(vitapkg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(vitapkg scrypto)
if (WIN32)
    target_link_libraries(vitapkg zlibstatic)
else ()
    target_link_libraries(vitapkg ZLIB::ZLIB)
endif ()