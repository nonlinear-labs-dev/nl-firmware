package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.google.gwt.dom.client.NativeEvent;
import com.google.gwt.event.dom.client.ChangeEvent;
import com.google.gwt.event.dom.client.ChangeHandler;
import com.google.gwt.user.client.ui.FileUpload;
import com.google.gwt.user.client.ui.RootPanel;

public class TextUpload {

	public interface TextUploadedHandler {
		void onTextUploaded(String fileName, String text);
	}

	public TextUpload(final TextUploadedHandler handler, String acceptedFiles) {
		final FileUpload upload = new FileUpload();

		upload.setName("uploadFormElement");
		upload.getElement().setAttribute("accept", acceptedFiles);
		upload.addChangeHandler(new ChangeHandler() {

			@Override
			public void onChange(ChangeEvent event) {
				loadFile(event.getNativeEvent(), handler);
				RootPanel.get().remove(upload);
			}
		});

		upload.click();
		RootPanel.get().add(upload);
	}

	private native void loadFile(NativeEvent evt, TextUploadedHandler receiver) /*-{
																				var files = evt.target.files;
																				if (files != null && files.length > 0) {
																				var file = files[0];
																				var reader = new FileReader();
																				reader.onload = function(e) {
																				var text = reader.result;
																				receiver.@com.nonlinearlabs.client.world.overlay.belt.presets.TextUpload.TextUploadedHandler::onTextUploaded(Ljava/lang/String;Ljava/lang/String;)(file.name, text);
																				}
																				reader.readAsText(file);
																				}
																				}-*/;

}
