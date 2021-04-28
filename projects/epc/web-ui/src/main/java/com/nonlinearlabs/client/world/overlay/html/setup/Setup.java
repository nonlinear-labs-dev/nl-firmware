package com.nonlinearlabs.client.world.overlay.html.setup;

import com.google.gwt.core.client.GWT;
import com.google.gwt.dom.client.DivElement;
import com.google.gwt.dom.client.InputElement;
import com.google.gwt.dom.client.Style.Display;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.uibinder.client.UiField;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.ListBox;
import com.google.gwt.user.client.ui.RadioButton;
import com.google.gwt.user.client.ui.TextArea;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.AftertouchCCMapping;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.AftertouchCurve;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BenderCCMapping;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BenderCurve;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.DisplayScaling;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.EditParameter;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.MidiReceiveChannel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.MidiReceiveChannelSplit;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.MidiSendChannel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.MidiSendChannelSplit;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.PedalCCMapping;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.PedalType;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.RibbonCCMapping;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.SelectionAutoScroll;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.StripeBrightness;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.VelocityCurve;
import com.nonlinearlabs.client.presenters.DeviceInformation;
import com.nonlinearlabs.client.presenters.DeviceInformationProvider;
import com.nonlinearlabs.client.presenters.DeviceSettings;
import com.nonlinearlabs.client.presenters.DeviceSettingsProvider;
import com.nonlinearlabs.client.presenters.LocalSettings;
import com.nonlinearlabs.client.presenters.LocalSettingsProvider;
import com.nonlinearlabs.client.presenters.MidiSettings;
import com.nonlinearlabs.client.presenters.MidiSettingsProvider;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.useCases.SystemSettings;
import com.nonlinearlabs.client.world.overlay.html.Range;

public class Setup extends Composite {
	interface SetupUiBinder extends UiBinder<HTMLPanel, Setup> {
	}

	private static SetupUiBinder ourUiBinder = GWT.create(SetupUiBinder.class);

	@UiField
	Button deviceSettingsButton, uiSettingsButton, uiMidiButton, systemInfoButton, aboutButton;

	@UiField
	DivElement deviceSettings, uiSettings, midiSettings, systemInfo, about;

	@UiField
	ListBox velocityCurve, aftertouchCurve, benderCurve, pedal1Type, pedal2Type, pedal3Type, pedal4Type,
			selectionAutoScroll, editParameter, scalingFactor, stripeBrightness, midiReceiveChannel, midiReceiveChannelSplit,
			midiSendChannel, midiSendChannelSplit, pedal1Mapping, 
			pedal2Mapping, pedal3Mapping, pedal4Mapping, ribbon1Mapping, ribbon2Mapping, benderMapping, aftertouchMapping;

	@UiField
	Label pedal1DisplayString, pedal2DisplayString, pedal3DisplayString, pedal4DisplayString,
			editSmoothingTimeDisplayString, freeMemory, uiVersion, dateTime, uiHead, uiBranch, uiCommits, uiCommitDate, uiUsedRam, uiTotalRam;

	@UiField
	InputElement pedal1Slider, pedal2Slider, pedal3Slider, pedal4Slider;

	@UiField
	RadioButton presetGlitchSuppressionOn, presetGlitchSuppressionOff, showContextMenusOn, showContextMenusOff,
			presetDragDropOn, presetDragDropOff, bitmapCacheOn, bitmapCacheOff, developerOptionsOn, developerOptionsOff,
			highlightChangedOn, highlightChangedOff, syncPartsOn, syncPartsOff, receivePCOn, receivePCOff, receiveNotesOn, 
			receiveNotesOff, receiveControllersOn, receiveControllersOff, sendPCOn, sendPCOff, sendNotesOn, 
			sendNotesOff, sendControllersOn, sendControllersOff, localNotesOn, 
			localNotesOff, localControllersOn, localControllersOff, highVeloCCOn, highVeloCCOff, enable14Bit, disable14Bit;

	@UiField
	Label transitionTimeDisplayString, tuneReferenceDisplayString;

	@UiField
	InputElement transitionTimeSlider, tuneReferenceSlider;

	Range transitionTimeSliderRange, tuneReferenceSliderRange;

	@UiField
	InputElement editSmoothingTimeSlider;

	@UiField
	TextArea deviceName;

	@UiField
	Button saveDeviceName, storeInitSound, resetInitSound;

	Range editSmoothingTimeRange;
	Range pedal1Range, pedal2Range, pedal3Range, pedal4Range;

