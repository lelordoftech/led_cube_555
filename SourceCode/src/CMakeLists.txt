cmake_minimum_required (VERSION 3.5.1)
set (PROJECT_NAME cube555)
project (${PROJECT_NAME})

# create avr hex
MACRO(AVR_CREATE_HEX name)
ADD_CUSTOM_COMMAND(TARGET ${name} POST_BUILD COMMAND avr-objcopy ARGS -O ihex -R.eeprom
   ${name}
   ../Exe/${PROJECT_NAME}.hex)
ENDMACRO(AVR_CREATE_HEX)

SET(CMAKE_SYSTEM_NAME "Generic")
SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)
SET(CSTANDARD "-std=gnu99")
SET(CDEBUG "-gstabs")
SET(CWARN "-Wall -Wstrict-prototypes")
#SET(CTUNING "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
SET(COPT "-Os")
SET(CMCU "-mmcu=atmega16")
#SET(CDEFS "-DF_CPU=16000000UL")
SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA}")
SET(CXXFLAGS "${CMCU} ${CDEFS} ${CINCS} ${COPT}")
SET(CMAKE_C_FLAGS  ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})

#target_include_directories (Hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set ( MAIN_FILE
   effect.c
   character.c
   main.c)
add_executable (${PROJECT_NAME}.obj ${MAIN_FILE})

AVR_CREATE_HEX(${PROJECT_NAME}.obj)
