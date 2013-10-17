package ChainLinkFX;

import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JFrame;
import javax.swing.JButton;
import java.awt.GridLayout;
import javax.swing.JSlider;
import javax.swing.event.*;
import java.awt.event.*;

public class IOPanel extends JPanel{
	ChainPanel chainPanel;
	
	public IOPanel(ChainPanel _chainPanel){
		chainPanel = _chainPanel;
		setLayout(new GridLayout(2,2));
		add(new JLabel("input"));
		add(new JLabel("output"));
		add(new JLabel("i slider goes here"));
		add(new JLabel("o slider goes here"));
	}
}
