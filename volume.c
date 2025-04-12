#include "volume.h"
#include "utils.h"
#include <curses.h>
#include <math.h>

/**
 * Initializes the volume display window using ncurses, given the number of channels in the input.
 *
 * @param num_chan number of channels in the input; directly affects the height of the window.
 */
void init_vol_win(int num_chan) {
  VOL_WIN = newwin(num_chan + 1, WIN_WIDTH, 0, 0);
  waddstr(VOL_WIN, "Volume:\n");
}

/**
 * Renders the volume representation of the given input buffer.
 * The volume is rendered as a line of '=' characters of length between 0 and WIN_WIDTH.
 *
 * @param inputBuffer Input buffer to render the volume for.
 * @param framesPerBuffer Number of frames in the buffer.
 */
void streamCallBackVolume(
    const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, int num_input_channels, int num_output_channels
) {
  float *in = (float *) inputBuffer;
  float *out = (float *) outputBuffer;

  const int NUM_INPUT_CHANNELS = num_input_channels;
  float channelVolumes[NUM_INPUT_CHANNELS];

  for (unsigned long channelNum = 0; channelNum < NUM_INPUT_CHANNELS; channelNum++) {
    channelVolumes[channelNum] = 0.0f;
  }

  for (unsigned long i = 0; i < framesPerBuffer * NUM_INPUT_CHANNELS; i += NUM_INPUT_CHANNELS) {
    for (unsigned long channelNum = 0; channelNum < NUM_INPUT_CHANNELS; channelNum++) {
      channelVolumes[channelNum] = fmaxf(channelVolumes[channelNum], fabsf(in[i + channelNum]));
    }
  }

  int initial_x;
  int initial_y;
  getyx(VOL_WIN, initial_y, initial_x);

  for (unsigned long channelNum = 0; channelNum < NUM_INPUT_CHANNELS; channelNum++) {
    wmove(VOL_WIN, VOL_INIT_Y + channelNum + 1, VOL_INIT_X);
    for (int i = 0; i < WIN_WIDTH; i++) {
      float barProportion = (float)i / ((float) WIN_WIDTH);
      if (barProportion <= channelVolumes[channelNum]) {
        waddch(VOL_WIN, '=');
      } else {
        waddch(VOL_WIN, ' ');
      }
    }
  }

  wmove(VOL_WIN, initial_y, initial_x);
}