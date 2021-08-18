import { ReactiveVar } from 'meteor/reactive-var';
import { Tracker } from 'meteor/tracker';
import { BankPresenter, PresetManagerPresenter, PresetPresenter } from './presenters';
import './preset-manager.html';

declare var Template;
declare var global;

Template.presetManager.onCreated(function () {
    const instance = this;
    instance.presenter = new ReactiveVar<PresetManagerPresenter>(PresetManagerPresenter.create());

    Tracker.autorun(() => {
        instance.presenter.set(PresetManagerPresenter.create());
    });
});

Template.presetManager.helpers({
    getBanks() {
        return Template.instance().presenter.get().banks.map(bank => BankPresenter.create(bank))
    }
});

Template.bank.helpers({
    getPresets() {
        return this.presets.map(preset => PresetPresenter.create(preset))
    }
});


