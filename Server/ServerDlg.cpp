// ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include <io.h>
#include <afxpriv2.h>

#include <winsock2.h>
#include <windows.h>
#include ".\serverdlg.h"

#pragma comment(lib, "wsock32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CServerDlg �Ի���



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hThreadHandle = 0;

	m_bListen = true;

	m_spIPicture.Release();

}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CServerDlg ��Ϣ�������

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetTimer( 1, 100, NULL );

	m_hThreadHandle = ::CreateThread( NULL, 0, &ThreadSocket, this, 0, &m_dwThreadId );

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	// ���ͼƬ�ļ����سɹ�������ʾͼƬ
	ShowPicture();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerDlg::LoadPicture( CString strPictureName )
{
	CFile file( strPictureName, CFile::modeRead );
	if (file.GetLength() == 0)
	{
		return;
	}
	CArchive loadArchive( &file, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_bForceFlat = FALSE;
	CArchiveStream arcstream(&loadArchive);
	m_spIPicture.Release();
	HRESULT hr = OleLoadPicture( &arcstream, 0, FALSE, IID_IPicture, (void**)&m_spIPicture);
	loadArchive.Close();
	file.Close();
}

void CServerDlg::ShowPicture()
{
	if( m_spIPicture )
	{
		long hmWidth,hmHeight; // HIMETRIC units
		m_spIPicture->get_Width( &hmWidth );
		m_spIPicture->get_Height( &hmHeight );

		int iHeight = MulDiv( hmHeight, 96, HIMETRIC_INCH);
		int iWidth  = MulDiv( hmWidth,  96, HIMETRIC_INCH);

		CDC* pDC = GetDC();
		CRect clientRect, winRect;
		GetClientRect( &clientRect );
		GetWindowRect( &winRect );

		MoveWindow( winRect.left, winRect.top, iWidth, iHeight );

		m_spIPicture->Render( *pDC, clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), 0, hmHeight, hmWidth, -hmHeight, NULL );
	}
}

DWORD WINAPI CServerDlg::ThreadSocket( LPVOID lpParam )
{
	CServerDlg *pThis = (CServerDlg *)lpParam;

	SOCKET server;

	WSADATA wsaData;
	int wsaret = WSAStartup( MAKEWORD(2,2), &wsaData );
	if( wsaret != 0 )
	{
		return 0;
	}

	sockaddr_in local;
	local.sin_family = AF_INET; //Address family
	local.sin_addr.s_addr = INADDR_ANY; //Wild card IP address
	local.sin_port = htons((u_short)8888); //port to use

	server = socket( AF_INET, SOCK_STREAM, 0 );
	if( server == INVALID_SOCKET )
	{
		return 0;
	}

	if( bind( server, (sockaddr*)&local, sizeof(local) ) != 0 )
	{
		return 0;
	}
	if( listen( server, 10 ) != 0 )
	{
		return 0;
	}

	SOCKET_STREAM_FILE_INFO		StreamFileInfo;
	memset( &StreamFileInfo, 0, sizeof(SOCKET_STREAM_FILE_INFO) );
	
	SOCKET client;
	sockaddr_in from;
	int fromlen = sizeof( from );
	while( pThis->m_bListen )
	{
		char temp[1024];
		memset( temp, 0, 1024 );

		client = accept( server, (struct sockaddr*)&from, &fromlen );

		int iLen = recv( client, temp, sizeof(SOCKET_STREAM_FILE_INFO), 0 );
		if( iLen == sizeof(StreamFileInfo) )
		{
			memcpy( &StreamFileInfo, temp, sizeof(StreamFileInfo) );

			CFile destFile( StreamFileInfo.szFileTitle, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
			UINT dwRead = 0;
			while( dwRead < StreamFileInfo.nFileSizeLow )
			{
				memset(temp,0,1024);

				UINT dw = recv( client, temp, 1024, 0 );
				destFile.Write(temp, dw);

				dwRead += dw;
			}

			SetFileTime( (HANDLE)destFile.m_hFile, &StreamFileInfo.ftCreationTime, &StreamFileInfo.ftLastAccessTime, &StreamFileInfo.ftLastWriteTime );
			destFile.Close();
			SetFileAttributes(StreamFileInfo.szFileTitle, StreamFileInfo.dwFileAttributes);

			pThis->LoadPicture( StreamFileInfo.szFileTitle );
		}
		closesocket(client);
	}
	closesocket(server);
	WSACleanup();

	return 0;
}

void CServerDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	m_bListen = false;

	KillTimer(1);
	CDialog::OnCancel();
}

void CServerDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if( m_spIPicture )
	{
		long hmWidth,hmHeight; // HIMETRIC units
		m_spIPicture->get_Width( &hmWidth );
		m_spIPicture->get_Height( &hmHeight );

		int iHeight = MulDiv( hmHeight, 96, HIMETRIC_INCH);
		int iWidth  = MulDiv( hmWidth,  96, HIMETRIC_INCH);

		CDC* pDC = GetDC();
		CRect clientRect, winRect;
		GetClientRect( &clientRect );
		GetWindowRect( &winRect );

		MoveWindow( winRect.left, winRect.top, iWidth, iHeight );

		m_spIPicture->Render( *pDC, clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), 0, hmHeight, hmWidth, -hmHeight, NULL );
	}


	CDialog::OnTimer(nIDEvent);
}
