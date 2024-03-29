package com.nonlinearlabs.client.presenters;

import java.util.Arrays;
import java.util.Set;
import java.util.TreeSet;

import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.editBuffer.AftertouchParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.BenderParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.MacroControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulationRouterParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterFactory;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.dataModel.editBuffer.PedalParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.PedalParameterModel.Modes;
import com.nonlinearlabs.client.dataModel.editBuffer.PhysicalControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.RibbonParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.RibbonParameterModel.ReturnModes;
import com.nonlinearlabs.client.dataModel.editBuffer.SendParameterModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;
import com.nonlinearlabs.client.presenters.ParameterPresenter.Modulation.ModRange;
import com.nonlinearlabs.client.tools.NLMath;
import com.nonlinearlabs.client.world.Range;
import com.nonlinearlabs.client.world.maps.parameters.PhysicalControlParameter.ReturnMode;

public class ParameterPresenterProvider extends Notifier<ParameterPresenter> {
	private ParameterPresenter presenter = new ParameterPresenter();

	static Set<Integer> handleOnlyParameters = new TreeSet<Integer>(
			Arrays.asList(135, 155, 254, 259, 264, 269, 284, 289, 274, 279, 243, 244, 245, 246));

	public ParameterPresenterProvider(ParameterId parameterId) {

		presenter.id = parameterId;

		BasicParameterModel p = EditBufferModel.get().getParameter(parameterId);
		p.onChange(e -> {
			updatePresenter(e);
			return true;
		});

		if (p instanceof ModulateableParameterModel) {
			var m = (ModulateableParameterModel) p;
			m.modSource.onChange(e -> {
				if (e != ModSource.None) {
					BasicParameterModel mcBPM = EditBufferModel.get().getParameter(new ParameterId(e));

					mcBPM.onChange(mc -> {
						if (mc != mcBPM)
							return false;

						updatePresenter(p);
						return true;
					});
				}
				return true;
			});
		}

		EditBufferModel.get().selectedParameter.onChange(id -> {
			boolean isSelected = presenter.id.getNumber() == id;
			if (isSelected != presenter.selected) {
				presenter.selected = isSelected;
				if (presenter.updateHash()) {
					notifyChanges();
					return true;
				}
			}

			if (presenter.updateHash())
				notifyChanges();

			return true;
		});

		if (parameterId.getNumber() == 356) {
			EditBufferModel.get().voiceGroup.onChange(v -> {
				updatePresenter(p);
				return true;
			});
		}

		EditBufferModel.get().voiceGroup.onChange(vg -> {
			updatePresenter(p);
			return true;
		});

		EditBufferModel.get().soundType.onChange(type -> {
			updatePresenter(p);
			return true;
		});

		SetupModel.get().systemSettings.highlightChangedParameters.onChange(highlight -> {
			updatePresenter(p);
			return true;
		});

		SetupModel.get().systemSettings.forceHighlightChangedParameters.onChange(highlight -> {
			updatePresenter(p);
			return true;
		});
	}

	private boolean isFillFromRightParameter(BasicParameterModel e) {
		if (EditBufferModel.get().voiceGroup.getValue() == VoiceGroup.II) {
			boolean isSplit = e.id.getNumber() == 356;
			boolean isFadeFrom = e.id.getNumber() == 396;
			return isSplit || isFadeFrom;
		}
		return false;
	}

	private boolean isParameterDisabled(BasicParameterModel e) {
		return isParameterDisabled(e.id.getNumber());
	}

	private boolean isParameterHidden(BasicParameterModel e) {
		return isParameterHidden(e.id.getNumber());
	}

	private boolean containsElement(int e, int[] arr) {
		for (int i : arr) {
			if (i == e)
				return true;
		}
		return false;
	}

	private boolean isParameterHidden(int num) {
		SoundType type = EditBufferModel.get().soundType.getValue();
		return containsElement(num, ParameterFactory.hiddenParametersBySoundType.get(type));
	}

