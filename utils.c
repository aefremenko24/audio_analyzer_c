#include "portaudio.h"
#include <stdlib.h>
#include "display.h"
#include <fftw3.h>
#include "curses.h"

/**
 * Check and output errors in the portaudio stream.
 *
 * @param err Portaudio error code.
 */
void checkErr(PaError err) {
  if (err != paNoError) {
    printf("PortAudio error: %s", Pa_GetErrorText(err));
    endwin();
    exit(EXIT_FAILURE);
  }
}