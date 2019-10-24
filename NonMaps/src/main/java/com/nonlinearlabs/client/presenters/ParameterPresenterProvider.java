package com.nonlinearlabs.client.presenters;

import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.setup.Setup.BooleanValues;

public class ParameterPresenterProvider extends Notifier<ParameterPresenter> {
	private ParameterPresenter presenter = new ParameterPresenter();

	public ParameterPresenterProvider(int parameterId) {
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
