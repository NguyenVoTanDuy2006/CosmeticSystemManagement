cmake_minimum_required(VERSION 3.16)

file(GLOB_RECURSE COMESTIC "Comestic/*.cpp")
file(GLOB_RECURSE TRADE "Trade/*.cpp")
file(GLOB_RECURSE MANAGEMENT "ManageSystem/*.cpp")

project(CMS)

add_executable(${PROJECT_NAME} main.cpp
    ${COMESTIC}
    ${TRADE}
    ${MANAGEMENT}
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    Comestic
    Trade
    ManageSystem)



