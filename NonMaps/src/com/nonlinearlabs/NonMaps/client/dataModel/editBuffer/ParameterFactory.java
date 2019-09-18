package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

import java.util.Arrays;
import java.util.HashSet;

public class ParameterFactory {
	private HashSet<Integer> modulateableParameters = new HashSet<Integer>(Arrays.asList(0, 2, 4, 6, 8, 10, 12, 19, 21,
			23, 25, 27, 29, 31, 38, 40, 42, 44, 46, 53, 57, 60, 64, 68, 71, 76, 78, 81, 83, 87, 90, 94, 98, 101, 106,
			108, 111, 113, 115, 119, 123, 127, 129, 133, 136, 138, 140, 144, 148, 150, 153, 156, 158, 160, 162, 164,
			169, 172, 175, 178, 181, 185, 188, 192, 194, 197, 199, 201, 204, 207, 209, 211, 214, 216, 219, 223, 225,
			227, 229, 233, 235, 238, 241, 250, 297, 299, 305, 308, 310));

	private HashSet<Integer> pedals = new HashSet<Integer>(Arrays.asList(254, 259, 264, 269));
	private HashSet<Integer> ribbons = new HashSet<Integer>(Arrays.asList(284, 289));
	private HashSet<Integer> benderAndAftertouch = new HashSet<Integer>(Arrays.asList(274, 279));
	private HashSet<Integer> macroControls = new HashSet<Integer>(Arrays.asList(243, 244, 245, 246));

	ParameterFactory() {

	}

	public BasicParameterModel create(int id) {
		if (modulateableParameters.contains(id))
			return new ModulateableParameter();

		if (pedals.contains(id))
			return new PedalParameter();

		if (ribbons.contains(id))
			return new RibbonParameter();

		if (benderAndAftertouch.contains(id))
			return new PhysicalControlParameter();

		if (macroControls.contains(id))
			return new MacroControlParameter();

		return new BasicParameterModel();
	}
}
