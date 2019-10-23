package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public abstract class ZoomReactingControl extends MapsControl {

	private static double fadeInRange = 1;

	public ZoomReactingControl(MapsLayout parent) {
		super(parent);
	}

	protected abstract double getBasicWidth();

	protected abstract double getBasicHeight();

	protected abstract double getMinHeight();

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
		setNonSize(getXDimension(levelOfDetail) + getWidthMargin(), getYDimension(levelOfDetail) + getHeightMargin());
	}

	protected double getYDimension(double levelOfDetail) {
		double levelOfDetailForFullVisibility = getLevelOfDetailForFullVisibility();
		double basicHeight = getBasicHeight();
		double minHeight = getMinHeight();
		return calcYDimension(levelOfDetail, levelOfDetailForFullVisibility, basicHeight, minHeight);
	}

	protected double calcYDimension(double levelOfDetail, double levelOfDetailForFullVisibility, double basicHeight,
			double minHeight) {
		if (levelOfDetail >= levelOfDetailForFullVisibility) {
			return basicHeight;
		} else if (levelOfDetail >= (levelOfDetailForFullVisibility - fadeInRange)) {
			double scale = levelOfDetail;
			double fullScale = levelOfDetailForFullVisibility;
			double diff = 1 - (fullScale - scale);
			double resHeight = minHeight + diff * (basicHeight - minHeight);
			return resHeight;
		}

		return minHeight;
	}

	protected double getXDimension(double levelOfDetail) {
		return getBasicWidth();
	}
}
