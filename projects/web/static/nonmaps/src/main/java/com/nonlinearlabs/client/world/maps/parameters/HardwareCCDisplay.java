package com.nonlinearlabs.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.google.gwt.canvas.dom.client.Context2d.TextBaseline;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.AftertouchCCMapping;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BenderCCMapping;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.PedalCCMapping;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.RibbonCCMapping;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.SystemSettings;
import com.nonlinearlabs.client.presenters.MidiSettings;
import com.nonlinearlabs.client.presenters.MidiSettingsProvider;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class HardwareCCDisplay extends ZoomReactingControl {

    private final int paramerNumber;
    private boolean isVisibleZoomlevel = false;
    private String displayString = new String("CC");

    public HardwareCCDisplay(MapsLayout parent, int parameterId)
    {
        super(parent);
        paramerNumber = parameterId;

        MidiSettingsProvider.get().register(e -> {
            update();
            return true;
        });
    }

    @Override
    protected double getBasicWidth() {
        return 50;
    }

    @Override
    protected double getBasicHeight() {
        return 15;
    }

    @Override
    protected double getMinHeight() {
        return 15;
    }

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return 5;
	}

    String getDisplayString()
    {
        return displayString;
    }

    @Override
	public void doFirstLayoutPass(double levelOfDetail) {
		super.doFirstLayoutPass(levelOfDetail);
		isVisibleZoomlevel = levelOfDetail >= getLevelOfDetailForFullVisibility();
	}

    public Parameter getParentParameterLayout() {
		Control parent = getParent();

		while (parent != null) {
			if (parent instanceof Parameter) {
				return (Parameter) parent;
			}

			parent = parent.getParent();
		}

		return null;
	}

	private RGB getFontColor() {
		if (getParentParameterLayout().isSelected())
			return getColorFontValueDisplaySelected();
		return getColorFontValueDisplay();
	}

    @Override
    public void draw(Context2d ctx, int invalidationMask) {

        int margin = 2;
        if (isVisibleZoomlevel) {
            Rect pixRect = getPixRect();

            double fontHeightInUnits = 8;
			double fontHeightInPixels = toYPixels(fontHeightInUnits);

			ctx.setTextAlign(TextAlign.LEFT);
			ctx.setFillStyle(getFontColor().toString());
			ctx.setTextBaseline(TextBaseline.BOTTOM);

			ctx.setFont(fontHeightInPixels + "px 'SSP-LW25'");
			ctx.fillText(displayString, pixRect.getLeft() + margin, pixRect.getBottom() + margin);
        }
    }

    static public String getPedalString(PedalCCMapping setting) {
        SystemSettings settings = SetupModel.get().systemSettings;
        boolean is14BitSupport = settings.enable14BitSupport.getBool();
        int idx = setting.ordinal();
        if(is14BitSupport) {
            return MidiSettings.PedalMapping.options[idx];
        } else {
            return MidiSettings.PedalMapping.optionsWithoutLSB[idx];
        }
    }
    
    static public String getRibbonString(RibbonCCMapping setting) {
        SystemSettings settings = SetupModel.get().systemSettings;
        boolean is14BitSupport = settings.enable14BitSupport.getBool();
        int idx = setting.ordinal();
        if(is14BitSupport) {
            return MidiSettings.RibbonMapping.options[idx];
        } else {
            return MidiSettings.RibbonMapping.optionsWithoutLSB[idx];
        }
    }

    static public String getBenderString(BenderCCMapping setting) {
        SystemSettings settings = SetupModel.get().systemSettings;
        boolean is14BitSupport = settings.enable14BitSupport.getBool();
        int idx = setting.ordinal();
        if(is14BitSupport) {
            return MidiSettings.BenderMapping.options[idx];
        } else {
            return MidiSettings.BenderMapping.optionsWithoutLSB[idx];
        }
    }

    static public String getAftertouchString(AftertouchCCMapping setting) {
        SystemSettings settings = SetupModel.get().systemSettings;
        boolean is14BitSupport = settings.enable14BitSupport.getBool();
        int idx = setting.ordinal();
        if(is14BitSupport) {
            return MidiSettings.AftertouchMapping.options[idx];
        } else {
            return MidiSettings.AftertouchMapping.optionsWithoutLSB[idx];
        }
    }

    void update() {
        SystemSettings settings = SetupModel.get().systemSettings;

        String oldString = displayString;

        switch(paramerNumber) {
            case 398:
            case 254:
                displayString = getPedalString(settings.pedal1Mapping.getValue());
                break;
            case 399:
            case 259:
                displayString = getPedalString(settings.pedal2Mapping.getValue());
                break;
            case 400:
            case 264:
                displayString = getPedalString(settings.pedal3Mapping.getValue());
                break;
            case 401:
            case 269:
                displayString = getPedalString(settings.pedal4Mapping.getValue());
                break;
            case 402:
            case 274:
                displayString = getBenderString(settings.benderMapping.getValue());
                break;
            case 403:
            case 279:
                displayString = getAftertouchString(settings.aftertouchMapping.getValue());
                break;
            case 404:
            case 284:
                displayString = getRibbonString(settings.ribbon1Mapping.getValue());
                break;
            case 405:
            case 289:
                displayString = getRibbonString(settings.ribbon2Mapping.getValue());
                break;
        }

        if(displayString == "None")
            displayString = "";

        if(!oldString.equals(displayString))
            invalidate(INVALIDATION_FLAG_UI_CHANGED);
    }
}
