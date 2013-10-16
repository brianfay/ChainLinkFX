package ChainLinkFX;

public class JNIBridge{
	public static native void initPA();
	public static native void terminatePA();
	public static native int getDeviceCount();
	//int for error checking
	public static native int addChain(int inputDeviceIndex, int outputDeviceIndex);
	public static native int removeChain(int chainIndex);
	public static native int addChainLink(int chainIndex, int effectType);
	public static native int removeChainLink(int chainIndex, int linkIndex);
	public static native Device constructDevice(int i);
	public static native int setParameter(int chainIndex, int linkIndex, int parameterIndex, float value);
}
