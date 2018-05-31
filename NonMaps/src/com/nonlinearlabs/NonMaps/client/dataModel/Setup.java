package com.nonlinearlabs.NonMaps.client.dataModel;


public class Setup {

	class Setting<T> extends DataModelEntity<T> {
		T value;
	}

	class EnumSetting extends Setting<String> {
		public EnumSetting(String... choices) {
		}
	}

	public class ValueSetting extends Setting<Double> {
		public double defaultValue = 0.0;
		public String scaling = "";
		public int coarseDenominator = 0;
		public int fineDenominator = 0;
	}

	private static Setup theInstance = new Setup();

	public Setting aftertouchCurve = new EnumSetting();
	public Setting autoLoad = new Setting<Boolean>();
	public Setting baseUnitUIMode = new EnumSetting();
	public Setting datetimeAdjustment = new Setting<Integer>();
	public Setting debugLevel = new EnumSetting();
	public Setting deviceName = new Setting<String>();
	public Setting editSmoothingTime = new ValueSetting();
	public Setting encoderAcceleration = new ValueSetting();
	public Setting indicateBlockedUI = new Setting<Boolean>();
	public Setting kioskMode = new Setting<Boolean>();
	public Setting noteShift = new Setting<Integer>();
	public Setting parameterEditmodeRibbonBehavior = new EnumSetting();
	public Setting passPhrase = new Setting<String>();
	public Setting pedal1Type = new EnumSetting();
	public Setting pedal2Type = new EnumSetting();
	public Setting pedal3Type = new EnumSetting();
	public Setting pedal4Type = new EnumSetting();
	public Setting benderOnPressedKeys = new Setting<Boolean>();
	public Setting presetDragEnabled = new Setting<Boolean>();
	public Setting presetGlitchSuppression = new Setting<Boolean>();
	public Setting presetStoreModeSetting = new EnumSetting();
	public Setting randomizeAmount = new ValueSetting();
	public Setting ribbonRelativeFactor = new ValueSetting();
	public Setting sendPresetAsLPCFallback = new Setting<Boolean>();
	public Setting signalFlowIndication = new Setting<Boolean>();
	public Setting ssid = new Setting<String>();
	public Setting transitionTime = new ValueSetting();
	public Setting velocityCurve = new EnumSetting();

	private Setup() {
	}

	static public Setup get() {
		return theInstance;
	}
}
