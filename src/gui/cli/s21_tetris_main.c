#include "../../brick_game/tetris/s21_tetris.h"

#define _POSIX_C_SOURCE 200809L
#include <unistd.h>  
#include <ncurses.h>


/**
 * @brief Основная функция игры.
 *
 * Функция инициализирует экран, запускает основной игровой цикл и завершает
 * работу.
 *
 * @return 0 В случае успешного завершения работы.
 */
int main() {
  init_gui();

  GameInfo_t GameInfo;
  figure next_figure, current_figure;
  GameState state;
  init_game(&GameInfo, &next_figure, &current_figure, &state);

  bool running = true;
  // Изменение: Убрана неиспользуемая переменная last_time
  render_game(GameInfo, state);

  while (running) {
    // Изменение: Убрано обновление last_time
    int input = getch();
    UserAction_t action = get_user_action(input);
    bool hold = (input != ERR);

    userInput(action, hold);
    GameInfo = updateCurrentState();
    render_game(GameInfo, state);

    if (state == GAMEOVER) {
      render_game(GameInfo, state);
      timeout(-1);
      getch();
      sleep(2);
      running = false;
    } else if (state == EXIT_STATE) {
      running = false;
    }

    usleep(100000); // 60 FPS (16.67 мс)
  }

  free_GameInfo_t_memory(&GameInfo);
  endwin();
  return 0;
}