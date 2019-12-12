package com.nonlinearlabs.client.dataModel.editBuffer;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;

public class ParameterId {
    private VoiceGroup group;
    private int number;

    public ParameterId(int number, VoiceGroup vg) {
        this.group = vg;
        this.number = number;
    }

    public ParameterId(ModSource modSrc) {
        this.group = modSrc.toParameterId().group;
        this.number = modSrc.toParameterId().number;
    }

    public ParameterId(String id) {
        String splits[] = id.split("-");
        this.group = VoiceGroup.valueOf(splits[0]);
        this.number = Integer.parseInt(splits[1]);
    }

    @Override
    public String toString() {
        return group.name() + "-" + Integer.toString(number);
    }

    public VoiceGroup getVoiceGroup() {
        return group;
    }

    public int getNumber() {
        return number;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ParameterId) {
            ParameterId other = (ParameterId) obj;
            return other.group == group && other.number == number;
        }
        return false;
    }
}
