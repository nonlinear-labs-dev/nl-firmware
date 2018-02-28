package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.Belt;

public class BeltPresetLayout extends OverlayLayout {

	private MenuArea menu;
	private StorePresetArea store;
	private BankControlArea bank;
	private LoadButtonArea load;
	private CurrentPresetNumber currentPreset;
	private DirectLoadButton autoLoad;
	private AdvancedBankInformation bankInfos;

	public BeltPresetLayout(Belt parent) {
		super(parent);

		addChild(menu = new MenuArea(this));
		addChild(store = new StorePresetArea(this));
		addChild(bank = new BankControlArea(this));
		addChild(load = new LoadButtonArea(this));
		addChild(bankInfos = new AdvancedBankInformation(this));
		addChild(currentPreset = new CurrentPresetNumber(this));
		addChild(autoLoad = new DirectLoadButton(this));
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

		if (w < 1000 * NonMaps.devicePixelRatio)
			menu.setSmall(true);
		else
			menu.setSmall(false);

		menu.doLayout(0, margin, w * 0.25, h - 2 * margin);
		left += menu.getRelativePosition().getWidth() + margin;

		load.doLayout(left, margin, w * 0.25, h - 2 * margin);
		right -= load.getRelativePosition().getWidth() + margin;
		right -= buttonDim + margin; // auto load button

		double widthNeededForStore = 3 * buttonDim + 2 * margin;

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
		store.doLayout(bank.getRelativePosition().getLeft() - margin - 3 * buttonDim, horizontalCenterLinePosition - 1.5 * buttonDim,
				3 * buttonDim, 2 * buttonDim);

		double loadLeft = bank.getRelativePosition().getRight() + margin;
		double maxLoadWidth = w - loadLeft - margin - buttonDim - margin;
		load.doLayout(bank.getRelativePosition().getRight() + margin, horizontalCenterLinePosition - buttonDim - margin / 2 - 3,
				maxLoadWidth + margin, 2 * buttonDim + margin);

		double rest = bank.getRelativePosition().getWidth();

		if (rest >= buttonDim) {
			double loadAreaLeft = load.getRelativePosition().getLeft();
			double upDownCenter = load.getPrevNext().getRelativePosition().getCenterPoint().getX();
			bankInfos.doLayout(loadAreaLeft + upDownCenter - buttonDim, bank.getHorizontalCenterLinePosition() - buttonDim * 3,
					buttonDim * 2, 2 * buttonDim + margin);
		} else {
			bankInfos.doLayout(0, 0, 0, 0);
		}

		autoLoad.doLayout(w - buttonDim * 1.6, horizontalCenterLinePosition - (buttonDim / 1.75), buttonDim, buttonDim);

		rest = (autoLoad.getRelativePosition().getLeft() - margin) - (load.getRelativePosition().getRight() + margin);
		double currentPresetLabel = 2 * buttonDim;

		if (rest >= currentPresetLabel) {
			currentPreset.doLayout(load.getPixRect().getRight(), horizontalCenterLinePosition - (buttonDim / 2), currentPresetLabel,
					buttonDim);
		} else {
			currentPreset.doLayout(0, 0, 0, 0);
		}
	}

	public void update(Node settingsNode, Node editBufferNode, Node presetManagerNode) {
		bank.update(presetManagerNode);
		autoLoad.update(settingsNode);
		store.update(settingsNode, presetManagerNode);
	}

	public BankControl getBankControl() {
		return bank.getBankControl();
	}

}