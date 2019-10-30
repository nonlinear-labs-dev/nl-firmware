package com.nonlinearlabs.client.presenters;

import java.util.HashSet;
import java.util.Set;

import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterFactory;
import com.nonlinearlabs.client.dataModel.editBuffer.ScaleOffsetParameterModel;

public class EditBufferPresenterProvider extends Notifier<EditBufferPresenter> {
    private static EditBufferPresenterProvider theProvider = new EditBufferPresenterProvider();
    private EditBufferPresenter presenter = new EditBufferPresenter();

    private EditBufferPresenterProvider() {
        monitorScaleParameters();
        monitorSelectedParameter();
    }

    private void monitorSelectedParameter() {
        EditBufferModel.selectedParameter.onChange(selectedParameterID -> {
            ParameterPresenterProviders.get().register(selectedParameterID, v -> {
                if (selectedParameterID != v.id)
                    return false;

                presenter.selectedParameter = v;
                notifyChanges();
                return true;
            });
            return true;
        });
    }

    private void monitorScaleParameters() {
        Set<Integer> changedScaleParameters = new HashSet<Integer>();
        for (int scaleParamID : ParameterFactory.getScaleOffsetParameters()) {
            ScaleOffsetParameterModel p = (ScaleOffsetParameterModel) EditBufferModel.findParameter(scaleParamID);

            p.value.value.onChange(v -> {
                double defaultValue = p.value.metaData.defaultValue.getValue();

                if (v != defaultValue)
                    changedScaleParameters.add(scaleParamID);
                else
                    changedScaleParameters.remove(scaleParamID);

                boolean any = !changedScaleParameters.isEmpty();

                if (presenter.isAnyScaleOffsetParameterNotDefault != any) {
                    presenter.isAnyScaleOffsetParameterNotDefault = any;
                    notifyChanges();
                }
                return true;
            });
        }
    }

    @Override
    public EditBufferPresenter getValue() {
        return presenter;
    }

    public static EditBufferPresenterProvider get() {
        return theProvider;
    }

    public static EditBufferPresenter getPresenter() {
        return get().presenter;
    }

}