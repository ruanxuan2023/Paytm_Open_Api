# Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.


set(target paytm_sdk_api)
add_app_libraries($<TARGET_FILE:${target}>)

add_library(${target} STATIC ${dummy_c_file})
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_lib_dir})

file(GLOB LIBS_PATH "${PROJECT_SOURCE_DIR}/app/paytm_api/lib/*.a")
target_link_libraries(${target} PUBLIC ${LIBS_PATH} httpclient pahomqtt)
target_link_libraries(${target} PRIVATE ${libc_file_name} ${libm_file_name} ${libgcc_file_name})

target_include_directories(${target} PUBLIC inc)
target_sources(${target} PUBLIC
	src/app_main.c
	src/server_ca_cert.c
)
