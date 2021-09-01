import { BlazeLayout } from 'meteor/kadira:blaze-layout';
import { FlowRouter } from 'meteor/kadira:flow-router';
import './c16-hwui/parameter-editor.ts';
import './c16-hwui/preset-manager.ts';

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

