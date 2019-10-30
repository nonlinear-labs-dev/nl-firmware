package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.presenters.ParameterPresenterProviders;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.maps.MapsLayout;

abstract class ValueControl extends ZoomReactingControl {

	protected ParameterPresenter presenter;

	ValueControl(MapsLayout parent, int parameterID) {
		super(parent);

		ParameterPresenterProviders.get().register(parameterID, v -> {
			presenter = v;
			return true;
		});
	}

	@Override
	public void getStateHash(Checksum crc) {
		super.getStateHash(crc);
		crc.eat(getValue());
	}

	protected boolean isBiPolar() {
		return presenter.bipolar;
	}

	public double getValue() {
		return presenter.controlPosition;
	}

	public String getDisplayValue() {
		return presenter.displayValues[0];
	}

	public Parameter getParentParameterLayout() {
		Control parent = getParent();

		while (parent != null) {
			if (parent instanceof Parameter) {
				return (Parameter) parent;
			}

			parent = parent.getParent();
		}

		return null;
	}
}
