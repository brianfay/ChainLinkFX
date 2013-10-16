package ChainLinkFX;

import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.BoxLayout;
import javax.swing.JSlider;
import javax.swing.event.*;
import java.awt.event.*;

public abstract class EffectPanel extends JPanel{
	ChainPanel chainPanel;
	RemoveButton removeButton;
		
	public EffectPanel(String name, ChainPanel _chainPanel){
		setLayout(new BoxLayout(this,BoxLayout.PAGE_AXIS));
		chainPanel = _chainPanel;
		JLabel title = new JLabel(name);
		removeButton = new RemoveButton(this);
		add(title);
		add(removeButton);
		setVisible(true);
	}
	
	private class RemoveButton extends JButton implements ActionListener{
		EffectPanel effectPanel;
		private RemoveButton(EffectPanel _effectPanel){
			super("Remove Effect");
			effectPanel = _effectPanel;
			addActionListener(this);
		}
		public void actionPerformed(ActionEvent e){
			effectPanel.chainPanel.removeChainLink(effectPanel);
			System.out.println("removing effect");
		}	
	}
	protected class ControlSlider extends JSlider implements ChangeListener{
		int parameterIndex;
		EffectPanel effectPanel;
		protected ControlSlider(int min, int max, int paramIndex, EffectPanel _effectPanel){
			super(min,max);
			parameterIndex = paramIndex;
			effectPanel = _effectPanel;
			addChangeListener(this);
		}
		public void stateChanged(ChangeEvent e) {
			//chain, effect, parameter, index, float, value
			int err = -1;
			//Object m = e.getSource();
			err = effectPanel.chainPanel.setParameter(parameterIndex,this.getValue(), effectPanel);
			if(err !=0 ){
				System.out.println("There was a problem setting an effect parameter.");
			}
		}
	}	
}
