cmake_minimum_required(VERSION 3.16)

file(GLOB_RECURSE COSMETIC "Cosmetic/*.cpp")
file(GLOB_RECURSE TRADE "Trade/*.cpp")
file(GLOB_RECURSE MANAGEMENT "ManageSystem/*.cpp")
file(GLOB_RECURSE PRODUCT_MANAGEMENT "CosmeticManagement/*.cpp")

project(CMS)

add_executable(${PROJECT_NAME} main.cpp
    ${COSMETIC}
    ${TRADE}
    ${MANAGEMENT}
    ${PRODUCT_MANAGEMENT}
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    Cosmetic
    Trade
    ManageSystem
    CosmeticManagement
)



