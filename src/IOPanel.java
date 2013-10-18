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
	JSlider inputSlider, outputSlider;
	public IOPanel(ChainPanel _chainPanel){
		chainPanel = _chainPanel;
		setLayout(new GridLayout(2,2));
		inputSlider = new ControlSlider(0, 100, 0, this);
		outputSlider = new ControlSlider(0, 100, 1, this);
		add(new JLabel("input"));
		add(new JLabel("output"));
		add(inputSlider);
		add(outputSlider);
	}
	
	private class ControlSlider extends JSlider implements ChangeListener{
		int parameterIndex;
		IOPanel ioPanel;
		protected ControlSlider(int min, int max, int paramIndex, IOPanel _ioPanel){
			super(min,max);
			parameterIndex = paramIndex;
			ioPanel = _ioPanel;
			addChangeListener(this);
		}
		public void stateChanged(ChangeEvent e) {
			//chain, effect, parameter, index, float, value
			int err = -1;
			err = ioPanel.chainPanel.setIO(parameterIndex,this.getValue());
			if(err != 0){
				System.out.println("There was a problem setting an effect parameter.");
			}
		}
	}
}
