package com.nonlinearlabs.NonMaps.client.dataModel;

public class Setup {

	public enum AftertouchCurve {
		soft, normal, hard
	};

	public enum BaseUnitUIMode {
		play, parameter_edit, banks, presets;
	}

	public enum BenderCurve {
		soft, normal, hard
	}

	public enum DebugLevel {
		debug, silent, error, warning, info, gassy
	}

	public enum EditModeRibbonBehaviour {
		relative, absolute
	}

	public enum PedalType {
		pot_tip_active, pot_ring_active, switch_closing, switch_opening
	}

	public enum PresetStoreMode {
		append, overwrite, insert
	}

	public enum VelocityCurve {
		very_soft, soft, normal, hard, very_hard
	}

	public enum BooleanValues {
		on, off
	}

	public enum SelectionAutoScroll {
		off, parameter, preset, parameter_and_preset
	}

	public enum EditParameter {
		always, if_selected, never
	}

	public enum DisplayScaling {
		percent_50, percent_75, percent_100, percent_125, percent_150
	}

	public enum StripeBrightness {
		off, percent_10, percent_25, percent_50
	}

	public interface SettingBase {
		public void fromString(String str);
	}

	public abstract class Setting<T> extends DataModelEntity<T> implements SettingBase {
		public T value;

		public Setting(T def) {
			value = def;
		}

		public void setValue(T v) {
			if (v != value) {
				value = v;
				notifyChanges();
			}
		}

		@Override
		protected T getValue() {
			return value;
		}
	}

	class IntegerSetting extends Setting<Integer> {

		public IntegerSetting() {
			super(0);
		}

		@Override
		public void fromString(String str) {
			setValue(Integer.parseInt(str));
		}
	}

	class StringSetting extends Setting<String> {
		public StringSetting() {
			super("");
		}

		@Override
		public void fromString(String str) {
			setValue(str);
		}
	}

	public class EnumSetting<T extends Enum<T>> extends Setting<T> {
		Class<T> klass;

		public EnumSetting(Class<T> c, T def) {
			super(def);
			klass = c;
		}

		@Override
		public void fromString(String str) {
			str = str.replace("-", "_");
			setValue(Enum.valueOf(klass, str.toLowerCase()));
		}
	}

	public class BooleanSetting extends EnumSetting<BooleanValues> {
		public BooleanSetting() {
			super(BooleanValues.class, BooleanValues.off);
		}
	}

	public class ValueSetting extends Setting<Double> {
		public double defaultValue = 0.0;
		public String scaling = "";
		public int coarseDenominator = 0;
		public int fineDenominator = 0;
		public boolean bipolar = false;

		public ValueSetting() {
			super(0.0);
		}

		@Override
		public void fromString(String str) {
			setValue(Double.parseDouble(str));
		}
	}

	private <T extends Enum<T>> EnumSetting<T> createEnumSettings(Class<T> c, T def) {
		return new EnumSetting<T>(c, def);
	}

	private static Setup theInstance = new Setup();

	public class SystemSettings {
		public EnumSetting<AftertouchCurve> aftertouchCurve = createEnumSettings(AftertouchCurve.class, AftertouchCurve.normal);
		public BooleanSetting autoLoad = new BooleanSetting();
		public EnumSetting<BaseUnitUIMode> baseUnitUIMode = createEnumSettings(BaseUnitUIMode.class, BaseUnitUIMode.parameter_edit);
		public EnumSetting<BenderCurve> benderCurve = createEnumSettings(BenderCurve.class, BenderCurve.normal);
		public IntegerSetting datetimeAdjustment = new IntegerSetting();
		public EnumSetting<DebugLevel> debugLevel = createEnumSettings(DebugLevel.class, DebugLevel.warning);
		public StringSetting deviceName = new StringSetting();
		public ValueSetting editSmoothingTime = new ValueSetting();
		public ValueSetting encoderAcceleration = new ValueSetting();
		public BooleanSetting indicateBlockedUI = new BooleanSetting();
		public BooleanSetting kioskMode = new BooleanSetting();
		public IntegerSetting noteShift = new IntegerSetting();
		public EnumSetting<EditModeRibbonBehaviour> editmodeRibbonBehavior = createEnumSettings(EditModeRibbonBehaviour.class,
				EditModeRibbonBehaviour.absolute);
		public StringSetting passPhrase = new StringSetting();
		public EnumSetting<PedalType> pedal1Type = createEnumSettings(PedalType.class, PedalType.pot_tip_active);
		public EnumSetting<PedalType> pedal2Type = createEnumSettings(PedalType.class, PedalType.pot_tip_active);
		public EnumSetting<PedalType> pedal3Type = createEnumSettings(PedalType.class, PedalType.pot_tip_active);
		public EnumSetting<PedalType> pedal4Type = createEnumSettings(PedalType.class, PedalType.pot_tip_active);
		public BooleanSetting benderOnPressedKeys = new BooleanSetting();
		public BooleanSetting presetDragEnabled = new BooleanSetting();
		public BooleanSetting presetGlitchSuppression = new BooleanSetting();
		public EnumSetting<PresetStoreMode> presetStoreModeSetting = createEnumSettings(PresetStoreMode.class, PresetStoreMode.append);
		public ValueSetting randomizeAmount = new ValueSetting();
		public ValueSetting ribbonRelativeFactor = new ValueSetting();
		public BooleanSetting sendPresetAsLPCFallback = new BooleanSetting();
		public BooleanSetting signalFlowIndication = new BooleanSetting();
		public StringSetting ssid = new StringSetting();
		public ValueSetting transitionTime = new ValueSetting();
		public EnumSetting<VelocityCurve> velocityCurve = createEnumSettings(VelocityCurve.class, VelocityCurve.normal);
	};

	public class LocalSettings {
		public EnumSetting<SelectionAutoScroll> selectionAutoScroll = createEnumSettings(SelectionAutoScroll.class,
				SelectionAutoScroll.parameter_and_preset);
		public EnumSetting<EditParameter> editParameter = createEnumSettings(EditParameter.class, EditParameter.if_selected);
		public BooleanSetting contextMenus = new BooleanSetting();
		public BooleanSetting presetDragDrop = new BooleanSetting();
		public EnumSetting<DisplayScaling> displayScaling = createEnumSettings(DisplayScaling.class, DisplayScaling.percent_100);
		public EnumSetting<StripeBrightness> stripeBrightness = createEnumSettings(StripeBrightness.class, StripeBrightness.percent_25);
		public BooleanSetting bitmapCache = new BooleanSetting();
		public BooleanSetting showDeveloperOptions = new BooleanSetting();
	}

	public SystemSettings systemSettings = new SystemSettings();
	public LocalSettings localSettings = new LocalSettings();

	private Setup() {
	}

	static public Setup get() {
		return theInstance;
	}
}
