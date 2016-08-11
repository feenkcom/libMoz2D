/**
 * Created by Aliaksei Syrel on 07/08/16.
 */
var Platform = require('./platform.js');

Platform.getPlatform().arch('i386');

var builder = Platform.getPlatform().builder();
builder.download();
builder.extract();
builder.patch();
builder.configure();
builder.export();
builder.compile();