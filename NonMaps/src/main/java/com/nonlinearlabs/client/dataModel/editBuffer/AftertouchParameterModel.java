package com.nonlinearlabs.client.dataModel.editBuffer;

public class AftertouchParameterModel extends PhysicalControlParameterModel {

    public AftertouchParameterModel(int id) {
        super(id);
    }

    @Override
    public boolean isReturning() {
        return true;
    }

}
