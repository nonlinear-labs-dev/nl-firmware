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

    public class FBToPolyParameterPrerequisites {
        //all prefixed with flowingThrough
        boolean fb_mix_fx = false;
        boolean fb_mix_lvl = false;
        boolean osc_a_pm_fb = false;
        boolean osc_b_pm_fb = false;
        boolean shp_a_fb_mix = false;
        boolean shp_b_fb_mix = false;

        public boolean calculateState() {
            return fb_mix_fx && fb_mix_lvl && (
                osc_a_pm_fb || osc_b_pm_fb || shp_a_fb_mix || shp_b_fb_mix
            );
        }
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

    public FBToPolyParameterPrerequisites isSingleSoundFeedbackToPolyActive = new FBToPolyParameterPrerequisites();
    public SerialFX serialFX = SerialFX.None;
    public SingleSoundFBToPoly fbToPolyArrowState = SingleSoundFBToPoly.FXI_To_Poly;

    public String currentPartName;
    public SoundType soundType;
}