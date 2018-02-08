package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.overlay.BankInfoDialog;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.PresetInfoDialog;

class MenuArea extends OverlayLayout {

	boolean isSmall = true;
	
	public boolean isSmall() {
		return isSmall;
	}

	public void setSmall(boolean isSmall) {
		this.isSmall = isSmall;
	}

	class MenuLabel extends Label {

		String text;
		
		public MenuLabel(OverlayLayout parent, String t) {
			super(parent);
			text = t;
			setFontHeightInMM(5);
		}

		@Override
		public String getDrawText(Context2d ctx) {
			return text;
		}
		
	}
	
	class MenuAndInfo extends OverlayLayout {
		
		MenuLabel label;
		OverlayControl btn;
		OverlayControl info;
		OverlayControl search;

		public MenuAndInfo(OverlayLayout parent, MenuLabel label, OverlayControl btn, OverlayControl info, OverlayControl search) {
			super(parent);

			this.label = label;
			this.btn = btn;
			this.info = info;
			this.search = search;
			
			addChild(label);
			addChild(btn);
			addChild(info);
			if(search != null)
				addChild(search);
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);
			double margin = getSmallButtonWidth() / 2.5;
			
			if(((MenuArea)getParent()).isSmall()) {
					label.setVisible(false);
					btn.doLayout(margin + margin, 0, getSmallButtonWidth(), h);
					info.doLayout(btn.getPixRect().getRight() + margin, 0, getSmallButtonWidth(), h);
					if(search != null)
						search.doLayout(info.getPixRect().getRight() + margin, 0, getSmallButtonWidth(), h);
			}
			else {
				label.setVisible(true);
				label.doLayout(margin, 0, getSmallButtonWidth() * 2, h);
				btn.doLayout(label.getPixRect().getRight() + margin, 0, getSmallButtonWidth(), h);
				info.doLayout(btn.getPixRect().getRight() + margin, 0, getSmallButtonWidth(), h);
				if(search != null)
					search.doLayout(info.getPixRect().getRight() + margin, 0, getSmallButtonWidth(), h);
			}
			
			
		}	
		
		public double getRight() {
			if(search != null)
				return search.getPixRect().getRight();
			else
				return info.getPixRect().getRight() + getSmallButtonWidth() * 1.5;
		}
	}

	private MenuAndInfo presets;
	private MenuAndInfo banks;

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

		presets = addChild(new MenuAndInfo(this, new MenuLabel(this, "Preset"), new MenuAreaPresetButton(this), new MenuAreaInfoButton(this) {
			
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
		}, new MenuAreaSearchButton(this)));

		banks = addChild(new MenuAndInfo(this, new MenuLabel(this, "Bank"), new MenuAreaBankButton(this), new MenuAreaInfoButton(this) {
			
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
		}, null));
	}

	@Override
	public BeltPresetLayout getParent() {
		return (BeltPresetLayout) super.getParent();
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		presets.doLayout(0, 0, presets.getRight(), h / 2);
		banks.doLayout(0, h / 2, banks.getRight(), h / 2);
	}

	private double getSmallButtonWidth() {
		return Millimeter.toPixels(11);
	}
	


	void roundRect(Context2d ctx, double x, double y, double width, double height, double radius) {
		  ctx.setFillStyle(new Gray(28).toString());
		  ctx.beginPath();
		  ctx.moveTo(x + radius, y);
		  ctx.lineTo(x + width - radius, y);
		  ctx.quadraticCurveTo(x + width, y, x + width, y + radius);
		  ctx.lineTo(x + width, y + height - radius);
		  ctx.quadraticCurveTo(x + width, y + height, x + width - radius, y + height);
		  ctx.lineTo(x + radius, y + height);
		  ctx.quadraticCurveTo(x, y + height, x, y + height - radius);
		  ctx.lineTo(x, y + radius);
		  ctx.quadraticCurveTo(x, y, x + radius, y);
		  ctx.closePath();
		  ctx.fill();
		}
	
	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		roundRect(ctx, presets.getPixRect().getLeft() + getSmallButtonWidth() / 2, presets.getPixRect().getTop() + 1, presets.getPixRect().getRight() - presets.getPixRect().getLeft() - getSmallButtonWidth() / 3, presets.getPixRect().getHeight() - 4, 6);
		roundRect(ctx, presets.getPixRect().getLeft() + getSmallButtonWidth() / 2, banks.getPixRect().getTop() + 1, presets.getPixRect().getRight() - presets.getPixRect().getLeft() - getSmallButtonWidth() / 3, banks.getPixRect().getHeight() - 4, 6);
		super.draw(ctx, invalidationMask);
	}
	
	
}
