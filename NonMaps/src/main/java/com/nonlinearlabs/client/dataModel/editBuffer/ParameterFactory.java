package com.nonlinearlabs.client.dataModel.editBuffer;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

public class ParameterFactory {
	static private HashSet<Integer> modulateableParameters = new HashSet<Integer>(Arrays.asList(0, 2, 4, 6, 8, 10, 12,
			19, 21, 23, 25, 27, 29, 31, 38, 40, 42, 44, 46, 53, 57, 60, 64, 68, 71, 76, 78, 81, 83, 87, 90, 94, 98, 101,
			106, 108, 111, 113, 115, 119, 123, 127, 129, 133, 136, 138, 140, 144, 148, 150, 153, 156, 158, 160, 162,
			164, 169, 172, 175, 178, 181, 185, 188, 192, 194, 197, 199, 201, 204, 207, 209, 211, 214, 216, 219, 223,
			225, 227, 229, 233, 235, 238, 241, 250, 297, 299, 305, 308, 310));

	static private HashSet<Integer> pedals = new HashSet<Integer>(Arrays.asList(254, 259, 264, 269));
	static private HashSet<Integer> ribbons = new HashSet<Integer>(Arrays.asList(284, 289));

	static private HashSet<Integer> macroControls = new HashSet<Integer>(Arrays.asList(243, 244, 245, 246));
	static private HashSet<Integer> modulationRouters = new HashSet<Integer>(
			Arrays.asList(255, 256, 257, 258, 260, 261, 262, 263, 265, 266, 267, 268, 270, 271, 272, 273, 275, 276, 277,
					278, 280, 281, 282, 283, 285, 286, 287, 288, 290, 291, 292, 293));
	static private HashSet<Integer> scaleOffsetParameters = new HashSet<Integer>(
			Arrays.asList(313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323));

	static public BasicParameterModel create(int id) {
		if (modulateableParameters.contains(id))
			return new ModulateableParameterModel(id);

		if (pedals.contains(id))
			return new PedalParameterModel(id);

		if (ribbons.contains(id))
			return new RibbonParameterModel(id);

		if (id == 274)
			return new BenderParameterModel(id);

		if (id == 279)
			return new AftertouchParameterModel(id);

		if (macroControls.contains(id))
			return new MacroControlParameterModel(id);

		if (modulationRouters.contains(id))
			return new ModulationRouterParameterModel(id);

		if (scaleOffsetParameters.contains(id))
			return new ScaleOffsetParameterModel(id);

		return new BasicParameterModel(id);
	}

	static public Set<Integer> getModulationRouters() {
		return modulationRouters;
	}

	static public Set<Integer> getScaleOffsetParameters() {
		return scaleOffsetParameters;
	}

}
