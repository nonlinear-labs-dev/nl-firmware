package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RenameDialog;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.MacroControlParameter;
import com.nonlinearlabs.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class MacroControlContextMenu extends ParameterContextMenu {

	public MacroControlContextMenu(OverlayLayout parent, final MacroControlParameter param) {
		super(parent, param);

		addChild(new ContextMenuItem(this, "Rename ...") {
			@Override
			public Control click(Position eventPoint) {
				RenameDialog.open(param);
				return super.click(eventPoint);
			}
		});

		addChild(new ContextMenuItem(this, "Mod Reset") {
			@Override
			public Control click(Position eventPoint) {
				EditBufferUseCases.get().resetModulation(param.getParameterNumber());
				return super.click(eventPoint);
			}
		});
	}
}
