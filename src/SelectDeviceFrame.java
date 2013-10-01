package ChainLinkFX;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JComboBox;
import javax.swing.BoxLayout;
import java.util.ArrayList;
import java.awt.event.*;

public class SelectDeviceFrame extends JFrame{
	ChainLinkFX chainLinkFX;
	private JComboBox<String> inputDeviceBox;
	private JComboBox<String> outputDeviceBox;
	private ArrayList<Device> deviceList;
	public SelectDeviceFrame(ChainLinkFX app){
		super("Select Device");
		deviceList = new ArrayList<Device>();
		getDevices();
		JPanel containerPanel = new JPanel();
		containerPanel.setLayout(new BoxLayout(containerPanel,BoxLayout.PAGE_AXIS));
		chainLinkFX = app;
		setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
		addWindowListener(new WindowAdapter(){
			@Override
			public void windowClosing(WindowEvent we){
				JNIBridge.terminatePA();
				System.exit(0);
			}
			});
		containerPanel.add(new JLabel("Input device:"));
		inputDeviceBox = new JComboBox<String>();
		containerPanel.add(inputDeviceBox);
		for(int i = 0; i < deviceList.size(); i++){
			Device d = deviceList.get(i);
			inputDeviceBox.addItem(d.name);
		}
		outputDeviceBox = new JComboBox<String>();
		containerPanel.add(new JLabel("Output device:"));
		containerPanel.add(outputDeviceBox);
		for(int i = 0; i < deviceList.size(); i++){
			Device d = deviceList.get(i);
			outputDeviceBox.addItem(d.name);
		}
		containerPanel.add(new OkayButton("Okay",this));
		add(containerPanel);
		pack();
		setVisible(true);
	}
	
	private class OkayButton extends JButton implements ActionListener{
		JFrame theFrame;
		private OkayButton(String name, JFrame frame){
			super(name);
			theFrame = frame;
			addActionListener(this);
		}
		public void actionPerformed(ActionEvent e){
			Device inputDevice = deviceList.get(inputDeviceBox.getSelectedIndex());
			Device outputDevice = deviceList.get(outputDeviceBox.getSelectedIndex());
			chainLinkFX.selectAudioDevice(inputDevice, outputDevice);
			theFrame.setVisible(false);
		}
	}
	
	private void getDevices(){
		int numDevices = JNIBridge.getDeviceCount();
		for(int i =0; i < numDevices; i++){
			deviceList.add(JNIBridge.constructDevice(i));
		}
	}
}
