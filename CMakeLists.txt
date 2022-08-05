project(stealth_logging)
cmake_minimum_required(VERSION 3.0)

add_library(stealth_logging stealth_logging.h stealth_timer.h stealth_logging.cpp)

install(FILES stealth_logging.h stealth_timer.h DESTINATION include)
install(TARGETS stealth_logging DESTINATION lib)
