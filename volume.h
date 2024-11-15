#include <curses.h>

/// Data structure representing the volume and frequency view windows
WINDOW *VOL_WIN;

/**
 * Renders the volume representation of the given input buffer.
 * The volume is rendered as a line of '=' characters of length between 0 and WIN_WIDTH.
 *
 * @param inputBuffer Input buffer to render the volume for.
 * @param framesPerBuffer Number of frames in the buffer.
 */
void streamCallBackVolume(
    const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, int num_input_channels, int num_output_channels
);

/**
 * Initializes the volume display window using ncurses, given the number of channels in the input.
 *
 * @param num_chan number of channels in the input; directly affects the height of the window.
 */
void init_vol_win(int num_chan);