package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;

public class ContextMenuButton extends SVGImage {

	public ContextMenuButton(Control parent) {
		super(parent, "Menu_A_Enabled.svg");
	}

	@Override
	public Control mouseDown(Position pos) {
		Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();
		Parameter p = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().getSelection();
		ContextMenu m = p.createContextMenu(o);

		if (m != null)
			return o.setContextMenu(pos, m);

		return super.mouseDown(pos);
	}
}
