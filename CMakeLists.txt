cmake_minimum_required(VERSION 3.16)
project(LSNX VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ==== Thư mục con (đổi theo đúng tên folder của bạn) ====
set(PRODUCT_DIR   "${CMAKE_SOURCE_DIR}/Product")
set(TRADE_DIR     "${CMAKE_SOURCE_DIR}/Trade")
set(WIDGETS_DIR   "${CMAKE_SOURCE_DIR}/Widget Card")   # có khoảng trắng -> nhớ để trong ""
set(ROUTER_DIR    "${CMAKE_SOURCE_DIR}/Router")
set(LOGIN_DIR     "${CMAKE_SOURCE_DIR}/LOGIN+TRANGCHU/UiUx")  # chỗ bạn để Login/MainMenu

# ==== App sources ====
qt_add_executable(LSNX
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
    "${ROUTER_DIR}/app_router.h"          # nếu có app_router.cpp thì thêm luôn

    # Login + Main Menu  <<< QUAN TRỌNG
    "${LOGIN_DIR}/loginpage.cpp"          "${LOGIN_DIR}/loginpage.h"          "${LOGIN_DIR}/login_form_gradient.ui"
    "${LOGIN_DIR}/mainmenu.cpp"           "${LOGIN_DIR}/mainmenu.h"           "${LOGIN_DIR}/MainMenu.ui"

    # Resources
    icon.qrc
)

# ==== BACKEND ====
set(BACKEND_DIR "${CMAKE_SOURCE_DIR}/BACKEND")

# Cho phép include các header backend
target_include_directories(LSNX PRIVATE
    "${BACKEND_DIR}"
    "${BACKEND_DIR}/Cosmetic"
    "${BACKEND_DIR}/CosmeticManagement"
    "${BACKEND_DIR}/Trade"
    "${BACKEND_DIR}/TradeManagement"
    "${BACKEND_DIR}/ManageSystem"

    # include path cho UI mới sắp xếp
    "${PRODUCT_DIR}"
    "${TRADE_DIR}"
    "${WIDGETS_DIR}"
    "${ROUTER_DIR}"
    "${LOGIN_DIR}"
)

# Gom toàn bộ .cpp của backend
file(GLOB_RECURSE BACKEND_SRC CONFIGURE_DEPENDS "${BACKEND_DIR}/*.cpp")
target_sources(LSNX PRIVATE ${BACKEND_SRC})

target_link_libraries(LSNX PRIVATE Qt6::Widgets)
set_target_properties(LSNX PROPERTIES WIN32_EXECUTABLE TRUE)
qt_finalize_executable(LSNX)

# Copy Data cạnh .exe
add_custom_command(TARGET LSNX POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/Data"
          "$<TARGET_FILE_DIR:LSNX>/Data")
