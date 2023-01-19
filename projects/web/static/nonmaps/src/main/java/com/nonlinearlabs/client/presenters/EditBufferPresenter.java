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

    public enum LayerToFX {
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, NONE
    }

    public enum LayerFXToOut {
        PartI,
        PartII,
        PartI_II,
        None
    }

    public class FBToPolyParameterPrerequisites {
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

    public class LayerToFXParameterStates {
        double outmix_I_lvl;
        double outmix_II_lvl;
        double to_fx_I;
        double to_fx_II;

        public LayerToFX calculate() {
            var outmix_I_infinite = outmix_I_lvl == 0;
            var outmix_II_infinite = outmix_II_lvl == 0;
            var outmix_I_open = outmix_I_lvl > 0;
            var outmix_II_open = outmix_II_lvl > 0;
            var FX_I_TO_I_ONLY = to_fx_I == 0;
            var FX_II_TO_II_ONLY = to_fx_II == 0;
            var FX_I_TO_II_ONLY = to_fx_I == 1;
            var FX_II_TO_I_ONLY = to_fx_II == 1;
            var FX_I_TO_BOTH = to_fx_I > 0 && to_fx_I < 1;
            var FX_II_TO_BOTH = to_fx_II > 0 && to_fx_II < 1;

            if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_BOTH)
                return LayerToFX.M;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_II_ONLY)
                return LayerToFX.I;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_I_ONLY)
                return LayerToFX.K;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_BOTH)
                return LayerToFX.L;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_I_ONLY)
                return LayerToFX.H;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_BOTH)
                return LayerToFX.J;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_II_ONLY)
                return LayerToFX.G;
            else if(outmix_I_open && outmix_II_infinite && FX_I_TO_BOTH)
                return LayerToFX.E;
            else if(outmix_I_open && outmix_II_infinite && FX_I_TO_II_ONLY)
                return LayerToFX.C;
            else if(outmix_I_open && outmix_II_infinite && FX_I_TO_I_ONLY)
                return LayerToFX.A;
            else if(outmix_II_open && outmix_I_infinite && FX_II_TO_BOTH)
                return LayerToFX.F;
            else if(outmix_II_open && outmix_I_infinite && FX_II_TO_I_ONLY)
                return LayerToFX.D;
            else if(outmix_II_open && outmix_I_infinite && FX_II_TO_II_ONLY)
                return LayerToFX.B;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_I_ONLY)
                return LayerToFX.N;
            else if(outmix_I_open && outmix_II_open && FX_II_TO_II_ONLY && FX_I_TO_II_ONLY)
                return LayerToFX.O;
            else if(outmix_I_infinite && outmix_II_infinite)
                return LayerToFX.NONE;
            
            return LayerToFX.NONE;
        }
    }

    public class LayerFXToOutParameterStates {
        double part_vol_I;
        double part_vol_II;
        double part_mute_I;
        double part_mute_II;

        public LayerFXToOut calculate() {
            if(part_vol_I > 0 && (part_vol_II == 0 || part_mute_II == 1))
            return LayerFXToOut.PartI;
            else if(part_vol_II > 0 && (part_vol_I == 0 || part_mute_I == 1))
            return LayerFXToOut.PartII;
            else if(part_vol_I > 0 && part_vol_II > 0)
            return LayerFXToOut.PartI_II;
            else if(part_vol_I == 0 && part_vol_II == 0)
                return LayerFXToOut.None;
            return LayerFXToOut.None;
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

    public LayerFXToOutParameterStates layerFXToOut = new LayerFXToOutParameterStates();
    public LayerToFXParameterStates layerToFX = new LayerToFXParameterStates();
    public FBToPolyParameterPrerequisites isSingleSoundFeedbackToPolyActive = new FBToPolyParameterPrerequisites();
    public SerialFX serialFX = SerialFX.None;
    public SingleSoundFBToPoly fbToPolyArrowState = SingleSoundFBToPoly.FXI_To_Poly;
    public SingleSoundPolyToFX polyToFX = SingleSoundPolyToFX.Poly_To_FXI;
    public SingleSoundFXToOut fxToOut = SingleSoundFXToOut.FXI;

    public String currentPartName;
    public SoundType soundType;
}