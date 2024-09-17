project (WebMonDiagTest)
enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test Network HttpServer)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test Network HttpServer)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(presenter_test
    presenter_test.cpp
    ../src/core/iwebserverdiag.h
    ../src/core/ipresenter.h
    ../src/core/logger.h
    ../src/core/logger.cpp
    ../src/core/serverpresenter.cpp
    ../src/core/serverpresenter.h
    ../src/core/web/webpageloader.h
    ../src/core/web/webpageloader.cpp
    ../src/core/web/webserverdata.h
    ../src/core/web/webserverdata.cpp
)
add_test(NAME presenter_test COMMAND presenter_test)
target_link_libraries(presenter_test PRIVATE Qt${QT_VERSION_MAJOR}::Test)

add_executable(webserverdiag_test
    integration/webserverdiag_test.cpp
    ../src/core/ipresenter.h
    ../src/core/logger.h
    ../src/core/logger.cpp
    ../src/core/serverpresenter.cpp
    ../src/core/serverpresenter.h
    ../src/core/web/webpageloader.h
    ../src/core/web/webpageloader.cpp
    ../src/core/web/webserverdata.h
    ../src/core/web/webserverdata.cpp
    ../src/core/web/webserverdiag.h
    ../src/core/web/webserverdiag.cpp
)
add_test(NAME webserverdiag_test COMMAND webserverdiag_test)
target_link_libraries(webserverdiag_test PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt::Network Qt6::HttpServer)

