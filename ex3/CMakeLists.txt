# basic CmakeLists.txt for a project in clion
cmake_minimum_required(VERSION 3.10)
project(ex3)

set(CMAKE_CXX_STANDARD 11)

add_executable(ex3 main.c queue.h queue.c boundedbuffer.c boundedbuffer.h producer.c producer.h dispatcher.h dispatcher.c coeditor.h coeditor.c screenmanager.c screenmanager.h unboundedBuffer.c unboundedBuffer.h)
