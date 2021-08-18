import { Tracker } from 'meteor/tracker';
import './preset-manager.html';

declare var Template;

class PresetManagerPresenter {
    banks = new Array<string>();

    static create(): PresetManagerPresenter {
        try {
            var presenter = new PresetManagerPresenter();
            presenter.banks = global.sync.queryItem("/preset-manager").banks;
            return presenter;
        }
        catch (err) {
            return new PresetManagerPresenter();
        }
    }
}

class BankPresenter {
    name: string;
    orderNumber: number;
    presets: Array<string>;

    static create(uuid: string): BankPresenter {
        try {
            var presenter = new BankPresenter();
            var pm = global.sync.queryItem("/preset-manager");
            var bank = global.sync.queryItem("/bank/" + uuid);
            presenter.name = bank.name;
            presenter.orderNumber = pm.banks.findIndex((e: string) => e == uuid) + 1;
            presenter.presets = bank.presets;
            return presenter;
        }
        catch (err) {
            return new BankPresenter();
        }
    }
}


class PresetPresenter {
    name: string;
    orderNumber: number;

    static create(uuid: string): PresetPresenter {
        try {
            var presenter = new PresetPresenter();
            var preset = global.sync.queryItem("/preset/" + uuid);
            var bank = global.sync.queryItem("/bank/" + preset["bank-uuid"]);

            presenter.name = preset.name;
            presenter.orderNumber = bank.presets.findIndex((e: string) => e == uuid) + 1;
            return presenter;
        }
        catch (err) {
            return new PresetPresenter();
        }
    }
}

Template.presetManager.onCreated(function () {
    const instance = this;
    instance.presenter = new ReactiveVar<PresetManagerPresenter>(PresetManagerPresenter.create());

    Tracker.autorun(() => {
        instance.presenter.set(PresetManagerPresenter.create());
    });
});

Template.presetManager.helpers({
    getBanks() {
        return Template.instance().presenter.get().banks;
    }
});

Template.bank.onCreated(function () {
    const instance = this;
    instance.presenter = new ReactiveVar<BankPresenter>(BankPresenter.create(instance.data));

    Tracker.autorun(() => {
        instance.presenter.set(BankPresenter.create(instance.data));
    });
});

Template.bank.helpers({
    getNumber() {
        return Template.instance().presenter.get().orderNumber;
    },
    getName() {
        return Template.instance().presenter.get().name;
    },
    getPresets() {
        return Template.instance().presenter.get().presets;
    }
});

Template.preset.onCreated(function () {
    const instance = this;
    instance.presenter = new ReactiveVar<PresetPresenter>(PresetPresenter.create(instance.data));

    Tracker.autorun(() => {
        instance.presenter.set(PresetPresenter.create(instance.data));
    });
});

Template.preset.helpers({
    getNumber() {
        return Template.instance().presenter.get().orderNumber;
    },
    getName() {
        return Template.instance().presenter.get().name;
    }
});
