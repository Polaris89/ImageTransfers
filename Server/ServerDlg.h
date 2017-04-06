// ServerDlg.h : ͷ�ļ�
//

#pragma once
#include <afxsock.h>		// MFC socket extensions

#include <vector>

using namespace std;


#define HIMETRIC_INCH 2540


typedef struct _SOCKET_STREAM_FILE_INFO {

	TCHAR       szFileTitle[128];                   //�ļ��ı�����
	DWORD       dwFileAttributes;                   //�ļ�������
	FILETIME    ftCreationTime;                     //�ļ��Ĵ���ʱ��
	FILETIME    ftLastAccessTime;                   //�ļ���������ʱ��
	FILETIME    ftLastWriteTime;                    //�ļ�������޸�ʱ��
	DWORD       nFileSizeHigh;                      //�ļ���С�ĸ�λ˫��
	DWORD       nFileSizeLow;                       //�ļ���С�ĵ�λ˫��
	DWORD       dwReserved0;                        //������Ϊ0
	DWORD       dwReserved1;                        //������Ϊ0

} SOCKET_STREAM_FILE_INFO, * PSOCKET_STREAM_FILE_INFO;

// CServerDlg �Ի���
class CServerDlg : public CDialog
{
// ����
public:
	CServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
