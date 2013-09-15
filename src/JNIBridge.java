package ChainLinkFX;

public class JNIBridge{
	public static native void initPA();
	public static native void terminatePA();
	public static native int getDeviceCount();
	public static native Device constructDevice(int i);
}
