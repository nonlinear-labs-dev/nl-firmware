package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class RecallArea extends OverlayLayout {

	public RecallValue value;
	public RecallButton button;
	
	protected RecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(button = new RecallButton(this) {
			@Override
			public Control click(Position p) {
				if(!isActive())
					return null;
				
				EditBufferModel eb = EditBufferModel.get();

				if(eb.findParameter(eb.selectedParameter.getValue()).isChanged()) {
					NonMaps.get().getServerProxy().recallCurrentParameterFromPreset();
					getParent().getParent().invalidate(INVALIDATION_FLAG_UI_CHANGED);
					return this;
				}
				return null;
			}
		});
		addChild(value = new RecallValue(this) {
			@Override
			public String getDrawText(Context2d ctx) {
				EditBufferModel eb = EditBufferModel.get();
				
				if(!isActive())
					return "";
				
				Parameter param = NonMaps.get().getNonLinearWorld().getParameterEditor().findParameter(eb.selectedParameter.getValue());
				return param.getDecoratedValue(true, eb.findParameter(eb.selectedParameter.getValue()).originalValue.getValue());
			}
		});
	}
	
	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
	
		button.setActive(isChanged());
		value.setActive(isChanged());
		
		final double padding = Millimeter.toPixels(2);
		final double textWidth = value.calculateTextWidth() + padding;
		final double buttonX = w / 2 - textWidth / 2;
		final double buttonY = h / 6;
		final double twothirds = (h / 3) * 2;
		
		value.doLayout(0, 0, w, h);		
		button.doLayout(buttonX, buttonY, textWidth, twothirds);
	}
	
	public boolean isChanged() {
		EditBufferModel eb = EditBufferModel.get();
		BasicParameterModel bpm = eb.findParameter(eb.selectedParameter.getValue());
		return bpm.value.value.getValue() != bpm.originalValue.getValue();
	}
}
