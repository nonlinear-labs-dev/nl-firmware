package com.nonlinearlabs.client.dataModel.editBuffer;

public class BenderParameterModel extends PhysicalControlParameterModel {

    public BenderParameterModel(ParameterId id) {
        super(id);
    }

    @Override
    public boolean isReturning() {
        return true;
    }

}
