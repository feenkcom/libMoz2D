module.exports = {
    project: {
        name: 'Moz2D',
        version: {
            major: 1,
            minor: 0
        },
        cmake: 3.0
    },

    build: {
        // relative to this config file, will be bounded to @TOPSRCDIR@
        sources: 'mozilla-central',
        // relative to sources dir (mozilla-central)
        objects: 'obj-mozilla',
        // relative to this config file
        patches: 'patches',
        makefile: 'backend.mk',
        url: 'https://hg.mozilla.org/mozilla-central/archive/tip',
        format: '.tar.gz',
        arch: 'i386', // i386, x86_64
        packages: {
            general: [
                'gfx/2d',
                'gfx/cairo/cairo/src',
                'gfx/cairo/libpixman/src',
                'gfx/config',
                'gfx/gl',
                'gfx/graphite2/src',
                'gfx/harfbuzz/src',
                'gfx/ots/src',
                'gfx/qcms',
                'gfx/skia',
                'gfx/src',
                'gfx/thebes',

                'modules/brotli',
                'modules/woff2',
                'modules/zlib/src',

                'memory/fallible',
                'memory/mozalloc',

                'mfbt',

                'dom/encoding',

                'xpcom/glue',
                'xpcom/string',

                'config/external/nspr/libc',
                'config/external/nspr/pr',
                'config/external/nspr/ds',
                'config/external/icu/common',
                'config/external/icu/stubdata',
                'config/external/icu/i18n',

                'intl/locale',
                'intl/unicharutil/util/internal',

                'mozglue/misc'
            ],
            mac: [
                'intl/locale/mac'
            ]
        },
        bindings: {
            general: {
                sources: [ 'src' ],
                packages: [
                    'wrapper',
                    'telemetry',
                    'preferences',
                    'services',
                    'undefined'
                ]
            }
        },
        libraries: {
            mac: [ 'libs/mac/*.dylib' ],
            linux: [ ],
            win: [ ]
        },
        flags: {
            c: {
                cross: {
                    general: '-mssse3 -msse4.1',
                    mac: '',
                    win: '',
                    linux: ''
                },
                i386: {
                    linux: '-m32'
                },
                x86_64: {
                    linux: '-m64'
                }
            },
            cxx: {
                cross: {
                    general: '-mssse3 -msse4.1 -fexceptions',
                    mac: '',
                    win: '',
                    linux: ''
                },
                i386: {
                    linux: '-m32'
                },
                x86_64: {
                    linux: '-m64'
                }
            },
            asm: {
                cross: {
                    general: '',
                    mac: '',
                    win: '',
                    linux: ''
                },
                i386: {
                    mac: '-arch i386 -m32',
                    linux: '-m32'
                },
                x86_64: {
                    linux: '-m64'
                }
            },
            linker: {
                general: '',
                mac: '-framework CoreFoundation -framework OpenGL -framework IOSurface -framework Cocoa -framework CoreServices -framework CoreGraphics -framework CoreText -framework QuartzCore -framework IOKit',
                win: '',
                linux: ''
            },
            sources: [
                {
                    source: '${MOZ_TOP_PATH}/xpcom/glue/nsThreadUtils.cpp',
                    flags: '-DMOZILLA_INTERNAL_API'
                }
            ]
        },
        defines: {
            general: '-DMOZILLA_EXTERNAL_LINKAGE -DMOZ_DUMP_PAINTING -DXPCOM_GLUE_USE_NSPR'
        },
        includes: [
            '${MOZ_TOP_OBJ_PATH}/dist/include',
            '${MOZ_TOP_OBJ_PATH}/dist/include/nspr',
            '${MOZ_TOP_PATH}/intl/locale'
        ]
    },

    config: {
        status: 'config.status',
        mozconfig: '.mozconfig',
        flags: '-j4',
        cross_compile: true,
        modules: {
            enabled: [
                'tree-freetype'
            ],
            disabled: [
                'sandbox',
                'printing',
                'gio',
                'dbus',
                'synth-speechd',
                'websms-backend',
                'dbm',
                'accessibility',
                'webrtc',
                'webspeech',
                'webspeechtestbackend',
                'permissions',
                'negotiateauth',
                'pref-extensions',
                'system-extension-dirs',
                'gamepad',
                'crashreporter',
                'updater',
                'parental-controls',
                'content-sandbox',
                'mozril-geoloc',
                'necko-wifi',
                'cookies',
                'ctypes'
            ]
        }
    },
    // visitor pattern
    accept: function (visitor) {
        return visitor.visitConfig(module.exports);
    }
};