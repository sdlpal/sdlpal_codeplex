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

#include "main.h"

extern WORD g_rgPlayerPos[3][3][2];

static int g_iCurMiscMenuItem = 0;
static int g_iCurSubMenuItem = 0;

VOID
PAL_PlayerInfoBox(
   PAL_POS         pos,
   WORD            wPlayerRole,
   INT             iTimeMeter,
   BYTE            bTimeMeterColor,
   BOOL            fUpdate
)
/*++
  Purpose:

    Show the player info box.

  Parameters:

    [IN]  pos - the top-left corner position of the box.

    [IN]  wPlayerRole - the player role ID to be shown.

    [IN]  iTimeMeter - the value of time meter. 0 = empty, 100 = full.

    [IN]  bTimeMeterColor - the color of time meter.

    [IN]  fUpdate - whether to update the screen area or not.

  Return value:

    None.

--*/
{
   SDL_Rect        rect;
   BYTE            bPoisonColor;
   int             i, iPartyIndex;
   WORD            wMaxLevel, w;

   //
   // Draw the box
   //
   PAL_RLEBlitToSurface(PAL_SpriteGetFrame(gpSpriteUI, SPRITENUM_PLAYERINFOBOX),
      gpScreen, pos);

   //
   // Draw the player face
   //
   wMaxLevel = 0;
   bPoisonColor = 0xFF;

   for (iPartyIndex = 0; iPartyIndex <= gpGlobals->wMaxPartyMemberIndex; iPartyIndex++)
   {
      if (gpGlobals->rgParty[iPartyIndex].wPlayerRole == wPlayerRole)
      {
         break;
      }
   }

   if (iPartyIndex <= gpGlobals->wMaxPartyMemberIndex)
   {
      for (i = 0; i < MAX_POISONS; i++)
      {
         w = gpGlobals->rgPoisonStatus[i][iPartyIndex].wPoisonID;

         if (w != 0 &&
            gpGlobals->g.rgObject[w].poison.wPoisonLevel <= 3)
         {
            if (gpGlobals->g.rgObject[w].poison.wPoisonLevel >= wMaxLevel)
            {
               wMaxLevel = gpGlobals->g.rgObject[w].poison.wPoisonLevel;
               bPoisonColor = (BYTE)(gpGlobals->g.rgObject[w].poison.wColor);
            }
         }
      }
   }

   if (gpGlobals->g.PlayerRoles.rgwHP[wPlayerRole] == 0)
   {
      //
      // Always use the black/white color for dead players
      // and do not use the time meter
      //
      bPoisonColor = 0;
      iTimeMeter = 0;
   }

   if (bPoisonColor == 0xFF)
   {
      PAL_RLEBlitToSurface(PAL_SpriteGetFrame(gpSpriteUI, SPRITENUM_PLAYERFACE_FIRST + wPlayerRole),
         gpScreen, PAL_XY(PAL_X(pos) - 2, PAL_Y(pos) - 4));
   }
   else
   {
      PAL_RLEBlitMonoColor(PAL_SpriteGetFrame(gpSpriteUI, SPRITENUM_PLAYERFACE_FIRST + wPlayerRole),
         gpScreen, PAL_XY(PAL_X(pos) - 2, PAL_Y(pos) - 4), bPoisonColor, 0);
   }

   //
   // Draw a border for the Time Meter
   //
   rect.x = PAL_X(pos) + 31;
   rect.y = PAL_Y(pos) + 4;
   rect.w = 1;
   rect.h = 6;
   SDL_FillRect(gpScreen, &rect, 0xBD);

   rect.x += 39;
   SDL_FillRect(gpScreen, &rect, 0xBD);

   rect.x = PAL_X(pos) + 32;
   rect.y = PAL_Y(pos) + 3;
   rect.w = 38;
   rect.h = 1;
   SDL_FillRect(gpScreen, &rect, 0xBD);

   rect.y += 7;
   SDL_FillRect(gpScreen, &rect, 0xBD);

   //
   // Draw the Time meter bar
   //
   if (iTimeMeter >= 100)
   {
      rect.x = PAL_X(pos) + 33;
      rect.y = PAL_Y(pos) + 6;
      rect.w = 36;
      rect.h = 2;
      SDL_FillRect(gpScreen, &rect, 0x2C);
   }
   else if (iTimeMeter > 0)
   {
      rect.x = PAL_X(pos) + 33;
      rect.y = PAL_Y(pos) + 5;
      rect.w = iTimeMeter * 36 / 100;
      rect.h = 4;
      SDL_FillRect(gpScreen, &rect, bTimeMeterColor);
   }

   //
   // Draw the HP and MP value
   //
   PAL_RLEBlitToSurface(PAL_SpriteGetFrame(gpSpriteUI, SPRITENUM_SLASH), gpScreen,
      PAL_XY(PAL_X(pos) + 49, PAL_Y(pos) + 14));
   PAL_DrawNumber(gpGlobals->g.PlayerRoles.rgwMaxHP[wPlayerRole], 4,
      PAL_XY(PAL_X(pos) + 47, PAL_Y(pos) + 16), kNumColorYellow, kNumAlignRight);
   PAL_DrawNumber(gpGlobals->g.PlayerRoles.rgwHP[wPlayerRole], 4,
      PAL_XY(PAL_X(pos) + 26, PAL_Y(pos) + 13), kNumColorYellow, kNumAlignRight);

   PAL_RLEBlitToSurface(PAL_SpriteGetFrame(gpSpriteUI, SPRITENUM_SLASH), gpScreen,
      PAL_XY(PAL_X(pos) + 49, PAL_Y(pos) + 24));
   PAL_DrawNumber(gpGlobals->g.PlayerRoles.rgwMaxMP[wPlayerRole], 4,
      PAL_XY(PAL_X(pos) + 47, PAL_Y(pos) + 26), kNumColorCyan, kNumAlignRight);
   PAL_DrawNumber(gpGlobals->g.PlayerRoles.rgwMP[wPlayerRole], 4,
      PAL_XY(PAL_X(pos) + 26, PAL_Y(pos) + 23), kNumColorCyan, kNumAlignRight);

   //
   // Update the screen area if needed
   //
   if (fUpdate)
   {
      rect.x = PAL_X(pos) - 2;
      rect.y = PAL_Y(pos) - 4;
      rect.w = 77;
      rect.h = 39;

      VIDEO_UpdateScreen(&rect);
   }
}

