#include <portaudio.h>
#include "display.h"
#include "volume.h"
#include "frequencies.h"
#include <fftw3.h>
#include <ctype.h>
#include <string.h>
#include "dispatch.h"

/**
 * Processes a single buffer and displays its visual representation on the screen.
 *
 * @param inputBuffer Input buffer in the current callback.
 * @param outputBuffer Output buffer in the current callback. (not used)
 * @param framesPerBuffer Number of frames in the buffer.
 * @param timeInfo Timestamps indicating capture and output times. (not used)
 * @param statusFlags Flags for input and output buffers. (not used)
 * @param userData Callback data used for FFT computations.
 * @return 0 if the data was successfully displayed.
 */
static int streamCallBack(
    const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
    void *userData
) {
  (void) timeInfo;
  (void) statusFlags;

  float *in = (float *) inputBuffer;
  float *out = (float *) outputBuffer;

  streamCallBackVolume(inputBuffer, outputBuffer, framesPerBuffer, num_input_channels, num_output_channels);

  streamCallBackFrequencies(inputBuffer, outputBuffer, framesPerBuffer, userData);

  update_global_buffer(in, out);

  refresh_screen();

  return 0;
}

/**
 * Initializes a PulseAudio stream.
 */
void init_stream() {
  PaError err = Pa_Initialize();
  checkErr(err);
}

/**
 * Closes the stream and cleans up all the allocated memory used during the program runtime.
 *
 * @param stream Stream to be closed.
 * @param currentSpectroData Spectro data used for FFT computations.
 */
void close_stream(PaStream *stream, streamCallbackData *currentSpectroData) {
  PaError err = Pa_CloseStream(stream);
  checkErr(err);

  err = Pa_Terminate();
  checkErr(err);

  fftw_destroy_plan(currentSpectroData->p);
  fftw_free(currentSpectroData->in);
  fftw_free(currentSpectroData->out);
  fftw_free(currentSpectroData);

  del_screen();
}

/**
 * Runs the stream processing for the selected device from start to finish.
 *
 * @param deviceSelection User's device selection.
 * @param currentSpectroData Spectro data used for FFT processing.
 * @param err PulseAudio error code.
 */
void process_stream(int inputDeviceSelection, int outputDeviceSelection, streamCallbackData *currentSpectroData) {

  PaStreamParameters inputParameters;
  PaStreamParameters outputParameters;

  memset(&inputParameters, 0, sizeof(inputParameters));
  inputParameters.channelCount = Pa_GetDeviceInfo(inputDeviceSelection)->maxInputChannels;
  inputParameters.device = inputDeviceSelection;
  inputParameters.hostApiSpecificStreamInfo = NULL;
  inputParameters.sampleFormat = paFloat32;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputDeviceSelection)->defaultLowInputLatency;

  if (outputDeviceSelection >= 0) {
    memset(&outputParameters, 0, sizeof(outputParameters));
    outputParameters.channelCount = Pa_GetDeviceInfo(outputDeviceSelection)->maxOutputChannels;
    outputParameters.device = outputDeviceSelection;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputDeviceSelection)->defaultLowInputLatency;
    num_output_channels = outputParameters.channelCount;
  }

  num_input_channels = inputParameters.channelCount;
  init_screen(num_input_channels);

  PaStream *stream;
  PaError err = Pa_OpenStream(
      &stream,
      &inputParameters,
      outputDeviceSelection >= 0 ? &outputParameters : NULL,
      SAMPLE_RATE,
      FRAMES_PER_BUFFER,
      paNoFlag,
      streamCallBack,
      currentSpectroData
  );
  checkErr(err);

  err = Pa_StartStream(stream);
  checkErr(err);

  unsigned char input = '\0';
  while (input != ' ' && input != 'r') {
    input = tolower(getch());
    if (input == 'r') {
      close_stream(stream, currentSpectroData);
      init_stream();
      currentSpectroData = init_spectro_data();
      return process_stream(inputDeviceSelection, outputDeviceSelection, currentSpectroData);
    }
  }

  close_stream(stream, currentSpectroData);
}