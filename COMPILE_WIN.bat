@ECHO OFF
REM This file compiles the Findin program using the Microsoft (MSVC) compiler.
cl main.c file_search_utils.c file_information_node.c /Fe"findin"