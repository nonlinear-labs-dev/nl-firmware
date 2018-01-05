package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class GUISettingsPage extends SetupPage {

	private ContextMenusSetting contextMenuSettings;
	private PresetDragDropSetting presetDragDropSetting;
	private EditParameterSetting editParameterSetting;

	protected GUISettingsPage(SetupPages parent) {
		super(parent);

		addChild(new SelectionAutoScrollSetting(this));
		addChild(new Divider(this));
		addChild(editParameterSetting = new EditParameterSetting(this));
		addChild(new Divider(this));
		addChild(contextMenuSettings = new ContextMenusSetting(this));
		addChild(new Divider(this));
		addChild(presetDragDropSetting = new PresetDragDropSetting(this));
		addChild(new Divider(this));
		addChild(new DisplayScalingFactor(this));
		addChild(new Divider(this));
		addChild(new StripeBrightness(this));
		addChild(new Divider(this));
		addChild(new BitmapCache(this));
		addChild(new Divider(this));
		addChild(new ShowDeveloperOptions(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double lineHeight = getButtonDimension();
		double margin = getButtonDimension() / 2;
		double yPos = 0;

		for (OverlayControl c : getChildren()) {
			c.doLayout(margin, yPos, w - 2 * margin, lineHeight);
			yPos += c.getRelativePosition().getHeight();
		}
	}

	public PresetDragDropSetting getPresetDragDropSetting() {
		return presetDragDropSetting;
	}

	public ContextMenusSetting getContextMenuSettings() {
		return contextMenuSettings;
	}

	public EditParameterSetting getEditParameterDragSetting() {
		return editParameterSetting;
	}

}
