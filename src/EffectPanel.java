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
	RemoveButton removeButton;
	private int chainIndex, linkIndex;
		
	public EffectPanel(String name, int _chainIndex, int _linkIndex){
		chainIndex = _chainIndex;
		linkIndex = _linkIndex;
		setLayout(new BoxLayout(this,BoxLayout.PAGE_AXIS));
		JLabel title = new JLabel(name);
		removeButton = new RemoveButton("Remove Effect");
		add(title);
		add(removeButton);
		setVisible(true);
	}
		
	//need to override this
	public void remove(){

	}
	
	private class RemoveButton extends JButton implements ActionListener{
		private RemoveButton(String name){
			super(name);
			addActionListener(this);
		}
		public void actionPerformed(ActionEvent e){
			//JNIBridge.removeChainLink();
			remove();
			System.out.println("removing effect");
		}	
	}
	protected class ControlSlider extends JSlider implements ChangeListener{
		int parameterIndex;
		protected ControlSlider(int min, int max, int paramIndex){
			super(min,max);
			parameterIndex = paramIndex;
			addChangeListener(this);
		}
		public void stateChanged(ChangeEvent e) {
			//chain, effect, parameter, index, float, value
			int err = -1;
			Object m = e.getSource();
			err = JNIBridge.setParameter(chainIndex,linkIndex,parameterIndex,this.getValue());
			if(err !=0 ){
				System.out.println("There was a problem setting an effect parameter.");
			}
		}
	}	
}
