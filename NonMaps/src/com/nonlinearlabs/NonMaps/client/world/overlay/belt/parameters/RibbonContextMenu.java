package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.HardwareSources.Ribbon;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class RibbonContextMenu extends ParameterContextMenu {

	public RibbonContextMenu(OverlayLayout parent, final Ribbon param) {
		super(parent, param);

		addChild(new ContextMenuItem(this, "Non-Return") {
			@Override
			public Control click(Position eventPoint) {
				getNonMaps().getServerProxy().setRibbonReturnMode(param.getParameterID(), "stay");
				return super.click(eventPoint);
			}
		});

		addChild(new ContextMenuItem(this, "Return to center") {
			@Override
			public Control click(Position eventPoint) {
				getNonMaps().getServerProxy().setRibbonReturnMode(param.getParameterID(), "return");
				return super.click(eventPoint);
			}
		});
	}
}
