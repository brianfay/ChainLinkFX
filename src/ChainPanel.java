package ChainLinkFX;

import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JComboBox;
import javax.swing.JButton;
import javax.swing.JFrame;
import java.util.ArrayList;
import javax.swing.BoxLayout;
import java.awt.event.*;
import java.awt.Dimension;

public class ChainPanel extends JPanel{
	private MainFrame frame;
	private JComboBox<String> effectComboBox;
	private ArrayList<EffectPanel> effectList;
	private AddButton addButton;

	public ChainPanel(MainFrame _frame){
		frame = _frame;
		setLayout(new BoxLayout(this,BoxLayout.PAGE_AXIS));
		effectComboBox = new JComboBox<String>();
		for(int i = 0; i < EffectInfo.effectNameList.length; i++){
			String s = EffectInfo.effectNameList[i];
			effectComboBox.addItem(s);
		}
		effectList = new ArrayList<EffectPanel>();
		effectComboBox.setPreferredSize(new Dimension(70,20));
		addButton = new AddButton("Add Effect", this);
		add(new JLabel("Chain"));
		JPanel comboBoxContainerPanel = new JPanel();
		comboBoxContainerPanel.add(effectComboBox);
		add(comboBoxContainerPanel);
		add(addButton);
		setVisible(true);
	}
	
	public void removeChainLink(EffectPanel effectPanel){
		int linkIndex = getLinkIndex(effectPanel);
		frame.removeChainLink(this, linkIndex);
		remove(effectPanel);
		effectList.remove(effectPanel);
		frame.repaint();
	}
	
	public void addChainLink(int selectedEffect, EffectPanel effectPanel){
		add(effectPanel);
		effectList.add(effectPanel);
		int linkIndex = getLinkIndex(effectPanel);
		frame.addChainLink(this, linkIndex, selectedEffect);
		frame.repaint();
	}
	
	public int setParameter(int parameterIndex, int value, EffectPanel effectPanel){
		int linkIndex = getLinkIndex(effectPanel);
		frame.setParameter(this, linkIndex, parameterIndex, value);
		return 0;
	}
	
	public int getLinkIndex(EffectPanel effectPanel){
		int linkIndex = -1;
		for(int i = 0; i < effectList.size(); i++){
			EffectPanel e = effectList.get(i);
			if(e.equals(effectPanel)){
				linkIndex = i;
			}
		}
		System.out.println("EffectList size: " + effectList.size());
		System.out.println("Link index: " + linkIndex);
		if(linkIndex < 0){
			System.out.println("Could not determine link index");
		}
		return linkIndex;
	}
	
	private class AddButton extends JButton implements ActionListener{
		ChainPanel chainPanel;
		private AddButton(String name, ChainPanel _chainPanel){
			super(name);
			chainPanel = _chainPanel;
			addActionListener(this);
		}
		public void actionPerformed(ActionEvent e){
			int selectedEffect = effectComboBox.getSelectedIndex();
			EffectPanel effectPanel = EffectInfo.addEffectPanel(selectedEffect, chainPanel);
			chainPanel.addChainLink(selectedEffect, effectPanel);
		}
	}
}
