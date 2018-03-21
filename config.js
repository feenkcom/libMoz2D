module.exports = {
    project: {
        name: 'Moz2D',
        version: {
            major: 1,
            minor: 2
        },
        cmake: 3.0,
        installer: 'install'
    },

    build: {
        // relative to this config file, will be bounded to @TOPSRCDIR@
        sources: 'mozilla-central-52',
        // relative to sources dir (mozilla-central)
        objects: 'obj-mozilla',
        // relative to this config file
        patches: 'patches',
        // binary output folder, relative to this config file
        bin: 'build',
        makefile: 'backend.mk',
        mozilla_config: 'mozilla-config.h',
        url: 'https://hg.mozilla.org/mozilla-central/archive/',
        version: 'FIREFOX_AURORA_52_BASE',
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

                'modules/zlib/src',
                'modules/brotli',
                'modules/woff2',
                'modules/libpref',

                'media/libpng',

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
                'config/external/icu/i18n',

                'intl/locale',
                'intl/unicharutil/util/internal',

                'mozglue/misc'
            ],
            mac: [
                'intl/locale/mac'
            ],
            linux: [
                'intl/locale/unix'
            ],
            win: [
                'intl/locale/windows'
            ]
        },
        bindings: {
            general: {
                sources: ['src'],
                packages: [
                    'wrapper',
                    'telemetry',
                    'services',
                    'undefined'
                ]
            },
            mac: {
                sources: ['src/undefined/mac', 'src/gpu/mac' ],
                packages: ['undefined_mac', 'gpu_mac' ]
            },
            linux: {
                sources: ['src/undefined/unix', 'src/gpu/unix'],
                packages: ['undefined_unix', 'gpu_unix' ]
            },
			win: {
                sources: ['src/undefined/windows', 'src/gpu/windows' ],
                packages: ['undefined_windows', 'gpu_windows' ]
            }
        },
        libraries: {
            mac: [
                'CoreFoundation',
                'OpenGL',
                'IOSurface',
                'Cocoa',
                'CoreServices',
                'CoreGraphics',
                'CoreText',
                'QuartzCore',
                'IOKit'
            ],
            linux: [
                'GL',
                'dl',
                'X11',
                'X11-xcb',
                'xcb',
                'xcb-shm',
                'Xt',
                'Xext',
                'Xrender',
                'fontconfig',
                'freetype',
                'cairo',
                'gtk-x11-2.0',
                'gdk-x11-2.0',
                'atk-1.0',
                'gdk_pixbuf-2.0',
                'gtk-3',
                'gdk-3',
                'gobject-2.0',
                'pango-1.0',
                'pangocairo-1.0',
                'glib-2.0'
            ],
            win: [
				'winmm.lib',
				'wsock32.lib',
				// GetFileVersionInfoSizeW
				'version.lib',
				// ScriptFreeCache
				'usp10.lib',
				// Required by cairo print surface
				'msimg32.lib',
				'kernel32.lib',
				'shell32.lib',
				'user32.lib',
				'gdi32.lib',
				'advapi32.lib',
				'd2d1.lib',
				'psapi.lib',
				'dbghelp.lib',
				'dwrite.lib',
				'dxgi.lib',
				'dxguid.lib', //IDirectDraw7
				'd3d9.lib',
				'd3d10.lib',
				'd3d11.lib',
				'opengl32.lib' //wglGetCurrentContext
			]
        },
        flags: {
            c: {
                cross: {
                    general: '',
                    mac: '-mssse3 -msse4.1',
                    // for some strange reason -FI does not work with CMake
                    win: '/FI${MOZ_TOP_OBJ_PATH}/mozilla-config.h',
                    linux: '-mssse3 -msse4.1 -fPIC'
                },
                i386: {
                    linux: ''
                },
                x86_64: {
                    linux: '-m64'
                }
            },
            cxx: {
                cross: {
                    general: '',
                    mac: '-mssse3 -msse4.1 -fexceptions -DUSE_NS_ABORT_OOM',
                    // for some strange reason -FI does not work with CMake
                    win: '/FI${MOZ_TOP_OBJ_PATH}/mozilla-config.h -DSK_CPU_SSE_LEVEL=31 -DSK_CPU_SSE_LEVEL=41',
                    linux: '-mssse3 -msse4.1 -fexceptions -fPIC -DUSE_NS_ABORT_OOM'
                },
                i386: {
                    linux: ''
                },
                x86_64: {
                    linux: '-m64',
                    win: '/DUSE_NS_ABORT_OOM'
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
                cross: {
                    general: '',
                    mac: '',
                    win: '',
                    linux: '-Wl,--no-undefined'
                }
            },
            sources: [{
                source: '${MOZ_TOP_PATH}/xpcom/glue/nsThreadUtils.cpp',
                flags: '-DMOZILLA_INTERNAL_API'
            }]
        },
        defines: {
            general: '-DMOZILLA_EXTERNAL_LINKAGE -DMOZ_DUMP_PAINTING -DXPCOM_GLUE_USE_NSPR -DLIBRARY_EXPORTS -DCAIRO_HAS_PDF_SURFACE -DCAIRO_HAS_SVG_SURFACE -DFT_CONFIG_OPTION_USE_PNG -DCAIRO_HAS_PNG_FUNCTIONS -DPNG_bKGD_SUPPORTED -DPNG_WRITE_PACKSWAP_SUPPORTED -DPNG_WRITE_USER_TRANSFORM_SUPPORTED',
            linux: ''
        },
        undefines: {
            general: [
                'MOZ_CRASHREPORTER',
                'MOZ_CRASHREPORTER_ENABLE_PERCENT',
                'MOZ_LOGGING',
                'MOZ_MEMORY',
                'MOZ_ENABLE_PROFILER_SPS'
            ],
			win: [
			    // SEH exceptions does not work, disable them
			    // https://blogs.msdn.microsoft.com/zhanli/2010/06/25/structured-exception-handling-seh-and-c-exception-handling/
			    'HAVE_SEH_EXCEPTIONS'
			]
        },
        includes: [
            '${MOZ_TOP_OBJ_PATH}/dist/include',
            '${MOZ_TOP_OBJ_PATH}/dist/include/nspr',
            '${MOZ_TOP_PATH}/intl/locale'
        ],
        excludes: {
            linux: ['nsprpub/lib/ds/plvrsion.c']
        }
    },

    config: {
        status: 'config.status',
        mozconfig: '.mozconfig',
        flags: '',
        options: {
            i386: {
                linux: [
                    '--target=i686-pc-linux-gnu',
                    '--x-libraries=/usr/lib32'
                ]
            },
            x86_64: {
                win: [
                    '--target=x86_64-pc-mingw32',
                    '--host=x86_64-pc-mingw32'
                ]
            }
        },
        exports: {
            cross: {
                mac: [],
                linux: [],
                win: [ 'AS=yasm.EXE' ]
            }
        },
        cross_compile: true,
        modules: {
            general: {
                enabled: [
                    'optimize=-O2'
                ],
                disabled: [
                    'debug',
                    'sandbox',
                    'gio',
                    'dbus',
                    'synth-speechd',
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
            },
            linux: {
                enabled: []
            },
            win: {
                disabled: []
            }
        }
    },
    // visitor pattern
    accept: function(visitor) {
        return visitor.visitConfig(module.exports);
    }
};
