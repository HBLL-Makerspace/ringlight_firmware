cmake_minimum_required(VERSION 3.23)
project(TEST)

add_compile_options(-Wall -Werror)
set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions(UNIT_TEST)

set(INCLUDE_DIR include)
set(TEST_SRCS 
  test/comm_send_frame.c
  test/drivers/usart.c

  src/comm/comm_handler.c
  src/comm/comm_parser.c
  src/commands/commands.c
)

add_executable(test ${TEST_SRCS})
target_include_directories(test PUBLIC ${INCLUDE_DIR})