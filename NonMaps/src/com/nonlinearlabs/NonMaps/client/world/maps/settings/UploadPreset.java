package com.nonlinearlabs.NonMaps.client.world.maps.settings;

import com.google.gwt.dom.client.NativeEvent;
import com.google.gwt.event.dom.client.ChangeEvent;
import com.google.gwt.event.dom.client.ChangeHandler;
import com.google.gwt.user.client.ui.FileUpload;
import com.google.gwt.user.client.ui.RootPanel;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.maps.NonPosition;

public class UploadPreset extends Setting {

	protected UploadPreset(DeveloperSettings parent) {
		super(parent, "Upload Reaktor Preset", "Go!");
	}

	@Override
	public Control onValueButtonMouseDown(Position pos) {
		final FileUpload upload = new FileUpload();
		upload.setName("uploadFormElement");
		upload.addChangeHandler(new ChangeHandler() {

			@Override
			public void onChange(ChangeEvent event) {
				loadFile(event.getNativeEvent(), new TextUploadedHandler() {

					@Override
					public void onTextUploaded(String text) {
						NonMaps.theMaps.getServerProxy().uploadReaktorPreset(text);
					}
				});

				RootPanel.get().remove(upload);
			}
		});

		upload.click();
		RootPanel.get().add(upload);
		return this;
	}

	@Override
	public ContextMenu createMenu(NonPosition pos) {
		return null;
	}

	private interface TextUploadedHandler {
		void onTextUploaded(String text);
	}

	private native void loadFile(NativeEvent evt, TextUploadedHandler receiver) /*-{
		var files = evt.target.files;
		if (files != null && files.length > 0) {
			var file = files[0];
			var reader = new FileReader();
			reader.onload = function(e) {
				var text = reader.result;
				receiver.@com.nonlinearlabs.NonMaps.client.world.maps.settings.UploadPreset.TextUploadedHandler::onTextUploaded(Ljava/lang/String;)(text);
			}
			reader.readAsText(file);
		}
	}-*/;

	@Override
	public void setDefault() {
		// TODO Auto-generated method stub

	}
}
