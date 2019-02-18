package com.nonlinearlabs.NonMaps.client.world.overlay.belt;

import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class ParameterCompareButton extends SVGImage {

	public ParameterCompareButton(Control parent) {
		super(parent, "PreRecall_B_Enabled.svg", "PreRecall_B_Active.svg", "PreRecall_B_Disabled.svg");
	}
	
	public static boolean inCompare = false;

	@Override
	public int getSelectedPhase() {
		
		
		if(EditBufferModel.get().isAnyParamChanged()) {
			if(inCompare) {
				return 1;
			} else {
				return 0;
			}
		} else {
			if(inCompare) {
				return 1;
			}
		}
		
		return 2;
	}
	
	@Override
	public Control click(Position eventPos) {
		if(inCompare || EditBufferModel.get().isAnyParamChanged()) {
			inCompare = !inCompare;
		}
		return this;
	}
	
	

}
