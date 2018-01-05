package com.nonlinearlabs.NonMaps.client.world;

import com.google.gwt.core.client.Scheduler;
import com.google.gwt.event.dom.client.ChangeEvent;
import com.google.gwt.event.dom.client.ChangeHandler;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.event.dom.client.KeyCodes;
import com.google.gwt.event.logical.shared.CloseEvent;
import com.google.gwt.event.logical.shared.CloseHandler;
import com.google.gwt.user.client.Command;
import com.google.gwt.user.client.Event;
import com.google.gwt.user.client.Event.NativePreviewEvent;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.FlowPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.PopupPanel;
import com.google.gwt.user.client.ui.RadioButton;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.TextArea;
import com.google.gwt.user.client.ui.TextBox;
import com.google.gwt.user.client.ui.Widget;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterEditor;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.GWTDialog;

public class AppendOverwriteInsertPresetDialog extends GWTDialog {

	private static AppendOverwriteInsertPresetDialog theDialog;

	public enum Action {
		INSERT, OVERWRITE, APPEND, DEFAULT
	}

	public interface Handler {
		public void onActionChoosen(String newName, String info, Action action);
	}

	private TextBox nameField;
	private TextArea infoField;
	private Handler handler;
	private boolean nameWasChangedByUser = false;
	private String originalName;
	private String generatedName;
	private static Action lastChoosen = Action.APPEND;

	public static AppendOverwriteInsertPresetDialog open(String name, String autoGeneratedName, Handler h) {
		if (theDialog != null)
			theDialog.hide();

		theDialog = new AppendOverwriteInsertPresetDialog(name, autoGeneratedName, h);

		theDialog.addCloseHandler(new CloseHandler<PopupPanel>() {

			@Override
			public void onClose(CloseEvent<PopupPanel> event) {
				theDialog = null;
			}
		});

		return theDialog;
	}

	private AppendOverwriteInsertPresetDialog(String name, String autoGeneratedName, Handler h) {
		this.handler = h;

		this.originalName = name;
		this.generatedName = autoGeneratedName;

		RootPanel.get().add(this);
		getElement().addClassName("nonmaps-dialog modal-dialog");

		setAnimationEnabled(true);
		setGlassEnabled(true);
		addHeader("Store Preset");
		setWidget(createRootPanel());

		Scheduler.get().scheduleFinally(new Command() {
			public void execute() {
				nameField.selectAll();
				nameField.setFocus(true);
			}
		});

		setNameText();
		initalShow();
	}

	private FlowPanel createRootPanel() {
		FlowPanel panel = new FlowPanel();
		panel.add(createLabelPanel());
		panel.add(createPresetInfoPanel());
		panel.add(createOptionsPanel());
		panel.add(createButtonsPanel());
		return panel;
	}

	private Widget createPresetInfoPanel() {
		FlexTable panel = new FlexTable();
		panel.setWidget(0, 0, new Label("Preset Info"));
		panel.setWidget(0, 1, infoField = new TextArea());
		return panel;
	}

	private FlowPanel createButtonsPanel() {
		FlowPanel buttons = new FlowPanel();
		buttons.getElement().addClassName("save-commands");

		Button ok = new Button("OK");
		ok.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				store();
			}
		});

		Button cancel = new Button("Cancel");
		cancel.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				cancel();
			}
		});

		buttons.add(ok);
		buttons.add(cancel);
		return buttons;
	}

	private FlowPanel createOptionsPanel() {
		FlowPanel options = new FlowPanel();
		options.getElement().addClassName("save-options");

		final RadioButton append = new RadioButton("save-mode", "Append");
		append.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				onModeChanged(Action.APPEND);
			}
		});

		final RadioButton overwrite = new RadioButton("save-mode", "Overwrite");
		overwrite.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				onModeChanged(Action.OVERWRITE);
			}
		});

		final RadioButton insert = new RadioButton("save-mode", "Insert");
		insert.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				onModeChanged(Action.INSERT);
			}
		});

		options.add(append);
		options.add(overwrite);
		options.add(insert);

		switch (lastChoosen) {
		case APPEND:
			append.setValue(true);
			break;
		case DEFAULT:
			append.setValue(true);
			break;
		case INSERT:
			insert.setValue(true);
			break;
		case OVERWRITE:
			overwrite.setValue(true);
			break;
		default:
			break;
		}
		return options;
	}

	private FlowPanel createLabelPanel() {
		FlowPanel label = new FlowPanel();

		nameField = new TextBox();
		nameField.setWidth("20em");
		label.add(nameField);

		nameField.addChangeHandler(new ChangeHandler() {

			@Override
			public void onChange(ChangeEvent event) {
				nameWasChangedByUser = true;
			}
		});
		return label;
	}

	private void setNameText() {
		String name = pickName();
		if (name.isEmpty())
			name = "New preset";
		nameField.setText(name);
	}

	private String pickName() {
		if (nameWasChangedByUser)
			return nameField.getText();

		if (shouldUseOriginalName())
			return originalName;

		return generatedName;
	}

	private void onModeChanged(Action a) {
		lastChoosen = a;
		setNameText();
	}

	private boolean shouldUseOriginalName() {
		ParameterEditor paramEdit = NonMaps.theMaps.getNonLinearWorld().getParameterEditor();
		PresetManager mgr = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		boolean modified = paramEdit.isModified();
		boolean overwrite = lastChoosen == Action.OVERWRITE;
		Preset p = mgr.findLoadedPreset();
		boolean storeInPlace = mgr.findSelectedPreset() == p;

		return !modified || (storeInPlace && overwrite);
	}

	@Override
	protected void onPreviewNativeEvent(NativePreviewEvent event) {
		super.onPreviewNativeEvent(event);
		switch (event.getTypeInt()) {
		case Event.ONKEYDOWN:
			if (event.getNativeEvent().getKeyCode() == KeyCodes.KEY_ESCAPE) {
				hide();
			} else if (event.getNativeEvent().getKeyCode() == KeyCodes.KEY_ENTER) {
				store();
			}
			break;
		}
	}

	protected void store() {
		handler.onActionChoosen(nameField.getText(), infoField.getText(), lastChoosen);
		commit();
	}

	@Override
	protected void commit() {
		AppendOverwriteInsertPresetDialog.this.hide();
		NonMaps.theMaps.captureFocus();
	}

	private void cancel() {
		AppendOverwriteInsertPresetDialog.this.hide();
		NonMaps.theMaps.captureFocus();
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

	public static void close() {
		if (theDialog != null)
			theDialog.cancel();

	}

}