package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.ParameterInfoDialog;
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

		Belt belt = NonMaps.get().getNonLinearWorld().getViewport().getOverlay().getBelt();
		if (belt.isHidden() || !belt.isParameterView()) {

			addChild(new ContextMenuItem(this, "Open Tab") {
				@Override
				public Control click(Position eventPoint) {

					belt.openTab(Belt.BeltTab.Parameter);
					if (belt.isHidden())
						belt.toggle();

					return super.click(eventPoint);
				}
			});
		}
	}
}
