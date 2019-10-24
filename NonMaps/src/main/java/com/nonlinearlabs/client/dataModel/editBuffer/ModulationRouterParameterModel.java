package com.nonlinearlabs.client.dataModel.editBuffer;

public class ModulationRouterParameterModel extends BasicParameterModel {

    public ModulationRouterParameterModel(int id) {
        super(id);
    }

    public int getAssociatedMacroControlID() {
        return (id % 5) + 243;
    }

}
