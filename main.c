#include <stdlib.h>

#include "volume.h"
#include "display.h"
#include "frequencies.h"
#include "user_prompts.h"
#include "stream.h"

int main() {
  PaError err;

  init_stream(&err);
  streamCallbackData *currentSpectroData = init_spectro_data();
  int inputDeviceSelection = prompt_device(Input);
  int outputDeviceSelection = prompt_device(Output);
  process_stream(inputDeviceSelection, outputDeviceSelection, currentSpectroData, err);
  endwin();

  return EXIT_SUCCESS;
}