	private boolean isLocalDisabled(SendParameterModel m) {
		return !m.enabled.getBool();
	}

	private boolean isParameterDisabled(int num) {
		SoundType type = EditBufferModel.get().soundType.getValue();
		return containsElement(num, ParameterFactory.disabledParametersBySoundType.get(type));
	}

	private void updatePresenter(BasicParameterModel e) {
		presenter.parameterInfo = e.info.getValue();
		presenter.isBoolean = e.value.metaData.isBoolean.getBool();
		presenter.drawCenterReturnIndicator = false;
		presenter.drawZeroReturnIndicator = false;
		presenter.isReturning = false;
		presenter.drawHandleOnly = handleOnlyParameters.contains(e.id.getNumber());
		presenter.highlightChanged = false;
		presenter.modulation.isModulateable = false;
		presenter.modulation.lowerClipping = false;
		presenter.modulation.upperClipping = false;
		presenter.controlPosition = e.value.getQuantizedAndClipped(true);
		presenter.defaultPosition = e.value.getQuantizedAndClipped(e.value.metaData.defaultValue.getValue(), true);
		presenter.bipolar = e.value.metaData.bipolar.getValue() == BooleanValues.on;
		presenter.fillFromRightEnabled = isFillFromRightParameter(e);
		presenter.disabled = isParameterDisabled(e);
		presenter.hidden = isParameterHidden(e);
		presenter.isDefault = presenter.controlPosition == presenter.defaultPosition;
		presenter.isFine = e.value.metaData.coarseDenominator != e.value.metaData.fineDenominator;

		presenter.displayValues = new String[] { e.value.getDecoratedValue(true, true),
				e.value.getDecoratedValue(false, true) };
		presenter.originalParameterValueDecoratedString = e.value.getDecoratedValue(true, e.originalValue.getValue(),
				true);

		presenter.shortName = e.shortName.getValue();
		presenter.longName = e.longName.getValue();
		presenter.locked = e.locked.getBool();

		presenter.valueChanged = isValueChanged(e);

		presenter.showContextMenu = false;
		presenter.isMacroControl = false;
		presenter.relative = false;

		if (e instanceof ModulateableParameterModel)
			updatePresenter((ModulateableParameterModel) e);

		if (e instanceof PhysicalControlParameterModel)
			updatePresenter((PhysicalControlParameterModel) e);

		if (e instanceof SendParameterModel)
			updatePresenter((SendParameterModel) e);

		boolean McMetaChanged = false;

		if (e instanceof MacroControlParameterModel) {
			updatePresenter((MacroControlParameterModel) e);
			McMetaChanged = isMCMetaChanged((MacroControlParameterModel) e);
		}

		if (e instanceof ModulationRouterParameterModel)
			updatePresenter((ModulationRouterParameterModel) e);

		presenter.changed = presenter.valueChanged || presenter.modulation.isModAmountChanged
				|| presenter.modulation.isModSourceChanged || McMetaChanged;

		boolean highlight = SetupModel.get().systemSettings.highlightChangedParameters.isTrue();
		boolean forceHighlight = SetupModel.get().systemSettings.forceHighlightChangedParameters.isTrue();
		presenter.highlightChanged = presenter.changed && (highlight || forceHighlight);

		boolean changed = presenter.changed;
		if (e.group == null) {
			presenter.fullNameWithGroup = "---";
		} else {
			String groupName = e.group.longName.getValue();
			if (e.id.getNumber() == 356) {
				// although the split parameter lives in the global 'Split' group, it is shown
				// in both parts
				groupName = "Part";
			}
			presenter.fullNameWithGroup = groupName + "   \u2013   " + e.longName.getValue() + (changed ? " *" : "");
		}

		if (presenter.updateHash()) {
			notifyChanges();
		}
	}

	static public boolean isValueChanged(BasicParameterModel e) {
		int denominator = e.value.metaData.fineDenominator.getValue();
		long rVal = Math.round(e.value.value.getValue() * denominator);
		long rOgVal = Math.round(e.originalValue.getValue() * denominator);
		return rVal != rOgVal;
	}

