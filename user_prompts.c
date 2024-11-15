#include "utils.h"
#include "portaudio.h"
#include "stdlib.h"

/**
 * Displays a list of all available CoreAudio(macOS) or ALSA (Linux) devices
 * and prompts the user to choose the one they wish to work with.
 *
 * @param signalType Type of signal the user is working with. Either Input or Output.
 * @return Device number selected by the user. Value between 1 and number of devices available.
 */
int prompt_device(enum SignalType signalType) {
  if (signalType == Output) {
    char selectOutput;
    printf("Would you like to select an output source? (y/n)\n");
    scanf(" %c", &selectOutput);
    if (selectOutput != 'y') {
      return -1;
    }
  }

  int numDevices = Pa_GetDeviceCount();
  if (numDevices < 0) {
    printf("Encountered error while getting the number of devices.\n");
    exit(EXIT_FAILURE);
  } else if (numDevices == 0) {
    printf("No devices found on this machine.\n");
    exit(EXIT_SUCCESS);
  }
  printf("Number of devices: %d\n", numDevices);

  const PaDeviceInfo *deviceInfo;

  for (int device_index = 0; device_index < numDevices; device_index++) {
    deviceInfo = Pa_GetDeviceInfo(device_index);
    printf("Device # %d\n", device_index);
    printf("\tName: %s\n", deviceInfo->name);
    printf("\tMax Input Channels: %d\n", deviceInfo->maxInputChannels);
    printf("\tMax Output Channels: %d\n", deviceInfo->maxOutputChannels);
    printf("\tDefault Sample Rate: %f\n", deviceInfo->defaultSampleRate);
  }

  char* signal_type = signalType == Input ? "input" : "output";
  int deviceSelection;
  printf("\nWhich %s device would you like to use? Enter a number from 0 to %d %s\n", signal_type, numDevices - 1, "below:");
  int input_check;
  input_check = scanf("%d", &deviceSelection);

  while (input_check != 1 || deviceSelection < 0 || deviceSelection > numDevices - 1) {
    printf("Device index entered is invalid. Try again:\n");
    input_check = scanf("%d", &deviceSelection);
  }

  while (signalType == Input && Pa_GetDeviceInfo(deviceSelection)->maxInputChannels == 0) {
    printf("This device has no input channels. Select a different one.\n");
    scanf("%d", &deviceSelection);
  }

  while (signalType == Output && Pa_GetDeviceInfo(deviceSelection)->maxOutputChannels == 0) {
    printf("This device has no output channels. Select a different one.\n");
    scanf("%d", &deviceSelection);
  }

  return deviceSelection;
}
