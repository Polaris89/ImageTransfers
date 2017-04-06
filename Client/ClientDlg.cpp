// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include ".\clientdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "wsock32.lib")

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

// CClientDlg 对话框
CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	
	pThread = NULL;
	threadhandle = 0;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedSend)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()

// CClientDlg 消息处理程序
BOOL CClientDlg::OnInitDialog()
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
	CString strAddress = "127.0.0.1";
	GetDlgItem(IDC_IPADDRESS1)->SetWindowText(strAddress);

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint()
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
		int x = (rect.Width()  - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientDlg::OnBnClickedButton1()
{
	CFileDialog	Dlg(TRUE);
	if (Dlg.DoModal() != IDOK)
		return;

	GetDlgItem(IDC_EDIT1)->SetWindowText(Dlg.GetPathName());
}

void CClientDlg::OnBnClickedSend()
{
	//if (pThread != NULL)
	//{
	//	CloseHandle(pThread);
	//}

	//pThread = NULL;
	//isThreadRunning = true;
	//pThread = AfxBeginThread(Thread_Send, &isThreadRunning);
	//threadhandle = pThread->m_hThread;

	threadhandle = ::CreateThread(NULL, 0, &Thread_Send, this, 0, &m_dwThreadId);
}

DWORD WINAPI CClientDlg::Thread_Send(LPVOID lpParam)
{
	CClientDlg *pThis = (CClientDlg *)lpParam;

	bool pContinue = pThis->isThreadRunning;
	while (true)
	{
		//..捞镑俊 风橇 矫 荐青窍咯具 且 悼累阑 急攫茄促.
		pThis->ImageSend_Function();
		AfxMessageBox("Image Send", MB_OK | MB_ICONERROR);
		Sleep(5000);
	}
	return 0;
}

bool CClientDlg::DestroyThread(void)
{
	
	//if (pThread != NULL)
	//	::WaitForSingleObject(threadhandle, INFINITE);
	//CloseHandle(threadhandle);
	return true;
}

void CClientDlg::ImageSend_Function(void)
{
	CString strFileName, strIpAddress;
	this->GetDlgItem(IDC_EDIT1)->GetWindowText(strFileName);
	this->GetDlgItem(IDC_IPADDRESS1)->GetWindowText(strIpAddress);
	CFile myFile;
	if (!myFile.Open(strFileName, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox("Image File Select!", MB_OK | MB_ICONERROR);
		return;
	}

	SOCKET_STREAM_FILE_INFO		StreamFileInfo;
	WIN32_FIND_DATA             FindFileData;

	FindClose(FindFirstFile(strFileName, &FindFileData));
	memset(&StreamFileInfo, 0, sizeof(SOCKET_STREAM_FILE_INFO));
	strcpy(StreamFileInfo.szFileTitle, myFile.GetFileTitle());

	StreamFileInfo.dwFileAttributes = FindFileData.dwFileAttributes;
	StreamFileInfo.ftCreationTime = FindFileData.ftCreationTime;
	StreamFileInfo.ftLastAccessTime = FindFileData.ftLastAccessTime;
	StreamFileInfo.ftLastWriteTime = FindFileData.ftLastWriteTime;
	StreamFileInfo.nFileSizeHigh = FindFileData.nFileSizeHigh;
	StreamFileInfo.nFileSizeLow = FindFileData.nFileSizeLow;

	

	//////////////////////////////////////////////////////////////////////////
	//server俊 立加且 家南阑 急攫茄促.
	SOCKET server;

	//WSADATA is a struct that is filled up by the call
	//to WSAStartup
	//这是一个结构体，用于调用函数WSAStartup时作为参数
	WSADATA wsaData;

	//The sockaddr_in specifies the address of the socket
	//for TCP/IP sockets. Other protocols use similar structures.
	//存放ip地址的结构体
	sockaddr_in local;

	//初始化winsock，每次必须最先调用
	int wsaret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaret != 0)
	{
		return;
	}

	//填充服务器的ip地址和端口号
	local.sin_family = AF_INET; //Address family
	local.sin_addr.s_addr = inet_addr(strIpAddress); //Wild card IP address
	local.sin_port = htons((u_short)8888); //port to use

	//建立一个tcp socket
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (server == INVALID_SOCKET)
	{
		return;
	}

	//接受请求后，实际同客户端socket进行交互的SOCKET client
	connect(server, (struct sockaddr *)&local, sizeof(local));

	//快急 扁惯蔼阑 焊郴绊  8byte 沥档绰 沥焊蔼阑 焊辰促
	char *buffer;
	CString deviceName = "dddddddddd";
	int len_1 = deviceName.GetLength();
	buffer = new char[len_1 + 1];
	for (int i = 0; i < len_1; i++)
	{
		buffer[i] = deviceName[i];
	}
	send(server, (char *)&deviceName, sizeof(SOCKET_STREAM_FILE_INFO), 0);
	Sleep(10);

	//send(server, (char *)&StreamFileInfo, sizeof(SOCKET_STREAM_FILE_INFO), 0);
	//Sleep(10);

	UINT dwRead = 0;
	char temp[1024];
	while (dwRead < StreamFileInfo.nFileSizeLow)
	{
		memset(temp, 0, 1024);
		UINT dw = myFile.Read(temp, 1024);
		send(server, temp, dw, 0);
		dwRead += dw;
		Sleep(10);
	}
	myFile.Close();

	//关闭连接
	closesocket(server);

	WSACleanup();
	return;
}