package ChainLinkFX;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JTabbedPane;
import javax.swing.JButton;
import javax.swing.BoxLayout;
import javax.swing.JOptionPane;
import javax.swing.Icon;
import java.util.ArrayList;
import javax.swing.JScrollPane;
import java.awt.event.*;
import java.awt.Dimension;

public class MainFrame extends JFrame{
	
	Device inputDevice, outputDevice;
	JTabbedPane streamPane;
	AddButton addButton;
	RemoveButton removeButton;
	ArrayList<ChainPanel> chainList;
	
	
	public MainFrame(Device _inputDevice, Device _outputDevice){
		super("ChainLinkFX");
		inputDevice = _inputDevice;
		outputDevice =  _outputDevice;
		chainList = new ArrayList<ChainPanel>();
		init();	
	}
	
	public void init(){
		setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
		addWindowListener(new WindowAdapter(){
			@Override
			public void windowClosing(WindowEvent we){
				JNIBridge.terminatePA();
				System.exit(0);
			}
			});
		//setPreferredSize(new Dimension(800,800));
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
	
	public int setParameter(ChainPanel chainPanel, int linkIndex, int parameterIndex, int value){
		int chainIndex = -1;
		for(int i = 0; i < chainList.size(); i++){
			if(chainPanel.equals(chainList.get(i))){
				chainIndex = i;
				System.out.println("setting param on... chainIndex = " + chainIndex);
			}
		}
		int err = 0;
		if(chainIndex >=0){
			err = JNIBridge.setParameter(chainIndex, linkIndex, parameterIndex, value);
			if(err != 0){
				System.out.println("There was a problem setting the parameter.");
			}
		}
		return err;
	}
	
	public int setIO(ChainPanel chainPanel, int parameterIndex, int value){
		int chainIndex = -1;
		for(int i = 0; i < chainList.size(); i++){
			if(chainPanel.equals(chainList.get(i))){
				chainIndex = i;
				System.out.println("setting param " + parameterIndex + " on chain " + chainIndex);
			}
		}
		int err = 0;
		if(chainIndex >=0){
			//sending -1 instead of 0, because setParameter adds 1 to link index
			err = JNIBridge.setParameter(chainIndex, -1, parameterIndex, value);
			if(err != 0){
				System.out.println("There was a problem setting the parameter.");
			}
		}
		return err;
	}
	
	public void addChainLink(ChainPanel chainPanel, int linkIndex, int selectedEffect){
		//actually don't need the linkIndex at this point but since
		//everything about this design is terrible already I'll leave it just in case
		int chainIndex = -1;
		for(int i = 0; i < chainList.size(); i++){
			if(chainPanel.equals(chainList.get(i))){
				chainIndex = i;
				System.out.println("adding... chainIndex = " + chainIndex);
			}
		}
		int err = JNIBridge.addChainLink(chainIndex, selectedEffect);
	}
	
	public void removeChainLink(ChainPanel chainPanel, int linkIndex){
		int chainIndex = -1;
		for(int i = 0; i < chainList.size(); i++){
			if(chainPanel.equals(chainList.get(i))){
				chainIndex = i;
				System.out.println("removing... chainIndex = " + chainIndex);
				System.out.println("linkIndex = " + linkIndex);
			}
		}
		int err = JNIBridge.removeChainLink(chainIndex, linkIndex);
	}
	
	private class AddButton extends JButton implements ActionListener{
		MainFrame theFrame;
		private AddButton(String name, MainFrame frame){
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
			ChainPanel newChainPanel = new ChainPanel(theFrame);
			theFrame.chainList.add(newChainPanel);
			JScrollPane scrollPane = new JScrollPane(newChainPanel);
			scrollPane.setPreferredSize(new Dimension(500,600));
			streamPane.addTab("Chain", scrollPane);
			theFrame.pack();
			theFrame.repaint();
		}
	}
	private class RemoveButton extends JButton implements ActionListener{
		MainFrame theFrame;
		private RemoveButton(String name, MainFrame frame){
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
					int tabIndex = streamPane.getSelectedIndex();
					theFrame.chainList.remove(chainList.get(tabIndex));
					streamPane.removeTabAt(tabIndex);
				}
			}
			theFrame.pack();	
		}	
	}
}
