package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.recall;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Dimension;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public abstract class RecallButton extends OverlayControl {
	boolean active;	
	
	public RecallButton(OverlayLayout parent) {
		super(parent);
		active = false;
	}

	public boolean isActive() {
		return active;
	}
	
	public void setActive(boolean b) {
		active = b;
	}
	
	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		if(isActive()) {				
			getPixRect().drawRoundedRect(ctx, Rect.ROUNDING_ALL, Millimeter.toPixels(1), 1, new Gray(77), null);
			drawTriangle(ctx, new Gray(77));
		}
	}
	
	private void drawTriangle(Context2d ctx, RGB color) {
		Rect movedToRight = getPixRect().copy().getMovedBy(new Dimension(getPixRect().getWidth(), 0)).copy();
		movedToRight = movedToRight.getReducedBy(movedToRight.getHeight() / 3);
		
		ctx.beginPath();
		ctx.moveTo(movedToRight.getLeft(), movedToRight.getTop());
		ctx.lineTo(movedToRight.getLeft() + Millimeter.toPixels(3), movedToRight.getCenterPoint().getY());
		ctx.lineTo(movedToRight.getLeft(), movedToRight.getBottom());
		ctx.lineTo(movedToRight.getLeft(), movedToRight.getTop());
		ctx.setFillStyle(color.toString());
		ctx.closePath();
		ctx.fill();
	}
}
