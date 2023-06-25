cmake_minimum_required(VERSION 3.16)

project(RISCV C CXX)

find_package(SDL2 REQUIRED)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE COMMON_SRCS CONFIGURE_DEPENDS ${SRC_DIR}/common/*.c ${SRC_DIR}/common/*.cpp)
file(GLOB_RECURSE SIMULATED_SRCS CONFIGURE_DEPENDS ${SRC_DIR}/simulated/*.c ${SRC_DIR}/simulated/*.cpp)

set(CHARMAP_BINARY ${CMAKE_BINARY_DIR}/charmap.bin)
set(CHARMAP_MIF ${SRC_DIR}/common/charmap.mif)
add_custom_command(
  OUTPUT ${CHARMAP_BINARY}
  DEPENDS ${CHARMAP_MIF} ${CMAKE_SOURCE_DIR}/scripts/mif_to_binary.py
  COMMAND python ${CMAKE_SOURCE_DIR}/scripts/mif_to_binary.py ${CHARMAP_MIF} ${CHARMAP_BINARY}
)

set(ICMC_RAM_MIF ${SRC_DIR}/common/icmc_ram.mif)
set(ICMC_RAM_CPP ${CMAKE_BINARY_DIR}/icmc_ram.cpp)
add_custom_command(
  OUTPUT ${ICMC_RAM_CPP}
  DEPENDS ${ICMC_RAM_MIF} ${CMAKE_SOURCE_DIR}/scripts/mif_to_c.py
  COMMAND python ${CMAKE_SOURCE_DIR}/scripts/mif_to_c.py ${ICMC_RAM_MIF} ${ICMC_RAM_CPP}
)


add_executable(RISCVSimulator ${COMMON_SRCS} ${SIMULATED_SRCS} ${CHARMAP_BINARY} ${ICMC_RAM_CPP})
target_include_directories(RISCVSimulator PRIVATE ${SDL2_INCLUDE_DIRS} ${SRC_DIR})
target_link_libraries(RISCVSimulator ${SDL2_LIBRARIES})
target_compile_definitions(RISCVSimulator PRIVATE RISCV_SIMULATED CHARMAP_FILE="${CHARMAP_BINARY}")
