package com.nonlinearlabs.client.presenters;

public class EditBufferPresenter {
    public boolean allParametersLocked = false;
    public boolean isAnyParameterLocked = false;
    public boolean isAnyParameterChanged = false;
    public boolean isAnyScaleOffsetParameterNotDefault = false;
    public String loadedPresetUUID = "";
    public ParameterPresenter selectedParameter;
}