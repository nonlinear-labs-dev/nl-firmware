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
		
		if(!isActive())
			return "";
		
		EditBufferModel eb = EditBufferModel.get();
		BasicParameterModel bpm = eb.getSelectedParameter();
		
		if(bpm instanceof ModulateableParameter) {
			int mcParamID = ((ModulateableParameter)bpm).mcParameterID.getValue();
			
			BasicParameterModel mc = eb.findParameter(mcParamID);
			Parameter mcparam = NonMaps.get().getNonLinearWorld().getParameterEditor().findParameter(mcParamID);
			
			return mcparam.getDecoratedValue(true, mc.originalValue.getValue());
		}
		
		return "";
	}

}
