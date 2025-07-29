cmake_minimum_required(VERSION 3.16)

file(GLOB_RECURSE COSMETIC "Cosmetic/*.cpp")
file(GLOB_RECURSE PRODUCT_MANAGEMENT "CosmeticManagement/*.cpp")
file(GLOB_RECURSE TRADE "Trade/*.cpp")
file(GLOB_RECURSE TRADE_MANAGEMENT "TradeManagement/*.cpp")
file(GLOB_RECURSE MANAGEMENT "ManageSystem/*.cpp")

project(CMS)

add_executable(${PROJECT_NAME} main.cpp
    ${COSMETIC}
    ${TRADE}
    ${MANAGEMENT}
    ${PRODUCT_MANAGEMENT}
    ${TRADE_MANAGEMENT}
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    Cosmetic
    Trade
    ManageSystem
    CosmeticManagement
    TradeManagement
)



