# All rights reserved.
#
cmake_minimum_required(VERSION 3.13)
set(SOURCE_TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BINARY_TOP_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_C_COMPILER_FORCED   1)
set(CMAKE_CXX_COMPILER_FORCED 1)
set(WITH_WERROR 1)
set(out_hex_dir ${CMAKE_CURRENT_BINARY_DIR}/hex)
set(out_lib_dir ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(out_inc_dir ${CMAKE_CURRENT_BINARY_DIR}/include)
set(tools_dir ${CMAKE_CURRENT_SOURCE_DIR}/tools)
set(aboot_dir  ${SOURCE_TOP_DIR}/../../Tools/aboot)
set(module_version $ENV{module_version})
set(core_stub_o ${CMAKE_CURRENT_SOURCE_DIR}/../kernel/${module_version}/core_stub/core_stub.o)
set(flash_ldscript ${SOURCE_TOP_DIR}/ldscripts/app_flashimg.ld)
set(cmd_mkappimg dtools mkappimg)
set(ctool_sign ctools appSign)
include(cmake/core_config.cmake)
include(cmake/toolchain-gcc.cmake)
include(cmake/extension.cmake)
set(BUILD_SHARED_LIBS OFF)
project(core C CXX ASM)
if($ENV{para_flash_size} MATCHES "16")
	add_definitions(-DBOARD_FLASH_SIZE_16M)
elseif($ENV{para_flash_size} MATCHES "8")
    add_definitions(-DBOARD_FLASH_SIZE_8M)
elseif($ENV{para_flash_size} MATCHES "4")
    set(flash_ldscript ${SOURCE_TOP_DIR}/ldscripts/app_flashimg_xip.ld)
    add_definitions(-DBOARD_FLASH_SIZE_4M)
endif()
include_directories(
    components/inc
    ../kernel/${module_version}/inc
    components/newlib/include
    components/lwip
    components/lwip/lwip
    components/lwip/arch
    components/lwip/ipv4
    components/lwip/ipv6
    components/lwip/netif
    components/mbedtls/include
    components/cislvgl/include
    components/net/pahomqtt/inc
    components/net/json/cJSON
    components/inc/lcd
)
function(cpp_only target file)
    add_library(${target} OBJECT ${file})
    set_source_files_properties(${file} PROPERTIES LANGUAGE C)
    target_compile_options(${target} PRIVATE -E -P -x c)
endfunction()
function(add_appimg target ldscript)
    set(gen_ldscript ${target}_ldscript)
    set(target_map_file ${out_hex_dir}/${target}.map)
    set(target_img_file ${out_hex_dir}/${target}.bin)
    cpp_only(${gen_ldscript} ${ldscript})
    add_executable(${target} ${ARGN} ${core_stub_o})
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${out_hex_dir})
    target_link_libraries(${target} PRIVATE -T $<TARGET_OBJECTS:${gen_ldscript}>)
    target_link_libraries(${target} PRIVATE -Wl,-Map=${target_map_file} -nostdlib -Wl,--gc-sections)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${cmd_mkappimg} $<TARGET_FILE:${target}> ${target_img_file}
        BYPRODUCTS ${target_img_file} ${target_map_file}
    )
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${ctool_sign} -p ASR -t ${aboot_dir} ${target_img_file}
        DEPENDS ${target_img_file}
        WORKING_DIRECTORY ${SOURCE_TOP_DIR}
    )
endfunction()
message("Board Flash Size: $ENV{para_flash_size}M")
add_subdirectory_if_exist(components)
add_subdirectory_if_exist(app)
set(target customer_app)
add_appimg(${target} ${flash_ldscript}  examples/soundbox.c)
target_link_libraries(${target} PRIVATE paytm_sdk_api)
# target_link_libraries(${target} PRIVATE ${libc_file_name} ${libm_file_name} ${libgcc_file_name})
