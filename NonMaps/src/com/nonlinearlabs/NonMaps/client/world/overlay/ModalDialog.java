package com.nonlinearlabs.NonMaps.client.world.overlay;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.Label;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.tools.CallableVoid;

public class ModalDialog extends GWTDialog {

	protected CallableVoid ok;
	protected CallableVoid cancel;
	
	static int modalPopupLeft = 0;
	static int modalPopupTop = 0;
	
	public ModalDialog(String message, CallableVoid okAction, CallableVoid cancelAction) {
		ok = okAction;
		cancel = cancelAction;
		setModal(true);
		setWidth("20em");
		addHeader("Confirm!");
		
		HTMLPanel panel = new HTMLPanel("");
		HTMLPanel buttons = new HTMLPanel("");
		panel.add(new Label(message, true));
		
		Button okButton, cancelButton;
		
		buttons.add(okButton = new Button("OK", new ClickHandler() {
			
			@Override
			public void onClick(ClickEvent arg0) {
				ok.action();
				commit();
			}
		}));
		buttons.add(cancelButton = new Button("Cancel", new ClickHandler() {
			
			@Override
			public void onClick(ClickEvent arg0) {
				cancel.action();
				commit();
			}
		}));
		
		okButton.getElement().addClassName("modal-button-button");
		cancelButton.getElement().addClassName("modal-button-button");
		buttons.getElement().addClassName("modal-button-div");
		panel.add(buttons);
		add(panel);
	}
	
	@Override
	protected void setLastPopupPos(int popupLeft, int popupTop) {
		modalPopupLeft = popupLeft;
		modalPopupTop = popupTop;
	}

	@Override
	protected int getLastPopupPosTop() {
		return modalPopupTop;
	}

	@Override
	protected int getLastPopupPosLeft() {
		return modalPopupLeft;
	}

	@Override
	protected void commit() {
		hide();
		NonMaps.get().getNonLinearWorld().getViewport().getOverlay().removeModal(this);
		NonMaps.theMaps.captureFocus();
		NonMaps.theMaps.getNonLinearWorld().requestLayout();
	}

}
