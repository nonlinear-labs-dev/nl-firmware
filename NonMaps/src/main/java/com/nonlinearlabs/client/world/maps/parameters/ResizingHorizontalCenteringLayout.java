package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.LayoutHorizontal;
import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;

class ResizingHorizontalCenteringLayout extends LayoutHorizontal {

	ResizingHorizontalCenteringLayout(MapsLayout parent) {
		super(parent);
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
		double maxX = getLeftMargin();
		double y = getTopMargin();
		double maxHeight = 0;

		for (MapsControl c : getChildren()) {
			c.doFirstLayoutPass(levelOfDetail);
			c.moveTo(maxX, y);
			maxX += c.getNonPosition().getWidth();
			maxHeight = Math.max(maxHeight, c.getNonPosition().getHeight());
		}

		double maxWidth = maxX;
		maxX = getLeftMargin();

		for (MapsControl c : getChildren()) {
			c.doSecondLayoutPass(maxWidth, maxHeight);
			double ctrlY = (maxHeight - c.getNonPosition().getHeight()) / 2.0;
			c.moveTo(maxX, ctrlY);
			maxX += c.getNonPosition().getWidth();
		}

		setNonSize(maxWidth + getWidthMargin(), maxHeight + getHeightMargin());
	}
}
