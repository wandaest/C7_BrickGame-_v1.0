#include "brick_game/tetris/s21_tetris.h"
#include <check.h>
#include <ncurses.h>
#include <stdlib.h>

START_TEST(test_game_initialization_and_input) {
  GameInfo_t GameInfo;
  initialize_GameInfo_t(&GameInfo);

  figure next_figure;
  figure current_figure;
  int user_action = 0;
  GameState state = START;

  initialize_next_figure(&next_figure);

  UserAction_t action = get_user_action(user_action);

  UserInput(action, &state, &next_figure, &current_figure, &GameInfo);
  fail_unless(GameInfo.score == 0);
}
END_TEST

START_TEST(test_spawning) {
  GameInfo_t GameInfo;
  initialize_GameInfo_t(&GameInfo);

  figure next_figure;
  figure current_figure;
  GameState state1 = START;
  GameState state2 = MOVING;

  initialize_next_figure(&next_figure);
  initialize_next_figure(&current_figure);

  GameParameters params;
  params.state = &state1;
  params.next_figure = &next_figure;
  params.current_figure = &current_figure;

  spawn(&params, &GameInfo);

  fail_unless(*params.state == state2);

  free_GameInfo_t_memory(&GameInfo);
}
END_TEST

START_TEST(test_shift) {
  GameInfo_t GameInfo;
  initialize_GameInfo_t(&GameInfo);

  figure next_figure;
  figure current_figure;
  GameState state1 = START;

  initialize_next_figure(&next_figure);
  initialize_next_figure(&current_figure);

  GameParameters params;
  params.state = &state1;
  params.next_figure = &next_figure;
  params.current_figure = &current_figure;

  spawn(&params, &GameInfo);

  int figure_center_row_1 = params.current_figure->center.row;

  shifting(&params, &GameInfo);

  int figure_center_row_2 = params.current_figure->center.row;

  fail_unless(figure_center_row_2 == figure_center_row_1 + 1);

  free_GameInfo_t_memory(&GameInfo);
}
END_TEST

START_TEST(test_move_right) {
  GameInfo_t GameInfo;
  initialize_GameInfo_t(&GameInfo);

  figure next_figure;
  figure current_figure;
  GameState state1 = START;

  initialize_next_figure(&next_figure);
  initialize_next_figure(&current_figure);

  GameParameters params;
  params.state = &state1;
  params.next_figure = &next_figure;
  params.current_figure = &current_figure;

  spawn(&params, &GameInfo);

  int figure_center_col_1 = params.current_figure->center.col;

  move_figure_right(&params, &GameInfo);

  int figure_center_col_2 = params.current_figure->center.col;

  fail_unless(figure_center_col_2 == figure_center_col_1 + 1);

  free_GameInfo_t_memory(&GameInfo);
}

START_TEST(test_move_left) {
  GameInfo_t GameInfo;
  initialize_GameInfo_t(&GameInfo);

  figure next_figure;
  figure current_figure;
  GameState state1 = START;

  initialize_next_figure(&next_figure);
  initialize_next_figure(&current_figure);

  GameParameters params;
  params.state = &state1;
  params.next_figure = &next_figure;
  params.current_figure = &current_figure;

  spawn(&params, &GameInfo);

  int figure_center_col_1 = params.current_figure->center.col;

  move_figure_left(&params, &GameInfo);

  int figure_center_col_2 = params.current_figure->center.col;

  fail_unless(figure_center_col_2 == figure_center_col_1 - 1);

  free_GameInfo_t_memory(&GameInfo);
}
END_TEST

START_TEST(test_move_down) {
  GameInfo_t GameInfo;
  initialize_GameInfo_t(&GameInfo);

  figure next_figure;
  figure current_figure;
  GameState state1 = START;

  initialize_next_figure(&next_figure);
  initialize_next_figure(&current_figure);

  GameParameters params;
  params.state = &state1;
  params.next_figure = &next_figure;
  params.current_figure = &current_figure;

  spawn(&params, &GameInfo);

  move_figure_down(&params, &GameInfo);

  int figure_bottom = get_figure_bottom(*params.current_figure);

  fail_unless(figure_bottom == FIELD_ROWS - 1);

  free_GameInfo_t_memory(&GameInfo);
}
END_TEST

START_TEST(test_attach) {
  GameInfo_t GameInfo;
  initialize_GameInfo_t(&GameInfo);

  figure next_figure;
  figure current_figure;
  GameState state1 = START;

  initialize_next_figure(&next_figure);
  initialize_next_figure(&current_figure);

  GameParameters params;
  params.state = &state1;
  params.next_figure = &next_figure;
  params.current_figure = &current_figure;

  spawn(&params, &GameInfo);

  move_figure_down(&params, &GameInfo);

  attaching(&params, &GameInfo);

  fail_unless(*params.state == SPAWN);

  free_GameInfo_t_memory(&GameInfo);
}
END_TEST

START_TEST(test_rotation) {
  GameInfo_t GameInfo;
  initialize_GameInfo_t(&GameInfo);

  figure next_figure;
  figure current_figure;
  GameState state1 = START;

  initialize_next_figure(&next_figure);
  initialize_next_figure(&current_figure);

  GameParameters params;
  params.state = &state1;
  params.next_figure = &next_figure;
  params.current_figure = &current_figure;

  spawn(&params, &GameInfo);

  rotate(&params, &GameInfo);

  fail_unless(*params.state == MOVING);

  free_GameInfo_t_memory(&GameInfo);
}
END_TEST