	public Setup() {
		initWidget(ourUiBinder.createAndBindUi(this));

		transitionTimeSliderRange = Range.wrap(transitionTimeSlider);
		tuneReferenceSliderRange = Range.wrap(tuneReferenceSlider);

		pedal1Range = Range.wrap(pedal1Slider);
		pedal2Range = Range.wrap(pedal2Slider);
		pedal3Range = Range.wrap(pedal3Slider);
		pedal4Range = Range.wrap(pedal4Slider);

		setupTexts();
		connectEventHandlers();
		connectUpdate();

		switchPage(deviceSettingsButton, deviceSettings);
	}

	public void setupTexts() {
		NonMaps.get().getServerProxy().downloadEnumStrings("PedalTypes", (options) -> {
			fillListboxWithOptions(pedal1Type, options);
			fillListboxWithOptions(pedal2Type, options);
			fillListboxWithOptions(pedal3Type, options);
			fillListboxWithOptions(pedal4Type, options);

			DeviceSettings t = DeviceSettingsProvider.get().getPresenter();

			applyPedalValues(t.pedal1, pedal1Type, pedal1Range, pedal1DisplayString);
			applyPedalValues(t.pedal2, pedal2Type, pedal2Range, pedal2DisplayString);
			applyPedalValues(t.pedal3, pedal3Type, pedal3Range, pedal3DisplayString);
			applyPedalValues(t.pedal4, pedal4Type, pedal4Range, pedal4DisplayString);
		});

		fillListboxWithOptions(midiReceiveChannel, MidiSettings.ReceiveChannel.options);
		fillListboxWithOptions(midiReceiveChannelSplit, MidiSettings.ReceiveChannelSplit.options);

		fillListboxWithOptions(midiSendChannel, MidiSettings.SendChannel.options);
		fillListboxWithOptions(midiSendChannelSplit, MidiSettings.SendChannelSplit.options);

		fillListboxWithOptions(velocityCurve, DeviceSettings.VelocityCurve.options);
		fillListboxWithOptions(aftertouchCurve, DeviceSettings.AftertouchCurve.options);
		fillListboxWithOptions(benderCurve, DeviceSettings.BenderCurve.options);
		fillListboxWithOptions(selectionAutoScroll, LocalSettings.SelectionAutoScroll.options);
		fillListboxWithOptions(editParameter, LocalSettings.EditParameter.options);
		fillListboxWithOptions(scalingFactor, LocalSettings.DisplayScalingFactor.options);
		fillListboxWithOptions(stripeBrightness, LocalSettings.StripeBrightness.options);
		fillListboxWithOptions(pedal1Mapping, MidiSettings.PedalMapping.options);
		fillListboxWithOptions(pedal2Mapping, MidiSettings.PedalMapping.options);
		fillListboxWithOptions(pedal3Mapping, MidiSettings.PedalMapping.options);
		fillListboxWithOptions(pedal4Mapping, MidiSettings.PedalMapping.options);
		fillListboxWithOptions(ribbon1Mapping, MidiSettings.RibbonMapping.options);
		fillListboxWithOptions(ribbon2Mapping, MidiSettings.RibbonMapping.options);
		fillListboxWithOptions(benderMapping, MidiSettings.BenderMapping.options);
		fillListboxWithOptions(aftertouchMapping, MidiSettings.AftertouchMapping.options);

		fillRadioButtons(presetGlitchSuppressionOn, presetGlitchSuppressionOff,
				DeviceSettings.PresetGlitchSuppression.options);
		fillRadioButtons(showContextMenusOn, showContextMenusOff, LocalSettings.ContextMenus.options);
		fillRadioButtons(presetDragDropOn, presetDragDropOff, LocalSettings.PresetDragDrop.options);
		fillRadioButtons(bitmapCacheOn, bitmapCacheOff, LocalSettings.BitmapCache.options);
		fillRadioButtons(developerOptionsOn, developerOptionsOff, LocalSettings.ShowDeveloperOptions.options);
		fillRadioButtons(highlightChangedOn, highlightChangedOff, DeviceSettings.HighlightChanged.options);
		fillRadioButtons(syncPartsOn, syncPartsOff, DeviceSettings.SyncPartsAcrossUI.options);
		fillRadioButtons(receivePCOn, receivePCOff, MidiSettings.OnOffOption.options);
		fillRadioButtons(receiveControllersOn, receiveControllersOff, MidiSettings.OnOffOption.options);
		fillRadioButtons(receiveNotesOn, receiveNotesOff, MidiSettings.OnOffOption.options);
		fillRadioButtons(sendPCOn, sendPCOff, MidiSettings.OnOffOption.options);
		fillRadioButtons(sendNotesOn, sendNotesOff, MidiSettings.OnOffOption.options);
		fillRadioButtons(sendControllersOn, sendControllersOff, MidiSettings.OnOffOption.options);
		fillRadioButtons(localNotesOn, localNotesOff, MidiSettings.OnOffOption.options);
		fillRadioButtons(localControllersOn, localControllersOff, MidiSettings.OnOffOption.options);
		fillRadioButtons(highVeloCCOn, highVeloCCOff, MidiSettings.OnOffOption.options);
		fillRadioButtons(enable14Bit, disable14Bit, MidiSettings.OnOffOption.options);
	}

