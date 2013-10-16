package ChainLinkFX;

import javax.swing.JSlider;

public class SingleTapDelayPanel extends EffectPanel{
			
		JSlider timeSlider, feedbackSlider;
		
		public SingleTapDelayPanel(ChainPanel chainPanel){
			super("Single Tap Delay", chainPanel);
			timeSlider = new ControlSlider(1, 5000, 0, this);
			feedbackSlider = new ControlSlider(0, 100, 1, this);
			add(timeSlider);
			add(feedbackSlider);
		}
}
