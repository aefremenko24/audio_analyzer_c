#include <curses.h>
#include <fftw3.h>
#include <portaudio.h>

/// Data structure representing the volume and frequency view windows
WINDOW *VOL_WIN;
WINDOW *FREQ_WIN;

/// Width of the console display window in number of characters
#define WIN_WIDTH 100

/// The height of the frequency view window in number of lines
#define FREQ_WIN_HEIGHT 20

/// Rate at which the audio is sampled, in Hz
#define SAMPLE_RATE 44100.0

/// Number of data points collected in a single buffer
#define FRAMES_PER_BUFFER 256

/// Frequency display range minimum and maximum (normal human hearing range here)
#define SPECTRO_FREQ_START 20
#define SPECTRO_FREQ_END 20000

/// Width of the console display window in number of characters
#define WIN_WIDTH 100

/// Number of lines to be skipped between different view windows
#define MARGIN 2

/// x- and y-coordinates of the top left corner of the volume view window
#define VOL_INIT_X 0
#define VOL_INIT_Y 0

/**
 * Enum representing the type of signal the user is working with (input or output)
 */
enum SignalType {
  Input,
  Output
};

/// Number of input channels for the input source the program is working with.
int num_input_channels;

/// Number of output channels for the output source the program is working with.
int num_output_channels;

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
 * Check and output errors in the portaudio stream.
 *
 * @param err Portaudio error code.
 */
void checkErr(PaError err);