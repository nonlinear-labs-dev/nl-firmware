package com.nonlinearlabs.client.dataModel.editBuffer;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;

public class GroupId {
    private VoiceGroup group;
    private String name;

    public GroupId(String name, VoiceGroup vg) {
        this.group = vg;
        this.name = name;    
    }

    
    public GroupId(String id) {
        String splits[] = id.split("-");
        this.group = VoiceGroup.valueOf(splits[0]);
        this.name = splits[1];
    }

    @Override
    public String toString() {
        return group.name() + "-" + name;
    }

	public VoiceGroup getVoiceGroup() {
		return group;
	}

	public String getName() {
		return name;
    }
    
    @Override
    public boolean equals(Object obj) {
        if(obj instanceof GroupId)
        {
            GroupId other = (GroupId) obj;
            return other.group == group && other.name.equals(name);
        }
        return false;
    }
}
