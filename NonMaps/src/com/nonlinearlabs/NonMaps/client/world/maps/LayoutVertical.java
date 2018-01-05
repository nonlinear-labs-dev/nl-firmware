package com.nonlinearlabs.NonMaps.client.world.maps;

abstract class LayoutVertical extends MapsLayout {

	LayoutVertical(MapsLayout parent) {
		super(parent);
	}

	public enum Direction {
		LEFT, RIGHT, CENTER
	};

	protected NonDimension layoutChildren(double levelOfDetail) {
		double maxY = getTopMargin();
		double x = getLeftMargin();
		double maxWidth = 0;
		double padding = getPadding();

		for (MapsControl c : getChildren()) {
			c.doFirstLayoutPass(levelOfDetail);
			c.moveTo(x, maxY);
			maxY += c.getNonPosition().getHeight();
			maxY += padding;
			maxWidth = Math.max(maxWidth, c.getNonPosition().getWidth());
		}

		if (!getChildren().isEmpty())
			maxY -= padding;

		double maxHeight = maxY + getBottomMargin();
		maxY = getYMargin();

		Direction layoutDirection = getLayoutDirection();

		for (MapsControl c : getChildren()) {
			c.doSecondLayoutPass(maxWidth, maxHeight);

			switch (layoutDirection) {

			case CENTER:
				double centeredX = (maxWidth - c.getNonPosition().getWidth()) / 2.0;
				c.moveTo(centeredX + getXMargin(), maxY);
				break;

			case LEFT:
				c.moveTo(getXMargin(), maxY);
				break;

			case RIGHT:
				double left = maxWidth - c.getNonPosition().getWidth();
				c.moveTo(left + getXMargin(), maxY);
				break;

			default:
				break;
			}

			maxY += c.getNonPosition().getHeight();
			maxY += padding;
		}
		return new NonDimension(maxWidth + getWidthMargin(), maxHeight);
	}

	public Direction getLayoutDirection() {
		return Direction.CENTER;
	}

	public double getXMargin() {
		return 0;
	}

	public double getYMargin() {
		return 0;
	}
}
