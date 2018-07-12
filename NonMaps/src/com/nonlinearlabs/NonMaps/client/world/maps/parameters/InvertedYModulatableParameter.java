package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.EditParameter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public abstract class InvertedYModulatableParameter extends ModulatableParameter {

	public InvertedYModulatableParameter(MapsLayout parent, String s) {
		super(parent, s);
	}

	@Override
	public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
		boolean noDrag = Setup.get().localSettings.editParameter.getValue() == EditParameter.never;

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
