package ChainLinkFX;

import javax.swing.SwingUtilities;
import javax.swing.UIManager;
public class ChainLinkFX{
	
	SelectDeviceFrame selectDeviceFrame;
	MainFrame mainFrame;
	Device inputDevice, outputDevice;
	
	public ChainLinkFX(){
		//init portaudio here - terminate wherever window is closed
		JNIBridge.initPA();
		try{
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		}catch(Exception e){
			e.printStackTrace();
		}
		selectDeviceFrame = new SelectDeviceFrame(this);
		//select frame should instantiate audio device in this class
	}
	
	public static void main(String[] args){
		System.loadLibrary("ChainLinkFX");
		//may want to add -nogui support eventually
		
		if(args.length > 0){
				
		}
		else{
			SwingUtilities.invokeLater(new Runnable(){
			public void run(){
				ChainLinkFX chainLinkFX = new ChainLinkFX();
			}
			});	
		}
	}
	public void selectAudioDevice(Device inputDevice, Device outputDevice){
		mainFrame = new MainFrame(inputDevice, outputDevice);
	}
}
