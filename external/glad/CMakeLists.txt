project(Glad LANGUAGES C)

add_library(glad glad.h glad.c)
target_include_directories(glad PUBLIC include/)