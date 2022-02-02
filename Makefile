# Makefile
# Copyright
# 	(C) 2022 Kyle McDermott and Ryan Jeffrey
#
# This file is part of XDchess.
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 3, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth
# Floor, Boston, MA 02110-1301, USA.


CC=gcc
LDFLAGS=
CFLAGS=-c -std=c11
TARGET=xdchess

.PHONY: clean run

# Linking step.
$(TARGET): main.o board.o
	$(CC) -o $@ $? $(LDFLAGS)

# Building steps.
main.o: main.c
	$(CC) -o $@ $(CFLAGS) $<

board.o: board.c
	$(CC) -o $@ $(CFLAGS) $<

run: $(TARGET)
	./$(TARGET) $(RUNARGS)

# Remove the object files.
clean:
	rm *.o
