package ChainLinkFX;

public class EffectInfo{

	public static String[] effectNameList = {"Feedback Delay","Single Tap Delay"};
	
	public static EffectPanel addEffectPanel(int effectType, ChainPanel chainPanel){
		EffectPanel effectPanel;
		switch(effectType){
			case 1:
				System.out.println("what am i doing about the empty effect?");
				effectPanel = new FeedbackDelayPanel(chainPanel);
				break;
			case 2:
				effectPanel = new SingleTapDelayPanel(chainPanel);
				break;
			default:
				effectPanel = new FeedbackDelayPanel(chainPanel);
		}
		effectPanel.setVisible(true);
		return effectPanel;
	}
}