	static public boolean isValueChanged(ModulateableParameterModel mod) {
		boolean val = isValueChanged((BasicParameterModel) mod);
		boolean modAmt = mod.ogModAmount.getValue() != mod.modAmount.getValue().value.getValue();
		boolean modSrc = mod.ogModSource.getValue() != mod.modSource.getValue();
		return val || modAmt || modSrc;
	}

	static public boolean isMCMetaChanged(MacroControlParameterModel mc) {
		boolean info = mc.info.getValue() != mc.ogInfo.getValue();
		boolean name = mc.givenName.getValue() != mc.ogGivenName.getValue();
		return info || name;
	}

	private void updatePresenter(ModulationRouterParameterModel p) {
		PhysicalControlParameterModel m = (PhysicalControlParameterModel) EditBufferModel.get()
				.getParameter(p.getAssociatedPhysicalControlID());
		presenter.isBoolean = !m.isReturning();
	}

	private void updatePresenter(SendParameterModel p) {
		presenter.hidden = !isLocalDisabled(p);

		presenter.changed = false;
		presenter.valueChanged = false;

		PhysicalControlParameterModel sibling = p.getSibling();

		if (sibling instanceof RibbonParameterModel) {
			RibbonParameterModel r = (RibbonParameterModel) sibling;
			presenter.drawCenterReturnIndicator = r.mode.getValue() == ReturnModes.return_to_center;
			presenter.drawZeroReturnIndicator = false;
			presenter.isReturning = r.mode.getValue() != ReturnModes.non_return;
		}

		if (sibling instanceof PedalParameterModel) {
			PedalParameterModel r = (PedalParameterModel) sibling;
			presenter.drawCenterReturnIndicator = r.mode.getValue() == Modes.returnToCenter;
			presenter.drawZeroReturnIndicator = r.mode.getValue() == Modes.returnToZero;
			presenter.isReturning = r.mode.getValue() != Modes.stay;
		}

		if (sibling instanceof BenderParameterModel) {
			presenter.drawCenterReturnIndicator = true;
			presenter.drawZeroReturnIndicator = false;
			presenter.isReturning = true;
		}

		if (sibling instanceof AftertouchParameterModel) {
			presenter.drawCenterReturnIndicator = false;
			presenter.drawZeroReturnIndicator = true;
			presenter.isReturning = true;
		}

		presenter.drawCenterReturnIndicator = p.mode.getValue() == ReturnMode.Center;
		presenter.drawZeroReturnIndicator = p.mode.getValue() == ReturnMode.Zero;
		presenter.isReturning = p.mode.getValue() != ReturnMode.None;
	}