START_TEST(test_pause1) {
  GameInfo_t GameInfo;
  initialize_GameInfo_t(&GameInfo);

  figure next_figure;
  figure current_figure;
  GameState state1 = START;

  initialize_next_figure(&next_figure);
  initialize_next_figure(&current_figure);

  GameParameters params;
  params.state = &state1;
  params.next_figure = &next_figure;
  params.current_figure = &current_figure;

  spawn(&params, &GameInfo);

  paused(&params, &GameInfo);

  fail_unless(GameInfo.pause == 1);

  free_GameInfo_t_memory(&GameInfo);
}
END_TEST

START_TEST(test_pause2) {
  GameInfo_t GameInfo;
  initialize_GameInfo_t(&GameInfo);

  figure next_figure;
  figure current_figure;
  GameState state1 = START;

  initialize_next_figure(&next_figure);
  initialize_next_figure(&current_figure);

  GameParameters params;
  params.state = &state1;
  params.next_figure = &next_figure;
  params.current_figure = &current_figure;

  spawn(&params, &GameInfo);

  paused(&params, &GameInfo);
  shifting(&params, &GameInfo);

  fail_unless(GameInfo.pause == 0);

  free_GameInfo_t_memory(&GameInfo);
}
END_TEST

START_TEST(test_game_over) {
  GameInfo_t GameInfo;
  initialize_GameInfo_t(&GameInfo);

  figure next_figure;
  figure current_figure;
  GameState state1 = START;

  initialize_next_figure(&next_figure);
  initialize_next_figure(&current_figure);

  GameParameters params;
  params.state = &state1;
  params.next_figure = &next_figure;
  params.current_figure = &current_figure;

  spawn(&params, &GameInfo);

  gameover(&params, &GameInfo);

  fail_unless(*params.state == GAMEOVER);

  free_GameInfo_t_memory(&GameInfo);
}
END_TEST

START_TEST(test_exit_state) {
  GameInfo_t GameInfo;
  initialize_GameInfo_t(&GameInfo);

  figure next_figure;
  figure current_figure;
  GameState state1 = START;

  initialize_next_figure(&next_figure);
  initialize_next_figure(&current_figure);

  GameParameters params;
  params.state = &state1;
  params.next_figure = &next_figure;
  params.current_figure = &current_figure;

  spawn(&params, &GameInfo);

  exitstate(&params, &GameInfo);

  fail_unless(*params.state == EXIT_STATE);

  free_GameInfo_t_memory(&GameInfo);
}
END_TEST

START_TEST(test_initialize_O) {
  figure figure;
  initialize_O_figure(&figure);
  fail_unless(figure.type == O);
}
END_TEST

START_TEST(test_initialize_I) {
  figure figure;
  initialize_I_figure(&figure);
  fail_unless(figure.type == I);
}
END_TEST

START_TEST(test_initialize_S) {
  figure figure;
  initialize_S_figure(&figure);
  fail_unless(figure.type == S);
}
END_TEST

START_TEST(test_initialize_Z) {
  figure figure;
  initialize_Z_figure(&figure);
  fail_unless(figure.type == Z);
}
END_TEST

START_TEST(test_initialize_L) {
  figure figure;
  initialize_L_figure(&figure);
  fail_unless(figure.type == L);
}
END_TEST

START_TEST(test_initialize_J) {
  figure figure;
  initialize_J_figure(&figure);
  fail_unless(figure.type == J);
}
END_TEST

START_TEST(test_initialize_T) {
  figure figure;
  initialize_T_figure(&figure);
  fail_unless(figure.type == T);
}
END_TEST

START_TEST(test_update_speed) {
  GameInfo_t GameInfo;
  initialize_GameInfo_t(&GameInfo);

  GameInfo.score = 900;

  update_speed(&GameInfo);

  fail_unless(GameInfo.speed == 2);

  free_GameInfo_t_memory(&GameInfo);
}
END_TEST

START_TEST(test_get_user_action) {
  fail_unless(get_user_action(KEY_UP) == NOSIG);
  fail_unless(get_user_action(KEY_DOWN) == Down);
  fail_unless(get_user_action(KEY_LEFT) == Left);
  fail_unless(get_user_action(KEY_RIGHT) == Right);
  fail_unless(get_user_action(ESCAPE_KEY) == Terminate);
  fail_unless(get_user_action(ENTER_KEY) == Action);
  fail_unless(get_user_action(SPACE_KEY) == Pause);
  fail_unless(get_user_action('s') == Start);
}
END_TEST

Suite *tetris_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Tetris");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_game_initialization_and_input);
  tcase_add_test(tc_core, test_spawning);
  tcase_add_test(tc_core, test_shift);
  tcase_add_test(tc_core, test_move_right);
  tcase_add_test(tc_core, test_move_left);
  tcase_add_test(tc_core, test_move_down);
  tcase_add_test(tc_core, test_attach);
  tcase_add_test(tc_core, test_rotation);
  tcase_add_test(tc_core, test_pause1);
  tcase_add_test(tc_core, test_pause2);
  tcase_add_test(tc_core, test_game_over);
  tcase_add_test(tc_core, test_exit_state);
  tcase_add_test(tc_core, test_initialize_O);
  tcase_add_test(tc_core, test_initialize_I);
  tcase_add_test(tc_core, test_initialize_S);
  tcase_add_test(tc_core, test_initialize_Z);
  tcase_add_test(tc_core, test_initialize_L);
  tcase_add_test(tc_core, test_initialize_J);
  tcase_add_test(tc_core, test_initialize_T);
  tcase_add_test(tc_core, test_update_speed);
  tcase_add_test(tc_core, test_get_user_action);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = tetris_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}