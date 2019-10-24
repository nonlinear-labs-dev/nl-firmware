package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;

public class MCAmountRecallValue extends RecallValue {

	public MCAmountRecallValue(RecallArea parent) {
		super(parent);
	}

	@Override
	public String getDrawText(Context2d ctx) {

		if (!isActive())
			return "";

		BasicParameterModel bpm = EditBufferModel.getSelectedParameter();
		Parameter p = NonMaps.get().getNonLinearWorld().getParameterEditor().getSelection();
		if (p instanceof ModulatableParameter && bpm instanceof ModulateableParameterModel) {
			ModulatableParameter m = (ModulatableParameter) p;
			return m.getModulationAmount().getDecoratedValue(true,
					((ModulateableParameterModel) bpm).ogModAmount.getValue());
		}
		return null;
	}

}
