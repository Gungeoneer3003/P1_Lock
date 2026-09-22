# Fixed STM32L476RG / NUCLEO-L476RG platform support.

set(PLATFORM_DEFINITIONS
    USE_NUCLEO_64
    USE_HAL_DRIVER
    STM32L476xx
    $<$<CONFIG:Debug>:DEBUG>
)

set(PLATFORM_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/Core/Inc
    ${PROJECT_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc
    ${PROJECT_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy
    ${PROJECT_SOURCE_DIR}/Drivers/BSP/STM32L4xx_Nucleo
    ${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32L4xx/Include
    ${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Include
)

set(PLATFORM_APPLICATION_SOURCES
    ${PROJECT_SOURCE_DIR}/Core/Src/main.c
    ${PROJECT_SOURCE_DIR}/Core/Src/stm32l4xx_it.c
    ${PROJECT_SOURCE_DIR}/Core/Src/stm32l4xx_hal_msp.c
    ${PROJECT_SOURCE_DIR}/Core/Src/sysmem.c
    ${PROJECT_SOURCE_DIR}/Core/Src/syscalls.c
    ${PROJECT_SOURCE_DIR}/startup_stm32l476xx.s
)

set(PLATFORM_DRIVER_SOURCES
    ${PROJECT_SOURCE_DIR}/Core/Src/system_stm32l4xx.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
    ${PROJECT_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c
)

add_library(stm32_platform INTERFACE)
target_include_directories(stm32_platform INTERFACE ${PLATFORM_INCLUDE_DIRS})
target_compile_definitions(stm32_platform INTERFACE ${PLATFORM_DEFINITIONS})

add_library(stm32_drivers OBJECT ${PLATFORM_DRIVER_SOURCES})
target_link_libraries(stm32_drivers PUBLIC stm32_platform)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${PLATFORM_APPLICATION_SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE stm32_drivers ${TOOLCHAIN_LINK_LIBRARIES})

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    ADDITIONAL_CLEAN_FILES "${CMAKE_PROJECT_NAME}.map"
)
