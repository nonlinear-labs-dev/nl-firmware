package com.nonlinearlabs.client.dataModel.presetManager;

public class Doomable {
	private boolean doomed = false;

	public void setDoomed() {
		doomed = true;
	}

	public void revive() {
		doomed = false;
	}

	public boolean isDoomed() {
		return doomed;
	}
}
