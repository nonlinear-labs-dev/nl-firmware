package com.nonlinearlabs.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.google.gwt.canvas.dom.client.Context2d.TextBaseline;
import com.nonlinearlabs.client.presenters.MidiSettingsPresenter;
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

        MidiSettingsProvider.get().onChange(e -> {
            update(e);
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

    void update(MidiSettingsPresenter presenter) {
        String oldString = displayString;
        displayString = presenter.getCCStringFor(paramerNumber);
        if(!oldString.equals(displayString))
            invalidate(INVALIDATION_FLAG_UI_CHANGED);
    }
}
