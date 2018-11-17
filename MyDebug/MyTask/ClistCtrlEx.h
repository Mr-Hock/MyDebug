#pragma once


// ClistCtrlEx

class ClistCtrlEx : public CListCtrl
{
	DECLARE_DYNAMIC(ClistCtrlEx)

public:
	ClistCtrlEx();
	virtual ~ClistCtrlEx();

	DWORD GetSelectionSubItem()
	{
		LVHITTESTINFO nListInfo{};
		DWORD   nPos = GetMessagePos();
		CPoint   nPoint(LOWORD(nPos), HIWORD(nPos));
		ScreenToClient(&nPoint);
		nListInfo.pt = nPoint;
		nListInfo.flags = LVHT_ABOVE;
		SubItemHitTest(&nListInfo);

		return nListInfo.iSubItem;
	}

	VOID SetSelectionEx(DWORD nItem)
	{
		SendMessage(LVM_SETSELECTIONMARK, 0, nItem);
		SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		//ѡ��ʱ�ڶ�������ֵΪ1��ȡ��ѡ��ʱ�ڶ�������Ϊ0
	}
	DWORD GetSelectionItem()
	{
		return SendMessage(LVM_GETSELECTIONMARK, 0, 0);
	}

	BOOL IsSelectionItem(DWORD nItem)
	{
		return SendMessage(LVM_GETITEMSTATE, nItem, 2) == 2;
	}

	BOOL SetItemImageEx(int nItem, int nSubItem, CString lpszItem, CString nImage)
	{
		int nTempImage;
		_stscanf_s(nImage, TEXT("%d"), &nTempImage);
		return CListCtrl::SetItem(nItem, nSubItem, 2, lpszItem, nTempImage, 0, 0, 0);
	}
	BOOL SetItemImageEx(int nItem, int nSubItem, CString lpszItem, INT nImage)
	{
		//							��		��		  ��ʾ�ı�  ͼƬ����
		return CListCtrl::SetItem(nItem, nSubItem, 2, lpszItem, nImage, 0, 0, 0);
	}

	//INT GetSelectEx()
	//{
	//	INT nIndex;
	//	POSITION nListPos = { 0 };
	//	nListPos = GetFirstSelectedItemPosition();
	//	nIndex = GetNextSelectedItem(nListPos);
	//	return nIndex;
	//}


	CString GetItemDataStr(int nItem)
	{
		CString nTemp;
		nTemp.Format(TEXT("%d"), CListCtrl::GetItemData(nItem));
		return nTemp;
	}


	BOOL SetItemData(int nItem, CString szData)
	{
		DWORD dwData = 0;
		_stscanf_s(szData, TEXT("%d"), &dwData);
		return CListCtrl::SetItemData(nItem, dwData);
	}

	BOOL SetItemData(int nItem, DWORD_PTR dwData)
	{
		return CListCtrl::SetItemData(nItem, dwData);
	}

	BOOL SetItemText(_In_ int nItem, _In_ int nSubItem, _In_z_ LPCTSTR lpszText)
	{
		return CListCtrl::SetItemText(nItem, nSubItem, lpszText);
	}
	BOOL SetItemText(_In_ int nItem, _In_ int nSubItem, _In_z_ LPCSTR lpszText)
	{
		return CListCtrl::SetItemText(nItem, nSubItem, CString(lpszText));
	}
	BOOL SetItemText(_In_ int nItem, _In_ int nSubItem, DWORD Dword)
	{
		CString buf;
		buf.Format(L"%d", Dword);
		return CListCtrl::SetItemText(nItem, nSubItem, buf);
	}

	BOOL SetItemText(_In_ int nItem, _In_ int nSubItem, FILETIME time)
	{
		// ���ļ�ʱ��ת���ɱ��ص��ļ�ʱ��
		FileTimeToLocalFileTime(&time, &time);
		SYSTEMTIME st = { 0 };
		// ���ļ�ʱ��ת��ϵͳʱ��
		CString str;
		FileTimeToSystemTime(&time, &st);
		str.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		return CListCtrl::SetItemText(nItem, nSubItem, str);
	}

	int InsertItem(int nItem, DWORD lpszItem)
	{
		CString buf;
		buf.Format(L"%d", lpszItem);
		return 	CListCtrl::InsertItem(nItem, buf);
	}

	int InsertItem(_In_ int nItem, _In_z_ LPCTSTR lpszItem)
	{
		return 	CListCtrl::InsertItem(nItem, lpszItem);
	}

	int InsertItem(_In_ int nItem, _In_z_ LPCTSTR lpszItem, _In_ int nImage)
	{
		return 	CListCtrl::InsertItem(nItem, lpszItem, nImage);
	}
	int InsertItem(_In_ int nItem, _In_z_ LPCTSTR lpszItem, CString nImage)
	{
		int nTempImage;
		_stscanf_s(nImage, TEXT("%d"), &nTempImage);

		return 	CListCtrl::InsertItem(nItem, lpszItem, nTempImage);
	}

protected:
	DECLARE_MESSAGE_MAP()
};


