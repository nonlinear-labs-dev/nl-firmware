package com.nonlinearlabs.client.presenters;

import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.MacroControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulationRouterParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.PhysicalControlParameterModel;
import com.nonlinearlabs.client.dataModel.setup.Setup;
import com.nonlinearlabs.client.dataModel.setup.Setup.BooleanValues;
import com.nonlinearlabs.client.tools.NLMath;
import com.nonlinearlabs.client.world.Range;

public class ParameterPresenterProvider extends Notifier<ParameterPresenter> {
	private ParameterPresenter presenter = new ParameterPresenter();

	public ParameterPresenterProvider(int parameterId) {
		presenter.id = parameterId;

		BasicParameterModel p = EditBufferModel.findParameter(parameterId);
		p.onChange(e -> updatePresenter(e));

		EditBufferModel.selectedParameter.onChange(id -> {
			boolean isSelected = parameterId == id;
			if (isSelected != presenter.selected) {
				presenter.selected = isSelected;
				presenter.updateHash();
				notifyChanges();
			}
			return true;
		});
	}

	private boolean updatePresenter(BasicParameterModel e) {
		presenter.highlightChanged = false;
		presenter.modulation.isModulateable = false;
		presenter.controlPosition = e.value.value.getValue();
		presenter.bipolar = e.value.metaData.bipolar.getValue() == BooleanValues.on;

		presenter.displayValues = new String[] { e.value.getDecoratedValue(true, true),
				e.value.getDecoratedValue(false, true) };

		presenter.shortName = e.shortName.getValue();
		presenter.longName = e.longName.getValue();
		presenter.changed = e.isChanged();

		presenter.fullNameWithGroup = getFullNameWithGroup(e);
		presenter.showContextMenu = false;

		if (e instanceof ModulateableParameterModel)
			updatePresenter((ModulateableParameterModel) e);

		if (e instanceof PhysicalControlParameterModel)
			updatePresenter((PhysicalControlParameterModel) e);

		if (e instanceof MacroControlParameterModel)
			updatePresenter((MacroControlParameterModel) e);

		if (presenter.updateHash())
			notifyChanges();

		return true;
	}

