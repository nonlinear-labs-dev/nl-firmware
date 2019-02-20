package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class RecallArea extends OverlayLayout {


	class RecallValue extends Label {

		public RecallValue(OverlayLayout parent) {
			super(parent);
			setFontHeightInMM(5);
			setFontColor(RGB.lightGray());
		}

		@Override
		public String getDrawText(Context2d ctx) {
			EditBufferModel eb = EditBufferModel.get();
			
			if(!eb.findParameter(eb.selectedParameter.getValue()).isChanged())
				return "";
			
			Parameter param = NonMaps.get().getNonLinearWorld().getParameterEditor().findParameter(eb.selectedParameter.getValue());
			return param.getDecoratedValue(true, eb.findParameter(eb.selectedParameter.getValue()).originalValue.getValue());
		}
		
	}
	
	class RecallButton extends SVGImage {

		public boolean clicked = false;
		
		@Override
		public int getSelectedPhase() {
			EditBufferModel eb = EditBufferModel.get();
			if(!eb.findParameter(eb.selectedParameter.getValue()).isChanged())
				clicked = false;

			
			if(!eb.findParameter(eb.selectedParameter.getValue()).isChanged())
				return 2;
			if(clicked && eb.findParameter(eb.selectedParameter.getValue()).isChanged())
				return 0;
			
			return 1;
		}

		@Override
		public Control click(Position eventPoint) {
			EditBufferModel eb = EditBufferModel.get();

			if(eb.findParameter(eb.selectedParameter.getValue()).isChanged()) {
				NonMaps.get().getServerProxy().recallCurrentParameterFromPreset();
				getParent().getParent().invalidate(INVALIDATION_FLAG_UI_CHANGED);
				clicked = true;
				return this;
			}
			return null;
		}
		
		@Override
		public Control mouseUp(Position eventPoint) {
			clicked = false;
			return super.mouseUp(eventPoint);
		}

		public RecallButton(Control parent) {
			super(parent, "PreRecall_A_Active.svg", "PreRecall_A_Enabled.svg", "PreRecall_A_Disabled.svg");
		}
		
		
		
	}
	
	public RecallValue value;
	public RecallButton button;
	
	protected RecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(value = new RecallValue(this));
		addChild(button = new RecallButton(this));
	}
	
	@Override
	public void doLayout(double x, double y, double w, double h) {
		double buttonDim = Millimeter.toPixels(10);
		super.doLayout(x, y, w, h);		
		button.doLayout(0, 0, buttonDim, h);
		value.doLayout(buttonDim, 0, buttonDim * 4, h);

	}

	@Override
	public void setVisible(boolean v) {
		super.setVisible(v);
	}
}
