package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class RecallArea extends OverlayLayout {


	private class RecallValue extends Label {

		public RecallValue(OverlayLayout parent) {
			super(parent);
			setFontHeightInMM(5);
		}

		@Override
		public String getDrawText(Context2d ctx) {
			EditBufferModel eb = EditBufferModel.get();
			Parameter param = NonMaps.get().getNonLinearWorld().getParameterEditor().findParameter(eb.selectedParameter.getValue());
			return param.getDecoratedValue(true, eb.findParameter(eb.selectedParameter.getValue()).originalValue.getValue());
		}
		
	}
	
	private class RecallButton extends SVGImage {

		@Override
		public int getSelectedPhase() {
			EditBufferModel eb = EditBufferModel.get();

			if(eb.findParameter(eb.selectedParameter.getValue()).isChanged()) {
				return 1;
			} else {
				return 2;
			}
		}

		@Override
		public Control click(Position eventPoint) {
			EditBufferModel eb = EditBufferModel.get();

			if(eb.findParameter(eb.selectedParameter.getValue()).isChanged()) {
				NonMaps.get().getServerProxy().recallCurrentParameterFromPreset();
				return this;
			}
			return null;
		}

		public RecallButton(Control parent) {
			super(parent, "PreRecall_A_Active.svg", "PreRecall_A_Enabled.svg", "PreRecall_A_Disabled.svg");
		}
		
		
		
	}
	
	private RecallValue value;
	private RecallButton button;
	
	protected RecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(value = new RecallValue(this));
		addChild(button = new RecallButton(this));
	}
	
	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		value.doLayout(x, y, w / 2, h);
		button.doLayout(x + value.getPixRect().getWidth(), y, w/2, h);
	}

}