	private void updatePresenter(ModulateableParameterModel p) {
		presenter.modulation.isMCPosChanged = false;

		if (p.modSource.getValue() != ModSource.None) {
			BasicParameterModel mcBPM = EditBufferModel.get().getParameter(new ParameterId(p.modSource.getValue()));
			if (mcBPM != null && mcBPM instanceof MacroControlParameterModel) {
				presenter.modulation.isMCPosChanged = isValueChanged(mcBPM);
				presenter.changed = isValueChanged((MacroControlParameterModel) mcBPM);
			}
		}

		int denominator = p.modAmount.metaData.fineDenominator.getValue();
		int ogRounded = (int) (p.ogModAmount.getValue() * denominator);
		int nowRounded = (int) (p.modAmount.value.getValue() * denominator);
		presenter.modulation.isModAmountChanged = ogRounded != nowRounded;
		presenter.modulation.isModSourceChanged = p.ogModSource.getValue() != p.modSource.getValue();
		presenter.modulation.ogModSource = p.ogModSource.getValue();

		presenter.showContextMenu = true;
		presenter.modulation.isModulateable = true;
		presenter.modulation.isModulated = p.modSource.getValue() != ModSource.None;

		if (presenter.modulation.isModulated) {
			var isBipolar = p.value.metaData.bipolar.getBool();

			Range bounds = new Range(isBipolar ? -1.0 : 0, 1.0);
			MacroControlParameterModel mc = (MacroControlParameterModel) EditBufferModel.get()
					.getParameter(new ParameterId(p.modSource.getValue()));

			presenter.modulation.modulationSource = p.modSource.getValue();
			presenter.modulation.modulationSourceLabel = p.modSource.getValue().toString();

			var modRangeWidth = isBipolar ? 2 : 1;

			presenter.modulation.modRange.lower.raw = p.modBase.getValue().value.getValue();
			presenter.modulation.modRange.lower.clipped = bounds.clip(presenter.modulation.modRange.lower.raw);
			presenter.modulation.modRange.lower.clippedQuantized = p.value
					.getQuantizedAndClipped(presenter.modulation.modRange.lower.clipped, true);
			presenter.modulation.modRange.lower.outOfRange = bounds.outOfRange(presenter.modulation.modRange.lower.raw);

			presenter.modulation.modRange.upper.raw = presenter.modulation.modRange.lower.raw
					+ p.modAmount.getValue().value.getValue() * modRangeWidth;
			presenter.modulation.modRange.upper.clipped = bounds.clip(presenter.modulation.modRange.upper.raw);
			presenter.modulation.modRange.upper.clippedQuantized = p.value
					.getQuantizedAndClipped(presenter.modulation.modRange.upper.clipped, true);
			presenter.modulation.modRange.upper.outOfRange = bounds.outOfRange(presenter.modulation.modRange.lower.raw);

			presenter.modulation.modulationAmount = p.modAmount.getQuantizedAndClipped(true);

			String modAmountWithUnit = p.modAmount.getDecoratedValue(true, true);
			String modAmountWithoutUnit = p.modAmount.getDecoratedValue(false, true);
			presenter.modulation.amountDisplayValues = new String[] { "MC Amount: " + modAmountWithUnit,
					"MC Amount: " + modAmountWithoutUnit, "MC Amt: " + modAmountWithoutUnit,
					"Amt: " + modAmountWithoutUnit, modAmountWithoutUnit };

			{
				String clip = presenter.modulation.modRange.lower.outOfRange ? "! " : "";
				String with = p.value.getDecoratedValue(true, presenter.modulation.modRange.lower.clippedQuantized,
						true);
				String without = p.value.getDecoratedValue(false, presenter.modulation.modRange.lower.clippedQuantized,
						true);
				presenter.modulation.amountLowerBoundDisplayValues = new String[] { clip + "Lower Limit: " + with,
						clip + "Lower Limit: " + without, clip + "Lower: " + without, clip + "Lo: " + without,
						clip + without };
			}

			{
				String clip = presenter.modulation.modRange.upper.outOfRange ? "! " : "";
				String with = p.value.getDecoratedValue(true, presenter.modulation.modRange.upper.clippedQuantized,
						true);
				String without = p.value.getDecoratedValue(false, presenter.modulation.modRange.upper.clippedQuantized,
						true);
				presenter.modulation.amountUpperBoundDisplayValues = new String[] { clip + "Upper Limit: " + with,
						clip + "Upper Limit: " + without, clip + "Upper: " + without, clip + "Up: " + without,
						clip + without };
			}

			{
				String with = mc.value.getDecoratedValue(true, true);
				String without = mc.value.getDecoratedValue(false, true);
				presenter.modulation.macroControlDisplayValues = new String[] { "MC Position: " + with,
						"MC Position: " + without, "MC Pos: " + without, "Pos: " + without, without };
			}

			{
				presenter.modulation.originalModulationAmountDecoratedString = p.modAmount.getDecoratedValue(true,
						p.ogModAmount.getValue(), true);

				MacroControlParameterModel macroControl = (MacroControlParameterModel) EditBufferModel.get()
						.getParameter(new ParameterId(p.modSource.getValue()));
				presenter.modulation.originalModulationPositionDecoratedString = macroControl.value
						.getDecoratedValue(true, macroControl.originalValue.getValue(), true);

			}

			updateClipping(p);
		} else {
			presenter.modulation.modulationSourceLabel = "";
			presenter.modulation.modulationSource = ModSource.None;
			presenter.modulation.amountDisplayValues = new String[] { "" };
			presenter.modulation.amountLowerBoundDisplayValues = new String[] { "" };
			presenter.modulation.amountUpperBoundDisplayValues = new String[] { "" };
			presenter.modulation.macroControlDisplayValues = new String[] { "" };
			presenter.modulation.originalModulationAmountDecoratedString = "---";
			presenter.modulation.originalModulationPositionDecoratedString = "---";
			presenter.modulation.modRange = new ModRange(presenter.controlPosition);
		}

		presenter.modulation.modRange.left = presenter.modulation.modRange.lower.raw <= presenter.modulation.modRange.upper.raw
				? presenter.modulation.modRange.lower
				: presenter.modulation.modRange.upper;

		presenter.modulation.modRange.right = presenter.modulation.modRange.lower.raw <= presenter.modulation.modRange.upper.raw
				? presenter.modulation.modRange.upper
				: presenter.modulation.modRange.lower;
	}

