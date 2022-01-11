package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.SystemSettings;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.maps.parameters.HardwareCCDisplay;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class SendCCDisplay extends Label {
    public SendCCDisplay(OverlayLayout parent) {
        super(parent);
        EditBufferModel.get().selectedParameter.onChange(sel -> {
            onParameterSelectionChanged(sel);
            return true;
        });

        SetupModel.get().systemSettings.benderMapping.onChange(e -> {
            mappingUpdated();
            return true;
        });

        SetupModel.get().systemSettings.aftertouchMapping.onChange(e -> {
            mappingUpdated();
            return true;
        });

        SetupModel.get().systemSettings.ribbon1Mapping.onChange(e -> {
            mappingUpdated();
            return true;
        });

        SetupModel.get().systemSettings.ribbon2Mapping.onChange(e -> {
            mappingUpdated();
            return true;
        });

        SetupModel.get().systemSettings.pedal1Mapping.onChange(e -> {
            mappingUpdated();
            return true;
        });

        SetupModel.get().systemSettings.pedal2Mapping.onChange(e -> {
            mappingUpdated();
            return true;
        });

        SetupModel.get().systemSettings.pedal3Mapping.onChange(e -> {
            mappingUpdated();
            return true;
        });

        SetupModel.get().systemSettings.pedal4Mapping.onChange(e -> {
            mappingUpdated();
            return true;
        });

        setFontHeightInMM(5);
        setFontColor(new Gray(155));
    }

    private String displayString = "";
    private int selectedParameter = 0;

    @Override
    public String getDrawText(Context2d ctx) {
        return displayString;
    }

    @Override
    protected TextAlign getAlignment() {
        return TextAlign.LEFT;
    }

    private void onParameterSelectionChanged(int i) {
        if (selectedParameter != i) {
            selectedParameter = i;
            mappingUpdated();
        }
    }

    private void mappingUpdated() {
        SystemSettings settings = SetupModel.get().systemSettings;

        switch (selectedParameter) {
            case 398:
            case 254:
                displayString = HardwareCCDisplay.getPedalString(settings.pedal1Mapping.getValue());
                break;
            case 399:
            case 259:
                displayString = HardwareCCDisplay.getPedalString(settings.pedal2Mapping.getValue());
                break;
            case 400:
            case 264:
                displayString = HardwareCCDisplay.getPedalString(settings.pedal3Mapping.getValue());
                break;
            case 401:
            case 269:
                displayString = HardwareCCDisplay.getPedalString(settings.pedal4Mapping.getValue());
                break;
            case 402:
            case 274:
                displayString = HardwareCCDisplay.getBenderString(settings.benderMapping.getValue());
                break;
            case 403:
            case 279:
                displayString = HardwareCCDisplay.getAftertouchString(settings.aftertouchMapping.getValue());
                break;
            case 404:
            case 284:
                displayString = HardwareCCDisplay.getRibbonString(settings.ribbon1Mapping.getValue());
                break;
            case 405:
            case 289:
                displayString = HardwareCCDisplay.getRibbonString(settings.ribbon2Mapping.getValue());
                break;
            default:
                displayString = "";
                break;
        }

        if (displayString == "None")
            displayString = "";

        invalidate(INVALIDATION_FLAG_UI_CHANGED);
    }
}
