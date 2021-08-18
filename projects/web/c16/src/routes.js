import { BlazeLayout } from 'meteor/kadira:blaze-layout';
import { FlowRouter } from 'meteor/kadira:flow-router';
import './preset-manager.ts';


FlowRouter.route('/', {
    action() {
        BlazeLayout.render('presetManager', {});
    }
});

