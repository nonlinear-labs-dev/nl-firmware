import { ReactiveVar } from 'meteor/reactive-var';
import { Tracker } from 'meteor/tracker';
import { EditBufferPresenter, ParameterGroupPresenter, ParameterPresenter } from '../presenters';
import './preset-manager.html';


declare var Template;

Template.C16EditBuffer.onCreated(function () {
    const instance = this;
    instance.presenter = new ReactiveVar<EditBufferPresenter>(EditBufferPresenter.create());

    Tracker.autorun(() => {
        instance.presenter.set(EditBufferPresenter.create());
    });
});

Template.C16EditBuffer.helpers({
    getParameterGroups() {
        return Template.instance().presenter.get().groups.map((p: string) => ParameterGroupPresenter.create(p));
    }
});

Template.C16ParameterGroup.helpers({
    getParameters() {
        return this.parameters.map((p: string) => ParameterPresenter.create(p));
    }
});
