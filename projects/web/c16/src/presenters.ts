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
    uuid = "";
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
            presenter.uuid = uuid;
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
export { EditBufferPresenter };
export { ParameterGroupPresenter };
export { ParameterPresenter };

class EditBufferPresenter {
    groups = new Array<string>();

    static create(): EditBufferPresenter {
        try {
            var presenter = new EditBufferPresenter();
            var eb = global.sync.queryItem("/editbuffer");
            presenter.groups = Array.from(eb.parametergroups.flat());
            return presenter;
        }
        catch (err) {
            return new EditBufferPresenter();
        }
    }
}

class ParameterGroupPresenter {
    parameters = new Array<string>();
    id = "";
    name = "";

    static create(id: string): ParameterGroupPresenter {
        try {
            var presenter = new ParameterGroupPresenter();
            var group = global.sync.queryItem("/parametergroup/" + id);
            presenter.id = id;
            presenter.name = group.name;
            presenter.parameters = Array.from(group.parameters);
            return presenter;
        } catch (err) {
            return new ParameterGroupPresenter();
        }
    }
}


class ParameterPresenter {
    id = "";
    name = "";
    value = "";

    static create(id: string): ParameterPresenter {
        try {
            var presenter = new ParameterPresenter();
            var param = global.sync.queryItem("/parameter/" + id);
            presenter.id = id;
            presenter.name = param.name;
            presenter.value = param.value;
            return presenter;
        } catch (err) {
            return new ParameterPresenter();
        }
    }
}

