package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.presenters.ParameterPresenterProviders;

public class SmallParameterName extends LabelSmall {
	private ParameterPresenter presenter;

	public SmallParameterName(Parameter parent, int parameterID) {
		super(parent);

		ParameterPresenterProviders.get().registerForCurrentVoiceGroup(parameterID, v -> {
			presenter = v;
			return true;
		});
	}

	@Override
	protected double getBasicHeight() {
		return 20;
	}

	@Override
	protected double getBasicWidth() {
		return 80;
	}

	@Override
	protected double getMinHeight() {
		return 7.5;
	}

	protected String getDisplayText() {
		return presenter.shortName;
	}
}