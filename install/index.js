/**
 * Created by Aliaksei Syrel on 07/08/16.
 */

var Package = require('./package.js');
var Generator = require('./generator.js');
var Config = require('./../config.json');
var _ = require('./libs/underscore.js');

_(Config.packages.general.concat(Config.packages.mac)).each(function(name) {
    var pkg = new Package({
        name: name
    });
    new Generator().generate(pkg);
});

