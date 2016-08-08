/**
 * Created by Aliaksei Syrel on 07/08/16.
 */

var Generator = require('./generator.js');
var Platform = require('./platform.js');
var Builder = require('./builder.js');
var _ = require('./libs/underscore.js');

Platform.getPlatform().arch('i386');

//Platform.getPlatform().arch('x86_64');

//_(Platform.getPlatform().packages()).each(function(pkg) {
//    new Generator().generate(pkg);
//});
//
//new Generator().generate(Platform.getPlatform());

var builder = new Builder({ platform: Platform.getPlatform() });
builder.download();
builder.extract();
builder.patch();
builder.configure();
builder.export();