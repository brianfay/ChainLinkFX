package ChainLinkFX;

public class ChainLinkFX{
	
	GUI theGUI;
	AudioModel audioModel = new AudioModel();
	
	public ChainLinkFX(){
		theGUI = new GUI();
	}
	
	public static void main(String[] args){
		System.loadLibrary("ChainLinkFX");
		//may want to add -nogui support eventually
		if(args.length > 0){
				
		}
		else{
			ChainLinkFX chainLinkFX = new ChainLinkFX();
		}
	}
}
