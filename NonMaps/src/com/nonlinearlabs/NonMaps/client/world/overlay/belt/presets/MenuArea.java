package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.overlay.BankInfoDialog;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.PresetInfoDialog;

class MenuArea extends OverlayLayout {

	class MenuAndInfo extends OverlayLayout {

		MenuAreaButton btn;
		MenuAreaInfoButton info;

		public MenuAndInfo(OverlayLayout parent, MenuAreaButton btn, MenuAreaInfoButton info) {
			super(parent);

			this.btn = btn;
			this.info = info;

			addChild(btn);
			addChild(info);
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);
			btn.doLayout(0, 0, w - getSmallButtonWidth(), h);
			info.doLayout(w - getSmallButtonWidth(), 0, getSmallButtonWidth(), h);
		}
	}

	private MenuAndInfo presets;
	private MenuAndInfo banks;
	private MenuAreaSearchButton search;

	MenuArea(BeltPresetLayout parent) {
		super(parent);

		presets = addChild(new MenuAndInfo(this, new MenuAreaPresetButton(this), new MenuAreaInfoButton(this) {

			@Override
			public void toggle() {
				PresetInfoDialog.toggle();
			}

			@Override
			protected State getState() {
				return PresetInfoDialog.isShown() ? State.Active : State.Enabled;
			}
		}));

		banks = addChild(new MenuAndInfo(this, new MenuAreaBankButton(this), new MenuAreaInfoButton(this) {

			@Override
			public void toggle() {
				BankInfoDialog.toggle();
			}

			@Override
			protected State getState() {
				return BankInfoDialog.isShown() ? State.Active : State.Enabled;
			}
		}));

		search = addChild(new MenuAreaSearchButton(this));
	}

	@Override
	public BeltPresetLayout getParent() {
		return (BeltPresetLayout) super.getParent();
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		presets.doLayout(0, 0, w / 2, h / 2);
		banks.doLayout(0, h / 2, w / 2, h / 2);
		search.doLayout(w / 2, 0, w / 2, h / 2);
	}

	private double getSmallButtonWidth() {
		return Millimeter.toPixels(11);
	}
}
