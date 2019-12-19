package com.nonlinearlabs.client.world.maps.presets.bank.preset;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.Label;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.overlay.GWTDialog;

public class ChoosePresetPartDialog extends GWTDialog {

    static int modalPopupLeft = 0;
    static int modalPopupTop = 0;

    public ChoosePresetPartDialog(Preset p) {
        setModal(true);
        setWidth("20em");
        addHeader("Part of Dual Preset?");

        HTMLPanel panel = new HTMLPanel("");
        HTMLPanel buttons = new HTMLPanel("");
        panel.add(new Label("Which part of the dual preset do you want to load?", true));

        Button okButton, cancelButton;

        buttons.add(okButton = new Button("Part I", new ClickHandler() {

            @Override
            public void onClick(ClickEvent arg0) {
                commit();
            }
        }));
        buttons.add(cancelButton = new Button("Part II", new ClickHandler() {

            @Override
            public void onClick(ClickEvent arg0) {
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
