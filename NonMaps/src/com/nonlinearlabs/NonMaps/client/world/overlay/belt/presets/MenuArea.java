package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.overlay.BankInfoDialog;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.PresetInfoDialog;

class MenuArea extends OverlayLayout {

	class MenuAndInfo extends OverlayLayout {
		
		OverlayControl btn;
		OverlayControl info;

		public MenuAndInfo(OverlayLayout parent, OverlayControl btn, OverlayControl info) {
			super(parent);

			this.btn = btn;
			this.info = info;
			
			addChild(btn);
			addChild(info);
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);
			double margin = getSmallButtonWidth() / 2.5;
			btn.doLayout(margin, 0, getSmallButtonWidth(), h);
			info.doLayout(btn.getPixRect().getRight() + margin, 0, getSmallButtonWidth(), h);
		}
	}

	private MenuAndInfo presets;
	private MenuAndInfo banks;
	private MenuAreaSearchButton search;

	private PresetManager getPresetManager() {
		return NonMaps.get().getNonLinearWorld().getPresetManager();
	}
	
	boolean hasBank() {
		PresetManager pm = getPresetManager();
		String bankUUID = pm.getSelectedBank();

		if (bankUUID != null) {
			return pm.findBank(bankUUID) != null;
		}
		return false;
	}
	
	boolean hasPreset() {
		PresetManager pm = getPresetManager();
		String bankUUID = pm.getSelectedBank();

		if (bankUUID != null) {
			Bank bank = pm.findBank(bankUUID);
			if (bank != null) {
				String presetUUID = bank.getPresetList().getSelectedPreset();
				return bank.getPresetList().findPreset(presetUUID) != null;
			}
		}
		return false;
	}
	
	MenuArea(BeltPresetLayout parent) {
		super(parent);

		presets = addChild(new MenuAndInfo(this, new MenuAreaPresetButton(this), new MenuAreaInfoButton(this) {
			
			@Override
			public void toggle() {
				PresetInfoDialog.toggle();
			}
			
			@Override
			public int getSelectedPhase() {
				if(hasPreset())
					return PresetInfoDialog.isShown() ? drawStates.active.ordinal() : drawStates.normal.ordinal();
				else
					return drawStates.disabled.ordinal();
			}
		}));

		banks = addChild(new MenuAndInfo(this, new MenuAreaBankButton(this), new MenuAreaInfoButton(this) {
			
			@Override
			public void toggle() {
				if(hasBank())
					BankInfoDialog.toggle();
			}
			
			@Override
			public int getSelectedPhase() {
				if(hasBank())
					return BankInfoDialog.isShown() ? drawStates.active.ordinal() : drawStates.normal.ordinal();
				else
					return drawStates.disabled.ordinal();
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
		double rowWidth = (getSmallButtonWidth() / 2.5) * 2 + getSmallButtonWidth() * 2;
		presets.doLayout(0, 0, rowWidth, h / 2);
		banks.doLayout(0, h / 2, rowWidth, h / 2);
		search.doLayout(presets.getPixRect().getRight() + getSmallButtonWidth() / 2, 0, getSmallButtonWidth(), h / 2);
	}

	private double getSmallButtonWidth() {
		return Millimeter.toPixels(11);
	}
}
