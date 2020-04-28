package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.presenters.ParameterPresenterProviders;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.maps.MapsLayout;

abstract class ValueControl extends ZoomReactingControl {

	protected ParameterPresenter presenter;

	ValueControl(MapsLayout parent, int parameterID) {
		super(parent);

		ParameterPresenterProviders.get().registerForCurrentVoiceGroup(parameterID, v -> {
			if(EditBufferModel.get().soundType.getValue() == SoundType.Layer) {
				int paramNumber = v.id.getNumber();
				if(paramNumber == 249 || paramNumber == 250 || paramNumber == 252 || paramNumber == 253 || 
				   paramNumber == 364 || paramNumber == 365 || paramNumber == 366 || paramNumber == 367) 
				{
					presenter = ParameterPresenterProviders.get().getParameterPresenter(VoiceGroup.I, v.id.getNumber());
					return true;
				}
			}
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
		return presenter.displayValues[presenter.displayValues.length - 1];
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
