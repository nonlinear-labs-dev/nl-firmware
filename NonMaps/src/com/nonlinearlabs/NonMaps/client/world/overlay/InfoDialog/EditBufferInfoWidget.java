package com.nonlinearlabs.NonMaps.client.world.overlay.InfoDialog;

import com.google.gwt.event.dom.client.BlurEvent;
import com.google.gwt.event.dom.client.BlurHandler;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.event.dom.client.FocusEvent;
import com.google.gwt.event.dom.client.FocusHandler;
import com.google.gwt.event.dom.client.KeyCodes;
import com.google.gwt.event.dom.client.KeyPressEvent;
import com.google.gwt.event.dom.client.KeyPressHandler;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.TextArea;
import com.google.gwt.user.client.ui.TextBox;
import com.google.gwt.user.client.ui.Widget;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.ColorTag;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.ColorTag.Color;

public class EditBufferInfoWidget {
	private static EditBufferInfoWidget instance;
	private TextArea comment;
	private Label deviceName;
	private Label softwareVersion;
	private TextBox name;
	private Button[] colors;
	private Widget haveFocus = null;
	
	public Widget panel = null;

	static public EditBufferInfoWidget get() {
		if(instance == null)
			instance = new EditBufferInfoWidget();
		return instance;
	}
	
	Preset getEditBuffer() {
		return NonMaps.get().getNonLinearWorld().getPresetManager().getLoadedPreset();
	}
	
	private Button addEditBufferColorButton(ColorTag.Color c) {		
		Button b = new Button();
		if(c == ColorTag.Color.none)
			b.setHTML("<div style=\"font-size: smaller;\">✕</div>"); 
		b.getElement().addClassName("colortaggingfield-" + c.name());
		b.getElement().addClassName("colortaggingfield-template");
		b.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				NonMaps.get().getServerProxy().setEditBufferAttribute("color", c.toString());
				updateCurrentHighlight(b);
			}
		});
		return b;
	}
	
	public void updateCurrentHighlight(Button current) {
		for(Button b: colors) {
			if(b != current)
				b.getElement().removeClassName("colortag-current");
			else
				b.getElement().addClassName("colortag-current");
		}
	}
	
	public void updateInfo(Preset eb) {
		if (eb != null) {
			String presetName = eb.getCurrentName();
			deviceName.setText(eb.getAttribute("DeviceName"));
			softwareVersion.setText(eb.getAttribute("SoftwareVersion"));
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
		
			ColorTag.Color c = ColorTag.Color.toEnum(eb.getAttribute("color"));
			updateCurrentHighlight(colors[c.ordinal()]);
		}
	}
	
	protected EditBufferInfoWidget() {
		
		colors = new Button[7];
		
		HTMLPanel ebNameBox = new HTMLPanel("div", "");
		ebNameBox.getElement().addClassName("preset-name-and-pos");
		ebNameBox.add(name = new TextBox());
		name.getElement().addClassName("preset-name-box");

		HTMLPanel editBufferColorBox = new HTMLPanel("div", "");
		editBufferColorBox.getElement().addClassName("tag-div");
		editBufferColorBox.add(colors[Color.green.ordinal()] = addEditBufferColorButton(Color.green));
		editBufferColorBox.add(colors[Color.blue.ordinal()] = addEditBufferColorButton(Color.blue));
		editBufferColorBox.add(colors[Color.yellow.ordinal()] = addEditBufferColorButton(Color.yellow));
		editBufferColorBox.add(colors[Color.orange.ordinal()] = addEditBufferColorButton(Color.orange));
		editBufferColorBox.add(colors[Color.purple.ordinal()] = addEditBufferColorButton(Color.purple));
		editBufferColorBox.add(colors[Color.red.ordinal()] = addEditBufferColorButton(Color.red));
		editBufferColorBox.add(colors[Color.none.ordinal()] = addEditBufferColorButton(Color.none));
		
		FlexTable panel = new FlexTable();
		addRow(panel, "Name", ebNameBox);
		addRow(panel, "Comment", comment = new TextArea());
		addRow(panel, "Color Tag", editBufferColorBox);
		addRow(panel, "Device Name", deviceName = new Label(""));
		addRow(panel, "UI Version", softwareVersion = new Label(""));

		comment.addFocusHandler(new FocusHandler() {

			@Override
			public void onFocus(FocusEvent event) {
				haveFocus = comment;
			}
		});

		comment.addBlurHandler(new BlurHandler() {

			@Override
			public void onBlur(BlurEvent event) {
				haveFocus = null;
				String oldInfo = getEditBuffer().getAttribute("Comment");
;
				if (!oldInfo.equals(comment.getText())) {
					NonMaps.theMaps.getServerProxy().setEditBufferAttribute("Comment", comment.getText());
				}
			}
		});

		name.addFocusHandler(new FocusHandler() {

			@Override
			public void onFocus(FocusEvent event) {
				haveFocus = name;
			}
		});

		name.addBlurHandler(new BlurHandler() {
			@Override
			public void onBlur(BlurEvent event) {
				haveFocus = null;
				String oldName = getEditBuffer().getCurrentName();
				
				if (!oldName.equals(name.getText())) {
					NonMaps.theMaps.getServerProxy().renameEditBuffer(name.getText());
				}
			}
		});

		name.addKeyPressHandler(new KeyPressHandler() {

			@Override
			public void onKeyPress(KeyPressEvent arg0) {
				if (arg0.getCharCode() == KeyCodes.KEY_ENTER) {
					name.setFocus(false);
					comment.setFocus(true);
				}
			}
		});
		
		this.panel = panel;
	}
	
	private void addRow(FlexTable panel, String name, Widget content) {
		int c = panel.getRowCount();
		panel.setWidget(c, 0, new Label(name));
		panel.setWidget(c, 1, content);
	}
}
