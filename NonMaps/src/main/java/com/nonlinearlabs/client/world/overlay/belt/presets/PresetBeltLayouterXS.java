package com.nonlinearlabs.client.world.overlay.belt.presets;

public class PresetBeltLayouterXS extends PresetBeltLayouter {

	public PresetBeltLayouterXS(BeltPresetLayout layout) {
		super(layout);
	}

	@Override
	public boolean doLayout(double w, double h) {
		hide(layout.menu);
		hide(layout.currentPreset);
		hide(layout.autoLoad);

		double margin = getDefaultMargin();
		double left = margin;
		double right = w - margin;

		layout.store.doLayout(left, 0, h);
		left += layout.store.getRelativePosition().getWidth();

		layout.load.doLayout(right, 0, h);
		right -= layout.load.getRelativePosition().getWidth();

		double bankWidth = right - left;
		if (bankWidth < getMinBankControlWidth())
			return false;

		layout.bank.doLayout(left, 0, bankWidth, h);

		super.layoutLockSymbol(w);
		return true;
	}
}
