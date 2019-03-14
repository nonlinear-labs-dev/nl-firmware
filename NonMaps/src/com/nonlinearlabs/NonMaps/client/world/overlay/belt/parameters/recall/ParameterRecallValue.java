package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.recall;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;

public class ParameterRecallValue extends RecallValue {

	public ParameterRecallValue(RecallArea parent) {
		super(parent);
	}

	@Override
	public String getDrawText(Context2d ctx) {
		EditBufferModel eb = EditBufferModel.get();
		
		if(!isActive())
			return "";
		
		Parameter param = NonMaps.get().getNonLinearWorld().getParameterEditor().findParameter(eb.selectedParameter.getValue());
		return param.getDecoratedValue(true, eb.getSelectedParameter().originalValue.getValue());
	}
}
