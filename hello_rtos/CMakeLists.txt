
include(../FreeRTOS_Kernel_import.cmake)

add_executable(hello_rtos
  hello_rtos.c
)

target_include_directories(hello_rtos PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/..
            )

# pull in common dependencies
target_link_libraries(hello_rtos pico_stdlib FreeRTOS-Kernel-Heap4)

# enable usb output, disable uart output
pico_enable_stdio_usb(hello_rtos 1)
pico_enable_stdio_uart(hello_rtos 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_rtos)


