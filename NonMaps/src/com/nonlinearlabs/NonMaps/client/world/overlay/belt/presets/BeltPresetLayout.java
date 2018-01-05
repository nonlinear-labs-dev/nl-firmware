package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.Belt;

public class BeltPresetLayout extends OverlayLayout {

	private MenuArea menu;
	private StorePresetArea store;
	private BankControlArea bank;
	private LoadButtonArea load;
	private CurrentPresetNumber currentPreset;
	private AutoLoadSelectedPreset autoLoad;
	private AdvancedBankInformation bankInfos;

	public BeltPresetLayout(Belt parent) {
		super(parent);

		addChild(menu = new MenuArea(this));
		addChild(store = new StorePresetArea(this));
		addChild(bank = new BankControlArea(this));
		addChild(load = new LoadButtonArea(this));
		addChild(bankInfos = new AdvancedBankInformation(this));
		addChild(currentPreset = new CurrentPresetNumber(this));
		addChild(autoLoad = new AutoLoadSelectedPreset(this));
	}

	@Override
	public Belt getParent() {
		return (Belt) super.getParent();
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		final double buttonDim = getButtonDimension();
		final double margin = buttonDim / 2;
		double right = w - margin;
		double left = margin;

		menu.doLayout(left, margin, w * 0.25, h - 2 * margin);
		left += menu.getRelativePosition().getWidth() + margin;

		load.doLayout(left, margin, w * 0.25, h - 2 * margin);
		right -= load.getRelativePosition().getWidth() + margin;
		right -= buttonDim + margin; // auto load button

		double widthNeededForStore = 2 * buttonDim + 2 * margin;

		double availableForCenteredBankControl = 2 * Math.min(right - w / 2, w / 2 - (left + widthNeededForStore));
		boolean centerBankControl = availableForCenteredBankControl > Millimeter.toPixels(100);
		double maxBankControlWidth = Millimeter.toPixels(150);

		availableForCenteredBankControl = 0.8 * Math.min(maxBankControlWidth, availableForCenteredBankControl);

		if (centerBankControl) {
			bank.doLayout((w - availableForCenteredBankControl) / 2, 0, availableForCenteredBankControl, h);
		} else {
			bank.doLayout(left + widthNeededForStore, 0, Math.min(maxBankControlWidth, right - (left + widthNeededForStore)), h);
		}

		double horizontalCenterLinePosition = bank.getHorizontalCenterLinePosition();
		store.doLayout(bank.getRelativePosition().getLeft() - margin - 2 * buttonDim, horizontalCenterLinePosition - 1.5 * buttonDim,
				2 * buttonDim, 2 * buttonDim);

		double loadLeft = bank.getRelativePosition().getRight() + margin;
		double maxLoadWidth = w - loadLeft - margin - buttonDim - margin;
		load.doLayout(bank.getRelativePosition().getRight() + margin, horizontalCenterLinePosition - buttonDim - margin / 2 - 3 , maxLoadWidth
				+ margin, 2 * buttonDim + margin);

		double rest = bank.getRelativePosition().getWidth();

		if (rest >= buttonDim) {
			double loadAreaLeft = load.getRelativePosition().getLeft();
			double upDownCenter = load.getPrevNext().getRelativePosition().getCenterPoint().getX();
			bankInfos.doLayout(loadAreaLeft + upDownCenter - buttonDim / 2, bank.getHorizontalCenterLinePosition() - buttonDim * 3,
					buttonDim*2, 2 * buttonDim + margin);
		} else {
			bankInfos.doLayout(0, 0, 0, 0);
		}

		autoLoad.doLayout(w - buttonDim - margin, horizontalCenterLinePosition - (buttonDim / 2), buttonDim, buttonDim);

		rest = (autoLoad.getRelativePosition().getLeft() - margin) - (load.getRelativePosition().getRight() + margin);
		double currentPresetLabel = 2 * buttonDim;

		if (rest >= currentPresetLabel) {
			currentPreset.doLayout(load.getRelativePosition().getRight() + margin, horizontalCenterLinePosition - (buttonDim / 2), rest,
					buttonDim);
		} else {
			currentPreset.doLayout(0, 0, 0, 0);
		}

	}

	public void update(Node settingsNode, Node editBufferNode, Node presetManagerNode) {
		load.update(settingsNode, editBufferNode);
		bank.update(presetManagerNode);
		autoLoad.update(settingsNode);
		store.update(settingsNode, presetManagerNode);
	}

	public BankControl getBankControl() {
		return bank.getBankControl();
	}

}