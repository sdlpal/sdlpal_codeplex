//
// Copyright (c) 2009, Wei Mingzhi <whistler@openoffice.org>.
// All rights reserved.
//
// This file is part of SDLPAL.
//
// SDLPAL is free software: you can redistribute it and/or modify
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

#ifndef SCRIPT_H
#define SCRIPT_H

#ifdef __cplusplus
extern "C"
{
#endif

WORD
PAL_RunTriggerScript(
   WORD           wScriptEntry,
   WORD           wEventObjectID
);

WORD
PAL_RunAutoScript(
   WORD           wScriptEntry,
   WORD           wEventObjectID
);

extern BOOL       g_fScriptSuccess;

#ifdef __cplusplus
}
#endif

#endif
