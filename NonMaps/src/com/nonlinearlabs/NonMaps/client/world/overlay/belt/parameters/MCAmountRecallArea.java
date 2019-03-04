package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.ModulateableParameter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class MCAmountRecallArea extends OverlayLayout {
	public RecallValue value;
	public RecallButton button;
	
	protected MCAmountRecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(button = new RecallButton(this) {
			@Override
			public Control click(Position p) {
				if(!isActive())
					return null;
				
				EditBufferModel eb = EditBufferModel.get();

				if(eb.findParameter(eb.selectedParameter.getValue()).isChanged()) {
					NonMaps.get().getServerProxy().recallMcAmountForCurrentParameter();
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
				
				BasicParameterModel bpm = eb.findParameter(eb.selectedParameter.getValue());
				Parameter p = NonMaps.get().getNonLinearWorld().getParameterEditor().findParameter(eb.selectedParameter.getValue());
				if (p instanceof ModulatableParameter && bpm instanceof ModulateableParameter) {
					ModulatableParameter m = (ModulatableParameter) p;
					return m.getModulationAmount().getDecoratedValue(true, ((ModulateableParameter)bpm).ogModAmount.getValue());
				}
				return null;
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
		if(bpm instanceof ModulateableParameter) {
			ModulateableParameter modP = (ModulateableParameter)bpm;
			return modP.ogModAmount.getValue() != modP.modAmount.getValue();
		}
		return false;
	}

}