static BOOL
PAL_BattleUIIsActionValid(
   BATTLEUIACTION         ActionType
)
/*++
  Purpose:

    Check if the specified action is valid.

  Parameters:

    [IN]  ActionType - the type of the action.

  Return value:

    TRUE if the action is valid, FALSE if not.

--*/
{
   WORD     wPlayerRole, w;
   int      i;

   wPlayerRole = gpGlobals->rgParty[g_Battle.UI.wCurPlayerIndex].wPlayerRole;

   switch (ActionType)
   {
   case kBattleUIActionAttack:
   case kBattleUIActionMisc:
      break;

   case kBattleUIActionMagic:
      if (gpGlobals->rgPlayerStatus[wPlayerRole][kStatusSilence] != 0)
      {
         return FALSE;
      }
      break;

   case kBattleUIActionCoopMagic:
      if (gpGlobals->wMaxPartyMemberIndex == 0)
      {
         return FALSE;
      }
      for (i = 0; i <= gpGlobals->wMaxPartyMemberIndex; i++)
      {
         w = gpGlobals->rgParty[i].wPlayerRole;
         if (gpGlobals->g.PlayerRoles.rgwHP[w] < gpGlobals->g.PlayerRoles.rgwMaxHP[w] / 5 ||
            gpGlobals->rgPlayerStatus[w][kStatusSleep] != 0 ||
            gpGlobals->rgPlayerStatus[w][kStatusConfused] != 0 ||
            g_Battle.rgPlayer[i].flTimeMeter < 100 ||
            g_Battle.rgPlayer[i].state == kFighterAct)
         {
            return FALSE;
         }
      }
      break;
   }

   return TRUE;
}

static VOID
PAL_BattleUIDrawMiscMenu(
   WORD       wCurrentItem,
   BOOL       fConfirmed
)
/*++
  Purpose:

    Draw the misc menu.

  Parameters:

    [IN]  wCurrentItem - the current selected menu item.

    [IN]  fConfirmed - TRUE if confirmed, FALSE if not.

  Return value:

    None.

--*/
{
   int           i;
   BYTE          bColor;

   MENUITEM rgMenuItem[] = {
      // value   label                   enabled   position
      {  0,      BATTLEUI_LABEL_ITEM,    TRUE,     PAL_XY(16, 32)  },
      {  1,      BATTLEUI_LABEL_DEFEND,  TRUE,     PAL_XY(16, 50)  },
      {  2,      BATTLEUI_LABEL_AUTO,    TRUE,     PAL_XY(16, 68)  },
      {  3,      BATTLEUI_LABEL_FLEE,    TRUE,     PAL_XY(16, 86)  },
      {  4,      BATTLEUI_LABEL_STATUS,  TRUE,     PAL_XY(16, 104) }
   };

   //
   // Draw the box
   //
   PAL_CreateBox(PAL_XY(2, 20), 4, 1, 0, FALSE);

   //
   // Draw the menu items
   //
   for (i = 0; i < 5; i++)
   {
      bColor = MENUITEM_COLOR;

      if (i == wCurrentItem)
      {
         if (fConfirmed)
         {
            bColor = MENUITEM_COLOR_CONFIRMED;
         }
         else
         {
            bColor = MENUITEM_COLOR_SELECTED;
         }
      }

      PAL_DrawText(PAL_GetWord(rgMenuItem[i].wNumWord), rgMenuItem[i].pos, bColor,
         TRUE, FALSE);
   }
}

