cmake_minimum_required(VERSION 3.15.3)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Путь до тулчейна
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake")

# Генератор проекта
set(CMAKE_GENERATOR "Ninja")
#set (CMAKE_GENERATOR "Unix Makefiles" CACHE INTERNAL "" FORCE)
message("Generator is set to ${CMAKE_GENERATOR}")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
enable_language(C ASM)

message(${CMAKE_TOOLCHAIN_FILE})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
else()
    message("Build type = ${CMAKE_BUILD_TYPE}")
endif()

# Название проекта
project(Sketch)

# Задание конфигурации платы
set(core M3)
set(cpu_name "MDR32F9Qx")
set(board_name "Rudiron_Buterbrod_R9_16MHz")
# Название исполняемого файла
set(OUTPUT_NAME "${PROJECT_NAME}.elf")

# Sources
set(cmsis_path "${CMAKE_SOURCE_DIR}/system/Drivers/CMSIS/Device/MILANDR/${cpu_name}")
set(cmsis_include_path "${cmsis_path}/Include")
file(GLOB_RECURSE cmsis_include_sources
        ${cmsis_include_path}/*
)

set(cmsis_source_path "${cmsis_path}/Source")
set(cmsis_source_templates_path "${cmsis_source_path}/Templates")
file(GLOB_RECURSE cmsis_source_sources
        ${cmsis_source_path}/*
        ${cmsis_source_templates_path}/*
)

set(cmsis_startup_path "${cmsis_source_path}/Templates/gcc")
file(GLOB_RECURSE cmsis_startup_sources
        ${cmsis_startup_path}/*.S
)

set(std_periph_driver_path "${CMAKE_SOURCE_DIR}/system/Drivers/${cpu_name}_StdPeriph_Driver")
set(std_periph_driver_inc_path "${std_periph_driver_path}/inc")
set(std_periph_driver_inc_usb_path "${std_periph_driver_inc_path}/USB_Library")
set(std_periph_driver_src_path "${std_periph_driver_path}/src")
set(std_periph_driver_src_usb_path "${std_periph_driver_src_path}/USB_Library")
file(GLOB_RECURSE std_periph_driver_sources
        ${std_periph_driver_inc_path}/*
        ${std_periph_driver_inc_usb_path}/*
        ${std_periph_driver_src_path}/*
        ${std_periph_driver_src_usb_path}/*
)

set(system_path "${CMAKE_SOURCE_DIR}/system/${cpu_name}")
file(GLOB_RECURSE system_sources
        ${system_path}/*
)

set(sketch_path "${CMAKE_SOURCE_DIR}/sketch")
file(GLOB_RECURSE sketch_source
        ${sketch_path}/*
)

# Путь до скрипта линковщика
set(LINKER_FILE "${cmsis_startup_path}/${cpu_name}.ld")

# Перечень путей к файлам исходного кода
add_executable(${OUTPUT_NAME}
        ${system_sources}
        ${cmsis_source_sources}
        ${cmsis_startup_sources}
        ${cmsis_include_sources}
        ${std_periph_driver_sources}

        # Добавьте пути поиска исходных файлов здесь
        ${sketch_source}
        )

# Перечень путей к файлам заголовков
include_directories(
        ${cmsis_include_path}
        ${cmsis_source_path}
        ${cmsis_source_templates_path}
        ${cmsis_startup_path}
        ${std_periph_driver_inc_path}
        ${std_periph_driver_inc_usb_path}
        ${std_periph_driver_src_path}
        ${std_periph_driver_src_usb_path}
        ${system_path}

        # Добавьте пути поиска заголовков здесь
        ${sketch_path}
        )

# Флаги компиляции
target_compile_definitions(${OUTPUT_NAME} PUBLIC

        )

# Параметры компиляции
target_compile_options(${OUTPUT_NAME} PRIVATE
        "-mcpu=cortex-${core}"
        "-DUSE_${cpu_name}"
        "-D__STARTUP_CLEAR_BSS"
        "-D_start=main"
        "-pipe"
        "-mthumb"
        "-ffunction-sections"
        "-fdata-sections"
         "-flto" # включает оптимизацию линковщика
        # "-fno-rtti" # отключает динамическую идентификацию типов данных
        "-fno-exceptions" # отключает обработку исключений с++
        )

# Уровни оптимизации
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    target_compile_options(${OUTPUT_NAME} PRIVATE
            "-Og"
            "-g"
            )
else()
    target_compile_options(${OUTPUT_NAME} PRIVATE
            "-O2"
            )
endif()

# Параметры компоновки
target_link_options(${OUTPUT_NAME} PRIVATE
        "-T${LINKER_FILE}"
        "-mcpu=cortex-${core}"
        "-Wl,--gc-sections"
        "-specs=nano.specs"
        "-ffreestanding"#Не делать предположений о значениях имен функций из стандартной библиотеки
        # "-nostartfiles"#Предотвращает использование компилятором стандартных файлов запуска при компоновке
        "-Wl,--print-memory-usage"
        )

# Постобработка прошивки
add_custom_command(TARGET ${OUTPUT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O srec --srec-len=64 ${OUTPUT_NAME} ${PROJECT_NAME}.s19
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${OUTPUT_NAME} ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${OUTPUT_NAME} ${PROJECT_NAME}.bin)

# Вывод информации о размере памяти для продвинутых пользователей
# add_custom_command(TARGET ${OUTPUT_NAME}
# POST_BUILD
# COMMAND ${CMAKE_SIZE_UTIL} ${OUTPUT_NAME})
