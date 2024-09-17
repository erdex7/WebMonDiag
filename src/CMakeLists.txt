set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(BUILD_CLI "Build CLI application" ON)
option(BUILD_GUI "Build GUI application" ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core HttpServer)

set(PROJECT_CORE_SOURCES
        core/iwebserverdiag.h
        core/web/webpageloader.h
        core/web/webpageloader.cpp
        core/web/webserverdiag.h
        core/web/webserverdiag.cpp
        core/web/webserverdata.h
        core/web/webserverdata.cpp
        core/ipresenter.h
        core/iview.h
        core/logger.h
        core/logger.cpp
        core/serverpresenter.h
        core/serverpresenter.cpp
)

set(PROJECT_GUI_SOURCES
        gui/mainwindow.cpp
        gui/mainwindow.h
        gui/main.cpp
)

set(PROJECT_CLI_SOURCES
        cli/commandlineview.h
        cli/commandlineview.cpp
        cli/commandreader.h
        cli/commandreader.cpp
        cli/commands/changeendpointpath.h
        cli/commands/changeendpointpath.cpp
        cli/commands/changeresptime.h
        cli/commands/changeresptime.cpp
        cli/commands/changehttpcode.h
        cli/commands/changehttpcode.cpp
        cli/commands/changewebpage.h
        cli/commands/changewebpage.cpp
        cli/commands/icommand.h
        cli/main.cpp
)

add_library(wmdcore STATIC ${PROJECT_CORE_SOURCES})
target_link_libraries(wmdcore PRIVATE Qt6::Core Qt6::HttpServer)

if (BUILD_GUI)
    add_executable(wmdgui WIN32 ${PROJECT_GUI_SOURCES})
    target_link_libraries(wmdgui PRIVATE wmdcore Qt6::Widgets)
endif()

if (BUILD_CLI)
    add_executable(wmdcli ${PROJECT_CLI_SOURCES})
    target_link_libraries(wmdcli PRIVATE wmdcore Qt6::Core)
endif()
