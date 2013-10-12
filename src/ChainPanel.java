package ChainLinkFX;

import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JComboBox;
import javax.swing.JButton;
import java.util.ArrayList;
import javax.swing.BoxLayout;
import java.awt.event.*;

public class ChainPanel extends JPanel{
	private JComboBox<String> effectComboBox;
	private ArrayList<EffectPanel> effectList;
	private AddButton addButton;
	private int linkCount = 0;
	private int chainIndex;

	public ChainPanel(int _chainIndex){
		chainIndex = _chainIndex;
		setLayout(new BoxLayout(this,BoxLayout.PAGE_AXIS));
		effectComboBox = new JComboBox<String>();
		for(int i = 0; i < EffectInfo.effectNameList.length; i++){
			String s = EffectInfo.effectNameList[i];
			effectComboBox.addItem(s);
		}
		addButton = new AddButton("Add Effect", this);
		add(new JLabel("Chain # " + chainIndex));
		add(effectComboBox);
		add(addButton);
		setVisible(true);
	}
	private class AddButton extends JButton implements ActionListener{
		ChainPanel panel;
		private AddButton(String name, ChainPanel chainPanel){
			super(name);
			panel = chainPanel;
			addActionListener(this);
		}
		public void actionPerformed(ActionEvent e){
			int selectedEffect = effectComboBox.getSelectedIndex();
			JNIBridge.addChainLink(chainIndex, selectedEffect);
			panel.add(EffectInfo.addEffectPanel(chainIndex, (linkCount + 1), selectedEffect));
			panel.repaint();
			linkCount++;
		}
	}
}
