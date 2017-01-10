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
builder.stage('Welcome!');
builder.success(builder.tab('I will build a ' + Platform.getPlatform().arch() + ' version of Moz2D library'));

if (!_.isUndefined(options['--do'])) {
    switch(options['--do']) {
		case "download":
            builder.download();
			break;
		case "extract":
            builder.extract();
			break;
		case "patch":
            builder.patch();
			break;
        case "export":
            builder.export();
			break;
        case "compile":
            builder.compile();
			break;
        default:
            builder.error("Unknown operation: " + options['--do']);
    }
    process.exit(0);
}

builder.download();
builder.extract();
builder.patch();
builder.configure();
builder.export();
builder.compile();