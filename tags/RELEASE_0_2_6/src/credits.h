
/*
 * Osmo - a handy personal organizer
 *
 * Copyright (C) 2007 Tomasz Maka <pasp@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _CREDITS_H
#define _CREDITS_H

#include <glib.h>

typedef struct {
	gchar *tag;
	gchar *name;
	gchar *email;
} OsmoCreditEntry;

static OsmoCreditEntry credits_programming[] = {
	{ NULL, "Tomasz Mąka", "pasp@users.sourceforge.net" },
	{ NULL, "Piotr Mąka", "silloz@users.sourceforge.net" }
};

static OsmoCreditEntry credits_graphics[] = {
	{ NULL, "Maja Kocoń", "http://ironya.ll.pl" }
};

static OsmoCreditEntry credits_contributors[] = {
	{ NULL, "Markus Dahms", "mad@automagically.de" }
};

static OsmoCreditEntry credits_translators[] = {
	{ "ca", "Roger Adell", "roger.adell@gmail.com" },
	{ "cs", "Jaroslav Lichtblau", "dragonlord@seznam.cz" },
	{ "de", "Markus Dahms", "mad@automagically.de" },
    { "el", "Konstantinos Tsakaloglou", "tsakf@yahoo.com" },
	{ "es", "Nacho Alonso González", "nacho.alonso.gonzalez@gmail.com" },
	{ "fi", "Toivo Miettinen", "toivo.miettinen@panuma.fi" },
	{ "fr", "Jean-Jacques Moulinier", "postmaster@moulinier.net" },
    { NULL, "Rémi Roupsard", "remi.roupsard@gmail.com"},    /* fr */
	{ NULL, "Lylliann Essandre", "lylliann@hotmail.fr"},  /* fr */
	{ "hu", "Peter Polonkai", "polesz@nedudu.hu" },
	{ "it", "Bautz", "bautz@email.com" },
    { NULL, "Calogero Bonasia", "kbonasia@gmail.com" },
	{ "ja", "Norihiro YONEDA", "aoba@avis.ne.jp" },
	{ "lt", "Vaidotas Kazla", "joshas@gmail.com" },
	{ "nl", "Tiger!P", "tigerp@tigerp.net" },
	{ "pl", "Piotr Mąka", "silloz@users.sourceforge.net" },
	{ "pt", "Bruno Miguel", "brunoalexandremiguel@gmail.com" },
    { "ru", "Vyacheslav A. Trishkin", "dedovsk@mail.ru" },
	{ "sv", "Niklas Grahn", "terra.unknown@yahoo.com" },
    { "tr", "Hasan Yılmaz", "hasanyilmaz@users.sourceforge.net" },
    { "zh", "Sasaqqdan", "sasaqqdan@gmail.com" }
};

#endif
