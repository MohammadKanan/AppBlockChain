cmake_minimum_required(VERSION 3.16)

project(AppBlockchain LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
message("openSSL:"  ${OpenSSL_LIBRARIES} "OpenSSL found: Version ${OPENSSL_VERSION}")
else()
message("openSSL not found!")
endif()
set (PROJECT_HEADERS
    src/Transaction.h
    src/main.cpp
    src/Blockchain.h
    src/Block.h
)
set(PROJECT_SOURCES
        src/main.cpp
        src/Wallet.cpp
        src/Transaction.cpp
        src/Blockchain.cpp
        src/Block.cpp
)

add_executable(${PROJECT_NAME}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core crypto ${OpenSSL_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
