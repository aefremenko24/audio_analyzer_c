#include <portaudio.h>

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
);

/**
 * Initializes a PulseAudio stream.
 *
 * @param err PulseAudio error code.
 */
void init_stream(PaError *err);

/**
 * Closes the stream and cleans up all the allocated memory used during the program runtime.
 *
 * @param stream Stream to be closed.
 * @param currentSpectroData Spectro data used for FFT computations.
 * @param err PulseAudio error code.
 */
void close_stream(PaStream *stream, streamCallbackData *currentSpectroData, PaError err);

/**
 * Runs the stream processing for the selected device from start to finish.
 *
 * @param deviceSelection User's device selection.
 * @param currentSpectroData Spectro data used for FFT processing.
 * @param err PulseAudio error code.
 */
void process_stream(int inputDeviceSelection, int outputDeviceSelection, streamCallbackData *currentSpectroData, PaError err);