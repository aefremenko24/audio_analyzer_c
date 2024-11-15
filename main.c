#include <stdlib.h>

#include "volume.h"
#include "display.h"
#include "frequencies.h"
#include "user_prompts.h"
#include "stream.h"

int main() {
  init_stream();
  streamCallbackData *currentSpectroData = init_spectro_data();
  int inputDeviceSelection = prompt_device(Input);
  int outputDeviceSelection = prompt_device(Output);
  process_stream(inputDeviceSelection, outputDeviceSelection, currentSpectroData);
  endwin();

  return EXIT_SUCCESS;
}