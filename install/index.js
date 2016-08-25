/**
 * Created by Aliaksei Syrel on 07/08/16.
 */
var _ = require('./libs/underscore.js');
var Platform = require('./platform.js');


var options = _.chain(process.argv)
    .partition(function(each, index) { return index % 2 == 0; })
    .value();
options = _.object(options[0], options[1]);

if (!_.isUndefined(options['--arch']))
    Platform.getPlatform().arch(options['--arch']);

var builder = Platform.getPlatform().builder();
builder.download();
builder.extract();
builder.patch();
builder.configure();
builder.export();
builder.compile();