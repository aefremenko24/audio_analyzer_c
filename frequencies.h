#include <fftw3.h>

/// Data structure representing the frequency view window
WINDOW *FREQ_WIN;

/**
 * Contains the data used for a singular stream call back.
 */
typedef struct {

  /// Array of size FRAMES_PER_BUFFER * num_channels, containing amplitudes of the input wave in the current buffer.
  double *in;

  /// Array of size FRAMES_PER_BUFFER * num_channels, containing amplitudes of the output wave in the current buffer.
  double *out;

  /// Contains information required to compute an FFT of the buffered waveform.
  fftw_plan p;

  /// Starting x-coordinate of the computed FFT graph.
  int startIndex;

  /// Size of the delta between each consecutive x-coordinate on the FFT graph.
  int spectroSize;
} streamCallbackData;

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