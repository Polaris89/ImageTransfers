// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include <winsock2.h>


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

// CClientDlg �Ի���
class CClientDlg : public CDialog
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENT_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedButton1();
	
	CWinThread* pThread = NULL;
	HANDLE		threadhandle;
	DWORD		m_dwThreadId;
	bool        isThreadRunning;

	static DWORD WINAPI Thread_Send(LPVOID lpParam);
	void ImageSend_Function(void);
	bool DestroyThread(void);
};
