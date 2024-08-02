project(model_loading)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS ./*.cpp)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE renderer)