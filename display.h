#include "utils.h"

/**
NOTE: Coordinates used in this project assume (0, 0) is the top left corner of the terminal.
 Increasing y moves the cursor down; increasing x moves the cursor to the right.
 */

/**
 * Map representing the recent maximum measurement of the amplitude at each frequency.
 * Each value in the map will be decremented over time until new max is set.
 */
float current_max[WIN_WIDTH];

/**
 * Fills the current local-max map with 0s (initial state).
 */
void init_current_max();

/**
 * Decrements each value in the current local-max map by 0.03%.
 * Called every time streamCallBack is returned.
 */
void decrement_current_max();

/**
 * Initializes the ncurses screen with windows for each section of the view.
 *
 * @param num_chan number of channels in the input; affects the total height of all windows on the screen.
 */
void init_screen(int num_chan);

/**
 * Refreshes all ncurses windows on the screen.
 */
void refresh_screen();

/**
 * Deletes all ncurses windows on the screen.
 */
void del_screen();

/**
 * Display the current local maxima on the frequency view window.
 * The local maxima are rendered as '_' characters above each x-coordinate on the frequency graph.
 */
void display_current_max();