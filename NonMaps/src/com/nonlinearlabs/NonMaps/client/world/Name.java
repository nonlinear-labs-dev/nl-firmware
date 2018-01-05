package com.nonlinearlabs.NonMaps.client.world;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.ServerProxy;

public class Name {
	private String longName;
	private String shortName;

	public Name(String longName, String shortName) {
		this.setLongName(longName);
		this.setShortName(shortName);
	}

	public Name() {
		this.setLongName("");
		this.setShortName("");
	}

	public String getLongName() {
		return longName;
	}

	public String getShortName() {
		return shortName;
	}

	public void setLongName(String longName) {
		this.longName = longName;
	}

	public void setShortName(String shortName) {
		this.shortName = shortName;
	}

	public void copy(Name name) {
		setLongName(name.getLongName());
		setShortName(name.getShortName());
	}

	public boolean update(Node child) {
		try {
			String nodeName = child.getNodeName();
			String value = ServerProxy.getText(child);

			if (nodeName.equals("short-name")) {
				setShortName(value);
				return true;
			} else if (nodeName.equals("long-name")) {
				setLongName(value);
				return true;
			}
		} catch (Exception e) {

		}
		return false;
	}
}
