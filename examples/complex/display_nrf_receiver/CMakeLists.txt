# This file is part of Arduino_Core_Rudiron.

# Arduino_Core_Rudiron is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Arduino_Core_Rudiron is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with Arduino_Core_Rudiron.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.15.3)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Генератор проекта
set(CMAKE_GENERATOR "Ninja")
#set (CMAKE_GENERATOR "Unix Makefiles" CACHE INTERNAL "" FORCE)
message("Generator is set to ${CMAKE_GENERATOR}")

# Определение пути до библиотеки
string(FIND "$ENV{Arduino_Core_Rudiron_Path}" "Arduino_Core_Rudiron" PROJECT_FOUND)
if (${PROJECT_FOUND} MATCHES "-1")
    message("Arduino_Core_Rudiron_Path NOT FOUND. SPECIFY THE FULL PATH!")
    #при необходимости укажите полный путь к Arduino_Core_Rudiron
    set(ARDUIRON_CORE_RUDIRON_PATH "/../Arduino_Core_Rudiron")

else()
    #путь к проекту берется из системной переменной
    set(ARDUIRON_CORE_RUDIRON_PATH $ENV{Arduino_Core_Rudiron_Path})
endif ()
string(REPLACE "\\" "/" ARDUIRON_CORE_RUDIRON_PATH "${ARDUIRON_CORE_RUDIRON_PATH}")
message(ARDUIRON_CORE_RUDIRON_PATH = "${ARDUIRON_CORE_RUDIRON_PATH}")

# Путь до тулчейна
set(CMAKE_TOOLCHAIN_FILE "${ARDUIRON_CORE_RUDIRON_PATH}/arm-none-eabi-gcc.cmake")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
enable_language(C ASM)

# Название проекта
project(Sketch)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
else()
    message("Build type = ${CMAKE_BUILD_TYPE}")
endif()

# Задание конфигурации платы
set(core M3)
set(cpu_name "MDR32F9Qx")
set(board_name "Rudiron_Buterbrod_R9_16MHz")
# Импорт библиотеки
add_subdirectory("${ARDUIRON_CORE_RUDIRON_PATH}" Arduino_Core_Rudiron)

# Название исполняемого файла
set(OUTPUT_NAME "${PROJECT_NAME}.elf")

# Поиск исходных файлов в корневом каталоге
file(GLOB SKETCH_SOURCES
        *.c
        *.cpp
        )

# Поиск исходных файлов в подкаталоге
file(GLOB SKETCH_SUBFOLDER_SOURCES
#        подкаталог/*
)

# Перечень путей к файлам исходного кода
add_executable(${OUTPUT_NAME}
        ${ARDUIRON_CORE_RUDIRON_SOURCES}
        ${SKETCH_SOURCES}
        ${SKETCH_SUBFOLDER_SOURCES}

        # Добавьте пути поиска исходных файлов здесь
        ${nrf24l01_sources}
        ${SPI_sources}
        ${Wire_sources}
        ${Adafruit_BusIO_sources}
        ${Adafruit_GFX_Library_sources}
        ${Adafruit_ST7735_and_ST7789_Library_sources}
        )

# Перечень путей к файлам заголовков
include_directories(
        ${ARDUIRON_CORE_RUDIRON_INCLUDES}

        # Добавьте пути поиска заголовков здесь
        ${nrf24l01_includes}
        ${SPI_includes}
        ${Wire_includes}
        ${Adafruit_BusIO_includes}
        ${Adafruit_GFX_Library_includes}
        ${Adafruit_ST7735_and_ST7789_Library_includes}
        )

# Флаги компиляции
target_compile_definitions(${OUTPUT_NAME} PUBLIC
	ARDUINO=101
        ${board_name}
        ${cpu_name}
        # USB_IRQ -- включает обработчик прерывания USB
        # HCLK_DISABLE -- работа от внутреннего кварцевого резонатора
        NRF24_USE
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
        "-T${ARDUINO_CORE_RUDIRON_LINKER_FILE}"
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
