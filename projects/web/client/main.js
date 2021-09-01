import './build-time-info.js';
import './routes.js';
import { PlaygroundProxy } from './playground-proxy';
import { SyncedItemDatabase } from './synced-item-database';

sync = new SyncedItemDatabase();
proxy = new PlaygroundProxy();