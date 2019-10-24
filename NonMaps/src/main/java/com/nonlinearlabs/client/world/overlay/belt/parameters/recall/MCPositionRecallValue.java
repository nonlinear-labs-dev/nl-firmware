package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;

public class MCPositionRecallValue extends RecallValue {

	public MCPositionRecallValue(RecallArea parent) {
		super(parent);
	}

	@Override
	public String getDrawText(Context2d ctx) {

		if (!isActive())
			return "";

		EditBufferModel eb = EditBufferModel.get();
		BasicParameterModel bpm = eb.getSelectedParameter();

		if (bpm instanceof ModulateableParameterModel) {
			int mcParamID = ((ModulateableParameterModel) bpm).mcParameterID.getValue();

			BasicParameterModel mc = eb.findParameter(mcParamID);
			Parameter mcparam = NonMaps.get().getNonLinearWorld().getParameterEditor().findParameter(mcParamID);

			return mcparam.getDecoratedValue(true, mc.originalValue.getValue());
		}

		return "";
	}

}
