package com.nonlinearlabs.client.world.maps.presets.bank.preset;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.Label;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.overlay.GWTDialog;

public class ChoosePresetPartDialog extends GWTDialog {

    static int modalPopupLeft = 0;
    static int modalPopupTop = 0;
    Preset preset = null;

    public ChoosePresetPartDialog(Preset p, VoiceGroup target) {
        setModal(true);
        setWidth("20em");
        addHeader("Part of Preset?");

        preset = p;

        HTMLPanel panel = new HTMLPanel("");
        HTMLPanel buttons = new HTMLPanel("");
        if(preset.isDual())
            panel.add(new Label("Which part of the dual preset do you want to load into EditBuffer Part " + target.toString() + "?", true));
        else
            panel.add(new Label("Which of the FX groups do you want to load together with synth voice group into the Editbuffer Part " + target.toString() + "?", true));


        Button part1, part2, cancelButton;
        
        String I = p.getPartName(VoiceGroup.I);
        String II = p.getPartName(VoiceGroup.II);

        String buttonNameI = "";
        String buttonNameII = "";
        if(preset.isDual())
        {
            buttonNameI = "Part I";
            buttonNameII = "Part II";
        }
        else
        {
            buttonNameI = "FX I";
            buttonNameII = "FX II";
        }

        buttons.add(part1 = new Button(buttonNameI, new ClickHandler() {

            @Override
            public void onClick(ClickEvent arg0) {
                EditBufferUseCases.get().loadPresetPartIntoPart(preset.getUUID(), VoiceGroup.I, target);
                commit();
            }
        }));

        buttons.add(part2 = new Button(buttonNameII, new ClickHandler() {

            @Override
            public void onClick(ClickEvent arg0) {
                EditBufferUseCases.get().loadPresetPartIntoPart(preset.getUUID(), VoiceGroup.II, target);
                commit();
            }
        }));

        buttons.add(cancelButton = new Button("Cancel", new ClickHandler() {

            @Override
            public void onClick(ClickEvent arg0) {
                commit();
            }
        }));

        part1.getElement().addClassName("modal-button-button");
        part2.getElement().addClassName("modal-button-button");
        cancelButton.getElement().addClassName("modal-button-button");
        buttons.getElement().addClassName("modal-button-div");
        panel.add(buttons);
        add(panel);


        setPopupPosition(Window.getClientWidth() / 2 - panel.getOffsetWidth(), Window.getClientHeight() / 2 - panel.getOffsetHeight());
        show();
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
        NonMaps.theMaps.captureFocus();
        NonMaps.theMaps.getNonLinearWorld().requestLayout();

        modalPopupLeft = getAbsoluteLeft();
        modalPopupTop = getAbsoluteTop();
    }

}
