/********************************************************************

   wfhelp.h

   Copyright (c) Microsoft Corporation. All rights reserved.
   Licensed under the MIT License.

********************************************************************/

//
//
// Help Context String map:
//
// begin: 5000          IDM_*
//         ..
//        5000 + 2000   IDH_SYSMENU
//        5000 + 2001   MB_*  (message boxes)
//        5000 + 2099   IDH_SYSMENUCHILD
//        5000 + 3000   DLGs
//

#define IDH_HELPFIRST      5000
#define IDH_SYSMENU        (IDH_HELPFIRST + 2000)

#define IDH_MBFIRST           (IDH_HELPFIRST + 2001)
#define IDH_MB_FORMATCURERR   (IDH_MBFIRST + 0)
#define IDH_ASSOC_BROWSE      (IDH_MBFIRST + 1)
#define IDH_FONT              (IDH_MBFIRST + 3)
#define IDH_MBLAST            (IDH_HELPFIRST + 2098)

#define IDH_SYSMENUCHILD      (IDH_HELPFIRST + 2099)


#define IDH_DLGFIRST          (IDH_HELPFIRST + 3000)

#define IDH_DLG_PREV             (IDH_DLGFIRST + PREVIOUSDLG)
#define IDH_DLG_CONFIRMREPLACE   (IDH_DLGFIRST + CONFIRMREPLACE)
#define IDH_DLG_CONFIRMRMDIR     (IDH_DLGFIRST + CONFIRMRMDIR)
#define IDH_DLG_CONFIRMRENAME    (IDH_DLGFIRST + CONFIRMRENAME)
#define IDH_DLG_CONFIRMMOVE      (IDH_DLGFIRST + CONFIRMMOVE)
#define IDH_DLG_CONFIRMDEL       (IDH_DLGFIRST + CONFIRMDELETE)
#define IDH_DLG_LFNTOFATDLG (IDH_DLGFIRST + LFNTOFATDLG)
#define IDH_DLG_ASSOCIATEFILEDLG       (IDH_DLGFIRST + ASSOCIATEFILEDLG)
#define IDH_DLG_ASSOCIATEFILEDLGCONFIG (IDH_DLGFIRST + ASSOCIATEFILEDLGCONFIG)
#define IDH_FILE  (IDM_FILE + IDH_HELPFIRST)
#define IDH_OPEN  (IDM_OPEN + IDH_HELPFIRST)
#define IDH_PRINT    (IDM_PRINT + IDH_HELPFIRST)
#define IDH_ASSOCIATE   (IDM_ASSOCIATE + IDH_HELPFIRST)
#define IDH_SEARCH   (IDM_SEARCH + IDH_HELPFIRST)
#define IDH_RUN   (IDM_RUN  + IDH_HELPFIRST)
#define IDH_MOVE  (IDM_MOVE + IDH_HELPFIRST)
#define IDH_COPY  (IDM_COPY + IDH_HELPFIRST)
#define IDH_DELETE   (IDM_DELETE + IDH_HELPFIRST)
#define IDH_RENAME   (IDM_RENAME + IDH_HELPFIRST)
#define IDH_ATTRIBS  (IDM_ATTRIBS + IDH_HELPFIRST)
#define IDH_MAKEDIR  (IDM_MAKEDIR + IDH_HELPFIRST)
#define IDH_SELALL   (IDM_SELALL + IDH_HELPFIRST)
#define IDH_DESELALL    (IDM_DESELALL + IDH_HELPFIRST)
#define IDH_UNDO  (IDM_UNDO + IDH_HELPFIRST)
#define IDH_EXIT  (IDM_EXIT + IDH_HELPFIRST)
#define IDH_SELECT   (IDM_SELECT + IDH_HELPFIRST)
#define IDH_UNDELETE    (IDM_UNDELETE + IDH_HELPFIRST)
#define IDH_COPYTOCLIPBOARD (IDM_COPYTOCLIPBOARD + IDH_HELPFIRST)
#define IDH_COMPRESS  (IDM_COMPRESS + IDH_HELPFIRST)
#define IDH_UNCOMPRESS  (IDM_UNCOMPRESS + IDH_HELPFIRST)
#define IDH_GROUP_ATTRIBS  (IDM_GROUP_ATTRIBS + IDH_HELPFIRST)
#define IDH_DISK  (IDM_DISK + IDH_HELPFIRST)
#define IDH_DISKCOPY    (IDM_DISKCOPY + IDH_HELPFIRST)
#define IDH_LABEL    (IDM_LABEL + IDH_HELPFIRST)
#define IDH_FORMAT   (IDM_FORMAT + IDH_HELPFIRST)
#define IDH_SYSDISK  (IDM_SYSDISK + IDH_HELPFIRST)
#define IDH_CONNECT  (IDM_CONNECT + IDH_HELPFIRST)
#define IDH_DISCONNECT  (IDM_DISCONNECT + IDH_HELPFIRST)
#define IDH_CONNECTIONS (IDM_CONNECTIONS + IDH_HELPFIRST)
#define IDH_DRIVESMORE  (IDM_DRIVESMORE + IDH_HELPFIRST)

