package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.*;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.Belt;

public class ParameterContextMenu extends ContextMenu {

	public ParameterContextMenu(OverlayLayout parent, final Parameter param) {
		super(parent);

		addChild(new ContextMenuItem(this, "Parameter Info...") {
			@Override
			public Control click(Position eventPoint) {
				ParameterInfoDialog.toggle();
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
				return super.click(eventPoint);
			}
		});

		addChild(new ContextMenuItem(this, "Open Tab") {
			@Override
			public Control click(Position eventPoint) {
				NonMaps.get().getNonLinearWorld().getViewport().getOverlay().getBelt().openTab(Belt.BeltTab.Parameter);
				return super.click(eventPoint);
			}
		});
	}
}
