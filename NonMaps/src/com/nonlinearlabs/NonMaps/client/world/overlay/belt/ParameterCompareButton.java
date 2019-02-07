package com.nonlinearlabs.NonMaps.client.world.overlay.belt;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class ParameterCompareButton extends SVGImage {

	public ParameterCompareButton(Control parent) {
		super(parent, "PreRecall_B_Enabled.svg", "PreRecall_B_Active.svg", "PreRecall_B_Disabled.svg");
				
		EditBufferModel.get().selectedParameter.onChange(id -> setupChange(id));
		
		setupChange(EditBufferModel.get().selectedParameter.getValue());
	}
	
	private boolean setupChange(int id) {
		EditBufferModel.get().findParameter(id).value.onChange(e -> {
			//inCompare = false;
			return true;
		});
		return true;
	}
	
	public static boolean inCompare = false;

	@Override
	public int getSelectedPhase() {
		
		
		if(EditBufferModel.get().findParameter(EditBufferModel.get().selectedParameter.getValue()).isChanged()) {
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
		if(inCompare || EditBufferModel.get().findParameter(EditBufferModel.get().selectedParameter.getValue()).isChanged()) {
			inCompare = !inCompare;
			NonMaps.get().getNonLinearWorld().getPresetManager().getChildren().forEach(c -> c.invalidate(INVALIDATION_FLAG_ZOOMED));
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
		return this;
	}
	
	

}
