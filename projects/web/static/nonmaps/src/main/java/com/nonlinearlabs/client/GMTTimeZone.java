package com.nonlinearlabs.client;

import java.util.Date;

import com.google.gwt.i18n.shared.TimeZone;

public class GMTTimeZone implements TimeZone {
	@Override
	public int getDaylightAdjustment(Date date) {
		return 0;
	}

	@Override
	public String getGMTString(Date date) {
		return null;
	}

	@Override
	public String getID() {
		return null;
	}

	@Override
	public String getISOTimeZoneString(Date date) {
		return null;
	}

	@Override
	public String getLongName(Date date) {
		return null;
	}

	@Override
	public int getOffset(Date date) {
		return 0;
	}

	@Override
	public String getRFCTimeZoneString(Date date) {
		return null;
	}

	@Override
	public String getShortName(Date date) {
		return null;
	}

	@Override
	public int getStandardOffset() {
		return 0;
	}

	@Override
	public boolean isDaylightTime(Date date) {
		return false;
	}
}
