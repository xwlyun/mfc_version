// versionDlg.h : 头文件
//

#pragma once
#include <map>
using namespace std;

// CversionDlg 对话框
class CversionDlg : public CDialog
{
// 构造
public:
	CversionDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VERSION_DIALOG };

	CListBox m_List;
	void BrowseAllFile(CString strDir);
	int m_CurFile;
	CString GetMd5(LPCWSTR FileDirectory);
	map<CString,CString> m_KV;
	CString GetAppPath();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

};
