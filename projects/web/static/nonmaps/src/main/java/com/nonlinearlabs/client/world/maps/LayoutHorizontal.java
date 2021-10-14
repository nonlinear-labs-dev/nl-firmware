package com.nonlinearlabs.client.world.maps;

public abstract class LayoutHorizontal extends MapsLayout {

	public LayoutHorizontal(MapsControl parent) {
		super(parent);
	}

	protected NonDimension layoutChildren(double levelOfDetail) {
		double maxX = 0;
		double y = getTopMargin();
		double maxHeight = 0;
		boolean first = true;

		for (MapsControl c : getChildren()) {
			if (!first)
				maxX += getPadding();

			c.doFirstLayoutPass(levelOfDetail);

			if (skipChildOnLayout(c)) {
				continue;
			}

			c.moveTo(maxX, y);
			maxX += c.getNonPosition().getWidth();
			maxHeight = Math.max(maxHeight, c.getNonPosition().getHeight());
			first = false;
		}

		first = true;
		double maxWidth = maxX;
		maxX = getLeftMargin();

		for (MapsControl c : getChildren()) {
			if (!first)
				maxX += getPadding();

			c.doSecondLayoutPass(maxWidth, maxHeight);

			if (skipChildOnLayout(c)) {
				continue;
			}

			c.moveTo(maxX, y);
			maxX += c.getNonPosition().getWidth();
			first = false;
		}

		return new NonDimension(maxX + getRightMargin(), maxHeight + getHeightMargin());
	}

	public boolean skipChildOnLayout(MapsControl c) {
		return false;
	}
}
