import { Tracker } from 'meteor/tracker';
import './preset-manager.html';

declare var Template;

class EditBufferPresenter {
    groups = new Array<string>();

    static create(): EditBufferPresenter {
        try {
            var presenter = new EditBufferPresenter();
            presenter.groups = global.sync.queryItem("/editbuffer").parametergroups;
            console.log(presenter.groups);
            return presenter;
        }
        catch (err) {
            return new EditBufferPresenter();
        }
    }
}

Template.editBuffer.onCreated(function () {
    const instance = this;
    instance.presenter = new ReactiveVar<EditBufferPresenter>(EditBufferPresenter.create());

    Tracker.autorun(() => {
        instance.presenter.set(EditBufferPresenter.create());
    });
});

Template.editBuffer.helpers({
    getParameterGroups() {
        return Template.instance().presenter.get().groups;
    }
});

class ParameterGroupPresenter {
    parameters = new Array<string>();
    id: string;
    name: string;

    static create(id: string): ParameterGroupPresenter {
        try {
            var presenter = new ParameterGroupPresenter();
            var group = global.sync.queryItem("/parametergroup/" + id);
            presenter.id = id;
            presenter.name = group.name;
            presenter.parameters = group.parameters;
            return presenter;
        } catch(err) {
            return new ParameterGroupPresenter();
        }
    }
}

Template.parameterGroup.onCreated(function () {
    const instance = this;
    instance.presenter = new ReactiveVar<ParameterGroupPresenter>(ParameterGroupPresenter.create(instance.data));

    Tracker.autorun(() => {
        console.log(instance.data);
        instance.presenter.set(ParameterGroupPresenter.create(instance.data));
    });
});

Template.parameterGroup.helpers({
    getParameters() {
        return Template.instance().presenter.get().parameters;
    }
});

class ParameterPresenter {
    id: string;
    name: string;
    value: string;

    static create(id: string): ParameterPresenter {
        try {
            var presenter = new ParameterPresenter();
            var param = global.sync.queryItem("/parameter/" + id);
            presenter.id = id;
            presenter.name = param.name;
            presenter.value = param.value;
            return presenter;
        } catch(err) {
            return new ParameterPresenter();
        }
    }
}

Template.parameter.onCreated(function () {
    const instance = this;
    instance.presenter = new ReactiveVar<ParameterPresenter>(ParameterPresenter.create(instance.data));

    Tracker.autorun(() => {
        instance.presenter.set(ParameterPresenter.create(instance.data));
    });
});

Template.parameter.helpers({
    getName() {
        return Template.instance().presenter.get().name;
    },
    getValue() {
        return Template.instance().presenter.get().value;
    }
});