package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Scale;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Name;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

abstract public class ScaleParameter extends ModulationSourceHighPriority {

	public ScaleParameter(MapsLayout parent) {
		super(parent);
		addChild(new LabelModulationSource(this, getName()));
		addChild(new SliderHorizontal(this));
		addChild(new ValueDisplaySmall(this));
	}

	public double getDefaultValue() {
		return 0.0;
	}

	private String stringsize(int key) {
		switch (key)
		{
			case 0:
				return "C";
			case 1:
				return "C#";
			case 2:
				return "D";
			case 3:
				return "D#";
			case 4:
				return "E";
			case 5:
				return "F";
			case 6:
				return "F#";
			case 7:
				return "G";
			case 8:
				return "G#";
			case 9:
				return "A";
			case 10:
				return "A#";
			case 11:
				return "B";
		}
		return "";
	}

	@Override
	public Name getName() {
		ScaleParameter baseKey = null;
		for(Control c: getParameterGroup().getChildren()) {
			if(c instanceof ScaleCol1)
			{
				for(Control param: ((ScaleCol1) c).getChildren()) {
					if(param instanceof ScaleParameter) {
						baseKey = (ScaleParameter) param;
						break;
					}
				}
			}
		}
		if(baseKey != null) {
			int offset = getParameterID() - baseKey.getParameterID();
			int key = (int)baseKey.getValue().getQuantizedClippedValue(false) + offset;
			String stringizedKey = stringsize(key % 12);
			return new Name(super.getName().getLongName(), "("+stringizedKey+")");
		}
		return super.getName();
	}

}