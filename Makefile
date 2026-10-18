# --- Компилятор и флаги ---
CC      := gcc
CFLAGS  := -Wall -Wextra -pedantic -std=c17 -Iinclude -g
AR      := ar
ARFLAGS := rcs

# --- Директории ---
SRC_DIR   := src
TEST_DIR  := tests
BUILD_DIR := build

# --- Файлы ---
LIB_SRC_HTTP   := $(SRC_DIR)/http.c
LIB_OBJ_HTTP   := $(BUILD_DIR)/http.o
LIB_HTTP       := $(BUILD_DIR)/libhttp.a

LIB_SRC_PARSER   := $(SRC_DIR)/parser.c
LIB_OBJ_PARSER   := $(BUILD_DIR)/parser.o
LIB_PARSER       := $(BUILD_DIR)/libparser.a

LIB_SRC_REQ   := $(SRC_DIR)/request.c
LIB_OBJ_REQ   := $(BUILD_DIR)/request.o
LIB_REQ       := $(BUILD_DIR)/libreq.a

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

.PHONY: all lib_http lib_parser lib_req app test clean run

# По умолчанию собираем всё основное (без тестов)
all: app

# --- Статические библиотеки ---
lib_http: $(LIB_HTTP)

$(LIB_HTTP): $(LIB_OBJ_HTTP)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_OBJ_HTTP): $(LIB_SRC_HTTP) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

lib_parser: $(LIB_PARSER)

$(LIB_PARSER): $(LIB_OBJ_PARSER)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_OBJ_PARSER): $(LIB_SRC_PARSER) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

lib_req: $(LIB_REQ)

$(LIB_REQ): $(LIB_OBJ_REQ)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_OBJ_REQ): $(LIB_SRC_REQ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Основное приложение ---
app: $(APP)

$(APP): $(APP_OBJ) $(LIB_HTTP) $(LIB_PARSER) $(LIB_REQ)
	$(CC) $^ -o $@

$(APP_OBJ): $(APP_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Тесты ---
test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_OBJ) $(LIB_HTTP) $(LIB_PARSER) $(LIB_REQ)
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