package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class FineButton extends SVGImage {
    private boolean fineStatus = false;
    private boolean isEnabled = true;
    private boolean isShiftPressed = false;

    public FineButton(Control parent)
    {
        super(parent, "Fine_Active_B.svg", "Fine_Disabled_B.svg", "Fine_Enabled_B.svg");
        
        NonMaps.get().getNonLinearWorld().onShiftStateChanged(state -> {
            isShiftPressed = state == BooleanValues.on;
            invalidate(INVALIDATION_FLAG_UI_CHANGED);
            return true;
        });
        
        SetupModel.get().localSettings.localFineEnabled.onChange(state -> {
            fineStatus = state == BooleanValues.on;
            invalidate(INVALIDATION_FLAG_UI_CHANGED);
            return true;
        });

        EditBufferModel.get().selectedParameter.onChange(i -> {
            BasicParameterModel bpm = EditBufferModel.get().getSelectedParameter();
            if(bpm != null)
            {
                isEnabled = bpm.value.metaData.fineDenominator.getValue() != bpm.value.metaData.coarseDenominator.getValue();

                if(!isEnabled && SetupModel.get().localSettings.localFineEnabled.getBool())
                {
                    SetupModel.get().localSettings.localFineEnabled.setValue(false);
                }

                setVisible(isEnabled);
                invalidate(INVALIDATION_FLAG_UI_CHANGED);
            }
            return true;
        });
    }

    @Override
    public Control click(Position mousePos) {
        if(isEnabled)
        {
            SetupModel.get().localSettings.localFineEnabled.toggle();
            return this;
        }
        else
        {
            return this;
        }
    }

    @Override 
    public Control doubleClick(Position pos) {
        return this;
    }

    @Override
    public int getSelectedPhase()
    {
        if(!isEnabled)
            return 1;
        return (fineStatus || isShiftPressed) ? 0 : 2;
    }
}
