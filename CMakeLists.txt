cmake_minimum_required(VERSION 3.16)

# Tên project mới
project(COSMETIC_MANAGEMENT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ==== Thư mục con ====
set(PRODUCT_DIR   "${CMAKE_SOURCE_DIR}/Product")
set(TRADE_DIR     "${CMAKE_SOURCE_DIR}/Trade")
set(WIDGETS_DIR   "${CMAKE_SOURCE_DIR}/Widget Card")   # có khoảng trắng -> để trong ""
set(ROUTER_DIR    "${CMAKE_SOURCE_DIR}/Router")
set(LOGIN_DIR     "${CMAKE_SOURCE_DIR}/LOGIN+TRANGCHU/UiUx")

# ==== App sources (target/exe mang tên mới) ====
qt_add_executable(COSMETIC_MANAGEMENT
    main.cpp

    # Trade (LSNX)
    "${TRADE_DIR}/LSNX.cpp"       "${TRADE_DIR}/LSNX.h"       "${TRADE_DIR}/LSNX.ui"

    # Product (SP)
    "${PRODUCT_DIR}/sp.cpp"       "${PRODUCT_DIR}/sp.h"       "${PRODUCT_DIR}/SP.ui"

    # Widgets
    "${WIDGETS_DIR}/TradeCardWidget.cpp"  "${WIDGETS_DIR}/TradeCardWidget.h"
    "${WIDGETS_DIR}/spcardwidget.cpp"     "${WIDGETS_DIR}/spcardwidget.h"
    "${WIDGETS_DIR}/flowlayout.cpp"       "${WIDGETS_DIR}/flowlayout.h"

    # Router
    "${ROUTER_DIR}/app_router.h"          # nếu có .cpp thì thêm vào

    # Login + Main Menu
    "${LOGIN_DIR}/loginpage.cpp"          "${LOGIN_DIR}/loginpage.h"          "${LOGIN_DIR}/login_form_gradient.ui"
    "${LOGIN_DIR}/mainmenu.cpp"           "${LOGIN_DIR}/mainmenu.h"           "${LOGIN_DIR}/MainMenu.ui"

    # Resources
    icon.qrc
)

# ==== BACKEND ====
set(BACKEND_DIR "${CMAKE_SOURCE_DIR}/BACKEND")
target_include_directories(COSMETIC_MANAGEMENT PRIVATE
    "${BACKEND_DIR}"
    "${BACKEND_DIR}/Cosmetic"
    "${BACKEND_DIR}/CosmeticManagement"
    "${BACKEND_DIR}/Trade"
    "${BACKEND_DIR}/TradeManagement"
    "${BACKEND_DIR}/ManageSystem"
    "${PRODUCT_DIR}" "${TRADE_DIR}" "${WIDGETS_DIR}" "${ROUTER_DIR}" "${LOGIN_DIR}"
)
file(GLOB_RECURSE BACKEND_SRC CONFIGURE_DEPENDS "${BACKEND_DIR}/*.cpp")
target_sources(COSMETIC_MANAGEMENT PRIVATE ${BACKEND_SRC})

target_link_libraries(COSMETIC_MANAGEMENT PRIVATE Qt6::Widgets)
set_target_properties(COSMETIC_MANAGEMENT PROPERTIES WIN32_EXECUTABLE TRUE)
qt_finalize_executable(COSMETIC_MANAGEMENT)

# Copy Data cạnh .exe (đổi target name ở đây)
add_custom_command(TARGET COSMETIC_MANAGEMENT POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/Data"
          "$<TARGET_FILE_DIR:COSMETIC_MANAGEMENT>/Data")
