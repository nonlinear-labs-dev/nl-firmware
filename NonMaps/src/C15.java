import java.util.LinkedList;

public class C15 {

	// MonoMorph
	// PolyMorph
	// Split
	// Layer
	// Seamless Preset Change
	// MacroControls
	// VoiceAlloc
	// ParamEdit
	// VoiceFree
	// Dualphone
	// Monophone
	// Polyphone

	class AudioParameterArray {
		int allTheParameters[] = new int[300];
	}

	class MorphableAudioParameters {
		AudioParameterArray m0; // morph 0
		AudioParameterArray m1; // morph 1
	}

	class Voice {
		MorphableAudioParameters parameters;
		AudioParameterArray morphResult;
		double morphPosition;
		public int key;

		public void doMorph() {
			interpolate(morphResult, parameters.m0, parameters.m1, morphPosition);
		}

		private void interpolate(AudioParameterArray target, AudioParameterArray from, AudioParameterArray to, double pos) {
		}

		public void applyModulation() {
		}

		public void noteOff() {
		}

		public boolean isFinished() {
			return false;
		}
	}

	class VoiceGroup {
		LinkedList<Voice> activeVoices = new LinkedList<Voice>();

		// points to previous selected preset
		int previousSlot = 0;

		// points to current preset - parameters in this slot - will be edited
		// by UI
		int currentSlot = 0;

		// 0 ... 1 => used for interpolation during preset transition
		double presetTransition = 1.0;

		// increment determining the transition speed
		double presetTransitionIncrement = 0.01;

		// result for interpolation during preset transition
		MorphableAudioParameters presetTransitionBuffer = new MorphableAudioParameters();

		private boolean getPresetTransitionSetting() {
			// return true if presets should be morphed into each other
			return false;
		}

		private int findFreeSlot() {
			// iterate all active voices and find a this.slot[] that is not used
			return 0;
		}

		private void perInstrument() {
			doPresetTransition();
		}

		private void doPresetTransition() {
			if (presetTransition < 1) { // interpolation in progress
				presetTransition += presetTransitionIncrement;

				interpolate(presetTransitionBuffer, slots[previousSlot], slots[currentSlot], presetTransition);

				if (presetTransition >= 1)
					finishPresetTransition();
			}
		}

		private void finishPresetTransition() {
			letAllActiveVoicesPointTo(slots[currentSlot]);
		}

		private void letAllActiveVoicesPointTo(MorphableAudioParameters parameters) {
			for (Voice v : activeVoices)
				v.parameters = parameters;
		}

		private void interpolate(MorphableAudioParameters target, MorphableAudioParameters from, MorphableAudioParameters to, double pos) {
		}

		private void perVoice() {
			for (Voice v : activeVoices) {
				v.doMorph();
				v.applyModulation();
			}
		}

		private void add(Voice v) {
			v.parameters = slots[currentSlot];
			activeVoices.add(v);
		}

		private void noteOff(int key) {
			for (Voice v : activeVoices) {
				if (v.key == key)
					v.noteOff();
			}
		}

		private void freeFinishedVoices() {
			for (Voice v : activeVoices) {
				if (v.isFinished()) {
					activeVoices.remove(v);
					voicePool.add(v);
				}
			}
		}

		private void perSample() {
			perInstrument();
			perVoice();
		}

		private void loadPreset(MorphableAudioParameters params) {
			// load the preset into a new slot
			int s = findFreeSlot();
			slots[s] = params;

			previousSlot = currentSlot;
			currentSlot = s;

			if (getPresetTransitionSetting()) {
				// start transition with next 'perSample'-call
				presetTransition = 0;
				presetTransitionBuffer = slots[previousSlot];
				letAllActiveVoicesPointTo(presetTransitionBuffer);
			} else {
				// current voices should stay at its parameters
				// new voices will use parameters in 'currentSlot'
			}
		}
	}

	LinkedList<Voice> voicePool = new LinkedList<Voice>();
	MorphableAudioParameters slots[] = new MorphableAudioParameters[20];
	VoiceGroup groupA = new VoiceGroup();
	VoiceGroup groupB = new VoiceGroup();

	public void noteOn(int key) {
		freeFinishedVoices();

		if (isSplitMode())
			allocSplitVoice(key);
		else if (isLayerMode())
			allocLayeredVoices();
		else
			allocSingleVoice(groupA);
	}

	public void noteOff(int key) {
		groupA.noteOff(key);
		groupB.noteOff(key);
	}

	public void loadPreset(MorphableAudioParameters params, int whichGroup) {
		if (whichGroup == 0)
			groupA.loadPreset(params);
		else
			groupB.loadPreset(params);
	}

	public void perSample() {
		groupA.perSample();
		groupB.perSample();
	}

	private void freeFinishedVoices() {
		groupA.freeFinishedVoices();
		groupB.freeFinishedVoices();
	}

	private void allocSplitVoice(int key) {
		if (key < getSplitKey())
			allocSingleVoice(groupA);
		else
			allocSingleVoice(groupB);
	}

	private void allocLayeredVoices() {
		allocSingleVoice(groupA);
		allocSingleVoice(groupB);
	}

	private void allocSingleVoice(VoiceGroup group) {
		group.add(allocVoice());
	}

	private int getSplitKey() {
		return 60;
	}

	private boolean isLayerMode() {
		return false;
	}

	private boolean isSplitMode() {
		return false;
	}

	private Voice allocVoice() {
		return voicePool.removeFirst();
	}
}
