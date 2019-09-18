package com.nonlinearlabs.NonMaps.client.presenters;

import com.nonlinearlabs.NonMaps.client.dataModel.Notifier;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.BooleanValues;

public class ParameterPresenterProvider extends Notifier<ParameterPresenter> {
	private ParameterPresenter presenter = new ParameterPresenter();

	public ParameterPresenterProvider(int parameterId) {
		BasicParameterModel p = EditBufferModel.get().findParameter(parameterId);
		p.onChange(e -> updatePresenter(e));

		EditBufferModel.get().selectedParameter.onChange(id -> {
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
		presenter.valueRange.left = e.value.value.getValue();
		presenter.valueRange.right = e.value.value.getValue();
		presenter.valueRange.value = e.value.value.getValue();
		presenter.bipolar = e.value.metaData.bipolar.getValue() == BooleanValues.on;
		presenter.displayValue = Stringizers.get().stringize(e.value.metaData.scaling.getValue(),
				e.value.value.getValue());
		presenter.shortName = e.shortName.getValue();

		if (presenter.updateHash())
			notifyChanges();

		return true;
	}

	@Override
	public ParameterPresenter getValue() {
		return presenter;
	}
}
