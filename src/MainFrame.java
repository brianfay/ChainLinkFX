package ChainLinkFX;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JTabbedPane;
import javax.swing.JButton;
import javax.swing.BoxLayout;
import javax.swing.JOptionPane;
import javax.swing.Icon;
import java.awt.event.*;

public class MainFrame extends JFrame{
	
	Device inputDevice, outputDevice;
	JTabbedPane streamPane;
	AddButton addButton;
	RemoveButton removeButton;
	
	
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
		JPanel containerPanel = new JPanel();
		containerPanel.setLayout(new BoxLayout(containerPanel,BoxLayout.PAGE_AXIS));
		streamPane = new JTabbedPane();
		addButton = new AddButton("Add Stream",this);
		removeButton = new RemoveButton("Remove Stream",this);
		
		containerPanel.add(streamPane);
		containerPanel.add(addButton);
		containerPanel.add(removeButton);
		
		streamPane.setVisible(true);
		addButton.setVisible(true);
		removeButton.setVisible(true);
		
		this.add(containerPanel);
		this.pack();
		this.setVisible(true);
	}
	
	private class AddButton extends JButton implements ActionListener{
		JFrame theFrame;
		private AddButton(String name, JFrame frame){
			super(name);
			theFrame = frame;
			addActionListener(this);
		}
		public void actionPerformed(ActionEvent e){
			int err = JNIBridge.addChain(inputDevice.deviceIndex, outputDevice.deviceIndex);
			if(err != 0){
				System.out.println("There was a problem initializing the chain.");
				return;
			}
			streamPane.add(new JPanel().add(new JLabel("Emptiness")));
			theFrame.pack();
			theFrame.repaint();
		}
	}
	private class RemoveButton extends JButton implements ActionListener{
		JFrame theFrame;
		private RemoveButton(String name, JFrame frame){
			super(name);
			theFrame = frame;
			addActionListener(this);
		}
		public void actionPerformed(ActionEvent e){
			//System.out.println(streamPane.getSelectedIndex());
			if(streamPane.getSelectedIndex() >= 0){
				int err = JNIBridge.removeChain(streamPane.getSelectedIndex());
				if(err != 0){
					return;
				}
				if(streamPane.getTabCount() > 0){
					//will the garbage collector get rid of the tabbed pane here?
					streamPane.removeTabAt(streamPane.getSelectedIndex());
				}
			}
			theFrame.pack();	
		}	
	}
}
