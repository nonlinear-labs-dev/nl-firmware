declare var global;

class PresetManagerPresenter {
    banks = new Array<string>();

    static create(): PresetManagerPresenter {
        try {
            var presenter = new PresetManagerPresenter();
            const pm = global.sync.queryItem("/preset-manager");
            presenter.banks = Array.from(pm.banks); // trigger exception if undefined
            return presenter;
        }
        catch (err) {
            return new PresetManagerPresenter();
        }
    }
}

class BankPresenter {
    name = "";
    uuid = "";
    orderNumber = 0;
    presets = new Array<string>();
    selected = false;

    static create(uuid: string): BankPresenter {
        try {
            var presenter = new BankPresenter();
            const pm = global.sync.queryItem("/preset-manager");
            const bank = global.sync.queryItem("/bank/" + uuid);
            presenter.name = bank.name;
            presenter.uuid = uuid;
            presenter.orderNumber = pm.banks.findIndex((e: string) => e == uuid) + 1;
            presenter.presets = Array.from(bank.presets); // trigger exception if undefined
            presenter.selected = pm['selected-bank'] == uuid;
            return presenter;
        }
        catch (err) {
            return new BankPresenter();
        }
    }
}


class PresetPresenter {
    name = "";
    orderNumber = 0;
    selected = false;
    loaded = false;

    static create(uuid: string): PresetPresenter {
        try {
            var presenter = new PresetPresenter();

            const preset = global.sync.queryItem("/preset/" + uuid);
            const bank = global.sync.queryItem("/bank/" + preset["bank-uuid"]);
            const editbuffer = global.sync.queryItem("/editbuffer");

            presenter.name = preset.name;
            presenter.orderNumber = bank.presets.findIndex((e: string) => e == uuid) + 1;
            presenter.selected = bank['selected-preset'] == uuid;
            presenter.loaded = editbuffer['loaded-preset'] == uuid;
            return presenter;
        }
        catch (err) {
            return new PresetPresenter();
        }
    }
}

export { PresetManagerPresenter };
export { BankPresenter };
export { PresetPresenter };

