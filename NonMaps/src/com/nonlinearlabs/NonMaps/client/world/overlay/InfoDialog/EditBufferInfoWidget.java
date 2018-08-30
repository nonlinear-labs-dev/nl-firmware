package com.nonlinearlabs.NonMaps.client.world.overlay.InfoDialog;

import com.google.gwt.event.dom.client.KeyCodes;
import com.google.gwt.user.client.ui.*;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.ColorTag;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.ColorTag.Color;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;

public class EditBufferInfoWidget {
	private static EditBufferInfoWidget instance;
	private static Node editBufferNode = null;
	private TextArea comment;
	private TextBox name;
	private Widget haveFocus = null;
	private ColorTagBox colorBox = new ColorTagBox() {
		@Override
		public void onBoxClick(Color c) {
			NonMaps.get().getServerProxy().setEditBufferAttribute("color", c.toString());	
		}
	};
	
	public Widget panel;

	static public EditBufferInfoWidget get() {
		if(instance == null)
			instance = new EditBufferInfoWidget();
		
		if(editBufferNode != null)
			instance.updateFromNode(editBufferNode);
		
		return instance;
	}
	
	private Preset getEditBuffer() {
		return NonMaps.get().getNonLinearWorld().getPresetManager().getLoadedPreset();
	}
	
	public void updateInfo(Preset eb) {
		if (eb != null) {
			String presetName = eb.getCurrentName();
			String commentText = eb.getAttribute("Comment");

			if (haveFocus != comment) {
				if (!commentText.equals(comment.getText())) {
					comment.setText(commentText);
				}
			}

			if (haveFocus != name) {
				if (!presetName.equals(name.getText())) {
					name.setText(presetName);
				}
			}
			colorBox.updateCurrentHighlight(ColorTag.Color.toEnum(eb.getAttribute("color")));
		}
	}
	
	protected EditBufferInfoWidget() {
		
		HTMLPanel ebNameBox = new HTMLPanel("div", "");
		ebNameBox.getElement().addClassName("preset-name-and-pos");
		ebNameBox.add(name = new TextBox());
		name.getElement().addClassName("preset-name-box");
		
		FlexTable panel = new FlexTable();
		addRow(panel, "Name", ebNameBox);
		addRow(panel, "Comment", comment = new TextArea());
		addRow(panel, "Color Tag", colorBox.getHTML());

		comment.addFocusHandler(event -> haveFocus = comment);

		comment.addBlurHandler(event -> {
			haveFocus = null;
			String oldInfo = getEditBuffer().getAttribute("Comment");

			if (!oldInfo.equals(comment.getText())) {
				NonMaps.theMaps.getServerProxy().setEditBufferAttribute("Comment", comment.getText());
			}
		});

		name.addFocusHandler(event -> haveFocus = name);

		name.addBlurHandler(event -> {
			haveFocus = null;
			String oldName = getEditBuffer().getCurrentName();

			if (!oldName.equals(name.getText())) {
				NonMaps.theMaps.getServerProxy().renameEditBuffer(name.getText());
			}
		});

		name.addKeyPressHandler(arg0 -> {
			if (arg0.getCharCode() == KeyCodes.KEY_ENTER) {
				name.setFocus(false);
				comment.setFocus(true);
			}
		});
		
		this.panel = panel;
	}
	
	private void addRow(FlexTable panel, String name, Widget content) {
		int c = panel.getRowCount();
		panel.setWidget(c, 0, new Label(name));
		panel.setWidget(c, 1, content);
	}

	public void setLastUpdateNode(Node ebNode) {
		if(ebNode != null)
			editBufferNode = ebNode;
	}
	
	public void updateFromNode(Node editBufferNode) {
		if(editBufferNode == null)
			return;
		
		setLastUpdateNode(editBufferNode);
		
		Node preset = editBufferNode.getChildNodes().item(0).getNextSibling();
		if(preset != null) {
			String color = "";
			String comment = "";
			
			Node child = preset.getFirstChild();
			while(child != null)
			{
				if(child.getNodeType() == Node.ELEMENT_NODE)
				{
					String nodeName = child.getAttributes().item(0).toString();
					switch(nodeName) {
						case "Comment":
							comment = child.getChildNodes().item(0).toString();
							break;
						case "color":
							color = child.getChildNodes().item(0).toString();
							break;
						default:
							break;
					}
				}
				child = child.getNextSibling();
			}
			
			if(this.name != haveFocus) {
				this.name.setText(preset.getAttributes().getNamedItem("name").toString());
			}
			if(this.comment != haveFocus) {
				this.comment.setText(comment);
			}
			colorBox.updateCurrentHighlight(ColorTag.Color.toEnum(color));
		}
	}
}
