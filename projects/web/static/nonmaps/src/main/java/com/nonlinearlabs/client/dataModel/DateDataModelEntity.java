package com.nonlinearlabs.client.dataModel;

import java.util.Date;

import com.google.gwt.i18n.client.DateTimeFormat;

public class DateDataModelEntity extends DataModelEntity<Date> {

	public DateDataModelEntity() {
		super(new Date());
	}

	@Override
	public void fromString(String str) {
		DateTimeFormat format = DateTimeFormat.getFormat("yyyy-MM-ddTHH:mm:ssZZZZ");
		setValue(format.parse(str));
	}
}