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
	private Rect selectionRect = null;

	public MoveSomeBanksLayer(MapsControl parent, Position pos) {
		super(parent);
		startPosition = pos;
		selectionRect = new Rect();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);
		selectionRect.fillAndStroke(ctx, new RGBA(255, 255, 255, 0.25), Millimeter.toPixels(0.1),
				new RGB(255, 255, 255));
	}

	public void update(Position pos) {
		selectionRect = new Rect(pos, startPosition);
	}

	public Rect getSelectionRect() {
		return selectionRect;
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
	}
}
