import { ReactiveVar } from 'meteor/reactive-var';
import { Template, TemplateStaticTyped } from 'meteor/templating';
import { PlaygroundProxy } from './playground-proxy';
import { SyncedItemDatabase } from './synced-item-database';
type AttributesFactory<Presenter, InnerState> = { [name: string]: (presenter: Presenter, inner: InnerState | null) => {} };
type EventHandlers<Presenter, InnerState> = { [name: string]: (event: JQuery.Event, presenter: Presenter, inner: InnerState | null) => void };

export class Globals {
    syncedDatabase: SyncedItemDatabase;
    playgroundProxy: PlaygroundProxy;
};

function createGlobals(): Globals {
    const db = new SyncedItemDatabase();
    const pg = new PlaygroundProxy(db);
    return { syncedDatabase: db, playgroundProxy: pg };
}

export const globals = createGlobals();

export function glue<Name extends string, Presenter, InnerState>(
    templateName: Name,
    presenterFactory: (id: string, innerState: InnerState | null) => Presenter,
    innerStateFactory: (() => InnerState) | null = null,
    attributes: AttributesFactory<Presenter, InnerState> | null = null,
    events: EventHandlers<Presenter, InnerState> | null = null) {

    const TemplateTyped = Template as TemplateStaticTyped<Name,
        {}, {
            presenter: ReactiveVar<Presenter | null>,
            inner: InnerState | null
        }
    >;

    let getter = function (what: string) {
        let p = TemplateTyped.instance().presenter.get();
        if (p) return p[what];
        return "";
    }

    let hookedEvents = {};
    for (var prop in events) {
        if (Object.prototype.hasOwnProperty.call(events, prop)) {
            let hooked = events[prop];
            hookedEvents[prop] = function (event: JQuery.Event) {
                return hooked(event, TemplateTyped.instance().presenter!.get()!, TemplateTyped.instance().inner);
            }
        }
    }

    let hookedAttributes = {};
    for (var prop in attributes) {
        if (Object.prototype.hasOwnProperty.call(attributes, prop)) {
            let hooked = attributes[prop];
            hookedAttributes[prop] = function () {
                let p = TemplateTyped.instance().presenter;
                if (p && p.get())
                    return hooked(p.get()!, TemplateTyped.instance().inner);
                return {};
            }
        }
    }

    TemplateTyped[templateName].events(hookedEvents);
    TemplateTyped[templateName].helpers({ ...hookedAttributes, ...{ get: getter } });
    TemplateTyped[templateName].onCreated(function () {
        TemplateTyped.instance().presenter = new ReactiveVar<Presenter | null>(null);
        TemplateTyped.instance().inner = innerStateFactory ? innerStateFactory() : null;

        this.autorun(() => {
            try {
                TemplateTyped.instance().presenter.set(presenterFactory(Template.currentData(), TemplateTyped.instance().inner));
            }
            catch (err) {
                TemplateTyped.instance().presenter.set(null);
            }
        });
    });
}

