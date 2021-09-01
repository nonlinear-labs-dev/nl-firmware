package com.nonlinearlabs.client.world.maps.parameters;

public class UnModulateableParameterName extends LabelLarge {

	public UnModulateableParameterName(Parameter p) {
		super(p);
	}

	@Override
	public Parameter getParent() {
		return (Parameter) super.getParent();
	}

	protected String getDisplayText() {
		return getParent().presenter.shortName;
	}

}
