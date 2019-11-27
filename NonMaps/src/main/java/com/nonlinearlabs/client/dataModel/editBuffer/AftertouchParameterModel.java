package com.nonlinearlabs.client.dataModel.editBuffer;

public class AftertouchParameterModel extends PhysicalControlParameterModel {

    public AftertouchParameterModel(ParameterId id) {
        super(id);
    }

    @Override
    public boolean isReturning() {
        return true;
    }

}
