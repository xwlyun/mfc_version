// versionDlg.h : ͷ�ļ�
//

#pragma once
#include <map>
using namespace std;

// CversionDlg �Ի���
class CversionDlg : public CDialog
{
// ����
public:
	CversionDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VERSION_DIALOG };

	CListBox m_List;
	void BrowseAllFile(CString strDir);
	int m_CurFile;
	CString GetMd5(LPCWSTR FileDirectory);
	map<CString,CString> m_KV;
	CString GetAppPath();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

};
