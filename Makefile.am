# This file is a part of Voody Blue Subtitler suit.
# 
# Author: Assen Totin <assen.totin@gmail.com>
#
# This software is released under GNU General Public License.
# See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
# for details.

SUBDIRS = src images po

dist_doc_DATA = AUTHORS BUGS ChangeLog COPYING INSTALL NEWS README TODO

rpm:
	rpmbuild -bb --define "_builddir @TOPLEVELDIR@" mate-applet-lockkeys.spec
 