	private void updateClipping(ModulateableParameterModel m) {
		if (m.modSource.getValue() != ModSource.None) {
			MacroControlParameterModel mc = (MacroControlParameterModel) EditBufferModel.get()
					.getParameter(new ParameterId(m.modSource.getValue()));
			double modAmount = m.modAmount.getClippedValue();
			boolean bipolar = m.value.metaData.bipolar.getBool();

			if (bipolar)
				modAmount *= 2;

			double srcValue = mc.value.getClippedValue();
			double value = m.value.getClippedValue();

			Range bounds = new Range(bipolar ? -1.0 : 0, 1.0);
			double left = (value - modAmount * srcValue);
			double right = left + modAmount;
			Range mod = new Range(left, right);
			Range modNormalized = new Range(mod.getLeft(), mod.getRight());
			modNormalized.normalize();

			double r = NLMath.quantize(modNormalized.getRight(), 1000);
			double l = NLMath.quantize(modNormalized.getLeft(), 1000);

			presenter.modulation.lowerClipping = bounds.outOfRange(l);
			presenter.modulation.upperClipping = bounds.outOfRange(r);
		}
	}

	private void updatePresenter(PhysicalControlParameterModel p) {
		presenter.changed = false;
		presenter.valueChanged = false;
		presenter.showContextMenu = p.id.getNumber() != 274 && p.id.getNumber() != 279;

		if (p instanceof RibbonParameterModel) {
			RibbonParameterModel r = (RibbonParameterModel) p;
			presenter.drawCenterReturnIndicator = r.mode.getValue() == ReturnModes.return_to_center;
			presenter.drawZeroReturnIndicator = false;
			presenter.isReturning = r.mode.getValue() != ReturnModes.non_return;
			presenter.relative = r.isRelative();
		}

		if (p instanceof PedalParameterModel) {
			PedalParameterModel r = (PedalParameterModel) p;
			presenter.drawCenterReturnIndicator = r.mode.getValue() == Modes.returnToCenter;
			presenter.drawZeroReturnIndicator = r.mode.getValue() == Modes.returnToZero;
			presenter.isReturning = r.mode.getValue() != Modes.stay;
		}

		if (p instanceof BenderParameterModel) {
			presenter.drawCenterReturnIndicator = true;
			presenter.drawZeroReturnIndicator = false;
			presenter.isReturning = true;
		}

		if (p instanceof AftertouchParameterModel) {
			presenter.drawCenterReturnIndicator = false;
			presenter.drawZeroReturnIndicator = true;
			presenter.isReturning = true;
		}
	}

	private void updatePresenter(MacroControlParameterModel p) {
		presenter.userGivenName = p.givenName.getValue();
		presenter.parameterInfo = p.info.getValue();
		presenter.showContextMenu = true;
		presenter.isMacroControl = true;
	}

	@Override
	public ParameterPresenter getValue() {
		return presenter;
	}

}
