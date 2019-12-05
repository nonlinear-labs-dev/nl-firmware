package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources.Pedal;
import com.nonlinearlabs.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class PedalContextMenu extends ParameterContextMenu {

	public PedalContextMenu(OverlayLayout parent, final Pedal param) {
		super(parent, param);

		String noneString = !param.presenter.isReturning ? "\uE0A4 Non-Return" : "\uE0A3 Non-Return";
		String zeroString = param.presenter.drawZeroReturnIndicator ? "\uE0A4 Return to zero" : "\uE0A3 Return to zero";
		String centerString = param.presenter.drawCenterReturnIndicator ? "\uE0A4 Return to center" : "\uE0A3 Return to center";

		addChild(new ContextMenuItem(this, noneString) {
			@Override
			public Control click(Position eventPoint) {
				getNonMaps().getServerProxy().setPedalMode(param.getParameterNumber(), "stay");
				return super.click(eventPoint);
			}
		});

		addChild(new ContextMenuItem(this, zeroString) {
			@Override
			public Control click(Position eventPoint) {
				getNonMaps().getServerProxy().setPedalMode(param.getParameterNumber(), "return-to-zero");
				return super.click(eventPoint);
			}
		});

		addChild(new ContextMenuItem(this, centerString) {
			@Override
			public Control click(Position eventPoint) {
				getNonMaps().getServerProxy().setPedalMode(param.getParameterNumber(), "return-to-center");
				return super.click(eventPoint);
			}
		});
	}
}
