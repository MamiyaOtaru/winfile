/********************************************************************

   wftabs.c

   window procs and other stuff for the tab bar

   Copyright (c) Microsoft Corporation. All rights reserved.
   Licensed under the MIT License.

********************************************************************/

#define PUBLIC           // avoid collision with shell.h
#include "winfile.h"
#include "treectl.h"
#include "lfn.h"
#include "wfcopy.h"
#include <commctrl.h>
#include <shlobj.h>

VOID RectTab(TABIND nTab, BOOL bFocusOn);
VOID InvalidateTab(TABIND tabInd);
INT  TabFromPoint(HWND hwnd, POINT pt);
VOID DrawTab(HDC hdc, INT x, INT y, TABIND tabInd, BOOL bCurrent, BOOL bFocus);
INT  KeyToTab(HWND hWnd, WORD nDriveLetter);
VOID NewTabDouble(TABIND tabInd, HWND hWnd);
VOID NewTab(TABIND tabInd, HWND hWnd);
INT GetTabOffset(boolean active);
WCHAR* rev_nth_strrchr(WCHAR* s, WCHAR c, int n);

INT currentTabRows = 1;
INT nTabCurrent = 0;

VOID
ResetTabInfo()
{
    PTABINFO pTabInfo;
    TAB tab;

    WCHAR szTemp[MAXPATHLEN * 2 + 10];  // BONK!  is this big enough?

    //
    // Initialize the count of tabs.
    //
    cTabs = 0;
    tab = 0;

    HDC hdc = GetDC(hwndTabBar);
    SIZE size;

    HWND activeHwnd = (HWND)SendMessage(hwndMDIClient, WM_MDIGETACTIVE, 0, 0L);

    for (HWND hwnd = GetWindow(hwndMDIClient, GW_CHILD); hwnd; hwnd = GetWindow(hwnd, GW_HWNDNEXT))
    {
        tab = GetDlgCtrlID(hwnd) - IDM_CHILDSTART;
        pTabInfo = &aTabInfo[tab];
        rgiTab[cTabs] = tab;
        pTabInfo->hwnd = hwnd;

        GetMDIWindowText(hwnd, szTemp, COUNTOF(szTemp));
        WCHAR origString[MAXPATHLEN];
        int len = wcslen(szTemp);
        wcscpy_s(origString, len + 1, szTemp);
        WCHAR* endString = rev_nth_strrchr(origString, (WCHAR)'\\', 2);
        int chars = wcslen(endString);
        int width = 1024;
        do {
            GetTextExtentPoint32(hdc, endString, chars, &size);
            width = size.cx;
            if (width > dxTab) {
                chars--;
            }
        } while (width > dxTab);

        wcsncpy_s(pTabInfo->szName, COUNTOF(pTabInfo->szName), endString, chars);
        pTabInfo->index = tab;
        if (pTabInfo->hwnd == activeHwnd) {
            nTabCurrent = pTabInfo->index;
        }
        pTabInfo->iOffset = GetTabOffset(tab == nTabCurrent);
        cTabs++;
    }

    InvalidateRect(hwndTabBar, NULL, TRUE);
   // EnumChildWindows(hwndMDIClient, EnumWindowsProc, 0);
    ReleaseDC(hwndTabBar, hdc);
}

WCHAR*
rev_nth_strrchr(WCHAR* s, WCHAR c, int n)
{
    WCHAR* nth = s;
    int len = wcslen(s);
    int matches = 0;

    int i = 0;
    for (i = len - 1; i >= 0 && matches < n; i--)
    {
        if (s[i] == c) {
            matches++;
        }
    }
    i++; // undo the last decrement that happens
    if (matches == n) {
        i++; // get starting past the last match
    }
    wcsncpy_s(nth, len+1, s + i, len-i);
    return nth;
}

