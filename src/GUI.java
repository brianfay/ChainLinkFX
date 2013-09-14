package ChainLinkFX;

import javax.swing.SwingUtilities;

public class GUI{
	FrameView theFrame;
	public GUI(){
		SwingUtilities.invokeLater(new Runnable(){
			public void run(){
				init();
			}
		});
	}
	private void init(){
		theFrame = new FrameView();
	}
}