	public void connectEventHandlers() {
		SystemSettings settings = SystemSettings.get();
		com.nonlinearlabs.client.useCases.LocalSettings locals = com.nonlinearlabs.client.useCases.LocalSettings.get();

		deviceSettingsButton.addClickHandler(e -> switchPage(deviceSettingsButton, deviceSettings));
		uiSettingsButton.addClickHandler(e -> switchPage(uiSettingsButton, uiSettings));
		systemInfoButton.addClickHandler(e -> switchPage(systemInfoButton, systemInfo));
		aboutButton.addClickHandler(e -> switchPage(aboutButton, about));
		uiMidiButton.addClickHandler(e -> switchPage(uiMidiButton, midiSettings));

		velocityCurve.addChangeHandler(
				e -> settings.setVelocityCurve(VelocityCurve.values()[velocityCurve.getSelectedIndex()]));
		aftertouchCurve.addChangeHandler(
				e -> settings.setAftertouchCurve(AftertouchCurve.values()[aftertouchCurve.getSelectedIndex()]));
		benderCurve
				.addChangeHandler(e -> settings.setBenderCurve(BenderCurve.values()[benderCurve.getSelectedIndex()]));
		pedal1Type.addChangeHandler(e -> settings.setPedal1Type(PedalType.values()[pedal1Type.getSelectedIndex()]));
		pedal2Type.addChangeHandler(e -> settings.setPedal2Type(PedalType.values()[pedal2Type.getSelectedIndex()]));
		pedal3Type.addChangeHandler(e -> settings.setPedal3Type(PedalType.values()[pedal3Type.getSelectedIndex()]));
		pedal4Type.addChangeHandler(e -> settings.setPedal4Type(PedalType.values()[pedal4Type.getSelectedIndex()]));
		selectionAutoScroll.addChangeHandler(e -> locals
				.setSelectionAutoScroll(SelectionAutoScroll.values()[selectionAutoScroll.getSelectedIndex()]));
		editParameter.addChangeHandler(
				e -> locals.setEditParameter(EditParameter.values()[editParameter.getSelectedIndex()]));
		scalingFactor.addChangeHandler(
				e -> locals.setDisplayScaling(DisplayScaling.values()[scalingFactor.getSelectedIndex()]));
		stripeBrightness.addChangeHandler(
				e -> locals.setStripeBrightness(StripeBrightness.values()[stripeBrightness.getSelectedIndex()]));

		presetGlitchSuppressionOn.addClickHandler(e -> settings.setPresetGlitchSuppression(BooleanValues.on));
		presetGlitchSuppressionOff.addClickHandler(e -> settings.setPresetGlitchSuppression(BooleanValues.off));

		showContextMenusOn.addClickHandler(e -> locals.setContextMenus(BooleanValues.on));
		showContextMenusOff.addClickHandler(e -> locals.setContextMenus(BooleanValues.off));

		presetDragDropOn.addClickHandler(e -> locals.setPresetDragDrop(BooleanValues.on));
		presetDragDropOff.addClickHandler(e -> locals.setPresetDragDrop(BooleanValues.off));

		bitmapCacheOn.addClickHandler(e -> locals.setBitmapCache(BooleanValues.on));
		bitmapCacheOff.addClickHandler(e -> locals.setBitmapCache(BooleanValues.off));

		developerOptionsOn.addClickHandler(e -> locals.showDeveloperOptions(BooleanValues.on));
		developerOptionsOff.addClickHandler(e -> locals.showDeveloperOptions(BooleanValues.off));

		editSmoothingTimeRange = Range.wrap(editSmoothingTimeSlider);
		editSmoothingTimeRange.addValueChangeHandler(
				e -> settings.setEditSmoothingTime(editSmoothingTimeRange.getValue().doubleValue()));

		syncPartsOn.addClickHandler(e -> settings.setSyncParts(BooleanValues.on));
		syncPartsOff.addClickHandler(e -> settings.setSyncParts(BooleanValues.off));

		pedal1Range.addValueChangeHandler(e -> EditBufferUseCases.get()
				.setParameterValue(new ParameterId(254, VoiceGroup.Global), e.getValue().doubleValue(), true));
		pedal2Range.addValueChangeHandler(e -> EditBufferUseCases.get()
				.setParameterValue(new ParameterId(259, VoiceGroup.Global), e.getValue().doubleValue(), true));
		pedal3Range.addValueChangeHandler(e -> EditBufferUseCases.get()
				.setParameterValue(new ParameterId(264, VoiceGroup.Global), e.getValue().doubleValue(), true));
		pedal4Range.addValueChangeHandler(e -> EditBufferUseCases.get()
				.setParameterValue(new ParameterId(269, VoiceGroup.Global), e.getValue().doubleValue(), true));

		saveDeviceName.addClickHandler(e -> settings.setDeviceName(deviceName.getValue()));

		highlightChangedOn.addValueChangeHandler(e -> settings.setHighlightChangedParameters(BooleanValues.on));
		highlightChangedOff.addValueChangeHandler(e -> settings.setHighlightChangedParameters(BooleanValues.off));

		transitionTimeSliderRange.addValueChangeHandler(v -> settings.setTransitionTime(v.getValue().doubleValue()));
		tuneReferenceSliderRange.addValueChangeHandler(v -> settings.setTuneReference(v.getValue().doubleValue()));

		resetInitSound.addClickHandler(e -> settings.resetInitSound());
		storeInitSound.addClickHandler(e -> settings.storeInitSound());

		midiReceiveChannel.addChangeHandler(e -> settings.setReceiveMidiChannel(MidiReceiveChannel.values()[midiReceiveChannel.getSelectedIndex()]));
		midiReceiveChannelSplit.addChangeHandler(e -> settings.setReceiveMidiChannelSplit(MidiReceiveChannelSplit.values()[midiReceiveChannelSplit.getSelectedIndex()]));
		receivePCOn.addValueChangeHandler(e -> settings.setReceiveProgramChanges(BooleanValues.on));
		receivePCOff.addValueChangeHandler(e -> settings.setReceiveProgramChanges(BooleanValues.off));
		receiveControllersOn.addValueChangeHandler(e -> settings.setReceiveControllers(BooleanValues.on));
		receiveControllersOff.addValueChangeHandler(e -> settings.setReceiveControllers(BooleanValues.off));
		receiveNotesOn.addValueChangeHandler(e -> settings.setReceiveNotes(BooleanValues.on));
		receiveNotesOff.addValueChangeHandler(e -> settings.setReceiveNotes(BooleanValues.off));

		midiSendChannel.addChangeHandler(e -> settings.setSendChannel(MidiSendChannel.values()[midiSendChannel.getSelectedIndex()]));
		midiSendChannelSplit.addChangeHandler(e -> settings.setSendChannelSplit(MidiSendChannelSplit.values()[midiSendChannelSplit.getSelectedIndex()]));
		sendPCOn.addValueChangeHandler(e -> settings.setSendProgramChanges(BooleanValues.on));
		sendPCOff.addValueChangeHandler(e -> settings.setSendProgramChanges(BooleanValues.off));
		sendNotesOn.addValueChangeHandler(e -> settings.setSendNotes(BooleanValues.on));
		sendNotesOff.addValueChangeHandler(e -> settings.setSendNotes(BooleanValues.off));
		sendControllersOn.addValueChangeHandler(e -> settings.setSendControllers(BooleanValues.on));
		sendControllersOff.addValueChangeHandler(e -> settings.setSendControllers(BooleanValues.off));

		localNotesOn.addValueChangeHandler(e -> settings.setLocalNotes(BooleanValues.on));
		localNotesOff.addValueChangeHandler(e -> settings.setLocalNotes(BooleanValues.off));
		localControllersOn.addValueChangeHandler(e -> settings.setLocalControllers(BooleanValues.on));
		localControllersOff.addValueChangeHandler(e -> settings.setLocalControllers(BooleanValues.off));

		pedal1Mapping.addChangeHandler(e -> settings.setPedal1Mapping(PedalCCMapping.values()[pedal1Mapping.getSelectedIndex()]));
		pedal2Mapping.addChangeHandler(e -> settings.setPedal2Mapping(PedalCCMapping.values()[pedal2Mapping.getSelectedIndex()]));
		pedal3Mapping.addChangeHandler(e -> settings.setPedal3Mapping(PedalCCMapping.values()[pedal3Mapping.getSelectedIndex()]));
		pedal4Mapping.addChangeHandler(e -> settings.setPedal4Mapping(PedalCCMapping.values()[pedal4Mapping.getSelectedIndex()]));
		ribbon1Mapping.addChangeHandler(e -> settings.setRibbon1Mapping(RibbonCCMapping.values()[ribbon1Mapping.getSelectedIndex()]));
		ribbon2Mapping.addChangeHandler(e -> settings.setRibbon2Mapping(RibbonCCMapping.values()[ribbon2Mapping.getSelectedIndex()]));
		aftertouchMapping.addChangeHandler(e -> settings.setAftertouchMapping(AftertouchCCMapping.values()[aftertouchMapping.getSelectedIndex()]));
		benderMapping.addChangeHandler(e -> settings.setPitchbendMapping(BenderCCMapping.values()[benderMapping.getSelectedIndex()]));
		highVeloCCOn.addValueChangeHandler(e -> settings.setHighVelocityCC(BooleanValues.on));
		highVeloCCOff.addValueChangeHandler(e -> settings.setHighVelocityCC(BooleanValues.off));
		enable14Bit.addValueChangeHandler(e -> settings.set14BitSupport(BooleanValues.on));
		disable14Bit.addValueChangeHandler(e -> settings.set14BitSupport(BooleanValues.off));
	}

