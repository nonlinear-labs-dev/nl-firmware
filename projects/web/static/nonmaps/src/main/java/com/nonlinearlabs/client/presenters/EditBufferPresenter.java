package com.nonlinearlabs.client.presenters;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.RGBA;

public class EditBufferPresenter {


	public enum SerialFX {
        FX_I_IN_II,
        FX_II_IN_I,
        None
    }

    public enum SingleSoundFBToPoly {
        FXI_To_Poly,
        FXII_To_Poly,
        FXI_FXII_To_Poly
    }

    public boolean allParametersLocked = false;
    public boolean isAnyParameterLocked = false;
    public boolean isAnyParameterChanged = false;
    public boolean isAnyScaleOffsetParameterNotDefault = false;

    public boolean layerFBI = false;
    public boolean layerFBII = false;

    public boolean splitFXToI = false;
    public boolean splitFXToII = false;

    public boolean layerFXToI = false;
    public boolean layerFXToII = false;

    public String loadedPresetUUID = "";
    public ParameterPresenter selectedParameter;
    public String voiceGroup = "";
    public RGB voiceGroupIndicationColor = RGBA.transparent();
    public VoiceGroup voiceGroupEnum = VoiceGroup.I;
    public boolean splitOverlap = false;

    public RGB voiceGroupI_ForegroundColor = RGBA.transparent();
    public RGB voiceGroupII_ForegroundColor = RGBA.transparent();
    public RGB voiceGroupI_BackgroundColor = RGBA.transparent();
    public RGB voiceGroupII_BackgroundColor = RGBA.transparent();

    public boolean isSingleSoundFeedbackToPolyActive = false;
    public SerialFX serialFX = SerialFX.None;
    public SingleSoundFBToPoly fbToPolyArrowState = SingleSoundFBToPoly.FXI_To_Poly;

    public String currentPartName;
    public SoundType soundType;
}