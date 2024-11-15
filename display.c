#include "volume.h"
#include "frequencies.h"

/**
 * Map representing the recent maximum measurement of the amplitude at each frequency.
 * Each value in the map will be decremented over time until new max is set.
 */
float current_max[WIN_WIDTH];

/**
 * Fills the current local-max map with 0s (initial state).
 */
void init_current_max() {
  for (int freq = 0; freq < WIN_WIDTH; freq++) {
    current_max[freq] = 0.0f;
  }
}

/**
 * Decrements each value in the current local-max map by 0.03%.
 * Called every time streamCallBack is returned.
 */
void decrement_current_max() {
  float decrement = 0.0003f * (float) FREQ_WIN_HEIGHT;
  for (int freq = 0; freq < WIN_WIDTH; freq++) {
    if (current_max[freq] * (float) FREQ_WIN_HEIGHT > decrement) {
      current_max[freq] -= decrement;
    }
  }
}

/**
 * Initializes the ncurses screen with windows for each section of the view.
 *
 * @param num_chan number of channels in the input; affects the total height of all windows on the screen.
 */
void init_screen(int num_chan) {
  init_current_max();
  initscr();
  init_vol_win(num_chan);
  init_freq_win(num_chan);
}

/**
 * Refreshes all ncurses windows on the screen.
 */
void refresh_screen() {
  wrefresh(VOL_WIN);
  wrefresh(FREQ_WIN);
}

/**
 * Deletes all ncurses windows on the screen.
 */
void del_screen() {
  delwin(VOL_WIN);
  delwin(FREQ_WIN);
}

/**
 * Display the current local maxima on the frequency view window.
 * The local maxima are rendered as '_' characters above each x-coordinate on the frequency graph.
 */
void display_current_max() {
  int initial_x;
  int initial_y;
  getyx(FREQ_WIN, initial_y, initial_x);

  for (int width_index = 0; width_index < WIN_WIDTH; width_index++) {
    int y_pos = 1 + FREQ_WIN_HEIGHT - (int) ((float) FREQ_WIN_HEIGHT * current_max[width_index]);

    wmove(FREQ_WIN, y_pos, width_index);
    waddch(FREQ_WIN, '_');
  }

  wmove(FREQ_WIN, initial_y, initial_x);
}