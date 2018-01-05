package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.HardwareSources.Pedal;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class PedalContextMenu extends ParameterContextMenu {

	public PedalContextMenu(OverlayLayout parent, final Pedal param) {
		super(parent, param);

		addChild(new ContextMenuItem(this, "Non-Return") {
			@Override
			public Control click(Position eventPoint) {
				getNonMaps().getServerProxy().setPedalMode(param.getParameterID(), "stay");
				return super.click(eventPoint);
			}
		});

		addChild(new ContextMenuItem(this, "Return to zero") {
			@Override
			public Control click(Position eventPoint) {
				getNonMaps().getServerProxy().setPedalMode(param.getParameterID(), "return-to-zero");
				return super.click(eventPoint);
			}
		});

		addChild(new ContextMenuItem(this, "Return to center") {
			@Override
			public Control click(Position eventPoint) {
				getNonMaps().getServerProxy().setPedalMode(param.getParameterID(), "return-to-center");
				return super.click(eventPoint);
			}
		});
	}
}
