package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.EditParameter;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public abstract class InvertedYModulatableParameter extends ModulatableParameter {

	public InvertedYModulatableParameter(MapsLayout parent, String s, int parameterID) {
		super(parent, s, parameterID);
	}

	@Override
	public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
		boolean noDrag = SetupModel.get().localSettings.editParameter.getValue() == EditParameter.never;

		if (isSelected() && !noDrag) {

			double xPix = newPoint.getX() - oldPoint.getX();
			double yPix = newPoint.getY() - oldPoint.getY();

			double pix = xPix;

			if (Math.abs(yPix) > Math.abs(xPix))
				pix = yPix;

			if (currentParameterChanger != null)
				currentParameterChanger.changeBy(fine, pix);
			return this;
		}

		return null;
	}

}
