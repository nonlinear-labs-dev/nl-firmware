package com.nonlinearlabs.client.presenters;

import com.nonlinearlabs.client.Checksum;
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

    public boolean fxIUnused = false;
    public boolean fxIIUnused = false;
    
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

    public boolean partIMuted = false;
    public boolean partIIMuted = false;

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

    @Override
    public int hashCode() {
        Checksum sum = new Checksum();
        sum.eat(fxIUnused);
        sum.eat(fxIIUnused);
        sum.eat(allParametersLocked);
        sum.eat(isAnyParameterLocked);
        sum.eat(isAnyParameterChanged);
        sum.eat(isAnyScaleOffsetParameterNotDefault);

        sum.eat(layerFBI);
        sum.eat(layerFBII);
        sum.eat(splitFXToI);
        sum.eat(splitFXToII);
        sum.eat(layerFXToI);
        sum.eat(layerFXToII);
    
        sum.eat(loadedPresetUUID);
        sum.eat(selectedParameter.hash);
        sum.eat(voiceGroup);
        sum.eat(voiceGroupIndicationColor.hashCode());
        sum.eat(voiceGroupEnum.ordinal());
        sum.eat(splitOverlap);
    
        sum.eat(voiceGroupI_ForegroundColor.hashCode());
        sum.eat(voiceGroupII_ForegroundColor.hashCode());
        sum.eat(voiceGroupI_BackgroundColor.hashCode());
        sum.eat(voiceGroupII_BackgroundColor.hashCode());
    
        sum.eat(currentPartName);
        sum.eat(soundType.ordinal());
        sum.eat(serialFX.ordinal());
    
        sum.eat(splitToFXArrow_I.ordinal());
        sum.eat(splitToFXArrow_II.ordinal());
        sum.eat(splitToOut.ordinal());
        sum.eat(split_fb_I_into_I);
        sum.eat(split_fb_I_into_II);
        sum.eat(split_fb_II_into_I);
        sum.eat(split_fb_II_into_II);

        sum.eat(layerFXToOut.ordinal());
        sum.eat(layerIToFXI);
        sum.eat(layerIToFXII);
        sum.eat(layerIIToFXI);
        sum.eat(layerIIToFXII);        

        sum.eat(isSingleSoundFeedbackToPolyActive);
        sum.eat(fbToPolyArrowState.ordinal());
        sum.eat(polyToFX.ordinal());
        sum.eat(fxToOut.ordinal());
        sum.eat(voiceGroup);

        sum.eat(partIIMuted);
        sum.eat(partIMuted);
        
        return (int) sum.getHash();
    }
}