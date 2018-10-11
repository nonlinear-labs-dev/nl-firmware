package com.nonlinearlabs.NonMaps.client.world.overlay;

import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.InfoDialog.EditBufferInfoWidget;
import com.nonlinearlabs.NonMaps.client.world.overlay.InfoDialog.PresetInfoWidget;

public class PresetInfoDialog extends GWTDialog {

	public static PresetInfoDialog theDialog;
	public static EditBufferInfoWidget editBufferInfoPage = null;
	public static PresetInfoWidget presetInfoPage = null;
	public static Node _editBufferNode = null;

	public Preset getCurrentPreset() {
		return NonMaps.get().getNonLinearWorld().getPresetManager().getSelectedPreset();
	}

	private PresetInfoDialog() {
		RootPanel.get().add(this);

		getElement().addClassName("preset-info-dialog");
		initalShow();
		setAnimationEnabled(true);
		setGlassEnabled(false);
		setModal(false);
		addHeader("Preset/EditBuffer Info");
		addContent();
		super.pushDialogToFront();
	}

	public static Preset getEditBuffer() {
		return NonMaps.get().getNonLinearWorld().getPresetManager().getLoadedPreset();
	}

	private EditBufferInfoWidget createEditBufferTab() {
		return EditBufferInfoWidget.get();
	}

	private PresetInfoWidget createPresetInfoTab() {
		return PresetInfoWidget.get();
	}

	private void addContent() {
		editBufferInfoPage = createEditBufferTab();
		presetInfoPage = createPresetInfoTab();
		if (_editBufferNode != null)
			editBufferInfoPage.updateFromNode(_editBufferNode);
		else
			editBufferInfoPage.updateInfo(getEditBuffer());

		presetInfoPage.updateInfo(getCurrentPreset(), false);

		HTMLPanel pane = new HTMLPanel("div", "");
		pane.getElement().addClassName("preset-info-edit-buffer-dialog-pane");
		HTMLPanel tabs = new HTMLPanel("div", "");
		tabs.getElement().addClassName("tabs");
		Button presetTab = new Button("Preset");
		presetTab.getElement().addClassName("preset tab");
		Button editBufferTab = new Button("EditBuffer");
		editBufferTab.getElement().addClassName("edit-buffer tab");
		HTMLPanel content = new HTMLPanel("div", "");
		content.getElement().addClassName("content");
		HTMLPanel tabRest = new HTMLPanel("div", "");
		tabRest.getElement().addClassName("tab-rest");

		pane.add(tabs);
		pane.add(content);
		tabs.add(presetTab);
		tabs.add(editBufferTab);
		tabs.add(tabRest);

		content.add(presetInfoPage.panel);
		presetTab.getElement().addClassName("selected");

		presetTab.addClickHandler(a -> {
			content.remove(0);
			content.add(presetInfoPage.panel);
			presetTab.getElement().addClassName("selected");
			editBufferTab.getElement().removeClassName("selected");
		});

		editBufferTab.addClickHandler(a -> {
			content.remove(0);
			content.add(editBufferInfoPage.panel);
			presetTab.getElement().removeClassName("selected");
			editBufferTab.getElement().addClassName("selected");
		});

		setWidget(pane);
	}

	@Override
	protected void commit() {
		hide();
		theDialog = null;
		NonMaps.theMaps.captureFocus();
		NonMaps.theMaps.getNonLinearWorld().requestLayout();
	}

	public static void toggle() {
		if (theDialog != null) {
			theDialog.commit();
		} else {
			if (!NonMaps.theMaps.getNonLinearWorld().getPresetManager().isEmpty())
				theDialog = new PresetInfoDialog();
		}
	}

	public static boolean isShown() {
		return theDialog != null;
	}

	public static void update(Preset preset) {
		if (theDialog != null) {
			theDialog.updateInfo(preset);
		}
	}

	private void updateInfo(Preset preset) {
		editBufferInfoPage.updateInfo(getEditBuffer());
		presetInfoPage.saveContent();
		presetInfoPage.updateInfo(preset, true);
		centerIfOutOfView();
	}

	private static int lastPopupLeft = -1;
	private static int lastPopupTop = -1;

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

	public static void update() {
		update(NonMaps.theMaps.getNonLinearWorld().getPresetManager().getSelectedPreset());
	}

	public void updateEditBuffer(Node editBufferNode) {
		editBufferInfoPage.updateFromNode(editBufferNode);
	}

	public static void setLastUpdateNode(Node editBufferNode) {
		_editBufferNode = editBufferNode;

		if (editBufferInfoPage != null)
			editBufferInfoPage.setLastUpdateNode(_editBufferNode);
	}
}