package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.recall;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.ModulateableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;

public class MCAmountRecallValue extends RecallValue {

	public MCAmountRecallValue(RecallArea parent) {
		super(parent);
	}

	@Override
	public String getDrawText(Context2d ctx) {
		
		if(!isActive())
			return "";
		
		BasicParameterModel bpm = EditBufferModel.get().getSelectedParameter();
		Parameter p = NonMaps.get().getNonLinearWorld().getParameterEditor().getSelection();
		if (p instanceof ModulatableParameter && bpm instanceof ModulateableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			return m.getModulationAmount().getDecoratedValue(true, ((ModulateableParameter)bpm).ogModAmount.getValue());
		}
		return null;
	}

}
