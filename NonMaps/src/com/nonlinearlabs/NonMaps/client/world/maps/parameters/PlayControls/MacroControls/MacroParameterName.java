package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.Name;

public class MacroParameterName extends Name {

	private String overriddenName;

	public MacroParameterName(MacroControlParameter macroControlParameter) {
	}

	public void overrideName(String name) {
		overriddenName = name;

		if (name.isEmpty())
			overriddenName = null;
	}

	public void resetOverriddenName() {
		overriddenName = null;
	}

	public String getEditName() {
		if (overriddenName != null && !overriddenName.isEmpty())
			return overriddenName;

		return "";
	}

	@Override
	public boolean update(Node child) {
		if (super.update(child))
			return true;

		String nodeName = child.getNodeName();

		if (nodeName.equals("givenName")) {
			try {
				String g = ServerProxy.getText(child);

				if (!g.equals(overriddenName)) {
					overrideName(g);

				}
			} catch (Exception e) {
				resetOverriddenName();
			}
			return true;
		}
		return false;
	}
}
