cmake_minimum_required(VERSION 3.10)
project(Compiler_GUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)


add_executable(Compiler_GUI main.cpp MultipurposeSequence.h MultipurposeChain.h BasicTable.h
        BasicGraph.h RegularExpressionParser.cpp RegularExpressionParser.h REBinaryTree.h
        TokenParser.cpp TokenParser.h LLParser.cpp LLParser.h Initialize.cpp Initialize.h
        mainwindow.h mainwindow.cpp displaysymboltable.h displaysymboltable.cpp)

target_link_libraries(Compiler_GUI Qt5::Core Qt5::Widgets Qt5::Gui)