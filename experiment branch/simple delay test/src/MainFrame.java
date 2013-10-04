package ChainLinkFX;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JTabbedPane;
import javax.swing.JButton;
import javax.swing.BoxLayout;
import javax.swing.JOptionPane;
import javax.swing.Icon;
import javax.swing.JSlider;
import javax.swing.event.*;
import java.awt.event.*;

public class MainFrame extends JFrame{
	
	Device inputDevice, outputDevice;
	
	JSlider timeSlider, feedbackSlider;
	
	public MainFrame(Device _inputDevice, Device _outputDevice){
		super("ChainLinkFX");
		inputDevice = _inputDevice;
		outputDevice =  _outputDevice;
		init();
	}
	
	public void init(){
		this.setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
		this.addWindowListener(new WindowAdapter(){
			@Override
			public void windowClosing(WindowEvent we){
				JNIBridge.terminatePA();
				System.exit(0);
			}
			});
		//here's the code to add one audio stream:
		JNIBridge.addChain(inputDevice.deviceIndex, outputDevice.deviceIndex);
		JPanel containerPanel = new JPanel();
		containerPanel.setLayout(new BoxLayout(containerPanel,BoxLayout.PAGE_AXIS));
		timeSlider = new MySlider(1, 5000);
		feedbackSlider = new MySlider(0,99);
		containerPanel.add(new JLabel("Delay Time in ms"));
		containerPanel.add(timeSlider);
		containerPanel.add(new JLabel("Feedback percentage"));
		containerPanel.add(feedbackSlider);
		this.add(containerPanel);
		this.pack();
		this.setVisible(true);
	}
	private class MySlider extends JSlider implements ChangeListener{
		private MySlider(int min, int max){
			super(min,max);
			addChangeListener(this);
		}
		public void stateChanged(ChangeEvent e) {
			//chain, effect, parameter, index, float, value
			int err = -1;
			Object m = e.getSource();
			if(m.equals(timeSlider)){
			err = JNIBridge.setParameter(0,0,0,timeSlider.getValue());		
			}
			else if(m.equals(feedbackSlider)){
			err =JNIBridge.setParameter(0,0,1,feedbackSlider.getValue());	
			}
			if(err !=0 ){
				System.out.println("There was a problem setting an effect parameter.");
			}
		}
	}
}
