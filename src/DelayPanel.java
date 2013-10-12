package ChainLinkFX;

import javax.swing.JSlider;

public class DelayPanel extends EffectPanel{
			
		JSlider timeSlider, feedbackSlider;
		
		public DelayPanel(int chainIndex, int linkIndex){
			super("Delay", chainIndex, linkIndex);
			timeSlider = new ControlSlider(1, 5000, 0);
			feedbackSlider = new ControlSlider(0, 100, 1);
			add(timeSlider);
			add(feedbackSlider);
		}
}
