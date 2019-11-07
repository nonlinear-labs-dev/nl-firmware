package com.nonlinearlabs.client.useCases;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;

public class ModulateableParameterUseCases {

    public static void setModulationSource(int parameterID, ModSource src) {
        ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.findParameter(parameterID);
        if (p.modSource.setValue(src)) {
            NonMaps.get().getServerProxy().setModulationSource(src);
            p.value.value.setValue(p.value.getQuantizedAndClipped(true));
        }
    }

}
