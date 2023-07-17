cmake_minimum_required(VERSION 3.10)

# to skip cmake to test a simple executable on a host
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(at32f105_usb_can_converter C CXX ASM)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

## version information
set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
set(VERSION_MINOR 1 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 0 CACHE STRING "Project patch version number.")


if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
	if (NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
	endif()
endif()


message(STATUS "Building project: ${PROJECT_NAME}")

message(STATUS "Start src cmake")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain.cmake)

set(MCU_ARCH cortex-m3)
set(MCU_NAME STM32F105xC)

# add_compile_options(-mfloat-abi=soft)
add_compile_options(-mcpu=${MCU_ARCH} -mthumb)
add_compile_options(-ffunction-sections -fdata-sections)
add_compile_options(-D${MCU_NAME} -DUSE_HAL_DRIVER -DUSE_STM3210C_EVAL)
# TODO: replace USE_STM3210C_EVAL

file(GLOB APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/app/src/*.c")

file(GLOB HAL_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/cmsis/gcc/startup_stm32f105xc.s"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_cortex.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_can.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_dma.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_exti.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_flash.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_flash_ex.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_gpio.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_gpio_ex.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_pcd.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_pcd_ex.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_pwr.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_rcc.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_rcc_ex.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_tim.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_tim_ex.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_hal_uart.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/stm32f1xx_ll_usb.c"
)

file(GLOB USB_DRIVER_SOURCES
	# "${CMAKE_CURRENT_SOURCE_DIR}/usb/class/src/usbd_hid.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/usb/class/src/usbd_customhid.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/usb/core/src/usbd_core.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/usb/core/src/usbd_ctlreq.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/usb/core/src/usbd_ioreq.c"
)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/include/common"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/include/device"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/include/hal"
	"${CMAKE_CURRENT_SOURCE_DIR}/usb/core/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/usb/class/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/app/include"
)

# print summary of configurations
message(STATUS "MCU_ARCH: ${MCU_ARCH}")
message(STATUS "MCU_NAME: ${MCU_NAME}")
message(STATUS "Specified C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Linker script: ${LINKER_SCRIPT}")

list(APPEND SOURCES
	${APP_SOURCES}
	${HAL_SOURCES}
	${USB_DRIVER_SOURCES}
)

message(STATUS "SOURCES:  ${SOURCES}")

set(LINKER_SCRIPT
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/src/cmsis/gcc/linker/STM32F105XC_FLASH.ld"
)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m3 -mthumb)
add_link_options(-specs=nosys.specs)

message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

target_link_options(${PROJECT_NAME}.elf PRIVATE -T ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${CMAKE_CXX_COMPILER_ID}_${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${CMAKE_CXX_COMPILER_ID}_${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
	COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
	COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}"
)