	public void connectUpdate() {
		DeviceSettingsProvider.get().register(t -> {
			applyPresenter(t);
			return true;
		});

		LocalSettingsProvider.get().register(t -> {
			applyPresenter(t);
			return true;
		});

		DeviceInformationProvider.get().register(t -> {
			applyPresenter(t);
			return true;
		});

		MidiSettingsProvider.get().register(t -> {
			applyPresenter(t);
			return true;
		});
	}

	private void fillListboxWithOptions(ListBox box, String[] options) {
		for (String s : options)
			box.addItem(s);
	}

	private void fillRadioButtons(RadioButton on, RadioButton off, String[] options) {
		on.setText(options[0]);
		off.setText(options[1]);
	}

	protected void applyPresenter(DeviceSettings t) {
		velocityCurve.setSelectedIndex(t.velocityCurve.selected);
		aftertouchCurve.setSelectedIndex(t.aftertouchCurve.selected);
		benderCurve.setSelectedIndex(t.benderCurve.selected);

		applyPedalValues(t.pedal1, pedal1Type, pedal1Range, pedal1DisplayString);
		applyPedalValues(t.pedal2, pedal2Type, pedal2Range, pedal2DisplayString);
		applyPedalValues(t.pedal3, pedal3Type, pedal3Range, pedal3DisplayString);
		applyPedalValues(t.pedal4, pedal4Type, pedal4Range, pedal4DisplayString);

		presetGlitchSuppressionOn.setValue(t.presetGlitchSuppression.value);
		presetGlitchSuppressionOff.setValue(!t.presetGlitchSuppression.value);
		editSmoothingTimeRange.setValue(t.editSmoothingTime.sliderPosition);
		editSmoothingTimeDisplayString.setText(t.editSmoothingTime.displayValue);

		syncPartsOn.setValue(t.syncParts.value);
		syncPartsOff.setValue(!t.syncParts.value);

		deviceName.setText(t.deviceName);

		highlightChangedOn.setValue(t.highlightChangedParameters.value);
		highlightChangedOff.setValue(!t.highlightChangedParameters.value);

		tuneReferenceDisplayString.setText(t.tuneReferenceDisplayString);
		tuneReferenceSliderRange.setValue(t.tuneReferenceValue);

		transitionTimeDisplayString.setText(t.transitionTimeDisplayString);
		transitionTimeSliderRange.setValue(t.transitionTimeValue);
	}

