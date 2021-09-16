package com.nonlinearlabs.client.world.overlay.menu;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.FloatingWindow;
import com.nonlinearlabs.client.world.overlay.undo.UndoTreeWindow;

public class ToggleViewMenuEntry extends MenuEntry {

	private FloatingWindow window;

	public ToggleViewMenuEntry(GlobalMenu menu, FloatingWindow wnd) {
		super(menu, wnd instanceof UndoTreeWindow ? "U" : "");
		window = wnd;
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return window.getMenuTitle();
	}

	@Override
	public String getCheckText() {
		return (window.isShown() ? "\uE0A4 " : "\uE0A3 ");
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		window.toggle();
		getParent().toggle();
		return this;
	}

}
