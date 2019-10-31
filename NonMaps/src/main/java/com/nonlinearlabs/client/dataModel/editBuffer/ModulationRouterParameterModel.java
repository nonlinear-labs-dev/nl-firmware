package com.nonlinearlabs.client.dataModel.editBuffer;

public class ModulationRouterParameterModel extends BasicParameterModel {

    public ModulationRouterParameterModel(int id) {
        super(id);
    }

    public int getAssociatedMacroControlID() {
        return (id % 5) + 243;
    }

    public int getAssociatedPhysicalControlID() {
        int l = id % 10;
        int ret = (id / 10) * 10 - 1;
        if (l >= 4)
            ret += 5;
        return ret;
    }

}