	public void applyPedalValues(DeviceSettings.Pedal src, ListBox type, Range slider, Label text) {
		type.setSelectedIndex(src.selected);
		slider.setValue(src.sliderPosition);
		text.setText(src.displayValue);
	}

	private void applyPresenter(LocalSettings t) {
		selectionAutoScroll.setSelectedIndex(t.selectionAutoScroll.selected);
		editParameter.setSelectedIndex(t.editParameter.selected);
		showContextMenusOn.setValue(t.contextMenus.value);
		showContextMenusOff.setValue(!t.contextMenus.value);
		presetDragDropOn.setValue(t.presetDragDrop.value);
		presetDragDropOff.setValue(!t.presetDragDrop.value);
		scalingFactor.setSelectedIndex(t.displayScalingFactor.selected);
		stripeBrightness.setSelectedIndex(t.stripeBrightness.selected);
		bitmapCacheOn.setValue(t.bitmapCache.value);
		bitmapCacheOff.setValue(!t.bitmapCache.value);
		developerOptionsOn.setValue(t.showDeveloperOptions.value);
		developerOptionsOff.setValue(!t.showDeveloperOptions.value);
	}

	private void applyPresenter(DeviceInformation t) {
		freeMemory.setText(t.freeDiscSpace);
		uiVersion.setText(t.uiVersion);
		uiBranch.setText(t.branch);
		uiHead.setText(t.head);
		uiCommits.setText(t.commits);
		dateTime.setText(t.dateTime);
		uiCommitDate.setText(t.commitDate);
		uiUsedRam.setText(t.usedRam);
		uiTotalRam.setText(t.totalRam);
	}

