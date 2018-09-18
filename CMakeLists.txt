cmake_minimum_required(VERSION 3.5)
set(CMAKE_SYSTEM_NAME Generic)
#set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING TRUE)

# force compiler settings
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# force cmake compilers
set(CMAKE_ASM_COMPILER    "arm-none-eabi-gcc")
set(CMAKE_C_COMPILER      "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER    "arm-none-eabi-g++")
set(ELF2BIN               "arm-none-eabi-objcopy")

# here starts the project
project(humanoid-mbed C CXX ASM)
set(CMAKE_CXX_STANDARD 11)

set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

include("${PROJECT_SOURCE_DIR}/cmake/mbed_directories.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/mbed_files.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/mbed_flags.cmake")

# uncomment below to have a verbose build process
# set(CMAKE_VERBOSE_MAKEFILE ON)

set(LD_SYS_LIBS "-Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys  -Wl,--end-group")

set(CMAKE_C_FLAGS "-std=gnu11 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer -Os -g1 -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=softfp -include mbed_config.h")
set(CMAKE_CXX_FLAGS "-std=gnu++11 -fno-rtti -Wvla -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer -Os -g1 -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=softfp  -include mbed_config.h")
set(CMAKE_ASM_FLAGS "-x assembler-with-cpp -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fmessage-length=0 -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections -funsigned-char -MMD -fno-delete-null-pointer-checks -fomit-frame-pointer -Os -g1 -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=softfp  -include mbed_config.h")
set(CMAKE_CXX_LINK_FLAGS "-Wl,--gc-sections -Wl,--wrap,main -Wl,--wrap,__malloc_r -Wl,--wrap,__free_r -Wl,--wrap,__realloc_r -Wl,--wrap,__memalign_r -Wl,--wrap,__calloc_r -Wl,--wrap,exit -Wl,--wrap,atexit -Wl,-n -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=softfp ")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${LD_SYS_LIBS} -T ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_pp.link_script.ld")

add_definitions(${MBED_FLAGS})

include_directories(
  ${MBED_DIRECTORIES}
  ${LIB_DIR}/XYZrobotServo
  ${LIB_DIR}/BufferSerial
  ${SRC_DIR}/utils
)

set(SOURCE_FILES
  ${MBED_FILES}
  ${LIB_DIR}/XYZrobotServo/XYZrobotServo.cpp
  ${LIB_DIR}/BufferSerial/BufferSerial.cpp
  ${SRC_DIR}/utils/ServoDiagnostics.cpp
  ${SRC_DIR}/utils/Sinusoidal.cpp
  ${SRC_DIR}/Cluster.cpp
  ${SRC_DIR}/DataController.cpp
  ${SRC_DIR}/Controll.cpp
  ${SRC_DIR}/config.h
  ${SRC_DIR}/init.h
  ${SRC_DIR}/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_custom_command(TARGET ${PROJECT_NAME}
	POST_BUILD COMMAND arm-none-eabi-objcopy
	-O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin)

set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS 1)

# add syslibs dependencies to create the correct linker order
target_link_libraries(${PROJECT_NAME} -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys)

add_custom_command(TARGET ${PROJECT_NAME} PRE_LINK
  COMMAND "arm-none-eabi-cpp" -E -P -Wl,--gc-sections -Wl,--wrap,main -Wl,--wrap,_malloc_r -Wl,--wrap,_free_r -Wl,--wrap,_realloc_r -Wl,--wrap,_memalign_r -Wl,--wrap,_calloc_r -Wl,--wrap,exit -Wl,--wrap,atexit -Wl,-n -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=softfp ${LIB_DIR}/mbed-os/targets/TARGET_STM/TARGET_STM32F7/TARGET_STM32F767xI/device/TOOLCHAIN_GCC_ARM/STM32F767xI.ld -o ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_pp.link_script.ld
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_pp.link_script.ld"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${ELF2BIN} -O ihex $<TARGET_FILE:${PROJECT_NAME}> $<TARGET_FILE:${PROJECT_NAME}>.hex
  COMMAND ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE:${PROJECT_NAME}>.hex"
)
