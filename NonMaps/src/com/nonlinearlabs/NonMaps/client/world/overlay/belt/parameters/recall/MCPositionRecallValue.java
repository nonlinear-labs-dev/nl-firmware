package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.recall;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.ModulateableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;

public class MCPositionRecallValue extends RecallValue {

	public MCPositionRecallValue(RecallArea parent) {
		super(parent);
	}

	@Override
	public String getDrawText(Context2d ctx) {
		EditBufferModel eb = EditBufferModel.get();
		
		if(!isActive())
			return "";
		
		BasicParameterModel bpm = eb.findParameter(eb.selectedParameter.getValue());
		if(bpm instanceof ModulateableParameter) {
			ModulateableParameter modP = (ModulateableParameter)bpm;
			BasicParameterModel mc = eb.findParameter(modP.mcParameterID.getValue());
			Parameter param = NonMaps.get().getNonLinearWorld().getParameterEditor().findParameter(modP.mcParameterID.getValue());
			return param.getDecoratedValue(true, mc.originalValue.getValue());
		}
		
		return "";
	}

}