BOOL 
CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    char class_name[100];
    char title[100];

    GetClassNameA(hwnd, class_name, sizeof(class_name));
    GetWindowTextA(hwnd, title, sizeof(title));

    //cout << "Window name : " << title << endl;
    //cout << "Class name  : " << class_name << endl;
    //cout << "hwnd        : " << hwnd << endl << endl;

    return TRUE;
}

INT
GetTabOffset(boolean active)
{
    int iBitmap = 0;
    if (active) {
        iBitmap = BM_IND_OPEN;
    }
    else {
        iBitmap = BM_IND_CLOSE;
    }
    return dxFolder * iBitmap;
}

VOID
GetTabRect(TABIND tabInd, PRECT prc)
{
   RECT rc;
   INT nTabsPerRow;

   GetClientRect(hwndTabBar, &rc);

   if (!dxTab)           // avoid div by zero
      dxTab++;

   nTabsPerRow = rc.right / dxTab;

   if (!nTabsPerRow)     // avoid div by zero
       nTabsPerRow++;

   prc->top = dyTab * (tabInd / nTabsPerRow);
   prc->bottom = prc->top + dyTab;

   prc->left = dxTab * (tabInd % nTabsPerRow);
   prc->right = prc->left + dxTab;
}

INT
TabFromPoint(HWND hwnd, POINT pt)
{
   RECT rc, rcTab;
   INT x, y;

   TABIND tabInd;

   if (!bTabBar || hwnd != hwndTabBar)
      return -1;

   GetClientRect(hwndTabBar, &rc);

   x = 0;
   y = 0;
   tabInd = 0;

   for (tabInd = 0; tabInd < cTabs; tabInd++) {
      rcTab.left = x;
      rcTab.right = x + dxTab;
      rcTab.top = y;
      rcTab.bottom = y + dyTab;
      InflateRect(&rcTab, -dyBorder, -dyBorder);

      if (PtInRect(&rcTab, pt))
         return tabInd;

      x += dxTab;

      if (x + dxTab > rc.right) {
         x = 0;
         y += dyTab;
      }
   }

   return -1;      // no hit
}

VOID
InvalidateTab(TABIND tabInd)
{
   RECT rc;

   //
   // Get out early if the tabbar is not visible.
   //
   if (!bTabBar)
      return;

   GetTabRect(tabInd, &rc);
   InvalidateRect(hwndTabBar, &rc, TRUE);
}


//
// void NEAR PASCAL RectTab(TABIND tabInd, BOOL bDraw)
//
// draw the highlight rect around the tab to indicate that it is
// the target of a drop action. // TODO drag to tabs?
//
// in:
//      tabInd  the tab to draw the rect around
//      bDraw   if TRUE, draw a rect around this tab
//              FALSE, erase the rect (draw the default rect)
//

VOID
RectTab(TABIND tabInd, BOOL bDraw)
{
   RECT rc, rcTab;
   HBRUSH hBrush;
   HDC hdc;

   GetTabRect(tabInd, &rc);
   rcTab = rc;
   InflateRect(&rc, -dyBorder, -dyBorder);

   if (bDraw) {

      hdc = GetDC(hwndTabBar);

      if (hBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOWTEXT))) {
         FrameRect(hdc, &rc, hBrush);
         DeleteObject(hBrush);
      }

      ReleaseDC(hwndTabBar, hdc);

   } else {
      InvalidateRect(hwndTabBar, &rcTab, TRUE);
      UpdateWindow(hwndTabBar);
   }
}

//
// void DrawTab(HDC hdc, int x, int y, int tabInd, BOOL bCurrent, BOOL bFocus)
//
// paint the tab icons in the standard state, given the
// tab with the focus and the current selection
//
// in:
//      hdc             dc to draw to
//      x, y            position to start (dxTab, dyTab are the extents)
//      tabInd          the tab to paint
//      bCurrent        draw as the current tab (pushed in)
//      bFocus          draw with the focus
//

