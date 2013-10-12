package ChainLinkFX;

import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JComboBox;

public class ChainPanel extends JPanel{
	private JComboBox<String> effectComboBox;
	int chainIndex;
	
	public ChainPanel(int _chainIndex){
		chainIndex = _chainIndex;
		
		effectComboBox = new JComboBox<String>();
		for(int i = 0; i < EffectInfo.effectNameList.length; i++){
			String s = EffectInfo.effectNameList[i];
			effectComboBox.addItem(s);
		}
		
		add(new JLabel("Chain # " + chainIndex));
		add(effectComboBox);
		setVisible(true);
	}
}