static WORD
PAL_BattleUIMiscMenuUpdate(
   VOID
)
/*++
  Purpose:

    Update the misc menu.

  Parameters:

    None.

  Return value:

    The selected item number. 0 if cancelled, 0xFFFF if not confirmed.

--*/
{
   //
   // Draw the menu
   //
   PAL_BattleUIDrawMiscMenu(g_iCurMiscMenuItem, FALSE);

   //
   // Process inputs
   //
   if (g_InputState.dwKeyPress & (kKeyUp | kKeyLeft))
   {
      g_iCurMiscMenuItem--;
      if (g_iCurMiscMenuItem < 0)
      {
         g_iCurMiscMenuItem = 0;
      }
   }
   else if (g_InputState.dwKeyPress & (kKeyDown | kKeyRight))
   {
      g_iCurMiscMenuItem++;
      if (g_iCurMiscMenuItem > 4)
      {
         g_iCurMiscMenuItem = 4;
      }
   }
   else if (g_InputState.dwKeyPress & kKeySearch)
   {
      return g_iCurMiscMenuItem + 1;
   }
   else if (g_InputState.dwKeyPress & kKeyMenu)
   {
      return 0;
   }

   return 0xFFFF;
}

static WORD
PAL_BattleUIMiscItemSubMenuUpdate(
   VOID
)
/*++
  Purpose:

    Update the item sub menu of the misc menu.

  Parameters:

    None.

  Return value:

    The selected item number. 0 if cancelled, 0xFFFF if not confirmed.

--*/
{
   int             i;
   BYTE            bColor;

   MENUITEM rgMenuItem[] = {
      // value   label                      enabled   position
      {  0,      BATTLEUI_LABEL_USEITEM,    TRUE,     PAL_XY(44, 62)  },
      {  1,      BATTLEUI_LABEL_THROWITEM,  TRUE,     PAL_XY(44, 80)  },
   };

   //
   // Draw the menu
   //
   PAL_BattleUIDrawMiscMenu(0, TRUE);

   PAL_CreateBox(PAL_XY(30, 50), 1, 1, 0, FALSE);

   //
   // Draw the menu items
   //
   for (i = 0; i < 2; i++)
   {
      bColor = MENUITEM_COLOR;

      if (i == g_iCurSubMenuItem)
      {
         bColor = MENUITEM_COLOR_SELECTED;
      }

      PAL_DrawText(PAL_GetWord(rgMenuItem[i].wNumWord), rgMenuItem[i].pos, bColor,
         TRUE, FALSE);
   }

   //
   // Process inputs
   //
   if (g_InputState.dwKeyPress & (kKeyUp | kKeyLeft))
   {
      g_iCurSubMenuItem = 0;
   }
   else if (g_InputState.dwKeyPress & (kKeyDown | kKeyRight))
   {
      g_iCurSubMenuItem = 1;
   }
   else if (g_InputState.dwKeyPress & kKeySearch)
   {
      return g_iCurSubMenuItem + 1;
   }
   else if (g_InputState.dwKeyPress & kKeyMenu)
   {
      return 0;
   }

   return 0xFFFF;
}

VOID
PAL_BattleUIShowText(
   LPCSTR        lpszText,
   WORD          wDuration
)
/*++
  Purpose:

    Show a text message in the battle.

  Parameters:

    [IN]  lpszText - the text message to be shown.

    [IN]  wDuration - the duration of the message, in milliseconds.

  Return value:

    None.

--*/
{
   strcpy(g_Battle.UI.szMsg, lpszText);
   g_Battle.UI.dwMsgShowTime = SDL_GetTicks() + wDuration;
}

VOID
PAL_BattleUIPlayerReady(
   WORD          wPlayerIndex
)
/*++
  Purpose:

    Start the action selection menu of the specified player.

  Parameters:

    [IN]  wPlayerIndex - the player index.

  Return value:

    None.

--*/
{
   g_Battle.UI.wCurPlayerIndex = wPlayerIndex;
   g_Battle.UI.state = kBattleUISelectMove;
   g_Battle.UI.wSelectedAction = 0;
   g_Battle.UI.MenuState = kBattleMenuMain;

   //
   // Play a voice which indicates the player is ready
   //
   SOUND_Play(gpGlobals->g.PlayerRoles.rgwReadySound[gpGlobals->rgParty[wPlayerIndex].wPlayerRole]);
}

static VOID
PAL_CreateBorderBox(
   PAL_POS           pos,
   INT               w,
   INT               h,
   BYTE              bFillColor
)
/*++
  Purpose:

    Draw a bordered box on the screen.

  Parameters:

    [IN]  pos - the position of the box.

    [IN]  w - the width of the box.

    [IN]  h - the height of the box.

    [IN]  bFillColor - the color which fills the box.

  Return value:

    None.

--*/
{
   SDL_Rect       rect;

   rect.x = PAL_X(pos);
   rect.y = PAL_Y(pos);
   rect.w = w;
   rect.h = 1;

   SDL_FillRect(gpScreen, &rect, MENUITEM_COLOR_CONFIRMED);

   rect.y += h - 1;

   SDL_FillRect(gpScreen, &rect, MENUITEM_COLOR_CONFIRMED);

   rect.y = PAL_Y(pos);
   rect.w = 1;
   rect.h = h;

   SDL_FillRect(gpScreen, &rect, MENUITEM_COLOR_CONFIRMED);

   rect.x += w - 1;

   SDL_FillRect(gpScreen, &rect, MENUITEM_COLOR_CONFIRMED);

   rect.x = PAL_X(pos) + 1;
   rect.y = PAL_Y(pos) + 1;
   rect.w = w - 2;
   rect.h = h - 2;

   SDL_FillRect(gpScreen, &rect, bFillColor);
}

