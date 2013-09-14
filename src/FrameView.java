package ChainLinkFX;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.JButton;
import javax.swing.BoxLayout;
import java.awt.event.*;

public class FrameView extends JFrame{
	
	JTabbedPane streamPane;
	AddButton addButton;
	RemoveButton removeButton;
	
	
	public FrameView(){
		super("ChainLinkFX");
		init();	
	}
	
	public void init(){
		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
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
			streamPane.addTab("Stream" + (streamPane.getTabCount() + 1),new JButton("cool"));
			theFrame.pack();
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
			if(streamPane.getTabCount() > 0){
				streamPane.removeTabAt(streamPane.getSelectedIndex());
			}	
			theFrame.pack();	
		}
	}
}
