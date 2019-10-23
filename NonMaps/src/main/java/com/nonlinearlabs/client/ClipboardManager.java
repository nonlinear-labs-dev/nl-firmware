package com.nonlinearlabs.client;

import com.google.gwt.xml.client.Node;
import com.google.gwt.xml.client.NodeList;

public final class ClipboardManager {

	public enum ClipboardContent {
		empty, bank, preset, mulpreset
	};

	private ClipboardContent clipboardState;

	public ClipboardManager() {
		clipboardState = ClipboardContent.empty;
	}

	public ClipboardContent getClipboardState() {
		return clipboardState;
	}

	public void update(Node clipboardInfo) {
		if (ServerProxy.didChange(clipboardInfo)) {
			NodeList children = clipboardInfo.getChildNodes();
			for (int i = 0; i < children.getLength(); i++) {
				Node child = children.item(i);

				if (child.getNodeName().equals("#text")) // GWT creates strange
															// empty nodes
															// called "text": we
															// ignore them
				{
					continue;
				}
				if (ServerProxy.getText(child) == "1") {
					if (child.getNodeName().equals("contains-bank")) {
						clipboardState = ClipboardContent.bank;
						return;
					}
					if (child.getNodeName().equals("contains-preset")) {
						clipboardState = ClipboardContent.preset;
						return;
					}
					if (child.getNodeName().equals("contains-multiple-presets")) {
						clipboardState = ClipboardContent.mulpreset;
						return;
					}
				}
			}
			clipboardState = ClipboardContent.empty;
		}
	}
}
