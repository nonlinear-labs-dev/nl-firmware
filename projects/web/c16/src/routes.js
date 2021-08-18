import { BlazeLayout } from 'meteor/kadira:blaze-layout';
import { FlowRouter } from 'meteor/kadira:flow-router';
import './preset-manager.ts';
import './parameter-editor.ts';


FlowRouter.route('/', {
    action() {
        BlazeLayout.render('editBuffer', {});
    }
});

