package ChainLinkFX;

public class EffectInfo{
	public static String[] effectNameList = {"Empty","Delay","Single Tap Delay"};
	
	public static EffectPanel addEffectPanel(int effectType, ChainPanel chainPanel){
		EffectPanel effectPanel;
		switch(effectType){
			case 0:
				System.out.println("what am i doing about the empty effect?");
				effectPanel = new DelayPanel(chainPanel);
				break;
			case 1:
				effectPanel = new DelayPanel(chainPanel);
				break;
			default:
				effectPanel = new DelayPanel(chainPanel);
				break;
		}
		effectPanel.setVisible(true);
		return effectPanel;
	}
}
