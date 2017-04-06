// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include ".\clientdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "wsock32.lib")

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

// CClientDlg �Ի���
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

// CClientDlg ��Ϣ�������
BOOL CClientDlg::OnInitDialog()
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
	CString strAddress = "127.0.0.1";
	GetDlgItem(IDC_IPADDRESS1)->SetWindowText(strAddress);

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
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
		int x = (rect.Width()  - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
		//..�̰��� ���� �� �����Ͽ��� �� ������ �����Ѵ�.
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
	//server�� ������ ������ �����Ѵ�.
	SOCKET server;

	//WSADATA is a struct that is filled up by the call
	//to WSAStartup
	//����һ���ṹ�壬���ڵ��ú���WSAStartupʱ��Ϊ����
	WSADATA wsaData;

	//The sockaddr_in specifies the address of the socket
	//for TCP/IP sockets. Other protocols use similar structures.
	//���ip��ַ�Ľṹ��
	sockaddr_in local;

	//��ʼ��winsock��ÿ�α������ȵ���
	int wsaret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaret != 0)
	{
		return;
	}

	//����������ip��ַ�Ͷ˿ں�
	local.sin_family = AF_INET; //Address family
	local.sin_addr.s_addr = inet_addr(strIpAddress); //Wild card IP address
	local.sin_port = htons((u_short)8888); //port to use

	//����һ��tcp socket
	server = socket(AF_INET, SOCK_STREAM, 0);
	if (server == INVALID_SOCKET)
	{
		return;
	}

	//���������ʵ��ͬ�ͻ���socket���н�����SOCKET client
	connect(server, (struct sockaddr *)&local, sizeof(local));

	//�켱 ��߰��� ������  8byte ������ �������� ������
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

	//�ر�����
	closesocket(server);

	WSACleanup();
	return;
}