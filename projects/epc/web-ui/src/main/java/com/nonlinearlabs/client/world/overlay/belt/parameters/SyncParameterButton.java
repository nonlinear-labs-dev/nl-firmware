package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public final class SyncParameterButton extends SVGImage {
    
    public SyncParameterButton(OverlayLayout parent) {
		super(parent, "Sync_Disabled.svg", "Sync_Enabled.svg");
        SetupModel.get().systemSettings.syncSplit.onChange(b -> {
            invalidate(INVALIDATION_FLAG_UI_CHANGED);
            return true;
        });
    }

	@Override
	public int getSelectedPhase() {
		return SetupModel.get().systemSettings.syncSplit.getBool() ? 1 : 0;
	}

	@Override
	public Control click(Position eventPoint) {
		EditBufferUseCases.get().toggleSyncSplit();
		return this;
    }
    
    @Override
    public Control doubleClick(Position p) {
        return this;
    }
}