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

    public enum SingleSoundPolyToFX {
        Poly_To_FXI,
        Poly_To_FXII,
        Poly_To_FXI_FXII
    }

    public enum SingleSoundFXToOut {
        FXI,
        FXII,
        FXI_FXII
    }

    public enum GenericArrowEnum {
        PartI,
        PartII,
        PartI_PartII,
        None
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

    public String currentPartName;
    public SoundType soundType;

    //arrow states
    //general
    public SerialFX serialFX = SerialFX.None;

    //split
    public GenericArrowEnum splitToFXArrow_I = GenericArrowEnum.None;
    public GenericArrowEnum splitToFXArrow_II = GenericArrowEnum.None;
    public GenericArrowEnum splitToOut = GenericArrowEnum.None;
    
    public boolean split_fb_I_into_I = false;
    public boolean split_fb_I_into_II = false;
    public boolean split_fb_II_into_I = false;
    public boolean split_fb_II_into_II = false;

    //layer
    public GenericArrowEnum layerFXToOut = GenericArrowEnum.None;
    public boolean layerIToFXI = false;
    public boolean layerIToFXII = false;
    public boolean layerIIToFXI = false;
    public boolean layerIIToFXII = false;
    
    //single
    public boolean isSingleSoundFeedbackToPolyActive = false;
    public SingleSoundFBToPoly fbToPolyArrowState = SingleSoundFBToPoly.FXI_To_Poly;
    public SingleSoundPolyToFX polyToFX = SingleSoundPolyToFX.Poly_To_FXI;
    public SingleSoundFXToOut fxToOut = SingleSoundFXToOut.FXI;
}