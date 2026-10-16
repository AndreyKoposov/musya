# Минимальная требуемая версия CMake
cmake_minimum_required(VERSION 3.15)

# Имя проекта и язык
project(Musya C)

# Стандарт C
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Директория с заголовками
include_directories(${CMAKE_SOURCE_DIR}/include)

# Создание исполняемого файла из исходников
add_executable(musya
    src/echo_server.c
    src/http.c
)