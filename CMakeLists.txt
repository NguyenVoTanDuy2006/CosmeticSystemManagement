cmake_minimum_required(VERSION 3.16)
project(LSNX VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ========== App sources (UI) ==========
qt_add_executable(LSNX
    MANUAL_FINALIZATION
    main.cpp
    LSNX.cpp
    LSNX.h
    LSNX.ui
    TradeCardWidget.cpp
    TradeCardWidget.h
    icon.qrc
    SP.ui
    sp.h sp.cpp
    spcardwidget.h
    spcardwidget.cpp
    flowlayout.h
    flowlayout.cpp
    LOGIN+TRANGCHU/UiUx/login_form_gradient.ui LOGIN+TRANGCHU/UiUx/loginpage.cpp LOGIN+TRANGCHU/UiUx/loginpage.h LOGIN+TRANGCHU/UiUx/mainmenu.cpp LOGIN+TRANGCHU/UiUx/mainmenu.h LOGIN+TRANGCHU/UiUx/MainMenu.ui LOGIN+TRANGCHU/UiUx/Resources.qrc
    LOGIN+TRANGCHU/UiUx/login_form_gradient.ui
    app_router.h
)

# ========== BACKEND ==========
set(BACKEND_DIR ${CMAKE_SOURCE_DIR}/BACKEND)

# Cho phép include tất cả header trong BACKEND/*
target_include_directories(LSNX PRIVATE
    ${BACKEND_DIR}
    ${BACKEND_DIR}/Cosmetic
    ${BACKEND_DIR}/CosmeticManagement    # <-- THÊM DÒNG NÀY
    ${BACKEND_DIR}/Trade
    ${BACKEND_DIR}/TradeManagement
    ${BACKEND_DIR}/ManageSystem
)

# Thu thập toàn bộ .cpp trong BACKEND (mọi thư mục con)
file(GLOB_RECURSE BACKEND_SRC CONFIGURE_DEPENDS
    ${BACKEND_DIR}/*.cpp
)

# Thêm vào target (tránh liệt kê trùng lặp)
target_sources(LSNX PRIVATE ${BACKEND_SRC})

target_link_libraries(LSNX PRIVATE Qt6::Widgets)
set_target_properties(LSNX PROPERTIES WIN32_EXECUTABLE TRUE)
qt_finalize_executable(LSNX)

# ========== Copy Data cạnh file .exe ==========
add_custom_command(TARGET LSNX POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/Data"
          "$<TARGET_FILE_DIR:LSNX>/Data")
