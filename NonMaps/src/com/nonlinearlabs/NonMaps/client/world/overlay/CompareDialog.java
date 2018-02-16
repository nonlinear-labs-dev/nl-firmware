package com.nonlinearlabs.NonMaps.client.world.overlay;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.event.shared.HandlerRegistration;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.xml.client.Document;
import com.google.gwt.xml.client.NamedNodeMap;
import com.google.gwt.xml.client.Node;
import com.google.gwt.xml.client.NodeList;
import com.google.gwt.xml.client.XMLParser;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.ServerProxy.DownloadHandler;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PhysicalControlParameter.ReturnMode;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;

public class CompareDialog extends GWTDialog {

	Preset preset1, preset2;
	Document xml;

	static CompareDialog theDialog = null;

	public static void open(Preset p1) {
		theDialog = new CompareDialog(p1);
	}

	public static void open(Preset p1, Preset p2) {
		theDialog = new CompareDialog(p1, p2);
	}

	private CompareDialog(Preset p1) {
		preset1 = p1;
		preset2 = null;
		init();
	}

	private CompareDialog(Preset p1, Preset p2) {
		preset1 = p1;
		preset2 = p2;
		init();
	}

	private void init() {
		RootPanel.get().add(this);
		
		getElement().addClassName("preset-compare-dialog");

		initalShow();

		setAnimationEnabled(true);
		setGlassEnabled(false);
		setModal(false);

		addHeader("Preset Comparison Tree View");

		if (preset2 == null)
			getCsvOfEditBuffer();
		else
			getCsv();

	}

	private void getCsv() {
		NonMaps.theMaps.getServerProxy().getDifferencesOf2PresetsAsCsv(preset1.getUUID(), preset2.getUUID(), new DownloadHandler() {
			@Override
			public void onFileDownloaded(String text) {
				xml = XMLParser.parse(text);
				setup();
			}

			@Override
			public void onError() {
			}
		});
	}

	private void getCsvOfEditBuffer() {
		NonMaps.theMaps.getServerProxy().getDifferencesOfPresetsToEditbufferAsCsv(preset1.getUUID(), new DownloadHandler() {
			@Override
			public void onFileDownloaded(String text) {
				xml = XMLParser.parse(text);
				setup();
			}

			@Override
			public void onError() {
			}
		});
	}

	static int lastPopupLeft = -1;
	static int lastPopupTop = -1;

	@Override
	protected void setLastPopupPos(int popupLeft, int popupTop) {
		lastPopupLeft = popupLeft;
		lastPopupTop = popupTop;
	}

	@Override
	protected int getLastPopupPosTop() {
		return lastPopupTop;
	}

	@Override
	protected int getLastPopupPosLeft() {
		return lastPopupLeft;
	}

	@Override
	protected void commit() {
		hide();
		theDialog = null;
		NonMaps.theMaps.captureFocus();
		NonMaps.theMaps.getNonLinearWorld().requestLayout();
	}

	protected void setup() {
		HTMLPanel superPanel = new HTMLPanel("");
		Button b = new Button("Refresh");
		b.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				if (preset2 == null)
					getCsvOfEditBuffer();
				else
					getCsv();			
			}
		});
		superPanel.add(b);
		if (xml != null) {	
			NamedNodeMap diffNode = xml.getElementsByTagName("diff").item(0).getAttributes();
			String presetName1 = diffNode.getNamedItem("a").getNodeValue();
			String presetName2 = diffNode.getNamedItem("b").getNodeValue();
			
			if(preset1 == null)
				presetName1 = "Editbuffer";
			if(preset2 == null)
				presetName2 = "Editbuffer";
			
			int row = 0;
			FlexTable table = new FlexTable();
			table.getElement().addClassName("compare-tree");
			table.setText(row, 1, presetName1);
			table.setText(row, 2, presetName2);
			row++;
			table.setText(row, 1, "---");
			table.setText(row, 2, "---");
			row++;

			NodeList groups = xml.getElementsByTagName("group");
			int numGroups = groups.getLength();

			for (int numGroup = 0; numGroup < numGroups; numGroup++) {
				Node group = groups.item(numGroup);

				if (group.getNodeType() == Node.ELEMENT_NODE) {
					table.setText(row, 0, group.getAttributes().getNamedItem("name").getNodeValue());
					row++;

					NodeList params = group.getChildNodes();

					int numParams = params.getLength();

					for (int numParam = 0; numParam < numParams; numParam++) {
						Node param = params.item(numParam);

						if (param.getNodeType() == Node.ELEMENT_NODE) {

							String paramName = param.getAttributes().getNamedItem("name").getNodeValue();
							table.setWidget(row, 0, new HTMLPanel("span", "&#9584; " + paramName));
							table.getWidget(row, 0).getElement().addClassName("indent-1");
							row++;

							NodeList changes = param.getChildNodes();

							int numChanges = changes.getLength();

							for (int numChange = 0; numChange < numChanges; numChange++) {
								Node change = changes.item(numChange);

								if (change.getNodeType() == Node.ELEMENT_NODE) {
									if (change.getNodeName().equals("value")) {
										table.setText(row - 1, 1, change.getAttributes().getNamedItem("a").getNodeValue());
										table.setText(row - 1, 2, change.getAttributes().getNamedItem("b").getNodeValue());
									} else {
										table.setWidget(row, 0, new HTMLPanel("span", "&#9584; "
												+ translateChangeName(change.getNodeName())));
										table.getWidget(row, 0).getElement().addClassName("indent-2");
										String aValue = change.getAttributes().getNamedItem("a").getNodeValue();
										String bValue = change.getAttributes().getNamedItem("b").getNodeValue();
										table.setText(row, 1, translateChangeValue(paramName, change.getNodeName(), aValue));
										table.setText(row, 2, translateChangeValue(paramName, change.getNodeName(), bValue));
										row++;
									}
								}
							}
						}
					}
				}
			}
			superPanel.add(table);
			setWidget(superPanel);
		}
	}

	private String translateChangeValue(String paramName, String nodeName, String nodeValue) {
		switch (nodeName) {
		case "mc-select":
			return MacroControls.fromInt(Integer.parseInt(nodeValue)).toPrettyString();

		case "return-mode":
			return ReturnMode.fromInt(Integer.parseInt(nodeValue)).toString();

		case "name":
			return nodeValue;

		case "info":
			return nodeValue;

		case "behaviour":
			int b = Integer.parseInt(nodeValue);
			if (b == 0)
				return "Absolute";
			return "Relative";
		}
		return nodeValue;
	}

	private String translateChangeName(String nodeName) {
		switch (nodeName) {
		case "mc-amount":
			return "MC Amount";

		case "mc-select":
			return "MC Select";

		case "return-mode":
			return "Return Mode";

		case "name":
			return "Name";

		case "info":
			return "Info";

		case "behaviour":
			return "Behaviour";
		}
		return nodeName;
	}
}
