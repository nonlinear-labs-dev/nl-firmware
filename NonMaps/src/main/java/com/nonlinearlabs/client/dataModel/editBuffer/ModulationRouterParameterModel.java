package com.nonlinearlabs.client.dataModel.editBuffer;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;

public class ModulationRouterParameterModel extends BasicParameterModel {

    public ModulationRouterParameterModel(ParameterId id) {
        super(id);
    }

    public ParameterId getAssociatedMacroControlID() {
        return new ParameterId((id.getNumber() % 5) + 243, id.getVoiceGroup());
    }

    public ParameterId getAssociatedPhysicalControlID() {
        int l = id.getNumber() % 10;
        int ret = (id.getNumber() / 10) * 10 - 1;
        if (l >= 4)
            ret += 5;

        return new ParameterId(ret, VoiceGroup.Global);
    }

}
