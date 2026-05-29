cmake_minimum_required(VERSION 3.11)

project(libVRF
    VERSION 0.1.0
    LANGUAGES CXX
)


set(LIBVRF_VRF_DIR $ENV{MAK_VRFDIR} CACHE STRING "Directory of VR-Forces")
set(LIBVRF_VRL_DIR $ENV{MAK_VRLDIR} CACHE STRING "Directory of VR-Link")
set(LIBVRF_RTI_DIR $ENV{MAK_RTIDIR} CACHE STRING "Directory of RTI")

if (NOT LIBVRF_VRF_DIR)
    message(FATAL_ERROR "Cannot find VR-Forces directory.")
endif()
if (NOT LIBVRF_VRL_DIR)
    message(FATAL_ERROR "Cannot find VR-Link directory.")
endif()
if (NOT LIBVRF_RTI_DIR)
    message(FATAL_ERROR "Cannot find RTI directory.")
endif()


# Classify VR-Link libs into core, DIS, and HLA13
file(GLOB mak_lib_paths_combined
    ${LIBVRF_VRF_DIR}/lib64/*.lib
    ${LIBVRF_VRL_DIR}/lib64/*.lib
    ${LIBVRF_RTI_DIR}/lib/*.lib
)

foreach(lib_path ${mak_lib_paths_combined})
    string(FIND ${lib_path} "boost" result)
    if (NOT ${result} EQUAL -1)
        string(FIND ${lib_path} "-gd" result)
        if (NOT ${result} EQUAL -1)
            list(APPEND boost_dbg_libs ${lib_path})
        else()
            list(APPEND boost_rel_libs ${lib_path})
        endif()
        continue()
    endif()

    string(FIND ${lib_path} "DIS" result)
    if (NOT ${result} EQUAL -1)
        list(APPEND mak_dis_libs ${lib_path})
        continue()
    endif()

    string(FIND ${lib_path} "HLA13" result)
    if (NOT ${result} EQUAL -1)
        list(APPEND mak_hla13_libs ${lib_path})
        continue()
    endif()

    string(FIND ${lib_path} "HLA1516" result)
    if (NOT ${result} EQUAL -1)
        list(APPEND mak_hla1516_libs ${lib_path})
        continue()
    endif()

    string(FIND ${lib_path} "HLA1516e" result)
    if (NOT ${result} EQUAL -1)
        list(APPEND mak_hla1516e_libs ${lib_path})
        continue()
    endif()

    list(APPEND mak_libs ${lib_path})
endforeach()


set(boost_libs
    $<$<CONFIG:Debug>:${boost_dbg_libs}>
    $<$<CONFIG:Release>:${boost_rel_libs}>
    $<$<CONFIG:RelWithDebInfo>:${boost_rel_libs}>
    $<$<CONFIG:MinSizeRel>:${boost_rel_libs}>
)


set(mak_include_dir
    ${LIBVRF_VRF_DIR}/include
    ${LIBVRF_VRL_DIR}/include
    ${LIBVRF_RTI_DIR}/include
)
set(base_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/entt_view.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vector.cpp
)
set(base_include_dirs
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${mak_include_dir}
)
set(base_libs
    ${boost_libs}
    ${mak_libs}
)
set(cpp17_defs
    _HAS_AUTO_PTR_ETC=1
    _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
)


add_library(libvrf_dis STATIC ${base_sources})
add_library(libvrf::dis ALIAS libvrf_dis)
target_compile_definitions(libvrf_dis PUBLIC ${cpp17_defs})
target_include_directories(libvrf_dis PUBLIC ${base_include_dirs})
target_link_libraries(libvrf_dis PUBLIC ${base_libs} ${mak_dis_libs})
set_target_properties(libvrf_dis PROPERTIES FOLDER "libVRF")

add_library(libvrf_hla1516 STATIC ${base_sources})
add_library(libvrf::hla1516 ALIAS libvrf_hla1516)
target_compile_definitions(libvrf_hla1516 PUBLIC ${cpp17_defs})
target_include_directories(libvrf_hla1516 PUBLIC ${base_include_dirs})
target_link_libraries(libvrf_hla1516 PUBLIC ${base_libs} ${mak_hla1516_libs})
set_target_properties(libvrf_hla1516 PROPERTIES FOLDER "libVRF")

add_library(libvrf_hla1516e STATIC ${base_sources})
add_library(libvrf::hla1516e ALIAS libvrf_hla1516e)
target_compile_definitions(libvrf_hla1516e PUBLIC ${cpp17_defs})
target_include_directories(libvrf_hla1516e PUBLIC ${base_include_dirs})
target_link_libraries(libvrf_hla1516e PUBLIC ${base_libs} ${mak_hla1516e_libs})
set_target_properties(libvrf_hla1516e PROPERTIES FOLDER "libVRF")