	private void updatePresenter(ModulateableParameterModel p) {
		presenter.showContextMenu = true;
		presenter.modulation.isModulateable = true;

		presenter.modulation.isModulated = p.modSource.getValue() != ModSource.None;

		if (presenter.modulation.isModulated) {
			presenter.modulation.modulationSourceLabel = toString(p.modSource.getValue());

			double modAmount = p.modAmount.getQuantizedAndClipped(true);
			double value = p.value.getQuantized(true);
			boolean isBiPolar = p.value.metaData.bipolar.getBool();

			double modLeft = 0;
			double modRight = 0;

			if (isBiPolar)
				modAmount *= 2;

			MacroControlParameterModel mc = EditBufferModel.findParameter(p.modSource.getValue());

			modLeft = value - modAmount * mc.value.getQuantizedAndClipped(true);
			modRight = modLeft + modAmount;

			if (modRight < modLeft) {
				double tmp = modLeft;
				modLeft = modRight;
				modRight = tmp;
			}

			if (isBiPolar) {
				modLeft = NLMath.clamp(modLeft, -1.0, 1.0);
				modRight = NLMath.clamp(modRight, -1.0, 1.0);
			} else {
				modLeft = NLMath.clamp(modLeft, 0.0, 1.0);
				modRight = NLMath.clamp(modRight, 0.0, 1.0);
			}

			presenter.modulation.modulationRange.left = modLeft;
			presenter.modulation.modulationRange.right = modRight;

			String modAmountWithUnit = p.modAmount.getDecoratedValue(true, true);
			String modAmountWithoutUnit = p.modAmount.getDecoratedValue(false, true);
			presenter.modulation.amountDisplayValues = new String[] { "MC Amount: " + modAmountWithUnit,
					"MC Amount: " + modAmountWithoutUnit, "MC Amt: " + modAmountWithoutUnit,
					"Amt: " + modAmountWithoutUnit, modAmountWithoutUnit };

			if (p.value.metaData.bipolar.getBool())
				modAmount *= 2;

			double srcValue = mc.value.getClippedValue();

			Range bounds = new Range(p.value.metaData.bipolar.getBool() ? -1.0 : 0, 1.0);
			double left = (value - modAmount * srcValue);
			double right = left + modAmount;
			Range mod = new Range(left, right);
			Range modNormalized = new Range(mod.getLeft(), mod.getRight());
			modNormalized.normalize();

			double r = NLMath.quantize(modNormalized.getRight(), 1000);
			double l = NLMath.quantize(modNormalized.getLeft(), 1000);

			{

				String clip = bounds.outOfRange(modAmount >= 0 ? l : r) ? "! " : "";
				mod.clipTo(bounds);
				String with = p.value.getDecoratedValue(true, mod.getLeft(), true);
				String without = p.value.getDecoratedValue(false, mod.getLeft(), true);
				presenter.modulation.amountLowerBoundDisplayValues = new String[] { clip + "Lower Limit: " + with,
						clip + "Lower Limit: " + without, clip + "Lower: " + without, clip + "Lo: " + without,
						clip + without };
			}

			{
				String clip = bounds.outOfRange(modAmount >= 0 ? r : l) ? "! " : "";
				mod.clipTo(bounds);
				String with = p.value.getDecoratedValue(true, mod.getRight(), true);
				String without = p.value.getDecoratedValue(false, mod.getRight(), true);
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

		} else {
			presenter.modulation.modulationSourceLabel = "";
			presenter.modulation.amountDisplayValues = new String[] { "" };
			presenter.modulation.amountLowerBoundDisplayValues = new String[] { "" };
			presenter.modulation.amountUpperBoundDisplayValues = new String[] { "" };
			presenter.modulation.macroControlDisplayValues = new String[] { "" };
			presenter.modulation.modulationRange.left = presenter.modulation.modulationRange.right = presenter.controlPosition;
		}
	}

	private void updatePresenter(PhysicalControlParameterModel p) {
		presenter.showContextMenu = p.id != 274 && p.id != 279;
		boolean highlight = Setup.get().systemSettings.highlightChangedParameters.isTrue();
		boolean forceHighlight = Setup.get().systemSettings.forceHighlightChangedParameters.isTrue();
		presenter.highlightChanged = p.isChanged() && (highlight || forceHighlight);

		int idWithLargestAmount = 0;
		double largestAmount = 0;
		boolean isNegative = false;

		for (int i : p.getAssociatedModulationRouters()) {
			ModulationRouterParameterModel router = (ModulationRouterParameterModel) EditBufferModel.findParameter(i);
			double amount = router.value.getQuantizedAndClipped(true);
			if (Math.abs(amount) > largestAmount) {
				idWithLargestAmount = router.getAssociatedMacroControlID();
				largestAmount = Math.abs(amount);
				isNegative = amount < 0;
			}
		}

		if (idWithLargestAmount == 0)
			presenter.longName = "Not assigned";
		else {
			BasicParameterModel mc = EditBufferModel.findParameter(idWithLargestAmount);
			presenter.longName = (isNegative ? "-" : "") + mc.longName.getValue();
		}
	}

	private void updatePresenter(MacroControlParameterModel p) {
		presenter.userGivenName = p.givenName.getValue();
		presenter.showContextMenu = true;
	}

	private String toString(ModSource s) {
		switch (s) {
		case A:
			return "\uE000";

		case B:
			return "\uE001";

		case C:
			return "\uE002";

		case D:
			return "\uE003";

		default:
		}

		return "";
	}

	@Override
	public ParameterPresenter getValue() {
		return presenter;
	}

	public String getFullNameWithGroup(BasicParameterModel e) {
		boolean changed = e.isChanged();
		return e.group.longName + "   \u2013   " + e.longName.getValue() + (changed ? " *" : "");
	}
}
