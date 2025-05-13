CC = gcc
CFLAGS = -Wall -Wextra -Werror 
GCOV_FLAGS = -fprofile-arcs -ftest-coverage --coverage

SRC_DIR = .
OBJ_DIR = $(SRC_DIR)/obj
GCOV_DIR = $(SRC_DIR)/gcov_obj
BIN_DIR = $(SRC_DIR)/bin
EXEC = s21_tetris
TEST_EXEC = s21_tetris_tests

SRC_FILES = $(SRC_DIR)/brick_game/tetris/s21_tetris.c $(SRC_DIR)/gui/cli/s21_tetris_gui.c
MAIN_FILE = $(SRC_DIR)/gui/cli/s21_tetris_main.c
OBJ_FILES = $(OBJ_DIR)/s21_tetris.o $(OBJ_DIR)/s21_tetris_gui.o $(OBJ_DIR)/s21_tetris_main.o
TEST_FILES = $(SRC_DIR)/s21_tetris_test.c
TEST_OBJ_FILES = $(OBJ_DIR)/s21_tetris_test.o $(OBJ_DIR)/s21_tetris.o $(OBJ_DIR)/s21_tetris_gui.o

all: clean install gcov_report

$(BIN_DIR)/$(EXEC): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ_FILES) -o $@ $(CFLAGS) -lncurses -lrt -lm

$(OBJ_DIR)/s21_tetris.o: $(SRC_DIR)/brick_game/tetris/s21_tetris.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/s21_tetris_gui.o: $(SRC_DIR)/gui/cli/s21_tetris_gui.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/s21_tetris_main.o: $(MAIN_FILE)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/s21_tetris_test.o: $(TEST_FILES)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(shell pkg-config --cflags check)

test: $(TEST_OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(TEST_OBJ_FILES) -o $(BIN_DIR)/$(TEST_EXEC) $(CFLAGS) $(shell pkg-config --libs check) -lm -lncurses
	./$(BIN_DIR)/$(TEST_EXEC)

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/$(EXEC) $(BIN_DIR)/$(TEST_EXEC)
	rm -rf $(GCOV_DIR)/*

install: $(BIN_DIR)/$(EXEC)
	mkdir -p $(BIN_DIR)

uninstall:
	rm -f $(BIN_DIR)/$(EXEC)
	rm -f $(BIN_DIR)/$(TEST_EXEC)
	rmdir $(BIN_DIR) || true

dist: clean
	rm -rf tetris.tar.gz
	tar -czvf tetris.tar.gz *

dvi:
	makeinfo --html doc.texi
	open doc_html/index*

gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $(SRC_DIR)/brick_game/tetris/s21_tetris.c -o $(GCOV_DIR)/s21_tetris.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $(SRC_DIR)/gui/cli/s21_tetris_gui.c -o $(GCOV_DIR)/s21_tetris_gui.o
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $(TEST_FILES) -o $(GCOV_DIR)/s21_tetris_test.o $(shell pkg-config --cflags check)
	$(CC) $(GCOV_DIR)/s21_tetris.o $(GCOV_DIR)/s21_tetris_gui.o $(GCOV_DIR)/s21_tetris_test.o -o $(GCOV_DIR)/$(TEST_EXEC) $(CFLAGS) $(GCOV_FLAGS) $(shell pkg-config --libs check) -lm -lncurses
	./$(GCOV_DIR)/$(TEST_EXEC)
	lcov -t "s21_tetris" -o $(GCOV_DIR)/s21_tetris.info -c -d $(GCOV_DIR)
	genhtml -o $(GCOV_DIR) $(GCOV_DIR)/s21_tetris.info
	open $(GCOV_DIR)/index.html

format:
	clang-format -i $(SRC_FILES) $(MAIN_FILE) $(TEST_FILES) $(SRC_DIR)/brick_game/tetris/s21_tetris.h

leaks: install
	valgrind --tool=memcheck --leak-check=yes $(BIN_DIR)/$(EXEC)

git:
	git add $(SRC_FILES) $(MAIN_FILE) $(TEST_FILES) $(SRC_DIR)/brick_game/tetris/s21_tetris.h Makefile
	git commit -m 's21_BrickGame_v.1.0 $(shell date +"%d.%m.%Y | %H:%M:%S")'
	git push origin develop
	git status
