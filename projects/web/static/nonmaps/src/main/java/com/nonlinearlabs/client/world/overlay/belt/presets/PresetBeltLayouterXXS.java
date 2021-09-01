package com.nonlinearlabs.client.world.overlay.belt.presets;

public class PresetBeltLayouterXXS extends PresetBeltLayouter {

	public PresetBeltLayouterXXS(BeltPresetLayout layout) {
		super(layout);
	}

	@Override
	public boolean doLayout(double w, double h) {
		hide(layout.menu);
		hide(layout.store);
		hide(layout.currentPreset);
		hide(layout.autoLoad);

		double margin = getDefaultMargin();
		layout.load.doLayout(w - margin, 0, h);
		layout.bank.doLayout(margin, 0, w - margin - layout.load.getRelativePosition().getWidth() - margin - margin, h);

		super.layoutLockSymbol(w);
		return true;
	}
}
