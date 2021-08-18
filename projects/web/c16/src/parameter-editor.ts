import { Tracker } from 'meteor/tracker';
import './preset-manager.html';

declare var Template;

class EditBufferPresenter {
    parameters = new Array<string>();

    static create(): EditBufferPresenter {
        try {
            var presenter = new EditBufferPresenter();
            presenter.parameters = global.sync.queryItem("/editbuffer").parameters;
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
    getParameters() {
        return Template.instance().presenter.get().parameters;
    }
});