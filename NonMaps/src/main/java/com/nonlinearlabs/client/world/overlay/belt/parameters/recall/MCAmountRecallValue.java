package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenter;

public class MCAmountRecallValue extends RecallValue {

	public MCAmountRecallValue(RecallArea parent) {
		super(parent);
	}

	@Override
	public String getDrawText(Context2d ctx) {
		if (!isActive())
			return "";

		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		return p.modulation.originalModulationAmountDecoratedString;
	}

}
