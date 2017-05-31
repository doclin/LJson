add_executable(main main.cpp)
add_executable(main_addition main_addition.cpp)
target_link_libraries(main LJson_lib)
target_link_libraries(main_addition LJson_lib)