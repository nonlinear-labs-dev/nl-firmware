import { ReactiveVar } from 'meteor/reactive-var';
import { Tracker } from 'meteor/tracker';
import { PlaygroundProxy } from './playground-proxy';
import { BankPresenter, PresetManagerPresenter, PresetPresenter } from '../presenters';
import './preset-manager.html';

declare var proxy: PlaygroundProxy;
declare var Template;

Template.C16PresetManager.onCreated(function () {
    const instance = this;
    instance.presenter = new ReactiveVar<PresetManagerPresenter>(PresetManagerPresenter.create());

    Tracker.autorun(() => {
        instance.presenter.set(PresetManagerPresenter.create());
    });
});

Template.C16PresetManager.helpers({
    getBanks() {
        return Template.instance().presenter.get().banks.map(bank => BankPresenter.create(bank))
    }

    selectedBank() {
        return BankPresenter.create(Template.instance().presenter.selectBank);
    }
});

Template.C16Bank.helpers({
    getPresets() {
        return this.presets.map(preset => PresetPresenter.create(preset))
    }
});

Template.C16Bank.events({
    'click': function () {
        proxy.selectBank(this.uuid);
    }
});

Template.C16Preset.events({
    'click': function () {
        if (this.selected)
            proxy.loadPreset(this.uuid);
        else
            proxy.selectPreset(this.uuid);
    }
});


