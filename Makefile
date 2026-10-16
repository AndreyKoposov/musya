# --- Компилятор и флаги ---
CC      := gcc
CFLAGS  := -Wall -Wextra -pedantic -std=c17 -Iinclude
AR      := ar
ARFLAGS := rcs

# --- Директории ---
SRC_DIR   := src
TEST_DIR  := tests
BUILD_DIR := build

# --- Файлы ---
LIB_SRC   := $(SRC_DIR)/http.c
LIB_OBJ   := $(BUILD_DIR)/http.o
LIB       := $(BUILD_DIR)/libhttp.a

APP_SRC   := $(SRC_DIR)/echo_server.c
APP_OBJ   := $(BUILD_DIR)/echo_server.o
APP       := $(BUILD_DIR)/musya

TEST_SRC  := $(TEST_DIR)/test_http.c
TEST_OBJ  := $(BUILD_DIR)/test_http.o
TEST_BIN  := $(BUILD_DIR)/test_http

# --- Библиотеки для тестов (CUnit) ---
TEST_LIBS := -lcunit

# =========================================================
# Цели
# =========================================================

.PHONY: all lib app test clean run

# По умолчанию собираем всё основное (без тестов)
all: app

# --- Статическая библиотека ---
lib: $(LIB)

$(LIB): $(LIB_OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_OBJ): $(LIB_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Основное приложение ---
app: $(APP)

$(APP): $(APP_OBJ) $(LIB)
	$(CC) $^ -o $@

$(APP_OBJ): $(APP_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Тесты ---
test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_OBJ) $(LIB)
	$(CC) $^ $(TEST_LIBS) -o $@

$(TEST_OBJ): $(TEST_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Создание папки build ---
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# --- Запуск приложения ---
run: app
	./$(APP)

# --- Очистка ---
clean:
	rm -rf $(BUILD_DIR)