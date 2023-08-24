package com.nonlinearlabs.client.dataModel;

public class LongDataModelEntity extends DataModelEntity<Long> {

    public LongDataModelEntity() {
        super(0l);
    }

    @Override
    public void fromString(String str) {
        setValue(Long.parseLong(str));
    }
    
}
