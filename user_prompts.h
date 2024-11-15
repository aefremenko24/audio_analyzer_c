/**
 * Displays a list of all available CoreAudio(macOS) or ALSA (Linux) devices
 * and prompts the user to choose the one they wish to work with.
 *
 * @param signalType Type of signal the user is working with. Either Input or Output.
 * @return Device number selected by the user. Value between 1 and number of devices available.
 */
int prompt_device(enum SignalType signalType);