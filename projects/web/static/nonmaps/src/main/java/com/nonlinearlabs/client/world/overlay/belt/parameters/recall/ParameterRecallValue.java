package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;

public class ParameterRecallValue extends RecallValue {

	public ParameterRecallValue(RecallArea parent) {
		super(parent);
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return EditBufferPresenterProvider.getPresenter().selectedParameter.originalParameterValueDecoratedString;
	}
}
