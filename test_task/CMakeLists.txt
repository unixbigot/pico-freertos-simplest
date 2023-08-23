
include(../FreeRTOS_Kernel_import.cmake)

add_compile_definitions(APP_NAME="test_task")
add_compile_definitions(APP_VERSION="1.0")
add_compile_definitions(BUILD_NUMBER=${BUILD_NUMBER})

add_executable(test_task
  test_task.cpp
  ../common/Task.cpp
)

target_include_directories(test_task PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/..
            ${CMAKE_CURRENT_LIST_DIR}/../common
            )

# pull in common dependencies
target_link_libraries(test_task pico_stdlib FreeRTOS-Kernel-Heap4)

# enable usb output, disable uart output
pico_enable_stdio_usb(test_task 1)
pico_enable_stdio_uart(test_task 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(test_task)


