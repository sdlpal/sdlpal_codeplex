//
// Copyright (c) 2008, Wei Mingzhi <whistler@openoffice.org>.
// All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef MAGICMENU_H
#define MAGICMENU_H

#ifdef __cplusplus
extern "C"
{
#endif

WORD
PAL_MagicSelectionMenuUpdate(
   VOID
);

VOID
PAL_MagicSelectionMenuInit(
   WORD         wPlayerRole,
   BOOL         fInBattle,
   WORD         wDefaultMagic
);

WORD
PAL_MagicSelectionMenu(
   WORD         wPlayerRole,
   BOOL         fInBattle,
   WORD         wDefaultMagic
);

#ifdef __cplusplus
}
#endif
#endif