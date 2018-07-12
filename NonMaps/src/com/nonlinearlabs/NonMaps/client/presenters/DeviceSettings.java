package com.nonlinearlabs.NonMaps.client.presenters;

public class DeviceSettings {
	static public class VelocityCurve {
		static public String[] options = { "Very Soft", "Soft", "Normal", "Hard", "Very Hard" };
		public int selected = 2;
	}

	static public class AftertouchCurve {
		static public String[] options = { "Soft", "Normal", "Hard" };
		public int selected = 1;
	}

	static public class BenderCurve {
		static public String[] options = { "Soft", "Normal", "Hard" };
		public int selected = 1;
	}

	static public class Pedal {
		public String displayValue = "---";
		public double sliderPosition = 0;
		static public String[] options = { "Pot Tip Active", "Pot Ring Active", "Switch Closing", "Switch Opening" };
		public int selected = 0;
	}

	static public class PresetGlitchSurpression {
		static public String[] options = { "On", "Off" };
		public boolean value = false;
	}

	public class EditSmoothingTime {
		public String displayValue = "---";
		public double sliderPosition = 0;
	}

	public VelocityCurve velocityCurve = new VelocityCurve();
	public AftertouchCurve aftertouchCurve = new AftertouchCurve();
	public BenderCurve benderCurve = new BenderCurve();
	public Pedal pedal1 = new Pedal();
	public Pedal pedal2 = new Pedal();
	public Pedal pedal3 = new Pedal();
	public Pedal pedal4 = new Pedal();
	public PresetGlitchSurpression presetGlitchSurpession = new PresetGlitchSurpression();
	public EditSmoothingTime editSmoothingTime = new EditSmoothingTime();
	public String deviceName = "";
}
