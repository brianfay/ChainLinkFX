package ChainLinkFX;

public class Device{
    String name, hostApiName;
    int deviceIndex;
    int maxInputChannels;
    int maxOutputChannels;

	/* don't really care about these things on Java end
    double defaultLowInputLatency;
    double defaultLowOutputLatency;

    double defaultHighInputLatency;
    double defaultHighOutputLatency;

    double defaultSampleRate;
    */
    
    public Device(String _name, String _hostApiName, int _deviceIndex, int _maxInputChannels, int _maxOutputChannels){
		name = _name;
		hostApiName = _hostApiName;
		deviceIndex = _deviceIndex;
		maxInputChannels = _maxInputChannels;
		maxOutputChannels = _maxOutputChannels;
	}
}
