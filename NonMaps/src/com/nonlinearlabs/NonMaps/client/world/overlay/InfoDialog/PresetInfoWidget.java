package com.nonlinearlabs.NonMaps.client.world.overlay.InfoDialog;

import com.google.gwt.event.dom.client.KeyCodes;
import com.google.gwt.i18n.client.DateTimeFormat;
import com.google.gwt.i18n.client.DateTimeFormat.PredefinedFormat;
import com.google.gwt.user.client.ui.*;
import com.nonlinearlabs.NonMaps.client.GMTTimeZone;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.ColorTag;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.ColorTag.Color;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;

import java.util.Date;

public class PresetInfoWidget {
	
	private static PresetInfoWidget instance;
	private TextArea comment;
	private Label deviceName;
	private Label softwareVersion;
	private TextBox name;
	private Label storeTime;
	private Label bankName;
	private IntegerBox position;
	private Widget haveFocus = null;
	private Preset m_currentShownPreset = null;
	
	private ColorTagBox colorBox = new ColorTagBox() {
		@Override
		public void onBoxClick(Color c) {
			NonMaps.get().getServerProxy().setPresetAttribute(PresetInfoWidget.get().getCurrentPreset(), "color", c.toString());	
		}
	};
	
	public Widget panel;

	static public PresetInfoWidget get() {
		if(instance == null)
			instance = new PresetInfoWidget();
		return instance;
	}

	public Preset getCurrentPreset() {
		return NonMaps.get().getNonLinearWorld().getPresetManager().getSelectedPreset();
	}
	
	private String localizeTime(String iso) {
		try {
			DateTimeFormat f = DateTimeFormat.getFormat("yyyy-MM-ddTHH:mm:ssZZZZ");
			Date d = f.parse(iso);
			DateTimeFormat locale = DateTimeFormat.getFormat(PredefinedFormat.DATE_TIME_SHORT);
			return locale.format(d, new GMTTimeZone());
		} catch (Exception e) {
			return iso;
		}
	}
		
	public void updateInfo(Preset preset, boolean force) {
		m_currentShownPreset = preset;
		if (preset != null) {
			String presetName = preset.getCurrentName();
			deviceName.setText(preset.getAttribute("DeviceName"));
			softwareVersion.setText(preset.getAttribute("SoftwareVersion"));
			storeTime.setText(localizeTime(preset.getAttribute("StoreTime")));
			String commentText = preset.getAttribute("Comment");

			if (force || haveFocus != comment) {
				if (!commentText.equals(comment.getText())) {
					comment.setText(commentText);
				}
			}

			if (force || haveFocus != name) {
				if (!presetName.equals(name.getText())) {
					name.setText(presetName);
				}
			}

			if (force || haveFocus != position) {
				position.setText(preset.getPaddedNumber());
			}

			Bank bank = preset.getParent();
			bankName.setText(bank.getOrderNumber() + " - " + bank.getTitleName());
			
			colorBox.updateCurrentHighlight(ColorTag.Color.toEnum(preset.getAttribute("color")));
			
		}
	}
	
	protected PresetInfoWidget() {
		
		HTMLPanel presetNameAndPositionBox = new HTMLPanel("div", "");
		presetNameAndPositionBox.getElement().addClassName("preset-name-and-pos");
		presetNameAndPositionBox.add(position = new IntegerBox());
		presetNameAndPositionBox.add(name = new TextBox());
		position.getElement().addClassName("position-box");
		name.getElement().addClassName("preset-name-box");
		
		
		FlexTable panel = new FlexTable();
		addRow(panel, "Bank", bankName = new Label());
		addRow(panel, "Position/Name", presetNameAndPositionBox);
		addRow(panel, "Comment", comment = new TextArea());
		addRow(panel, "Color Tag", colorBox.getHTML());
		addRow(panel, "Last Change", storeTime = new Label(""));
		addRow(panel, "Device Name", deviceName = new Label(""));
		addRow(panel, "UI Version", softwareVersion = new Label(""));

		position.getElement().addClassName("gwt-TextBox");

		comment.addFocusHandler(event -> haveFocus = comment);

		comment.addBlurHandler(event -> {
			haveFocus = null;

			if (getCurrentPreset() != null) {
				String oldInfo = getCurrentPreset().getAttribute("Comment");

				if (!oldInfo.equals(comment.getText())) {
					NonMaps.theMaps.getServerProxy().setPresetAttribute(getCurrentPreset(), "Comment", comment.getText());
				}
			}
		});

		name.addFocusHandler(event -> haveFocus = name);

		name.addBlurHandler(event -> {
			haveFocus = null;

			if (getCurrentPreset() != null) {
				String oldName = getCurrentPreset().getCurrentName();

				if (!oldName.equals(name.getText())) {
					NonMaps.theMaps.getServerProxy().renamePreset(getCurrentPreset().getUUID(), name.getText());
				}
			}
		});

		name.addKeyPressHandler(arg0 -> {
			if (arg0.getCharCode() == KeyCodes.KEY_ENTER) {
				name.setFocus(false);
				comment.setFocus(true);
			}
		});

		position.addFocusHandler(event -> haveFocus = position);

		position.addBlurHandler(event -> {
			haveFocus = null;

			if (getCurrentPreset() != null) {
				int oldNumber = getCurrentPreset().getNumber();
				Integer newPos = position.getValue();
				if (newPos != null) {
					if (!newPos.equals(oldNumber)) {
						Bank bank = getCurrentPreset().getParent();
						int presetCount = bank.getPresetList().getPresetCount();
						int targetPos = newPos;
						targetPos = Math.max(targetPos, 1);
						targetPos = Math.min(targetPos, presetCount);

						if (targetPos == presetCount)
							NonMaps.theMaps.getServerProxy().movePresetBelow(getCurrentPreset(), bank.getLast());
						else if (targetPos > oldNumber)
							NonMaps.theMaps.getServerProxy().movePresetBelow(getCurrentPreset(), bank.getPreset(targetPos - 1));
						else
							NonMaps.theMaps.getServerProxy().movePresetAbove(getCurrentPreset(), bank.getPreset(targetPos - 1));
					}
				}
			}

			Preset p = getCurrentPreset();
			if(p != null)
			position.setText(p.getPaddedNumber());
		});

		position.addKeyPressHandler(arg0 -> {
			if (arg0.getCharCode() == KeyCodes.KEY_ENTER) {
				position.setFocus(false);
				name.setFocus(true);
			}
		});
		this.panel = panel;
		
		updateInfo(getCurrentPreset(), false);
	}
	
	private void addRow(FlexTable panel, String name, Widget content) {
		int c = panel.getRowCount();
		panel.setWidget(c, 0, new Label(name));
		panel.setWidget(c, 1, content);
	}

	public void saveContent() {
		if(m_currentShownPreset != null) {
			if(m_currentShownPreset.getCurrentName() != name.getText() && haveFocus == name) {
				m_currentShownPreset.setName(name.getText());
				name.setFocus(false);
				haveFocus = null;
			}
			if(m_currentShownPreset.getAttribute("Comment") != comment.getText())
				NonMaps.get().getServerProxy().setPresetAttribute(m_currentShownPreset, "Comment", comment.getText());
		}
	}
}
