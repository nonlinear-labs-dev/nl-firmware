import { ReactiveVar } from 'meteor/reactive-var';
import { Tracker } from 'meteor/tracker';
import { EditBufferPresenter, ParameterGroupPresenter, ParameterPresenter } from './presenters';
import './preset-manager.html';


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
        return Template.instance().presenter.get().groups.map((p: string) => ParameterGroupPresenter.create(p));
    }
});

Template.parameterGroup.helpers({
    getParameters() {
        return this.parameters.map((p: string) => ParameterPresenter.create(p));
    }
});
