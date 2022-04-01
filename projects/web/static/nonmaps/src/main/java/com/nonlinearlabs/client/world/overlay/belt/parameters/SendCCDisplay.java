package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.client.presenters.MidiSettingsPresenter;
import com.nonlinearlabs.client.presenters.MidiSettingsProvider;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class SendCCDisplay extends Label {
    public SendCCDisplay(OverlayLayout parent) {
        super(parent);

        MidiSettingsProvider.get().onChange(presenter -> {
            applyPresenter(presenter);
            return true;
        });

        setFontHeightInMM(5);
        setFontColor(new Gray(155));
    }

    private String displayString = "";

    @Override
    public String getDrawText(Context2d ctx) {
        return displayString;
    }

    @Override
    protected TextAlign getAlignment() {
        return TextAlign.LEFT;
    }

    private void applyPresenter(MidiSettingsPresenter presenter) {
        String oldDisplay = displayString;
        displayString = presenter.selectedParameterCCString;

        if (!oldDisplay.equals(displayString)) {
            invalidate(INVALIDATION_FLAG_UI_CHANGED);
        }
    }

}
