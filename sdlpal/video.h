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

#ifndef VIDEO_H
#define VIDEO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common.h"

extern SDL_Surface *gpScreen;
extern SDL_Surface *gpScreenBak;

INT
VIDEO_Init(
   WORD             wScreenWidth,
   WORD             wScreenHeight,
   BOOL             fFullScreen
);

VOID
VIDEO_Shutdown(
   VOID
);

VOID
VIDEO_UpdateScreen(
   const SDL_Rect  *lpRect
);

VOID
VIDEO_SetPalette(
   SDL_Color        rgPalette[256]
);

VOID
VIDEO_Resize(
   INT             w,
   INT             h
);

SDL_Color *
VIDEO_GetPalette(
   VOID
);

VOID
VIDEO_ToggleFullscreen(
   VOID
);

VOID
VIDEO_SaveScreenshot(
   VOID
);

VOID
VIDEO_BackupScreen(
   VOID
);

VOID
VIDEO_RestoreScreen(
   VOID
);

VOID
VIDEO_ShakeScreen(
   WORD           wShakeTime,
   WORD           wShakeLevel
);

VOID
VIDEO_SwitchScreen(
   WORD           wSpeed
);

VOID
VIDEO_FadeScreen(
   WORD           wSpeed
);

#ifdef __cplusplus
}
#endif

#endif
