package ChainLinkFX;

public class EffectInfo{
	public static String[] effectNameList = {"Empty","Delay","Single Tap Delay"};
	
	public static EffectPanel addEffectPanel(int chainIndex, int linkIndex, int effectType){
		EffectPanel effectPanel;
		switch(effectType){
			case 0:
				System.out.println("what am i doing about the empty effect?");
				effectPanel = new DelayPanel(chainIndex, linkIndex);
				break;
			case 1:
				effectPanel = new DelayPanel(chainIndex, linkIndex);
				break;
			default:
				effectPanel = new DelayPanel(chainIndex, linkIndex);
				break;
		}
		return effectPanel;
	}
}
