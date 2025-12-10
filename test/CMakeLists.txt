
enable_testing(true)
find_package(Qt6 REQUIRED COMPONENTS Test Widgets Network)


add_executable(Weather_test
    test_src/weatherservice_test.cpp
    test_src/mainwindow_test.cpp
    test_src/weatherservice_test.hpp
    test_src/mainwindow_test.hpp
)

target_include_directories(Weather_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(Weather_test PRIVATE
    WeatherWidget
    Qt6::Widgets
    Qt6::Network
    Qt6::Test
)

add_test(NAME Weather_Tests COMMAND Weather_test)
