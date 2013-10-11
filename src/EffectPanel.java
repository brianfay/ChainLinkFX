import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.BoxLayout;
import java.awt.event.*;

public abstract class EffectPanel extends JPanel{
		RemoveButton removeButton;
		
		public EffectPanel(String name){
			setLayout(new BoxLayout(this,BoxLayout.PAGE_AXIS));
			JLabel title = new JLabel(name);
			removeButton = new RemoveButton("Remove Effect");
			add(title);
			add(removeButton);
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
				System.out.println("removing effect");
		}	
	}
}
