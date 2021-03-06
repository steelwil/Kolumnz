//
//  Copyright (C) 2005 - William Bell 
//
//  This file is part of Kolumnz
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
//  Coded by William Bell
//  email: william.bell@absamail.co.za
//  Home Page: http://myweb.absamail.co.za/william.bell/
//
// NewGameDlg.h: interface for the CNewGameDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWGAMEDLG_H__B4856E25_D68D_11D4_9676_0080C87F47A1__INCLUDED_)
#define AFX_NEWGAMEDLG_H__B4856E25_D68D_11D4_9676_0080C87F47A1__INCLUDED_

class CNewGameDlg : public CDialogImpl<CNewGameDlg>, public CWinDataExchange<CNewGameDlg>  
{
public:
	enum { IDD = IDD_NEWGAME_DIALOG };

	CUpDownCtrl	m_HeightSpin;
	CUpDownCtrl	m_BaseSpin;
	int		m_Height;
	int		m_Base;
	CUpDownCtrl	m_ColorSpin;
	CUpDownCtrl	m_LengthSpin;
	int m_Length;
	int m_Colors;
	CString	m_Title;

	CKolumnzDoc* m_pDoc;

	BEGIN_MSG_MAP(CNewGameDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOpenSaveDlg)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CCustomizeDlg)
		DDX_TEXT(IDC_TITLE_EDIT, m_Title)
		DDX_INT(IDC_BASE_EDIT, m_Base)
		DDX_INT(IDC_HEIGHT_EDIT, m_Height)
		DDX_INT(IDC_LENGTH_EDIT, m_Length)
		DDX_INT(IDC_COLORS_EDIT, m_Colors)
	END_DDX_MAP()

	CNewGameDlg(CKolumnzDoc* pDoc)
	{
		m_pDoc = pDoc;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());

		m_HeightSpin.Attach(GetDlgItem(IDC_HEIGHT_SPIN));
		m_HeightSpin.SetRange(5, 32);
		m_Height = m_pDoc->m_Game.m_Height;

		m_BaseSpin.Attach(GetDlgItem(IDC_BASE_SPIN));
		m_BaseSpin.SetRange(5, 32);
		m_Base = m_pDoc->m_Game.m_Base;

		m_LengthSpin.Attach(GetDlgItem(IDC_LENGTH_SPIN));
		m_LengthSpin.SetRange(2, 5);
		m_Length = m_pDoc->m_Game.m_BlkSize;

		m_ColorSpin.Attach(GetDlgItem(IDC_COLORS_SPIN));
		m_ColorSpin.SetRange(2,15);
		m_Colors = m_pDoc->m_Game.m_Colors;
		
		m_Title = "My New Kolumnz Game";
		int blkType = m_pDoc->m_Game.m_Colors;
		int blkSize = 0;
		DoDataExchange(FALSE);
		return TRUE;
	}

	LRESULT OnOpenSaveDlg(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CFileDlg dlg(FALSE);
		dlg.m_ofn.lpstrTitle = "Save Game";
		if (dlg.DoModal() == IDOK)
		{
			m_pDoc->OpenNewGameFile(dlg.m_szFileName);

			DoDataExchange(TRUE);
			
			strncpy(m_pDoc->m_Game.m_Comment, m_Title, 31);
			m_pDoc->m_Game.m_Comment[31] = 0;
			m_pDoc->m_Game.m_Base = m_Base;
			m_pDoc->m_Game.m_Height = m_Height;
			
			m_pDoc->SaveGameFile();
			// Update the main form's title & MRU list
			//::PostMessage(GetParent(), WM_NEWGAME, NULL, NULL);
			EndDialog(wID);
		}
		return 0;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

};

#endif // !defined(AFX_NEWGAMEDLG_H__B4856E25_D68D_11D4_9676_0080C87F47A1__INCLUDED_)