VOID
DrawTab(HDC hdc, INT x, INT y, TABIND tabInd, BOOL bCurrent, BOOL bFocus)
{
   RECT rc;
   DWORD rgb;
   TAB tab = 0;
   for (int nTab = 0; nTab < cTabs; nTab++) {
       if (aTabInfo[nTab].index == tabInd) {
           tab = nTab;
       }
   }

   rc.left = x;
   rc.right = x + dxTab;
   rc.top = y;
   rc.bottom = y + dyTab;

   rgb = GetSysColor(COLOR_BTNTEXT);

   if (bCurrent) {
      HBRUSH hbr;

      if (hbr = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT))) {
         if (bFocus) {
            rgb = GetSysColor(COLOR_HIGHLIGHTTEXT);
            FillRect(hdc, &rc, hbr);
         } else {
            InflateRect(&rc, -dyBorder, -dyBorder);
            FrameRect(hdc, &rc, hbr);
         }
         DeleteObject(hbr);
      }
   }

   if (bFocus)
      DrawFocusRect(hdc, &rc);

   SetBkMode(hdc, TRANSPARENT);

   rgb = SetTextColor(hdc, rgb);
   TextOut(hdc, x + dxFolder+(dyBorder*6), y + (dyTab - dyText) / 2, aTabInfo[tab].szName, wcslen(aTabInfo[tab].szName));
   SetTextColor(hdc, rgb);

   BitBlt(hdc, x + 4*dyBorder, y + (dyTab - dyFolder) / 2, dxFolder, dyFolder,
      hdcMem, GetTabOffset(tab == nTabCurrent), 0, SRCCOPY | NOMIRRORBITMAP);
}

VOID
TabsDropObject(HWND hWnd, LPDROPSTRUCT lpds)
{
    TABIND tabInd;
    TCHAR szPath[MAX_FILESYSNAME];
    LPTSTR pFrom;
    BOOL bIconic;
    HWND hwndChild;

    hwndChild = hwndDropChild ? hwndDropChild :
    (HWND)SendMessage(hwndMDIClient, WM_MDIGETACTIVE, 0, 0L);

    bIconic = IsIconic(hwndChild);

    if (bIconic) {
UseCurDir:
      SendMessage(hwndChild, FS_GETDIRECTORY, COUNTOF(szPath), (LPARAM)szPath);
    } else {

      tabInd = TabFromPoint(lpds->hwndSink, lpds->ptDrop);

      if (tabInd < 0)
          goto UseCurDir;
      // this searches windows in the zorder then asks dos
      // if nothing is found...

    }
    // ** jay TODO close button on tabs?
}


VOID
TabsPaint(HWND hWnd, INT nTabFocus, INT nTabCurrent)
{
   RECT rc;
   INT nTab;

   HDC hdc;
   PAINTSTRUCT ps;

   INT x, y;
   HANDLE hOld;
   INT cTabRows, cTabsPerRow;

   GetClientRect(hWnd, &rc);

   hdc = BeginPaint(hWnd, &ps);

   if (!rc.right) {

      EndPaint(hWnd, &ps);
      return;
   }

   hOld = SelectObject(hdc, hFont);

   cTabsPerRow = rc.right / dxTab;

   if (!cTabsPerRow)
       cTabsPerRow++;

   x = 0;
   y = 0;
   for (nTab = 0; nTab < cTabs; nTab++) {

      if (GetFocus() != hWnd)
         nTabFocus = -1;

      DrawTab(hdc, x, y, nTab, nTabCurrent == nTab, nTabFocus == nTab);
      x += dxTab;

      if (x + dxTab > rc.right) {
         x = 0;
         y += dyTab;
      }
   }

   if (hOld)
      SelectObject(hdc, hOld);

   EndPaint(hWnd, &ps);

   cTabRows = ((cTabs - 1) / cTabsPerRow) + 1;
   if (currentTabRows != cTabRows) {
       MDIClientSizeChange(hwndFrame, TABBAR_FLAG);
       InvalidateRect(hwndTabBar, NULL, TRUE); // keeps tab bar from being fuzzy
   }
   currentTabRows = cTabRows;
}

