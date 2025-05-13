#include "../../brick_game/tetris/s21_tetris.h"

#include <ncurses.h>

// Макросы для удобного отображения текста и символов в ncurses.
#define MVPRINTW(y, x, ...)                                                    \
  mvprintw(BOARD_START_ROW + (y), BOARD_START_ROW + (x), __VA_ARGS__)
#define MVADDCH(y, x, c)                                                       \
  mvaddch(BOARD_START_ROW + (y), BOARD_START_ROW + (x), c)

// Макрос для добавления блока на экран с использованием цветового эффекта.
// Отображает два блока с цветом, заданным параметром x, в указанном окне w.
#define ADD_BLOCK(w, x)                                                        \
  waddch((w), ' ' | A_REVERSE | COLOR_PAIR(x));                                \
  waddch((w), ' ' | A_REVERSE | COLOR_PAIR(x))

// Макрос для добавления пустого блока на экран.
// Отображает два пустых блока без цветового эффекта в указанном окне w.
#define ADD_EMPTY(w)                                                           \
  waddch((w), ' ');                                                            \
  waddch((w), ' ')

void init_gui(void) {
  srand(time(NULL));
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  timeout(0);  // Изменение: Неблокирующий ввод
  setlocale(LC_ALL, "");
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  display_overlay();
}

/**
 * @brief Инициализирует цветовую схему для игры.
 *
 * Функция настраивает цветовую схему для отображения блоков в игре, используя
 * цвета циан и черный.
 */
void init_colors(void) {
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
}

/**
 * @brief Отображает наложение элементов пользовательского интерфейса.
 *
 * Функция отображает прямоугольники для области счета, уровня, следующей фигуры
 * и кнопки "Press S to start".
 */
void display_overlay(void) {
  display_rectangle(0, BOARD_HEIGHT + 1, 0, BOARD_WIDTH * 2 + 1);
  display_rectangle(0, BOARD_HEIGHT + 1, BOARD_WIDTH * 2 + 2,
                    (BOARD_WIDTH + HUD_PANEL_WIDTH) * 2 + 3);

  mvprintw(1, BOARD_WIDTH * 2 + 3, "SCORE");
  mvprintw(4, BOARD_WIDTH * 2 + 3, "HIGH-SCORE");
  mvprintw(7, BOARD_WIDTH * 2 + 3, "LEVEL");
  mvprintw(10, BOARD_WIDTH * 2 + 3, "NEXT");

  int center_x = (FIELD_COLS * 2) / 2;
  int center_y = FIELD_ROWS / 2;

  int text_height = 2;
  int start_y = center_y - (text_height / 2);

  mvprintw(start_y, center_x - strlen("Press S") / 2, "Press S");
  mvprintw(start_y + 1, center_x - strlen("to start!") / 2, "to start!");
}

/**
 * @brief Отображает текущее состояние игры.
 *
 * Функция отображает состояние игрового поля, следующей фигуры и статистики
 * игры.
 *
 * @param GameInfo Структура GameInfo_t, содержащая информацию о текущем
 * состоянии игры.
 */
void display_current_game_state(GameInfo_t GameInfo) {
  display_next_figure(GameInfo);
  display_game_field(GameInfo);
  display_game_stats(GameInfo);
}

void render_game(GameInfo_t GameInfo, GameState state) {
  clear();
  display_overlay();
  display_next_figure(GameInfo);
  display_game_field(GameInfo);
  display_game_stats(GameInfo);
  if (state == PAUSE) {
    int center_x = (FIELD_COLS * 2) / 2, center_y = FIELD_ROWS / 2;
    mvprintw(center_y, center_x - strlen("PAUSED") / 2, "PAUSED");
  } else if (state == GAMEOVER) {
    int center_x = (FIELD_COLS * 2) / 2, center_y = FIELD_ROWS / 2;
    mvprintw(center_y, center_x - strlen("GAME OVER!") / 2, "GAME OVER!");
  }
  refresh();
}

/**
 * @brief Отображает следующую фигуру на экране.
 *
 * Функция выводит следующую фигуру на экран в указанной области.
 *
 * @param GameInfo Структура GameInfo_t, содержащая информацию о следующей
 * фигуре.
 */
void display_next_figure(GameInfo_t GameInfo) {
  for (int i = 0; i < NEXT_FIELD_ROWS; i++) {
    wmove(stdscr, 11 + i, 24);
    for (int j = 0; j < NEXT_FIELD_COLS; j++) {
      if (GameInfo.next[i][j] == 1) {
        ADD_BLOCK(stdscr, GameInfo.next[i][j]);
      } else {
        ADD_EMPTY(stdscr);
      }
    }
  }
}

/**
 * @brief Отображает игровое поле на экране.
 *
 * Функция выводит текущее состояние игрового поля на экран.
 *
 * @param GameInfo Структура GameInfo_t, содержащая информацию о текущем
 * состоянии игрового поля.
 */
void display_game_field(GameInfo_t GameInfo) {
  for (int i = 3; i < FIELD_ROWS; i++) {
    wmove(stdscr, i - 2, 1);
    for (int j = 0; j < FIELD_COLS; j++) {
      if (GameInfo.field[i][j] == 1) {
        ADD_BLOCK(stdscr, GameInfo.field[i][j]);
      } else {
        ADD_EMPTY(stdscr);
      }
    }
  }
}

/**
 * @brief Отображает прямоугольник на экране.
 *
 * Функция рисует прямоугольник, используя специальные символы для углов и
 * линий.
 *
 * @param top_y Верхняя координата Y прямоугольника.
 * @param bottom_y Нижняя координата Y прямоугольника.
 * @param left_x Левый X координат прямоугольника.
 * @param right_x Правый X координат прямоугольника.
 */
void display_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  mvaddch(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++)
    mvaddch(top_y, i, ACS_HLINE);
  mvaddch(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);
    mvaddch(i, right_x, ACS_VLINE);
  }

  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++)
    mvaddch(bottom_y, i, ACS_HLINE);
  mvaddch(bottom_y, i, ACS_LRCORNER);
}

/**
 * @brief Отображает статистику игры на экране.
 *
 * Функция выводит текущие значения счета, рекорда, уровня и состояния паузы.
 *
 * @param GameInfo Структура GameInfo_t, содержащая информацию о текущем
 * состоянии игры.
 */
void display_game_stats(GameInfo_t GameInfo) {
  mvprintw(2, BOARD_WIDTH * 2 + 4, "%d", GameInfo.score);
  mvprintw(5, BOARD_WIDTH * 2 + 4, "%d", GameInfo.high_score);
  mvprintw(8, BOARD_WIDTH * 2 + 4, "%d", GameInfo.level);
  if (GameInfo.pause == 1) {
    mvprintw(19, BOARD_WIDTH * 2 + 3, "PAUSED");
  } else {
    mvprintw(19, BOARD_WIDTH * 2 + 3, "      ");
  }
}

// Изменение: Обновлена для соответствия восьми кнопкам
UserAction_t get_user_action(int user_input) {
  UserAction_t action = NOSIG;
  switch (user_input) {
    case 's': case 'S': action = Start; break;
    case 'p': case 'P': case SPACE_KEY: action = Pause; break;
    case ESCAPE_KEY: action = Terminate; break;
    case KEY_LEFT: action = Left; break;
    case KEY_RIGHT: action = Right; break;
    case KEY_UP: action = Up; break;
    case KEY_DOWN: action = Down; break;
    case RETURN_KEY: action = Action; break;
    default: action = NOSIG; break;
  }
  return action;
}