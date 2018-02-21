package com.nonlinearlabs.NonMaps.client.world.overlay;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.xml.client.Document;
import com.google.gwt.xml.client.Element;
import com.google.gwt.xml.client.Node;
import com.google.gwt.xml.client.NodeList;
import com.google.gwt.xml.client.XMLParser;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.ServerProxy.DownloadHandler;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PhysicalControlParameter.ReturnMode;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;

public class CompareDialog extends GWTDialog {

	private Document xml;
	private Document presetAXml;
	private Document presetBXml;
	private Button loadPresetA;
	private Button loadPresetB;
	Preset preset1, preset2;
	String csvWithDiffs = "";
	
	String preset1PositionText;
	String preset2PositionText;
	
	FlexTable table = null;

	public static void open(Preset p1) {
		new CompareDialog(p1);
		NonMaps.get().getNonLinearWorld().getPresetManager().closeMultiSelection();
	}

	public static void open(Preset p1, Preset p2) {
		new CompareDialog(p1, p2);
		NonMaps.get().getNonLinearWorld().getPresetManager().closeMultiSelection();
	}

	private CompareDialog(Preset p) {
		init(p, null);
	}

	private CompareDialog(Preset p1, Preset p2) {
		init(p1, p2);
	}

	private void init(Preset p1, Preset p2) {
		NonMaps.get().getNonLinearWorld().getViewport().getOverlay().addCompareDialog(this);

		RootPanel.get().add(this);

		getElement().addClassName("preset-compare-dialog");

		initalShow();

		setAnimationEnabled(true);
		setGlassEnabled(false);
		setModal(false);

		addHeader("Preset Comparison Tree View");

		if (p2 == null)
			load(p1);
		else
			load(p1, p2);

	}

	private void load(Preset p1, Preset p2) {
		NonMaps.theMaps.getServerProxy().getDifferencesOf2PresetsAsCsv(p1.getUUID(), p2.getUUID(), new DownloadHandler() {
			@Override
			public void onFileDownloaded(String text) {
				xml = XMLParser.parse(text);
				setup();
			}

			@Override
			public void onError() {
			}
		});

		downloadPresets(p1, p2);
	}

	public void downloadPresets(Preset p1, Preset p2) {
		NonMaps.theMaps.getServerProxy().downloadPreset(p1 != null ? p1.getUUID() : "", new DownloadHandler() {
			@Override
			public void onFileDownloaded(String text) {
				presetAXml = XMLParser.parse(text);
				updateLoadButtonStates();
			}

			@Override
			public void onError() {
			}
		});

		NonMaps.theMaps.getServerProxy().downloadPreset(p2 != null ? p2.getUUID() : "", new DownloadHandler() {
			@Override
			public void onFileDownloaded(String text) {
				presetBXml = XMLParser.parse(text);
				updateLoadButtonStates();
			}

			@Override
			public void onError() {
			}
		});
	}

	private void load(Preset p1) {
		NonMaps.theMaps.getServerProxy().getDifferencesOfPresetsToEditbufferAsCsv(p1.getUUID(), new DownloadHandler() {
			@Override
			public void onFileDownloaded(String text) {
				xml = XMLParser.parse(text);
				setup();
			}

			@Override
			public void onError() {
			}
		});

		downloadPresets(p1, null);
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
		NonMaps.get().getNonLinearWorld().getViewport().getOverlay().removeCompareDialog(this);
		NonMaps.theMaps.captureFocus();
		NonMaps.theMaps.getNonLinearWorld().requestLayout();
	}

	protected void setup() {
		if (xml != null) {
			Element root = xml.getDocumentElement();
			Node positionNode = ServerProxy.getChild(root, "position");
			Node nameNode = ServerProxy.getChild(root, "name");

			int row = 0;
			table = new FlexTable();
			table.getElement().addClassName("compare-tree");
			preset1PositionText = positionNode.getAttributes().getNamedItem("a").getNodeValue();
			preset2PositionText = positionNode.getAttributes().getNamedItem("b").getNodeValue();
			table.setText(row, 1, preset1PositionText);
			table.setText(row, 2, preset2PositionText);
			row++;
			table.setText(row, 1, nameNode.getAttributes().getNamedItem("a").getNodeValue());
			table.setText(row, 2, nameNode.getAttributes().getNamedItem("b").getNodeValue());
			row++;
			addLoadButtons(row, table);
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
			setWidget(table);
		}
	}

	public void update() {
		updateLoadButtonStates();
	}

	private void updateLoadButtonStates() {
		if (xml != null) {
			Element root = xml.getDocumentElement();
			Node hashNode = ServerProxy.getChild(root, "hash");
			String a = hashNode.getAttributes().getNamedItem("a").getNodeValue();
			String b = hashNode.getAttributes().getNamedItem("b").getNodeValue();
			String ebHash = NonMaps.get().getNonLinearWorld().getParameterEditor().getHash();
			
			loadPresetA.setEnabled(!(presetAXml == null || a.equals(ebHash)));
			loadPresetB.setEnabled(!(presetBXml == null || b.equals(ebHash)));
		}
	}

	public void addLoadButtons(int row, FlexTable table) {
		table.setWidget(row, 1, loadPresetA = new Button("Load"));
		table.setWidget(row, 2, loadPresetB = new Button("Load"));

		updateLoadButtonStates();

		loadPresetA.addClickHandler(new ClickHandler() {

			@Override
			public void onClick(ClickEvent event) {
				NonMaps.get().getServerProxy().loadEditBuffer(presetAXml);
			}
		});

		loadPresetB.addClickHandler(new ClickHandler() {

			@Override
			public void onClick(ClickEvent event) {
				NonMaps.get().getServerProxy().loadEditBuffer(presetBXml);
			}
		});
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
