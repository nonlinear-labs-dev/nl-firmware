package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.overlay.ContextMenu;
import com.nonlinearlabs.client.world.overlay.Overlay;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class ContextMenuButton extends SVGImage {

	public ContextMenuButton(Control parent) {
		super(parent, "Menu_A_Enabled.svg");
	}

	private Control open(Position pos) {
		Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();
		Parameter p = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().getSelectedParameter();
		ContextMenu m = p.createContextMenu(o);
		o.setContextMenu(pos, m);
		return m;
	}

	@Override
	public Control mouseDown(Position pos) {
		Control m = open(pos);
		if (m != null)
			return m;
		return super.mouseDown(pos);
	}

	@Override
	public Control onContextMenu(Position pos) {
		Control m = open(pos);
		if (m != null)
			return m;
		return super.mouseDown(pos);
	};
}
