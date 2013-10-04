package ChainLinkFX;

public class Device{
	private static Device instance = null;
	
	protected Device(){
		
	}
    String name;
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
    
    public Device(String _name, int _deviceIndex, int _maxInputChannels, int _maxOutputChannels){
		name = _name;
		deviceIndex = _deviceIndex;
		maxInputChannels = _maxInputChannels;
		maxOutputChannels = _maxOutputChannels;
	}
}
