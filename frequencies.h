#include "utils.h"

/**
 * Initializes the frequency display window using ncurses, given the number of channels in the input.
 *
 * @param num_chan number of channels in the input; affects the initial y position of the window.
 */
void init_freq_win(int num_chan);

/**
 * Renders the volume representation of the given input buffer.
 * The volume is rendered as a line of '=' characters of length between 0 and WIN_WIDTH.
 *
 * @param inputBuffer Input buffer to compute and render the frequencies for.
 * @param framesPerBuffer Number of frames in the buffer.
 * @param userData Callback data used for FFT computations.
 */
void streamCallBackFrequencies(
    const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
    void *userData
);

/**
 * Initializes the spectro data used for FFT computations during callbacks.
 *
 * @return Spectro data (see streamCallbackData struct definition above)
 */
streamCallbackData *init_spectro_data();