package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.dataModel.editBuffer.RibbonParameterModel.TouchBehaviour;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.RGBA;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources.Ribbon;
import com.nonlinearlabs.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.client.world.overlay.OverlayControl;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class RibbonContextMenu extends ParameterContextMenu {

	public RibbonContextMenu(OverlayLayout parent, final Ribbon param) {
		super(parent, param);

		String noneString = !param.presenter.isReturning ?  "\uE0A4 Non-Return" : "\uE0A3 Non-Return";
		String centerString = param.presenter.drawCenterReturnIndicator ? "\uE0A4 Return to center" : "\uE0A3 Return to center";
		boolean isAbsolute = !param.presenter.relative;
		String absoluteString =  isAbsolute ? "\uE0A4 Absolute" : "\uE0A3 Absolute";
		String relativeString =  !isAbsolute ? "\uE0A4 Relative" : "\uE0A3 Relative";

		addChild(new ContextMenuItem(this, noneString) {
			@Override
			public Control click(Position eventPoint) {
				getNonMaps().getServerProxy().setRibbonReturnMode(param.getParameterNumber(), "stay");
				return super.click(eventPoint);
			}
		});

		addChild(new ContextMenuItem(this, centerString) {
			@Override
			public Control click(Position eventPoint) {
				getNonMaps().getServerProxy().setRibbonReturnMode(param.getParameterNumber(), "return");
				return super.click(eventPoint);
			}
		});

		addChild(new ContextMenuItem(this, "")
		{
			@Override
			public Control click(Position eventPoint) {
				return this;
			}

			@Override
			public void draw(Context2d ctx, int invalidationMask) {
				Rect pos = getPixRect();
				ctx.setFillStyle(new Gray(128).toString());
				ctx.beginPath();
				ctx.rect(pos.getLeft(), pos.getTop(), pos.getWidth(), pos.getHeight());				
				ctx.fill();
			}

			@Override
			public double getDesiredHeight() {
				return Millimeter.toPixels(0.5);
			}
		});

		addChild(new ContextMenuItem(this, relativeString) {
			@Override
			public Control click(Position eventPoint) {
				if(!isAbsolute)
					return this;
				getNonMaps().getServerProxy().setRibbonBehaviour(param.getParameterNumber(), "RELATIVE");
				return super.click(eventPoint);
			}
		});

		addChild(new ContextMenuItem(this, absoluteString) {
			@Override
			public Control click(Position eventPoint) {
				if(isAbsolute)
					return this;
				getNonMaps().getServerProxy().setRibbonBehaviour(param.getParameterNumber(), "ABSOLUTE");
				return super.click(eventPoint);
			}
		});
	}
}