//
// select proper MDI window (dbl - same as sinlge for now
//

VOID
NewTabDouble(TABIND tabInd, HWND hWnd)
{
    NewTab(tabInd, hWnd);
}

//
// select proper MDI window 
//

VOID
NewTab(TABIND tabInd, HWND hWnd)
{
    nTabCurrent = tabInd;
    DefFrameProc(hwndFrame, hwndMDIClient, WM_COMMAND, (&aTabInfo[tabInd])->index + IDM_CHILDSTART, 0);
    
    InvalidateRect(hWnd, NULL, TRUE);
}


/*--------------------------------------------------------------------------*/
/*                                                                          */
/*  TabsWndProc() -                                                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/

LRESULT
CALLBACK
TabsWndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
  INT nTab, nTabFocus;
  RECT rc;
  HWND hwndChild;

  hwndChild = (HWND)SendMessage(hwndMDIClient, WM_MDIGETACTIVE, 0, 0L);

  nTabFocus = (INT)GetWindowLongPtr(hWnd, GWL_CURTABFOCUS);

  switch (wMsg) {
      case WM_CREATE:
          {
          INT i;

          // Find the current tab, set the tab bitmaps

          if (hwndChild == 0)
             nTab = 0;
          else
             nTab = (INT)GetWindowLongPtr(hwndChild, GWL_TYPE);




          for (i=0; i < cTabs; i++) {

              if (rgiTab[i] == nTab) {
                  SetWindowLongPtr(hWnd, GWL_CURTABIND, i);
                  SetWindowLongPtr(hWnd, GWL_CURTABFOCUS, i);
              }

          }
          break;
          }

      case WM_VKEYTOITEM:
          KeyToTab(hWnd, (WORD)wParam);
          return -2L;
          break;

      case WM_KEYDOWN:
          switch (wParam) {

          case VK_ESCAPE:
                bCancelTree = TRUE;
                break;

          case VK_F6:   // like excel
          case VK_TAB:  
                {
                   HWND hwndTree, hwndDir, hwndSet, hwndNext;
                   BOOL bDir;
                   BOOL bChangeDisplay = FALSE;

                   GetTreeWindows(hwndChild, &hwndTree, &hwndDir);
                  
                   // Check to see if we can change to the directory window
                  
                   bDir = hwndDir != NULL;
                   if (bDir)
                   {
                      HWND hwndLB;

                      bChangeDisplay = (BOOL)GetWindowLongPtr(hwndDir, GWLP_USERDATA);

                      hwndLB = GetDlgItem (hwndDir, IDCW_LISTBOX);
                      if (hwndLB && !bChangeDisplay)
                      {
                         PVOID pv;
                         SendMessage (hwndLB, LB_GETTEXT, 0, (LPARAM) &pv);
                         bDir = pv != NULL;
                      }
                   }

                   if (GetKeyState(VK_SHIFT) < 0)
                   {
                      hwndTree = (!hwndTree) ? hWnd : hwndTree;

                      if (bDir)
                      {
                         hwndSet = hwndDir;
                         hwndNext = hwndTree;
                      }
                      else
                      {
                         hwndSet = hwndTree;
                      }
                   }
                   else
                   {
                      hwndSet = hwndTree ? hwndTree : (bDir ? hwndDir : hWnd);
                      hwndNext = hWnd;
                   }

                   SetFocus(hwndSet);
                   if ((hwndSet == hwndDir) && bChangeDisplay)
                   {
                       SetWindowLongPtr(hwndDir, GWL_NEXTHWND, (LPARAM)hwndNext);
                   }

                   break;
                }

          case VK_RETURN:               // same as single click
                NewTab(rgiTab[nTabFocus], hWnd);
                break;

          case VK_SPACE:                // same as single click

                NewTab(rgiTab[nTabFocus], hWnd);
                break;

          case VK_LEFT:
                nTab = max(nTabFocus-1, 0);
                break;

          case VK_RIGHT:
                nTab = min(nTabFocus+1, cTabs-1);
                break;
          }

          if ((wParam == VK_LEFT) || (wParam == VK_RIGHT)) {

                SetWindowLongPtr(hWnd, GWL_CURTABFOCUS, nTab);

                GetTabRect(nTabFocus, &rc);
                InvalidateRect(hWnd, &rc, TRUE);
                GetTabRect(nTab, &rc);
                InvalidateRect(hWnd, &rc, TRUE);
          } else if ((wParam >= CHAR_A) && (wParam <= CHAR_Z))
                KeyToTab(hWnd, (WORD)wParam);

          break;

      case FS_GETTAB:
          {
              POINT pt;

              POINTSTOPOINT(pt, lParam);
              nTab = TabFromPoint(hwndTabBar, pt);

              if (nTab < 0)
                  nTab = nTabCurrent;

              return rgiTab[nTab] + CHAR_A;
          }

      case WM_QUERYDROPOBJECT:
          /* Validate the format. */
          #define lpds ((LPDROPSTRUCT)lParam)

          // if (TabFromPoint(lpds->ptDrop) < 0)
          //    return FALSE;

          switch (lpds->wFmt) {
          case DOF_EXECUTABLE:
          case DOF_DIRECTORY:
          case DOF_MULTIPLE:
          case DOF_DOCUMENT:
                return(TRUE);
          default:
                return FALSE;
          }
          break;

      case WM_DROPOBJECT:
          // Turn off the status bar
          SendMessage(hwndStatus, SB_SIMPLE, 0, 0L);

          UpdateWindow(hwndStatus);

          TabsDropObject(hWnd, (LPDROPSTRUCT)lParam);
          return TRUE;

      case WM_SETFOCUS:
          SetWindowLongPtr(hwndChild, GWL_LASTFOCUS, (LPARAM)hWnd);
          // fall through

      case WM_KILLFOCUS:

          InvalidateTab(nTabFocus);
          break;

      case WM_PAINT:
          TabsPaint(hWnd, nTabFocus, nTabCurrent);
          break;


      case WM_MDIACTIVATE:
          //nTabDoubleClick = -1; // invalidate any cross window drive actions

        break;


      case WM_LBUTTONDOWN:
        {
           POINT pt;

           POINTSTOPOINT(pt, lParam);
           SetCapture(hWnd);  // make sure we see the WM_LBUTTONUP
           nTab = TabFromPoint(hwndTabBar, pt);

           // provide instant user feedback
           if (nTab >= 0) {
               RectTab(nTab, TRUE);
               SendMessage(hWnd, FS_SETTAB, nTab, 1L);
           }
        }
        break;

      case WM_LBUTTONUP:
        {
            POINT pt;

            POINTSTOPOINT(pt, lParam);

            ReleaseCapture();
            nTab = TabFromPoint(hwndTabBar, pt);
      }
          break;


      case FS_SETTAB:
          NewTab((TABIND)wParam, hWnd);

          break;


      default:
          return DefWindowProc(hWnd, wMsg, wParam, lParam);
  }

  return 0L;
}

/* Returns nDrive if found, else -1 */
INT
KeyToTab(HWND hWnd, WORD nTabNum)
{
    INT nTab;

    if (nTabNum > CHAR_9)
        nTabNum = CHAR_9;
    if (nTabNum < CHAR_0)
        nTabNum = CHAR_0;

    for (nTab = 0; nTab < cTabs; nTab++) {
        if (rgiTab[nTab] == (int)nTabNum) {

           // If same drive, don't steal
           SendMessage(hWnd, FS_SETTAB, nTab, 1L);
           return nTab;
        }
    }
    return -1;
}
