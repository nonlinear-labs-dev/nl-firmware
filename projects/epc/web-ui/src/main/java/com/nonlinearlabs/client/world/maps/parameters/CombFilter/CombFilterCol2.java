package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableNumericalControl;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSwitch;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.Spacer;

class CombFilterCol2 extends ParameterColumn {

	private class Pitch extends ModulatableParameter {

		private Pitch(MapsLayout parent) {
			super(parent, 115);
			addChild(new ModulateableNumericalControl(this, 115));
			addChild(new Spacer(this, 1, 2));
		}

		private boolean fine = false;

		@Override
		public Control mouseDown(Position eventPoint) {
			fine = eventPoint.getX() > getPixRect().getCenterPoint().getX();
			return super.mouseDown(eventPoint);
		}

		@Override
		public boolean forceFine(boolean f) {
			return fine;
		}

		@Override
		public Control doubleClick(Position pos) {
			if (pos.getX() < getPixRect().getCenterPoint().getX())
				setDefault();
			else {
				EditBufferUseCases.get().roundParameter(getParameterNumber());
			}
			return this;
		}
	}

	CombFilterCol2(MapsLayout parent) {
		super(parent);
		addChild(new Pitch(this));
		addChild(new ModulationSourceSlider(this, 118));
		addChild(new ModulationSourceSwitch(this, 117));
	}
}
