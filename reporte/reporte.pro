#
# OpenRPT report writer and rendering engine
# Copyright (C) 2001-2007 by OpenMFG, LLC
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
# Please contact info@openmfg.com with any questions on this license.
#

TEMPLATE = lib
CONFIG  += staticlib \
           qt3support
DEFINES += MAKELIB

DESTDIR = ../bin

HEADERS = openreports.h \
          barcodes.h \
          graph.h \
          orutils.h \
          orprerender.h \
          orprintrender.h \
          renderobjects.h \
          MetaSQL/metasql.h \
          common/builtinformatfunctions.h \
          common/labelsizeinfo.h \
          common/pagesizeinfo.h \
          common/parsexmlutils.h \
          common/querysource.h \
          common/reportpageoptions.h \
          common/xsqlquery.h \
          previewdialog.h \
          orcrosstab.h \
          crosstab.h \
          common/quuencode.h \
          common/memdbloader.h \
          common/parameter.h \
          common/builtinSqlFunctions.h
SOURCES = openreports.cpp \
          3of9.cpp \
          ext3of9.cpp \
          code128.cpp \
          codeean.cpp \
          graph.cpp \
          orutils.cpp \
          orprerender.cpp \
          orprintrender.cpp \
          renderobjects.cpp \
          MetaSQL/metasql.cpp \
          common/builtinformatfunctions.cpp \
          common/labelsizeinfo.cpp \
          common/pagesizeinfo.cpp \
          common/parsexmlutils.cpp \
          common/querysource.cpp \
          common/reportpageoptions.cpp \
          common/parameter.cpp \
          common/xsqlquery.cpp \
          previewdialog.cpp \
          crosstab.cpp \
          common/quuencode.cpp \
          common/memdbloader.cpp \
          common/builtinSqlFunctions.cpp
OTHER_FILES += openreports.txt \
    renderer_es.qm \
    renderer_es.qm

INCLUDEPATH += common

#The following line was inserted by qt3to4
QT += xml \
      sql \
      qt3support
