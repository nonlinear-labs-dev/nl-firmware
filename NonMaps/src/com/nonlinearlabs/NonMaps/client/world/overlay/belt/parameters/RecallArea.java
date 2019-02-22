package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Dimension;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class RecallArea extends OverlayLayout {


	class RecallValue extends Label {

		public RecallValue(RecallArea parent) {
			super(parent);
			setFontHeightInMM(5);
			setFontColor(new Gray(155));
		}

		@Override
		public String getDrawText(Context2d ctx) {
			EditBufferModel eb = EditBufferModel.get();
			
			if(!getParent().isChanged())
				return "";
			
			Parameter param = NonMaps.get().getNonLinearWorld().getParameterEditor().findParameter(eb.selectedParameter.getValue());
			return param.getDecoratedValue(true, eb.findParameter(eb.selectedParameter.getValue()).originalValue.getValue());
		}
		
		@Override
		public RecallArea getParent() {
			return (RecallArea)super.getParent();
		}

		public double getFontSizeInPixel() {
			return getFontHeight(getPixRect());
		}

		public double calculateTextWidth() {
			Context2d ctx = NonMaps.get().getCanvas().getContext2d();
			String text = value.getDrawText(ctx);
			ctx.setFont(value.getFontSizeInPixel() + "px 'Source Sans Pro LW25'");		
			return ctx.measureText(text).getWidth();
		}
	}
	
	class RecallButton extends OverlayControl {
		
		@Override
		public Control click(Position eventPoint) {
			EditBufferModel eb = EditBufferModel.get();

			if(eb.findParameter(eb.selectedParameter.getValue()).isChanged()) {
				NonMaps.get().getServerProxy().recallCurrentParameterFromPreset();
				getParent().getParent().invalidate(INVALIDATION_FLAG_UI_CHANGED);
				return this;
			}
			return null;
		}
		
		public RecallButton(RecallArea parent) {
			super(parent);
		}

		@Override
		public void draw(Context2d ctx, int invalidationMask) {
			if(getParent().isChanged()) {				
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
		
		@Override
		public RecallArea getParent() {
			return (RecallArea)super.getParent();
		}
	}
	
	public RecallValue value;
	public RecallButton button;
	
	protected RecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(button = new RecallButton(this));
		addChild(value = new RecallValue(this));
	}
	
	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		
		final double padding = Millimeter.toPixels(2);
		final double textWidth = value.calculateTextWidth() + padding;
		final double buttonX = w / 2 - textWidth / 2;
		final double buttonY = h / 6;
		final double twothirds = (h / 3) * 2;
		
		value.doLayout(0, 0, w, h);		
		button.doLayout(buttonX, buttonY, textWidth, twothirds);
	}
	
	@Override
	public void setVisible(boolean v) {
		super.setVisible(v);
	}
	
	public boolean isChanged() {
		EditBufferModel eb = EditBufferModel.get();
		return eb.findParameter(eb.selectedParameter.getValue()).isChanged();
	}
}