static VOID
PAL_BattleUIUseItem(
   VOID
)
/*++
  Purpose:

    Use an item in the battle UI.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   WORD       wSelectedItem;

   wSelectedItem = PAL_ItemSelectMenuUpdate();

   if (wSelectedItem != 0xFFFF)
   {
      if (wSelectedItem != 0)
      {
         g_Battle.UI.wActionType = kBattleActionUseItem;
         g_Battle.UI.wObjectID = wSelectedItem;

         if (gpGlobals->g.rgObject[wSelectedItem].item.wFlags & kItemFlagApplyToAll)
         {
            g_Battle.UI.state = kBattleUISelectTargetPlayerAll;
         }
         else
         {
            g_Battle.UI.wSelectedIndex = g_Battle.UI.wCurPlayerIndex;
            g_Battle.UI.state = kBattleUISelectTargetPlayer;
         }
      }
      else
      {
         g_Battle.UI.MenuState = kBattleMenuMain;
      }
   }
}

static VOID
PAL_BattleUIThrowItem(
   VOID
)
/*++
  Purpose:

    Throw an item in the battle UI.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   WORD wSelectedItem = PAL_ItemSelectMenuUpdate();

   if (wSelectedItem != 0xFFFF)
   {
      if (wSelectedItem != 0)
      {
         g_Battle.UI.wActionType = kBattleActionThrowItem;
         g_Battle.UI.wObjectID = wSelectedItem;

         if (gpGlobals->g.rgObject[wSelectedItem].item.wFlags & kItemFlagApplyToAll)
         {
            g_Battle.UI.state = kBattleUISelectTargetEnemyAll;
         }
         else
         {
            g_Battle.UI.wSelectedIndex = g_Battle.UI.wPrevEnemyTarget;
            g_Battle.UI.state = kBattleUISelectTargetEnemy;
         }
      }
      else
      {
         g_Battle.UI.MenuState = kBattleMenuMain;
      }
   }
}

VOID
PAL_BattleUIUpdate(
   VOID
)
/*++
  Purpose:

    Update the status of battle UI.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   int              i, j, x, y;
   WORD             wPlayerRole, w;

   //
   // Draw the player info boxes.
   //
   for (i = 0; i <= gpGlobals->wMaxPartyMemberIndex; i++)
   {
      wPlayerRole = gpGlobals->rgParty[i].wPlayerRole;

      j = TIMEMETER_COLOR_DEFAULT;
      if (gpGlobals->rgPlayerStatus[wPlayerRole][kStatusHaste] > 0)
      {
         j = TIMEMETER_COLOR_HASTE;
      }
      else if (gpGlobals->rgPlayerStatus[wPlayerRole][kStatusSlow] > 0)
      {
         j = TIMEMETER_COLOR_SLOW;
      }

      PAL_PlayerInfoBox(PAL_XY(91 + 77 * i, 165), wPlayerRole,
         (WORD)(g_Battle.rgPlayer[i].flTimeMeter), j, FALSE);

      // TODO: status
   }

   //
   // Show the text message if there is one.
   //
   if (SDL_GetTicks() < g_Battle.UI.dwMsgShowTime)
   {
      //
      // Draw a box.
      //
      PAL_CreateBorderBox(PAL_XY(1, 0), 318, 18, 0x52);

      //
      // Draw the text message.
      //
      PAL_DrawText(g_Battle.UI.szMsg, PAL_XY(160 - 4 * strlen(g_Battle.UI.szMsg), 2),
         MENUITEM_COLOR_CONFIRMED, TRUE, FALSE);
   }

   if (g_InputState.dwKeyPress & kKeyAuto)
   {
      g_Battle.UI.fAutoAttack = !g_Battle.UI.fAutoAttack;
   }
   else if (g_InputState.dwKeyPress & kKeyStatus)
   {
      PAL_PlayerStatus();
      return;
   }

   //
   // Draw the "auto attack" message if in the autoattack mode.
   //
   if (g_Battle.UI.fAutoAttack)
   {
      PAL_DrawText(PAL_GetWord(BATTLEUI_LABEL_AUTO), PAL_XY(280, 10),
         MENUITEM_COLOR, TRUE, FALSE);
   }

   if (g_Battle.UI.state != kBattleUIWait)
   {
      wPlayerRole = gpGlobals->rgParty[g_Battle.UI.wCurPlayerIndex].wPlayerRole;

      //
      // Cancel any actions if player is dead or sleeping or confused.
      //
      if (gpGlobals->g.PlayerRoles.rgwHP[wPlayerRole] == 0 ||
         gpGlobals->rgPlayerStatus[wPlayerRole][kStatusSleep] != 0 ||
         gpGlobals->rgPlayerStatus[wPlayerRole][kStatusConfused] != 0)
      {
         g_Battle.UI.state = kBattleUIWait;
         g_Battle.rgPlayer[g_Battle.UI.wCurPlayerIndex].flTimeMeter = 0;

         return; // don't go further
      }

      //
      // Draw the arrow on the player's head.
      //
      i = SPRITENUM_BATTLE_ARROW_CURRENTPLAYER_RED;
      if ((SDL_GetTicks() / BATTLE_FRAME_TIME) & 1)
      {
         i = SPRITENUM_BATTLE_ARROW_CURRENTPLAYER;
      }

      x = g_rgPlayerPos[gpGlobals->wMaxPartyMemberIndex][g_Battle.UI.wCurPlayerIndex][0] - 8;
      y = g_rgPlayerPos[gpGlobals->wMaxPartyMemberIndex][g_Battle.UI.wCurPlayerIndex][1] - 74;

      PAL_RLEBlitToSurface(PAL_SpriteGetFrame(gpSpriteUI, i), gpScreen, PAL_XY(x, y));
   }

   switch (g_Battle.UI.state)
   {
   case kBattleUISelectMove:
      //
      // Draw the icons
      //
      {
         struct {
            int               iSpriteNum;
            PAL_POS           pos;
            BATTLEUIACTION    action;
         } rgItems[] =
         {
            {SPRITENUM_BATTLEICON_ATTACK,    PAL_XY(27, 140), kBattleUIActionAttack},
            {SPRITENUM_BATTLEICON_MAGIC,     PAL_XY(0, 155),  kBattleUIActionMagic},
            {SPRITENUM_BATTLEICON_COOPMAGIC, PAL_XY(54, 155), kBattleUIActionCoopMagic},
            {SPRITENUM_BATTLEICON_MISCMENU,  PAL_XY(27, 170), kBattleUIActionMisc}
         };

         if (!PAL_BattleUIIsActionValid(rgItems[g_Battle.UI.wSelectedAction].action))
         {
            g_Battle.UI.wSelectedAction = 0;
         }

         for (i = 0; i < 4; i++)
         {
            if (g_Battle.UI.wSelectedAction == i)
            {
               PAL_RLEBlitToSurface(PAL_SpriteGetFrame(gpSpriteUI, rgItems[i].iSpriteNum),
                  gpScreen, rgItems[i].pos);
            }
            else if (PAL_BattleUIIsActionValid(rgItems[i].action))
            {
               PAL_RLEBlitMonoColor(PAL_SpriteGetFrame(gpSpriteUI, rgItems[i].iSpriteNum),
                  gpScreen, rgItems[i].pos, 0, -4);
            }
            else
            {
               PAL_RLEBlitMonoColor(PAL_SpriteGetFrame(gpSpriteUI, rgItems[i].iSpriteNum),
                  gpScreen, rgItems[i].pos, 0x10, -4);
            }
         }

         switch (g_Battle.UI.MenuState)
         {
         case kBattleMenuMain:
            if (g_InputState.dwKeyPress & kKeyUp)
            {
               g_Battle.UI.wSelectedAction = 0;
            }
            else if (g_InputState.dwKeyPress & kKeyDown)
            {
               g_Battle.UI.wSelectedAction = 3;
            }
            else if (g_InputState.dwKeyPress & kKeyLeft)
            {
               if (PAL_BattleUIIsActionValid(kBattleUIActionMagic))
               {
                  g_Battle.UI.wSelectedAction = 1;
               }
            }
            else if (g_InputState.dwKeyPress & kKeyRight)
            {
               if (PAL_BattleUIIsActionValid(kBattleUIActionCoopMagic))
               {
                  g_Battle.UI.wSelectedAction = 2;
               }
            }
            else if (g_InputState.dwKeyPress & kKeySearch)
            {
               switch (g_Battle.UI.wSelectedAction)
               {
               case 0:
                  //
                  // Attack
                  //
                  g_Battle.UI.wActionType = kBattleActionAttack;
                  if (PAL_PlayerCanAttackAll(gpGlobals->rgParty[g_Battle.UI.wCurPlayerIndex].wPlayerRole))
                  {
                     g_Battle.UI.state = kBattleUISelectTargetEnemyAll;
                  }
                  else
                  {
                     g_Battle.UI.wSelectedIndex = g_Battle.UI.wPrevEnemyTarget;
                     g_Battle.UI.state = kBattleUISelectTargetEnemy;
                  }
                  break;

               case 1:
                  //
                  // Magic
                  //
                  g_Battle.UI.MenuState = kBattleMenuMagicSelect;
                  PAL_MagicSelectionMenuInit(wPlayerRole, TRUE, 0);
                  break;

               case 2:
                  //
                  // Cooperative magic
                  //
                  w = gpGlobals->rgParty[g_Battle.UI.wCurPlayerIndex].wPlayerRole;
                  w = PAL_GetPlayerCooperativeMagic(w);

                  g_Battle.UI.wActionType = kBattleActionCoopMagic;
                  g_Battle.UI.wObjectID = w;

                  if (gpGlobals->g.rgObject[w].magic.wFlags & kMagicFlagUsableToEnemy)
                  {
                     if (gpGlobals->g.rgObject[w].magic.wFlags & kMagicFlagApplyToAll)
                     {
                        g_Battle.UI.state = kBattleUISelectTargetEnemyAll;
                     }
                     else
                     {
                        g_Battle.UI.wSelectedIndex = g_Battle.UI.wPrevEnemyTarget;
                        g_Battle.UI.state = kBattleUISelectTargetEnemy;
                     }
                  }
                  else
                  {
                     if (gpGlobals->g.rgObject[w].magic.wFlags & kMagicFlagApplyToAll)
                     {
                        g_Battle.UI.state = kBattleUISelectTargetPlayerAll;
                     }
                     else
                     {
                        g_Battle.UI.wSelectedIndex = g_Battle.UI.wCurPlayerIndex;
                        g_Battle.UI.state = kBattleUISelectTargetPlayer;
                     }
                  }
                  break;

               case 3:
                  //
                  // Misc menu
                  //
                  g_Battle.UI.MenuState = kBattleMenuMisc;
                  g_iCurMiscMenuItem = 0;
                  break;
               }
            }
            else if (g_InputState.dwKeyPress & kKeyDefend)
            {
               g_Battle.UI.wActionType = kBattleActionDefend;
               PAL_BattleCommitAction();
            }
            else if (g_InputState.dwKeyPress & kKeyForce)
            {
            }
            else if (g_InputState.dwKeyPress & kKeyFlee)
            {
               g_Battle.UI.wActionType = kBattleActionFlee;
               PAL_BattleCommitAction();
            }
            else if (g_InputState.dwKeyPress & kKeyUseItem)
            {
               g_Battle.UI.MenuState = kBattleMenuUseItemSelect;
               PAL_ItemSelectMenuInit(kItemFlagUsable);
            }
            else if (g_InputState.dwKeyPress & kKeyThrowItem)
            {
               g_Battle.UI.MenuState = kBattleMenuThrowItemSelect;
               PAL_ItemSelectMenuInit(kItemFlagThrowable);
            }
            else if (g_InputState.dwKeyPress & kKeyRepeat)
            {
            }
            else if (g_InputState.dwKeyPress & kKeyMenu)
            {
               g_Battle.rgPlayer[g_Battle.UI.wCurPlayerIndex].flTimeMeter = 100;
               g_Battle.rgPlayer[g_Battle.UI.wCurPlayerIndex].state = kFighterWait;
               g_Battle.UI.state = kBattleUIWait;
            }
            break;

         case kBattleMenuMagicSelect:
            w = PAL_MagicSelectionMenuUpdate();

            if (w != 0xFFFF)
            {
               g_Battle.UI.MenuState = kBattleMenuMain;

               if (w != 0)
               {
                  g_Battle.UI.wActionType = kBattleActionMagic;
                  g_Battle.UI.wObjectID = w;

                  if (gpGlobals->g.rgObject[w].magic.wFlags & kMagicFlagUsableToEnemy)
                  {
                     if (gpGlobals->g.rgObject[w].magic.wFlags & kMagicFlagApplyToAll)
                     {
                        g_Battle.UI.state = kBattleUISelectTargetEnemyAll;
                     }
                     else
                     {
                        g_Battle.UI.wSelectedIndex = g_Battle.UI.wPrevEnemyTarget;
                        g_Battle.UI.state = kBattleUISelectTargetEnemy;
                     }
                  }
                  else
                  {
                     if (gpGlobals->g.rgObject[w].magic.wFlags & kMagicFlagApplyToAll)
                     {
                        g_Battle.UI.state = kBattleUISelectTargetPlayerAll;
                     }
                     else
                     {
                        g_Battle.UI.wSelectedIndex = g_Battle.UI.wCurPlayerIndex;
                        g_Battle.UI.state = kBattleUISelectTargetPlayer;
                     }
                  }
               }
            }
            break;

         case kBattleMenuUseItemSelect:
            PAL_BattleUIUseItem();
            break;

         case kBattleMenuThrowItemSelect:
            PAL_BattleUIThrowItem();
            break;

         case kBattleMenuMisc:
            w = PAL_BattleUIMiscMenuUpdate();

            if (w != 0xFFFF)
            {
               g_Battle.UI.MenuState = kBattleMenuMain;

               switch (w)
               {
               case 1: // item
                  g_Battle.UI.MenuState = kBattleMenuMiscItemSubMenu;
                  g_iCurSubMenuItem = 0;
                  break;

               case 2: // defend
                  g_Battle.UI.wActionType = kBattleActionDefend;
                  PAL_BattleCommitAction();
                  break;

               case 3: // auto
                  g_Battle.UI.fAutoAttack = TRUE;
                  break;

               case 4: // flee
                  g_Battle.UI.wActionType = kBattleActionFlee;
                  PAL_BattleCommitAction();
                  break;

               case 5: // status
                  PAL_PlayerStatus();
                  break;
               }
            }
            break;

         case kBattleMenuMiscItemSubMenu:
            w = PAL_BattleUIMiscItemSubMenuUpdate();

            if (w != 0xFFFF)
            {
               g_Battle.UI.MenuState = kBattleMenuMain;

               switch (w)
               {
               case 1: // use
                  g_Battle.UI.MenuState = kBattleMenuUseItemSelect;
                  PAL_ItemSelectMenuInit(kItemFlagUsable);
                  break;

               case 2: // throw
                  g_Battle.UI.MenuState = kBattleMenuThrowItemSelect;
                  PAL_ItemSelectMenuInit(kItemFlagThrowable);
                  break;
               }
            }
            break;
         }
      }
      break;

   case kBattleUISelectTargetEnemy:
      x = -1;

      for (i = 0; i <= g_Battle.wMaxEnemyIndex; i++)
      {
         if (g_Battle.rgEnemy[i].wObjectID != 0)
         {
            x = i;
         }
      }

      if (x == -1)
      {
         g_Battle.UI.state = kBattleUISelectMove;
         break;
      }

      if (g_Battle.UI.wActionType == kBattleActionCoopMagic)
      {
         if (!PAL_BattleUIIsActionValid(kBattleActionCoopMagic))
         {
            g_Battle.UI.state = kBattleUISelectMove;
            break;
         }
      }

      if (g_Battle.UI.wSelectedIndex > x)
      {
         g_Battle.UI.wSelectedIndex = x;
      }

      for (i = 0; i <= x; i++)
      {
         if (g_Battle.rgEnemy[g_Battle.UI.wSelectedIndex].wObjectID != 0)
         {
            break;
         }
         g_Battle.UI.wSelectedIndex++;
         g_Battle.UI.wSelectedIndex %= x + 1;
      }

      //
      // Highlight the selected enemy
      //
      if ((SDL_GetTicks() / BATTLE_FRAME_TIME) & 1)
      {
         PAL_RLEBlitWithColorShift(PAL_SpriteGetFrame(g_Battle.rgEnemy[g_Battle.UI.wSelectedIndex].lpSprite, g_Battle.rgEnemy[g_Battle.UI.wSelectedIndex].wCurrentFrame),
            gpScreen, g_Battle.rgEnemy[g_Battle.UI.wSelectedIndex].pos, 7);
      }

      if (g_InputState.dwKeyPress & kKeyMenu)
      {
         g_Battle.UI.state = kBattleUISelectMove;
      }
      else if (g_InputState.dwKeyPress & kKeySearch)
      {
         g_Battle.UI.wPrevEnemyTarget = g_Battle.UI.wSelectedIndex;
         PAL_BattleCommitAction();
      }
      else if (g_InputState.dwKeyPress & (kKeyLeft | kKeyDown))
      {
         if (g_Battle.UI.wSelectedIndex != 0)
         {
            g_Battle.UI.wSelectedIndex--;
            while (g_Battle.UI.wSelectedIndex != 0 &&
               g_Battle.rgEnemy[g_Battle.UI.wSelectedIndex].wObjectID == 0)
            {
               g_Battle.UI.wSelectedIndex--;
            }
         }
      }
      else if (g_InputState.dwKeyPress & (kKeyRight | kKeyUp))
      {
         if (g_Battle.UI.wSelectedIndex < x)
         {
            g_Battle.UI.wSelectedIndex++;
            while (g_Battle.UI.wSelectedIndex < x &&
               g_Battle.rgEnemy[g_Battle.UI.wSelectedIndex].wObjectID == 0)
            {
               g_Battle.UI.wSelectedIndex++;
            }
         }
      }
      break;

   case kBattleUISelectTargetPlayer:
      j = SPRITENUM_BATTLE_ARROW_SELECTEDPLAYER;
      if ((SDL_GetTicks() / BATTLE_FRAME_TIME) & 1)
      {
         j = SPRITENUM_BATTLE_ARROW_SELECTEDPLAYER_RED;
      }

      //
      // Draw arrows on the selected player
      //
      x = g_rgPlayerPos[gpGlobals->wMaxPartyMemberIndex][g_Battle.UI.wSelectedIndex][0] - 8;
      y = g_rgPlayerPos[gpGlobals->wMaxPartyMemberIndex][g_Battle.UI.wSelectedIndex][1] - 67;

      PAL_RLEBlitToSurface(PAL_SpriteGetFrame(gpSpriteUI, j), gpScreen, PAL_XY(x, y));

      if (g_InputState.dwKeyPress & kKeyMenu)
      {
         g_Battle.UI.state = kBattleUISelectMove;
      }
      else if (g_InputState.dwKeyPress & kKeySearch)
      {
         PAL_BattleCommitAction();
      }
      else if (g_InputState.dwKeyPress & (kKeyLeft | kKeyDown))
      {
         if (g_Battle.UI.wSelectedIndex != 0)
         {
            g_Battle.UI.wSelectedIndex--;
         }
      }
      else if (g_InputState.dwKeyPress & (kKeyRight | kKeyUp))
      {
         if (g_Battle.UI.wSelectedIndex < gpGlobals->wMaxPartyMemberIndex)
         {
            g_Battle.UI.wSelectedIndex++;
         }
      }
      break;

   case kBattleUISelectTargetEnemyAll:
      if (g_Battle.UI.wActionType == kBattleActionCoopMagic)
      {
         if (!PAL_BattleUIIsActionValid(kBattleActionCoopMagic))
         {
            g_Battle.UI.state = kBattleUISelectMove;
            break;
         }
      }

      if ((SDL_GetTicks() / BATTLE_FRAME_TIME) & 1)
      {
         //
         // Highlight all enemies
         //
         for (i = g_Battle.wMaxEnemyIndex; i >= 0; i--)
         {
            if (g_Battle.rgEnemy[i].wObjectID == 0)
            {
               continue;
            }
            PAL_RLEBlitWithColorShift(PAL_SpriteGetFrame(g_Battle.rgEnemy[i].lpSprite, g_Battle.rgEnemy[i].wCurrentFrame),
               gpScreen, g_Battle.rgEnemy[i].pos, 7);
         }
      }
      if (g_InputState.dwKeyPress & kKeyMenu)
      {
         g_Battle.UI.state = kBattleUISelectMove;
      }
      else if (g_InputState.dwKeyPress & kKeySearch)
      {
         g_Battle.UI.wSelectedIndex = (WORD)-1;
         PAL_BattleCommitAction();
      }
      break;

   case kBattleUISelectTargetPlayerAll:
      j = SPRITENUM_BATTLE_ARROW_SELECTEDPLAYER;
      if ((SDL_GetTicks() / BATTLE_FRAME_TIME) & 1)
      {
         j = SPRITENUM_BATTLE_ARROW_SELECTEDPLAYER_RED;
      }
      for (i = 0; i <= gpGlobals->wMaxPartyMemberIndex; i++)
      {
         w = gpGlobals->g.rgObject[g_Battle.UI.wObjectID].magic.wMagicNumber;

         if (gpGlobals->g.lprgMagic[w].wType == kMagicTypeTransform)
         {
            if (i != g_Battle.UI.wCurPlayerIndex)
               continue;
         }

         //
         // Draw arrows on all players, despite of dead or not
         //
         x = g_rgPlayerPos[gpGlobals->wMaxPartyMemberIndex][i][0] - 8;
         y = g_rgPlayerPos[gpGlobals->wMaxPartyMemberIndex][i][1] - 67;

         PAL_RLEBlitToSurface(PAL_SpriteGetFrame(gpSpriteUI, j), gpScreen, PAL_XY(x, y));
      }

      if (g_InputState.dwKeyPress & kKeyMenu)
      {
         g_Battle.UI.state = kBattleUISelectMove;
      }
      else if (g_InputState.dwKeyPress & kKeySearch)
      {
         g_Battle.UI.wSelectedIndex = (WORD)-1;
         PAL_BattleCommitAction();
      }
      break;
   }

   //
   // Draw the numbers
   //
   for (i = 0; i < BATTLEUI_MAX_SHOWNUM; i++)
   {
      if (g_Battle.UI.rgShowNum[i].wTime > 0)
      {
         PAL_DrawNumber(g_Battle.UI.rgShowNum[i].wNum, 5, g_Battle.UI.rgShowNum[i].pos,
            g_Battle.UI.rgShowNum[i].color, kNumAlignMid);

         g_Battle.UI.rgShowNum[i].wTime--;
         g_Battle.UI.rgShowNum[i].pos =
            PAL_XY(PAL_X(g_Battle.UI.rgShowNum[i].pos), PAL_Y(g_Battle.UI.rgShowNum[i].pos) - 1);
      }
   }
}

VOID
PAL_BattleUIShowNum(
   WORD           wNum,
   PAL_POS        pos,
   NUMCOLOR       color
)
/*++
  Purpose:

    Show a number on battle screen (indicates HP/MP change).

  Parameters:

    [IN]  wNum - number to be shown.

    [IN]  pos - position of the number on the screen.

    [IN]  color - color of the number.

  Return value:

    None.

--*/
{
   int i;

   for (i = 0; i < BATTLEUI_MAX_SHOWNUM; i++)
   {
      if (g_Battle.UI.rgShowNum[i].wTime == 0)
      {
         g_Battle.UI.rgShowNum[i].wNum = wNum;
         g_Battle.UI.rgShowNum[i].pos = pos;
         g_Battle.UI.rgShowNum[i].color = color;
         g_Battle.UI.rgShowNum[i].wTime = 10;

         break;
      }
   }
}
