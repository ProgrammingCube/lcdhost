#
# Plugins.pro
#
# Johan Lindh <johan@linkdata.se> is the maintainer of this file. 
# Do not alter to this file unless you have explicit approval.
#
# If you find a bug or need a feature or change, submit an issue at
# http://code.google.com/p/lcdhost/issues/list
#
# To control which plugins get built you must create the file
# "PluginsConfig.prf" and define LH_PLUGINS_LIST in that. If
# you leave LH_PLUGINS_LIST empty, it will build them all.
#
# This file will create a default "PluginsConfig.prf" for you
# if it is missing when qmake runs.
#
# You should consider signing your plugins. Please read
# "linkdata/SignPlugin.pri" for details on how to do so.
#

defineTest(createFileWithText) {
    win32: system(echo $$quote($$2)>$$1)
    else: system(echo \'$$quote($$2)\'>$$1)
}

!exists($$PWD/PluginsConfig.prf) {
    warning("Plugins.pro: Missing PluginsConfig.prf, creating a default one")
    CONFIG_TEXT=\
$$quote("$$LITERAL_HASH Generated by Plugins.pro")\
\\n$$quote("LH_PLUGINS_LIST= $$LITERAL_HASH Leave empty to build all plugins")\
\\n$$quote("load($$PWD/linkdata/PluginsBase.prf")\
\\n$$quote("CONFIG(debug):DESTDIR=$$PWD/Debug")\
\\n$$quote("else:DESTDIR=$$PWD/Release")\
\\n$$quote("exists($$PWD/../private.pem) { $$LITERAL_HASH You should keep it outside the repo")\
\\n$$quote(" SIGNATURE_PRIVATE_FILE=$$PWD/../private.pem")\
\\n$$quote(" SIGNATURE_PUBLIC_URL=http://my.website/myplugin/public.pem")\
\\n$$quote(" include($$PWD/linkdata/SignPlugin.pri)")\
\\n$$quote("}")
    createFileWithText($$PWD/PluginsConfig.prf,$$CONFIG_TEXT)
    error("Please review $$PWD/PluginsConfig.prf, make adjustments as needed, and then rebuild")
}

load($$PWD/PluginsConfig.prf)
isEmpty(LH_PLUGINS_LIST): LH_PLUGINS_LIST=$$files(LH_*)

TARGET = Plugins
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = linkdata
SUBDIRS += $$LH_PLUGINS_LIST
