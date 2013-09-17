package ChainLinkFX;

import javax.swing.SwingUtilities;
import javax.swing.UIManager;

public class GUI{
	MainFrame theFrame;
	public GUI(){
		try{
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		}catch(Exception e){
			e.printStackTrace();
		}
		SwingUtilities.invokeLater(new Runnable(){
			public void run(){
				init();
			}
		});
	}
	private void init(){
		theFrame = new MainFrame();
	}
}
