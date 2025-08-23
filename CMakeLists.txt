cmake_minimum_required(VERSION 3.16)
project(CMS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.9.1/mingw_64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")

file(GLOB_RECURSE COSMETIC "Cosmetic/*.cpp")
file(GLOB_RECURSE PRODUCT_MANAGEMENT "CosmeticManagement/*.cpp")
file(GLOB_RECURSE TRADE "Trade/*.cpp")
file(GLOB_RECURSE TRADE_MANAGEMENT "TradeManagement/*.cpp")
file(GLOB_RECURSE MANAGEMENT "ManageSystem/*.cpp")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Test)

add_executable(${PROJECT_NAME} main.cpp
    ${COSMETIC}
    ${TRADE}
    ${MANAGEMENT}
    ${PRODUCT_MANAGEMENT}
    ${TRADE_MANAGEMENT}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    Cosmetic
    Trade
    ManageSystem
    CosmeticManagement
    TradeManagement
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/Zc:__cplusplus")
endif()


enable_testing()


# Tạo một file thực thi riêng cho việc test
add_executable(runTests
    ${COSMETIC}
    ${TRADE}
    ${MANAGEMENT}
    ${PRODUCT_MANAGEMENT}
    ${TRADE_MANAGEMENT}
    test_trademanagement.cpp
)
target_include_directories(
    runTests PUBLIC
    Cosmetic
    Trade
    ManageSystem
    CosmeticManagement
    TradeManagement
)

# Liên kết các thư viện cần thiết cho target test
target_link_libraries(runTests PRIVATE Qt6::Core Qt6::Test)

# Thêm test vào CTest
add_test(NAME TradeManagementTests COMMAND runTests)
