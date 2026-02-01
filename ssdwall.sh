#!/bin/sh
#----------------------------------------------------------------------
#
#  ssdwall.sh - script that runs to broadcast system messages
#
#  Copyright (C) 1999, American Power Conversion, Inc., All Rights Reserved
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#-----------------------------------------------------------------------*/

if [ -f "$1" ]; then
    cat $1 | wall
else
    echo "Usage: $0 <message file>"
fi
