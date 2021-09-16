package com.nonlinearlabs.client.world.overlay.menu;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class MenuHeader extends SVGImage {

	public MenuHeader(GlobalMenu menu) {
		super(menu, "View_Menu_Icon.svg");
	}

	@Override
	public GlobalMenu getParent() {
		return (GlobalMenu) super.getParent();
	}

	@Override
	public Control onContextMenu(Position pos) {
		getParent().toggle();
		return this;
	}

	@Override
	public Control click(Position eventPoint) {
		getParent().toggle();
		return this;
	}
}
