package com.nonlinearlabs.client;

import com.google.gwt.dom.client.Style.Unit;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.DialogBox;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.ListBox;

public class ChooseFileVersionDialog extends DialogBox {
	private ListBox list;

	public ChooseFileVersionDialog() {
	}

	public void show(final Runnable runnable) {
		setAnimationEnabled(true);
		setGlassEnabled(true);
		HTMLPanel panel = new HTMLPanel("");
		panel.getElement().addClassName("vertical");
		panel.add(new Label(
				"This file does not contain a file version. Either it has been saved with a buggy C15 software or it is corrupt.\n"
						+ "Please choose which file version the importer should assume and check the resulting presets carefully."));

		HTMLPanel hor = new HTMLPanel("");
		hor.getElement().addClassName("horizontal");
		hor.add(list = new ListBox());

		for (int i = 1; i <= 5; i++) {
			list.addItem("File Version " + i, Integer.toString(i));
		}

		Button b = new Button("Ok");
		hor.add(b);

		b.addClickHandler(new ClickHandler() {

			@Override
			public void onClick(ClickEvent event) {
				runnable.run();
				hide();
			}
		});

		panel.add(hor);
		setWidget(panel);
		getElement().getStyle().setWidth(50, Unit.PCT);
		center();
	}

	public String getChoice() {
		return list.getSelectedValue();
	}
}
