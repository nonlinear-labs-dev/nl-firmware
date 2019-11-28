package com.nonlinearlabs.client.dataModel.editBuffer;

public class ModulationRouterParameterModel extends BasicParameterModel {

    public ModulationRouterParameterModel(ParameterId id) {
        super(id);
    }

    public ParameterId getAssociatedMacroControlID() {
        return ParameterFactory.getMacroControlForRouter(id);
    }

    public ParameterId getAssociatedPhysicalControlID() {
        return ParameterFactory.getPhysicalControlForRouter(id);
    }

}
