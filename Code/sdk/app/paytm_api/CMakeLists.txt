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

if(LCD_EN)
file(GLOB LIBS_PATH "${PROJECT_SOURCE_DIR}/app/paytm_api/lib/LCD/*.a")
elseif(BT_EN)
file(GLOB LIBS_PATH "${PROJECT_SOURCE_DIR}/app/paytm_api/lib/BT/*.a")
elseif(PKT_EN)
file(GLOB LIBS_PATH "${PROJECT_SOURCE_DIR}/app/paytm_api/lib/PKT/*.a")
else()
file(GLOB LIBS_PATH "${PROJECT_SOURCE_DIR}/app/paytm_api/lib/LTE/*.a")
endif()
target_link_libraries(${target} PUBLIC ${LIBS_PATH} httpclient json pahomqtt )
target_link_libraries(${target} PRIVATE ${libc_file_name} ${libm_file_name} ${libgcc_file_name})

target_include_directories(${target} PUBLIC inc)
target_sources(${target} PUBLIC
	src/app_main.c
	src/server_ca_cert.c

	src/demo/ali_mqtt_demo.c
	src/demo/audio_demo.c
	src/demo/battery_demo.c
	src/demo/button_demo.c
	src/demo/devSys_demo.c
	src/demo/filesystem_demo.c
	src/demo/http_demo.c 
	src/demo/json_demo.c
	src/demo/led_demo.c
	src/demo/mqtt_demo.c
	src/demo/net_demo.c
	src/demo/ntp_demo.c
	src/demo/sim_demo.c
	src/demo/tasks_demo.c
	src/demo/timer_demo.c
	src/mqtt_demo_1.c
	)

if(BT_EN)
target_sources(${target} PUBLIC
	src/demo/bt_audio_demo.c
	)
endif()
