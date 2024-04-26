#!/bin/sh

echo Removing application...
rm -f /usr/local/share/applications/fau201-gui.desktop
rmdir --ignore-fail-on-non-empty /usr/local/share/applications
rm -f /usr/local/share/icons/hicolor/128x128/apps/fau201-gui.png
rmdir --ignore-fail-on-non-empty /usr/local/share/icons/hicolor/128x128/apps
rmdir --ignore-fail-on-non-empty /usr/local/share/icons/hicolor/128x128
rmdir --ignore-fail-on-non-empty /usr/local/share/icons/hicolor
rmdir --ignore-fail-on-non-empty /usr/local/share/icons
rm -f /usr/local/bin/fau201-gui
echo Removing source code files...
rm -rf /usr/local/src/fau201-gui
echo Done!
