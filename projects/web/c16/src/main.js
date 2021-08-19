import '../build-time-info.js';
import { PlaygroundProxy } from './playground-proxy';
import './routes.js';
import { SyncedItemDatabase } from './synced-item-database';

sync = new SyncedItemDatabase();
proxy = new PlaygroundProxy();

console.log(`Greetings from ${module.id}!`);
