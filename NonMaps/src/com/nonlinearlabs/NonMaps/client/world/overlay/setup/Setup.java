package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.FloatingWindow;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;

public class Setup extends FloatingWindow {

	private FloatingWindowHeader header;
	private SetupPages content;

	public Setup(Overlay parent) {
		super(parent);

		addChild(header = new SetupHeader(this));
		addChild(content = new SetupPages(this));
	}

	public void doLayout(double x, double y) {
		double w = Millimeter.toPixels(110);
		double lineHeight = Millimeter.toPixels(10);

		if (isHidden()) {
			super.doLayout(x, y, 0, 0);

		} else {
			double margin = Millimeter.toPixels(2);
			int heightInLines = 12;
			header.doLayout(0, 0, w, lineHeight);
			content.doLayout(margin, lineHeight, w - 2 * margin, heightInLines * lineHeight - margin);
			super.doLayout(x, y, w, (heightInLines + 1) * lineHeight);
		}
	}

	public PresetDragDropSetting getPresetDragDropSetting() {
		return content.getPresetDragDropSetting();
	}

	public ContextMenusSetting getContextMenuSettings() {
		return content.getContextMenuSettings();
	}

	public EditParameterSetting getEditParameterDragSetting() {
		return content.getEditParameterDragSetting();
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		return this;
	}

	@Override
	public Control wheel(Position eventPoint, double amount, boolean fine) {
		return this;
	}

	@Override
	public String getTitle() {
		return "Setup";
	}
}
