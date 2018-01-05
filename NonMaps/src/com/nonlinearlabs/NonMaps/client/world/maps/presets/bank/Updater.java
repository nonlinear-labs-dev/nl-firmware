package com.nonlinearlabs.NonMaps.client.world.maps.presets.bank;

import java.util.ArrayList;

import com.google.gwt.xml.client.Node;
import com.google.gwt.xml.client.NodeList;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.IPreset;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;
import com.nonlinearlabs.NonMaps.client.world.maps.NonPosition;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;

public class Updater {

	private Bank bank;

	public Updater(Bank bank) {
		this.bank = bank;
	}

	public void update(Node bank, boolean force) {
		if (force || ServerProxy.didChange(bank)) {
			String selectedPreset = bank.getAttributes().getNamedItem("selected-preset").getNodeValue();
			updateBankName(bank);
			updateBankPosition(bank);
			updateOrderNumber(bank);
			this.bank.updateAttributes(bank);
			this.bank.selectPreset(selectedPreset, Initiator.INDIRECT_USER_ACTION);
			updatePresets(bank, force);
			updateAttachment(bank);
			updateDateOfLastChange(bank);
			handleEmptyBank();
		}
	}

	private void updateAttachment(Node bank) {
		String masterUUID = ServerProxy.getChildText(bank, "attached-to");
		String direction = ServerProxy.getChildText(bank, "attached-direction");

		this.bank.setAttatchedTo(direction);
		this.bank.setMasterUUID(masterUUID);
	}

	private void updateDateOfLastChange(Node bank) {
		String d = ServerProxy.getChildText(bank, "date-of-last-change");
		String state = ServerProxy.getChildText(bank, "state");
		this.bank.setDateOfLastChange(d);
		this.bank.setImportExportState(state);
	}

	private void handleEmptyBank() {
		bank.showEmptyLabel(isBankEmpty());
	}

	private boolean isBankEmpty() {
		for (Control c : bank.getChildren()) {
			if (c instanceof IPreset)
				return false;
		}
		return true;
	}

	private void updatePresets(Node bank, boolean force) {
		NodeList presets = bank.getChildNodes();
		ArrayList<MapsControl> currentChildren = collectCurrentPresets();

		int presetChild = 0;

		for (int i = 0; i < presets.getLength(); i++) {
			Node child = presets.item(i);
			if (child != null && child.getNodeName().equals("preset")) {
				Preset presetUI = updatePreset(presetChild++, presets.item(i));
				currentChildren.remove(presetUI);
			}
		}

		removeObsoletePresets(currentChildren);
	}

	private ArrayList<MapsControl> collectCurrentPresets() {
		ArrayList<MapsControl> currentChildren = new ArrayList<MapsControl>();

		for (MapsControl c : this.bank.getChildren()) {
			if (c instanceof IPreset)
				currentChildren.add(c);
		}
		return currentChildren;
	}

	private void updateBankName(Node bank) {
		this.bank.header.setBankName(bank.getAttributes().getNamedItem("name").getNodeValue());
	}

	private void updateOrderNumber(Node bank) {
		String v = bank.getAttributes().getNamedItem("order-number").getNodeValue();
		try {
			int i = Integer.parseInt(v);
			this.bank.setOrderNumber(i);
		} catch (Exception e) {
		}
	}

	private void updateBankPosition(Node bank) {
		boolean updateServersBankPos = false;

		String x = bank.getAttributes().getNamedItem("x").getNodeValue();
		String y = bank.getAttributes().getNamedItem("y").getNodeValue();

		NonPosition bankPos = this.bank.getNonMaps().getNonLinearWorld().getViewport().getNonPosition().getCenterPoint();

		try {
			bankPos = new NonPosition(Double.parseDouble(x), Double.parseDouble(y));
		} catch (Exception ex) {
			updateServersBankPos = true;
		}

		if (this.bank.getNonPosition().moveTo(bankPos))
			this.bank.requestLayout();

		if (updateServersBankPos)
			this.bank.getNonMaps().getServerProxy().onBankPositionChanged(this.bank);
	}

	private void removeObsoletePresets(ArrayList<MapsControl> currentChildren) {
		boolean needsLayout = false;

		for (MapsControl control : currentChildren) {
			this.bank.removeChild(control);
			needsLayout = true;
		}

		if (needsLayout)
			this.bank.requestLayout();
	}

	private Preset updatePreset(int i, Node preset) {
		int j = 0;

		for (Control c : bank.getChildren()) {
			if (c instanceof Preset) {
				if (i == j) {
					Preset p = (Preset) c;
					p.update(j + 1, preset);
					return p;
				}
				j++;
			}
		}

		Preset presetUI = new Preset(this.bank);
		this.bank.addChild(presetUI);
		presetUI.update(j + 1, preset);
		this.bank.requestLayout();
		return presetUI;
	}
}
