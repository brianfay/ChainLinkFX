package ChainLinkFX;

import java.util.ArrayList;

public class ChainLinkFX{
	
	GUI theGUI;
	ArrayList streams = new ArrayList<StreamModel>();
	
	public ChainLinkFX(){
		theGUI = new GUI();
	}
	
	public static void main(String[] args){
		
		//may want to add -nogui support eventually
		if(args.length > 0){
				
		}
		else{
			ChainLinkFX chainLinkFX = new ChainLinkFX();
		}
	}
}
