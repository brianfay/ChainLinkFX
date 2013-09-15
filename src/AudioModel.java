package ChainLinkFX;

import java.util.ArrayList;

public class AudioModel{
	
	ArrayList<StreamModel> streams = new ArrayList<StreamModel>();
	ArrayList<Device> devices = new ArrayList<Device>();
	
	public AudioModel(){
		JNIBridge.initPA();
		initDevices();
	}

	public void initDevices(){
		devices.clear();
		int deviceCount = JNIBridge.getDeviceCount();
		for(int i = 0; i < deviceCount; i++){
			devices.add(JNIBridge.constructDevice(i));
		}
		//debug printing
		for(int i = 0; i < devices.size(); i++){
			Device d = devices.get(i);
			
			System.out.println("Device Name: " + d.name);
		}
	}
}
