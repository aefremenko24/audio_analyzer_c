#include <fftw3.h>
#include <math.h>
#include "display.h"
#include <stdlib.h>
#include "frequencies.h"

/**
 * Initializes the frequency display window using ncurses, given the number of channels in the input.
 *
 * @param num_chan number of channels in the input; affects the initial y position of the window.
 */
void init_freq_win(int num_chan) {
  FREQ_WIN = newwin(FREQ_WIN_HEIGHT, WIN_WIDTH, num_chan + 1 + MARGIN, 0);
  waddstr(FREQ_WIN, "Frequencies:\n");
}

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
) {
  float *in = (float *) inputBuffer;

  streamCallbackData *callbackData = (streamCallbackData *) userData;

  for (unsigned long i = 0; i < framesPerBuffer; i++) {
    callbackData->in[i] = in[i * 2];
  }

  int initial_x;
  int initial_y;
  getyx(FREQ_WIN, initial_y, initial_x);

  fftw_execute(callbackData->p);

  for (int i = 0; i < WIN_WIDTH; i++) {
    float freq = powf((float)i / ((float) WIN_WIDTH), 2);
    double proportion = callbackData->out[(int)((float)callbackData->startIndex + freq * (float)callbackData->spectroSize)] / 5;

    if (fabs(proportion) > current_max[i]) {
      current_max[i] = (float)fmin(fabs(proportion), 1.0);
    }

    for (int j = 1; j < FREQ_WIN_HEIGHT; j++) {
      float desired_level = (float) ((FREQ_WIN_HEIGHT) - j) / (float) FREQ_WIN_HEIGHT;
      wmove(FREQ_WIN, j, i + 1);

      if (proportion >= desired_level) {
        waddch(FREQ_WIN, 'o');
      } else {
        waddch(FREQ_WIN, ' ');
      }
    }
  }

  display_current_max();
  decrement_current_max();

  wmove(FREQ_WIN, initial_y, initial_x);
}

/**
 * Initializes the spectro data used for FFT computations during callbacks.
 *
 * @return Spectro data (see streamCallbackData struct definition above)
 */
streamCallbackData *init_spectro_data() {
  streamCallbackData *spectroData;

  spectroData = (streamCallbackData *) malloc(sizeof(streamCallbackData));
  spectroData->in = (double *) fftw_malloc(sizeof(double) * FRAMES_PER_BUFFER);
  spectroData->out = (double *) (double *) fftw_malloc(sizeof(double) * FRAMES_PER_BUFFER);
  if (spectroData->in == NULL || spectroData->out == NULL) {
    printf("Could not allocate spectro data.\n");
    exit(EXIT_FAILURE);
  }
  spectroData->p = fftw_plan_r2r_1d(FRAMES_PER_BUFFER, spectroData->in, spectroData->out,
                                    FFTW_R2HC, FFTW_ESTIMATE);

  float sampleRatio = FRAMES_PER_BUFFER / SAMPLE_RATE;
  spectroData->startIndex = (int)ceilf(sampleRatio * SPECTRO_FREQ_START);
  spectroData->spectroSize = (int)fmin(ceilf(sampleRatio * SPECTRO_FREQ_END),
                                       FRAMES_PER_BUFFER / 2.0)
                             - spectroData->startIndex;

  return spectroData;
}