#define IDH_SHAREAS  (IDM_SHAREAS + IDH_HELPFIRST)
#define IDH_STOPSHARE   (IDM_STOPSHARE + IDH_HELPFIRST)

#define IDH_TREE  (IDM_TREE + IDH_HELPFIRST)
#define IDH_EXPONE   (IDM_EXPONE + IDH_HELPFIRST)
#define IDH_EXPSUB   (IDM_EXPSUB + IDH_HELPFIRST)
#define IDH_EXPALL   (IDM_EXPALL + IDH_HELPFIRST)
#define IDH_COLLAPSE    (IDM_COLLAPSE + IDH_HELPFIRST)
#define IDH_ADDPLUSES   (IDM_ADDPLUSES + IDH_HELPFIRST)

#define IDH_VIEW  (IDM_VIEW + IDH_HELPFIRST)
#define IDH_VNAME    (IDM_VNAME + IDH_HELPFIRST)
#define IDH_VDETAILS    (IDM_VDETAILS + IDH_HELPFIRST)
#define IDH_VOTHER   (IDM_VOTHER + IDH_HELPFIRST)
#define IDH_BYNAME   (IDM_BYNAME + IDH_HELPFIRST)
#define IDH_BYTYPE   (IDM_BYTYPE + IDH_HELPFIRST)
#define IDH_BYSIZE   (IDM_BYSIZE + IDH_HELPFIRST)
#define IDH_BYDATE   (IDM_BYDATE + IDH_HELPFIRST)
#define IDH_VINCLUDE    (IDM_VINCLUDE + IDH_HELPFIRST)
#define IDH_REPLACE  (IDM_REPLACE + IDH_HELPFIRST)
#define IDH_TREEONLY    (IDM_TREEONLY + IDH_HELPFIRST)
#define IDH_DIRONLY  (IDM_DIRONLY + IDH_HELPFIRST)
#define IDH_BOTH  (IDM_BOTH + IDH_HELPFIRST)
#define IDH_SPLIT    (IDM_SPLIT + IDH_HELPFIRST)

#define IDH_OPTIONS  (IDM_OPTIONS + IDH_HELPFIRST)
#define IDH_CONFIRM  (IDM_CONFIRM + IDH_HELPFIRST)
#define IDH_FONT_MENU   (IDM_FONT + IDH_HELPFIRST)
#define IDH_CTBAR    (IDM_TOOLBARCUST + IDH_HELPFIRST)
#define IDH_TOOLBAR  (IDM_TOOLBAR + IDH_HELPFIRST)
#define IDH_DRIVEBAR    (IDM_DRIVEBAR + IDH_HELPFIRST)
#define IDH_TABBAR    (IDM_TABBAR + IDH_HELPFIRST)
#define IDH_STATUSBAR   (IDM_STATUSBAR + IDH_HELPFIRST)
#define IDH_MINONRUN    (IDM_MINONRUN + IDH_HELPFIRST)
#define IDH_INDEXONLAUNCH  (IDM_INDEXONLAUNCH + IDH_HELPFIRST)
#define IDH_SAVESETTINGS   (IDM_SAVESETTINGS + IDH_HELPFIRST)

#define IDH_EXTENSIONS  (IDM_EXTENSIONS + IDH_HELPFIRST)

#define IDH_WINDOW   (IDM_WINDOW + IDH_HELPFIRST)
#define IDH_CASCADE  (IDM_CASCADE + IDH_HELPFIRST)
#define IDH_TILEH    (IDM_TILEHORIZONTALLY + IDH_HELPFIRST)
#define IDH_TILEV    (IDM_TILE + IDH_HELPFIRST)
#define IDH_REFRESH  (IDM_REFRESH + IDH_HELPFIRST)
#define IDH_CLOSEALL    (IDM_CLOSEALL + IDH_HELPFIRST)
#define IDH_CHILDSTART  (IDM_CHILDSTART + IDH_HELPFIRST)
#define IDH_SAVE  (IDM_SAVE + IDH_HELPFIRST)
#define IDH_ARRANGE  (IDM_ARRANGE + IDH_HELPFIRST)
#define IDH_NEWWINDOW   (IDM_NEWWINDOW + IDH_HELPFIRST)
#define IDH_ONWOCON  (IDM_NEWWINONCONNECT + IDH_HELPFIRST)

#define IDH_HELPGENERAL  (IDM_HELP + IDH_HELPFIRST)
#define IDH_HELPINDEX   (IDM_HELPINDEX + IDH_HELPFIRST)
#define IDH_HELPKEYS    (IDM_HELPKEYS + IDH_HELPFIRST)
#define IDH_HELPCOMMANDS   (IDM_HELPCOMMANDS + IDH_HELPFIRST)
#define IDH_HELPPROCS   (IDM_HELPPROCS + IDH_HELPFIRST)
#define IDH_HELPHELP    (IDM_HELPHELP + IDH_HELPFIRST)
#define IDH_ABOUT    (IDM_ABOUT + IDH_HELPFIRST)

#define IDH_LOWERCASE   (IDM_LOWERCASE + IDH_HELPFIRST)
#define IDH_EXPANDTREE  (IDM_EXPANDTREE + IDH_HELPFIRST)
