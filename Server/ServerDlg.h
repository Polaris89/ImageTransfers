// ServerDlg.h : 头文件
//

#pragma once
#include <afxsock.h>		// MFC socket extensions

#include <vector>

using namespace std;


#define HIMETRIC_INCH 2540


typedef struct _SOCKET_STREAM_FILE_INFO {

	TCHAR       szFileTitle[128];                   //文件的标题名
	DWORD       dwFileAttributes;                   //文件的属性
	FILETIME    ftCreationTime;                     //文件的创建时间
	FILETIME    ftLastAccessTime;                   //文件的最后访问时间
	FILETIME    ftLastWriteTime;                    //文件的最后修改时间
	DWORD       nFileSizeHigh;                      //文件大小的高位双字
	DWORD       nFileSizeLow;                       //文件大小的低位双字
	DWORD       dwReserved0;                        //保留，为0
	DWORD       dwReserved1;                        //保留，为0

} SOCKET_STREAM_FILE_INFO, * PSOCKET_STREAM_FILE_INFO;

// CServerDlg 对话框
class CServerDlg : public CDialog
{
// 构造
public:
	CServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()

	void LoadPicture( CString strPictureName );
	void ShowPicture();
	static DWORD WINAPI ThreadSocket( LPVOID lpParam );
	virtual void OnCancel();

protected:
	// ATL smart pointer to IPicture
	CComQIPtr<IPicture>				m_spIPicture;

	HANDLE							m_hThreadHandle;
	DWORD							m_dwThreadId;

	bool							m_bListen;
	UINT							m_iTimer;
};
