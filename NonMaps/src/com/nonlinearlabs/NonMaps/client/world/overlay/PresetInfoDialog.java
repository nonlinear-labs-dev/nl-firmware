package com.nonlinearlabs.NonMaps.client.world.overlay;

import java.util.Date;

import com.google.gwt.event.dom.client.BlurEvent;
import com.google.gwt.event.dom.client.BlurHandler;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.event.dom.client.FocusEvent;
import com.google.gwt.event.dom.client.FocusHandler;
import com.google.gwt.event.dom.client.KeyCodes;
import com.google.gwt.event.dom.client.KeyPressEvent;
import com.google.gwt.event.dom.client.KeyPressHandler;
import com.google.gwt.event.dom.client.MouseDownEvent;
import com.google.gwt.event.dom.client.MouseDownHandler;
import com.google.gwt.event.shared.HandlerRegistration;
import com.google.gwt.i18n.client.DateTimeFormat;
import com.google.gwt.i18n.client.DateTimeFormat.PredefinedFormat;
import com.google.gwt.user.client.Event;
import com.google.gwt.user.client.Event.NativePreviewEvent;
import com.google.gwt.user.client.Event.NativePreviewHandler;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.IntegerBox;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.TabPanel;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.overlay.InfoDialog.EditBufferInfoWidget;
import com.nonlinearlabs.NonMaps.client.world.overlay.InfoDialog.PresetInfoWidget;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;

public class PresetInfoDialog extends GWTDialog {

	public static PresetInfoDialog theDialog;
	private Preset thePreset;
	public static EditBufferInfoWidget editBufferInfoPage = null;
	public static PresetInfoWidget presetInfoPage = null;
	public static Node _editBufferNode = null;

	
	public Preset getCurrentPreset() {
		return thePreset;
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

	EditBufferInfoWidget createEditBufferTab() {
		return EditBufferInfoWidget.get();
	}
	
	PresetInfoWidget createPresetInfoTab() {
		return PresetInfoWidget.get();
	}
	
	private void addContent() {
		editBufferInfoPage = createEditBufferTab();
		presetInfoPage = createPresetInfoTab();
		if(_editBufferNode != null)
			editBufferInfoPage.updateFromNode(_editBufferNode);
		else
			editBufferInfoPage.updateInfo(getEditBuffer());
		presetInfoPage.updateInfo(getCurrentPreset());
		
		TabPanel p = new TabPanel();
		p.add(presetInfoPage.panel, "Preset");
		p.getElement().addClassName("tab-1");
		p.add(editBufferInfoPage.panel, "EditBuffer");
		p.selectTab(0);
		setWidget(p);
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
		presetInfoPage.updateInfo(preset);
		centerIfOutOfView();
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

	public static void update() {
		update(NonMaps.theMaps.getNonLinearWorld().getPresetManager().getSelectedPreset());
	}

	public void updateEditBuffer(Node editBufferNode) {
		editBufferInfoPage.updateFromNode(editBufferNode);		
	}

	public static void setLastUpdateNode(Node editBufferNode) {
		_editBufferNode = editBufferNode;
		
		if(editBufferInfoPage != null)
			editBufferInfoPage.setLastUpdateNode(_editBufferNode);
	}
}