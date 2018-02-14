package com.nonlinearlabs.NonMaps.client.world.maps.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.RGBA;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;

public class MoveSomeBanksLayer extends MapsControl {

	private Position startPosition = null;
	private Position currentPosition = null;

	public MoveSomeBanksLayer(MapsControl parent, Position pos) {
		super(parent);
		startPosition = pos;
		currentPosition = pos;
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);

		double twoMM = Millimeter.toPixels(2);

		Rect minRect;

		if (currentPosition.getX() > startPosition.getX()) {
			if (currentPosition.getY() > startPosition.getY()) {
				minRect = new Rect(startPosition.getX(), startPosition.getY(), currentPosition.getX() - startPosition.getX(),
						currentPosition.getY() - startPosition.getY());
			} else {
				minRect = new Rect(startPosition.getX(), currentPosition.getY(), currentPosition.getX() - startPosition.getX(),
						startPosition.getY() - currentPosition.getY());
			}
		} else {
			if (currentPosition.getY() > startPosition.getY()) {
				minRect = new Rect(currentPosition.getX(), startPosition.getY(), startPosition.getX() - currentPosition.getX(),
						currentPosition.getY() - startPosition.getY());
			} else {
				minRect = new Rect(currentPosition.getX(), currentPosition.getY(), startPosition.getX() - currentPosition.getX(),
						startPosition.getY() - currentPosition.getY());
			}
		}

		setPixRect(minRect);

		minRect.applyPadding(-twoMM, -twoMM, -twoMM, -twoMM);
		minRect.drawRoundedArea(ctx, twoMM, Millimeter.toPixels(0.1), new RGBA(255, 255, 255, 0.25), new RGB(255, 255, 255));
	}

	public void update(Position pos) {
		currentPosition = pos;
	}
}
