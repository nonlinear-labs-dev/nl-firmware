import { BlazeLayout } from 'meteor/kadira:blaze-layout';
import { FlowRouter } from 'meteor/kadira:flow-router';
import './parameter-editor.ts';
import './preset-manager.ts';

FlowRouter.route('/', {
    action() {
        BlazeLayout.render('editBuffer', {});
    }
});

FlowRouter.route('/presets', {
    action() {
        BlazeLayout.render('presetManager', {});
    }
});

