// ServerDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CServerDlg 对话框



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


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetTimer( 1, 100, NULL );

	m_hThreadHandle = ::CreateThread( NULL, 0, &ThreadSocket, this, 0, &m_dwThreadId );

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	// 如果图片文件加载成功，则显示图片
	ShowPicture();
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
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