	private void applyPresenter(MidiSettings t) {
		midiReceiveChannel.setSelectedIndex(t.receiveChannel.selected);
		midiReceiveChannelSplit.setSelectedIndex(t.receiveChannelSplit.selected);
		receiveControllersOn.setValue(t.receiveControllers.value);
		receiveControllersOff.setValue(!t.receiveControllers.value);
		receiveNotesOn.setValue(t.receiveNotes.value);
		receiveNotesOff.setValue(!t.receiveNotes.value);
		receivePCOn.setValue(t.receiveProgramChanges.value);
		receivePCOff.setValue(!t.receiveProgramChanges.value);

		midiSendChannel.setSelectedIndex(t.sendChannel.selected);
		midiSendChannelSplit.setSelectedIndex(t.sendChannelSplit.selected);
		sendControllersOn.setValue(t.sendControllers.value);
		sendControllersOff.setValue(!t.sendControllers.value);
		sendPCOn.setValue(t.sendProgramChanges.value);
		sendPCOff.setValue(!t.sendProgramChanges.value);
		sendNotesOn.setValue(t.sendNotes.value);
		sendNotesOff.setValue(!t.sendNotes.value);
		
		localControllersOn.setValue(t.localControllers.value);
		localControllersOff.setValue(!t.localControllers.value);
		localNotesOn.setValue(t.localNotes.value);
		localNotesOff.setValue(!t.localNotes.value);

		pedal1Mapping.setSelectedIndex(t.pedalMapping1.selected);
		pedal2Mapping.setSelectedIndex(t.pedalMapping2.selected);
		pedal3Mapping.setSelectedIndex(t.pedalMapping3.selected);
		pedal4Mapping.setSelectedIndex(t.pedalMapping4.selected);		
		ribbon1Mapping.setSelectedIndex(t.ribbonMapping1.selected);	
		ribbon2Mapping.setSelectedIndex(t.ribbonMapping2.selected);
		aftertouchMapping.setSelectedIndex(t.aftertouchMapping.selected);
		benderMapping.setSelectedIndex(t.benderMapping.selected);
		highVeloCCOn.setValue(t.highVelocityCC.value);
		highVeloCCOff.setValue(!t.highVelocityCC.value);
		enable14Bit.setValue(t.enable14BitCC.value);
		disable14Bit.setValue(!t.enable14BitCC.value);
	}

	public void switchPage(Button btn, DivElement page) {
		deviceSettings.getStyle().setDisplay(Display.NONE);
		uiSettings.getStyle().setDisplay(Display.NONE);
		systemInfo.getStyle().setDisplay(Display.NONE);
		about.getStyle().setDisplay(Display.NONE);
		midiSettings.getStyle().setDisplay(Display.NONE);

		deviceSettingsButton.getElement().removeClassName("active");
		uiSettingsButton.getElement().removeClassName("active");
		systemInfoButton.getElement().removeClassName("active");
		aboutButton.getElement().removeClassName("active");
		uiMidiButton.getElement().removeClassName("active");

		btn.getElement().addClassName("active");
		page.getStyle().setDisplay(Display.BLOCK);
	}
}
