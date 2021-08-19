import { Tracker } from 'meteor/tracker';
import './preset-manager.html';
import { EditBufferPresenter, ParameterGroupPresenter, ParameterPresenter } from './presenters';

declare var Template;

Template.editBuffer.onCreated(function () {
    const instance = this;
    instance.presenter = new ReactiveVar<EditBufferPresenter>(EditBufferPresenter.create());

    Tracker.autorun(() => {
        instance.presenter.set(EditBufferPresenter.create());
    });
});

Template.editBuffer.helpers({
    getParameterGroups() {
        return Template.instance().presenter.get().groups.map(p => ParameterGroupPresenter.create(p));
    }
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
        return this.parameters.map(p => ParameterPresenter.create(p));
    }
});
