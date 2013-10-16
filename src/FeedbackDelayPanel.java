package ChainLinkFX;

import javax.swing.JSlider;

public class FeedbackDelayPanel extends EffectPanel{
			
		JSlider timeSlider, feedbackSlider;
		
		public FeedbackDelayPanel(ChainPanel chainPanel){
			super("Feedback Delay", chainPanel);
			timeSlider = new ControlSlider(1, 5000, 0, this);
			feedbackSlider = new ControlSlider(0, 100, 1, this);
			add(timeSlider);
			add(feedbackSlider);
		}
}
