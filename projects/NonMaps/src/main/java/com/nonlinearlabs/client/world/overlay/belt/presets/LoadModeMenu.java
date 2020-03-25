package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class LoadModeMenu extends OverlayLayout {

    private class DirectLoadButton extends SVGImage {
        DirectLoadButton(OverlayLayout parent) {
            super(parent, "Link_Enabled.svg", "Link_Active.svg");

            SetupModel.get().systemSettings.directLoad.onChange((v) -> {
                requestLayout();
                return true;
            });
        }

        private boolean isDirectLoadActive() {
            return SetupModel.get().systemSettings.directLoad.getBool();
        }

        @Override
        public int getSelectedPhase() {
            return isDirectLoadActive() ? 1 : 0;
        }
        
        @Override
        public Control click(Position eventPoint) {
            EditBufferUseCases.get().toggleDirectLoad();
            return this;
        }
    }

    private class LoadToPartButton extends SVGImage {
        LoadToPartButton(OverlayLayout parent) {
            super(parent, "Load_Disabled.svg", "Load_Enabled.svg", "Load_Active.svg");
        }

        @Override
        public int getSelectedPhase() {
            if(!isDualEditBuffer()) {
                return 0;
            } else {
                if(isLoadToPartActive()) {
                    return 2;
                } else {
                    return 1;
                }
            }
        }

        private boolean isDualEditBuffer() {
            return EditBufferPresenterProvider.getPresenter().soundType != SoundType.Single;
        }

        private boolean isLoadToPartActive() {
            return NonMaps.get().getNonLinearWorld().getPresetManager().isInLoadToPartMode();
        }

        @Override
        public Control click(Position eventPosition) {
            if(isDualEditBuffer()) {
                if(isLoadToPartActive()) {
                    NonMaps.get().getNonLinearWorld().getPresetManager().endLoadToPartMode();
                } else {
                    NonMaps.get().getNonLinearWorld().getPresetManager().startLoadToPartMode();
                }
                return this;
            } else {
                return null;
            }
        }
    }

    private DirectLoadButton loadButton;
    private LoadToPartButton partButton;

    LoadModeMenu(OverlayLayout parent) {
        super(parent);

        SetupModel.get().systemSettings.directLoad.onChange((d) -> {
            requestLayout();
            return true;
        });

        EditBufferModel.get().voiceGroup.onChange((vg) -> {
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
			return true;
		});

        addChild(loadButton = new DirectLoadButton(this));
        addChild(partButton = new LoadToPartButton(this));
    }

    @Override
    public void doLayout(double x, double y, double w, double h) {
        double width = getSmallButtonWidth();
        double margin = Millimeter.toPixels(1);

        double yPos = (h / 2) - (width / 2);

        loadButton.doLayout(0, yPos, width, width);
        partButton.doLayout(width + margin, yPos, width, width);

        super.doLayout(x, y, w, h);
    }

    public double getDesiredWidth() {
        return getSmallButtonWidth() + Millimeter.toPixels(1) + getSmallButtonWidth();
    }

    public double getSmallButtonWidth() {
        return Millimeter.toPixels(11);
    }
}
