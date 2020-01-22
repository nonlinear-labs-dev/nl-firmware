package com.nonlinearlabs.client.dataModel;

import java.util.HashSet;
import java.util.Set;

public class StringSetDataModelEntity extends DataModelEntity<Set<String>> {

	public StringSetDataModelEntity() {
		super(new HashSet<String>());
	}

	@Override
	public void fromString(String str) {
		assert (false);
	}
}
