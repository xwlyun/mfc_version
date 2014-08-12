// versionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "version.h"
#include "versionDlg.h"
#include <Wincrypt.h>//获取md5
#include "tinyxml.h"

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

//遍历文件夹
void CversionDlg::BrowseAllFile(CString strDir)
{
     if(strDir == _T("")) 
     { 
         return;
     }
     else
     {
         if(strDir.Right(1) != _T("//"))
		 {
              strDir += L"//"; 
		 }
         strDir =strDir+_T("*.*"); 
     }
     CFileFind finder; 
     CString strPath; 
     BOOL bWorking = finder.FindFile(strDir); 
	while(bWorking) 
	{ 
		bWorking = finder.FindNextFile(); 
		strPath = finder.GetFilePath(); 
		if(finder.IsDirectory() && !finder.IsDots()) 
		{
			BrowseAllFile(strPath); //递归调用
		}
		else if(!finder.IsDirectory() && !finder.IsDots()) 
		{
			CString xml_k = strPath.Mid(m_CurFile);
			CString xml_v = GetMd5(strPath);

			m_List.AddString(xml_k + L"---" + xml_v);
			m_KV.insert(make_pair<CString,CString>(xml_k,xml_v));
		}
	} 
}

bool CreateXmlFile(string& szFileName)
{
	//创建xml文件,szFilePath为文件保存的路径,若创建成功返回true,否则false
    try
    {
        //创建一个XML的文档对象。
        TiXmlDocument *myDocument = new TiXmlDocument();
        ////创建一个根元素并连接。
        //TiXmlElement *RootElement = new TiXmlElement("Persons");
        //myDocument->LinkEndChild(RootElement);
        ////创建一个Person元素并连接。
        //TiXmlElement *PersonElement = new TiXmlElement("Person");
        //RootElement->LinkEndChild(PersonElement);
        ////设置Person元素的属性。
        //PersonElement->SetAttribute("ID", "1");
        ////创建name元素、age元素并连接。
        //TiXmlElement *NameElement = new TiXmlElement("name");
        //TiXmlElement *AgeElement = new TiXmlElement("age");
        //PersonElement->LinkEndChild(NameElement);
        //PersonElement->LinkEndChild(AgeElement);
        ////设置name元素和age元素的内容并连接。
        //TiXmlText *NameContent = new TiXmlText("周星星");
        //TiXmlText *AgeContent = new TiXmlText("22");
        //NameElement->LinkEndChild(NameContent);
        //AgeElement->LinkEndChild(AgeContent);
        //CString appPath = GetAppPath();
        //string seperator = "\\";
        //string fullPath = appPath.GetBuffer(0) +seperator+szFileName;
        //myDocument->SaveFile(fullPath.c_str());//保存到文件
    }
    catch (string& e)
    {
        return false;
    }
    return true;
}

CString CversionDlg::GetAppPath()
{
	//获取应用程序根目录
    TCHAR modulePath[MAX_PATH];
    GetModuleFileName(NULL, modulePath, MAX_PATH);
    CString strModulePath(modulePath);
    strModulePath = strModulePath.Left(strModulePath.ReverseFind(_T('\\')));
    return strModulePath;
}

CString CversionDlg::GetMd5(LPCWSTR FileDirectory)
{
	CString re = L"";

	HANDLE hFile=CreateFile(FileDirectory,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
	if (hFile==INVALID_HANDLE_VALUE)                                        
	{
		//如果CreateFile调用失败
		//提示CreateFile调用失败，并输出错误号。visual studio中可在“工具”>“错误查找”中利用错误号得到错误信息。
		//cout<<"CreateFile go wrong :"<<GetLastError()<<endl;                
		CloseHandle(hFile);
	}
	HCRYPTPROV hProv=NULL;
	if(CryptAcquireContext(&hProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT)==FALSE)       
	{
		//获得CSP中一个密钥容器的句柄
		//cout<<"CryptAcquireContext go wrong:"<<GetLastError()<<endl;
	}
	HCRYPTPROV hHash=NULL;
	if(CryptCreateHash(hProv,CALG_MD5,0,0,&hHash)==FALSE)     
	{
		//初始化对数据流的hash，创建并返回一个与CSP的hash对象相关的句柄。这个句柄接下来将被CryptHashData调用。
		//cout<<"CryptCreateHash go wrong:"<<GetLastError()<<endl;
	}
	DWORD dwFileSize=GetFileSize(hFile,0);    //获取文件的大小
	if (dwFileSize==0xFFFFFFFF)               //如果获取文件大小失败
	{
		//cout<<"GetFileSize go wrong:"<<GetLastError()<<endl;
	}
	byte* lpReadFileBuffer=new byte[dwFileSize];
	DWORD lpReadNumberOfBytes;
	if (ReadFile(hFile,lpReadFileBuffer,dwFileSize,&lpReadNumberOfBytes,NULL)==0)        
	{
		//读取文件
		//cout<<"ReadFile go wrong:"<<GetLastError()<<endl;
	}
    if(CryptHashData(hHash,lpReadFileBuffer,lpReadNumberOfBytes,0)==FALSE)     
	{
		 //hash文件
		//cout<<"CryptHashData go wrong:"<<GetLastError()<<endl;
	}

	delete[] lpReadFileBuffer;
	CloseHandle(hFile);          //关闭文件句柄
    BYTE *pbHash;
	DWORD dwHashLen=sizeof(DWORD);
	//以下注释掉的代码不用使用，因为已经知道md5值就占32个字节，没有必要通过CryptGetHashParam函数来得到字节数。
	/*
	BYTE *pbHashSize;
	if (!(pbHashSize=(byte*)malloc(dwHashLen)))      //为pbHashSize分配内存
	{
		cout<<"memory allocation failed:"<<GetLastError()<<endl;
	}
	//将第二个参数的值设为HP_HASHSIZE。dwHashLen中存放着hash值的字节数。这个调用必须在将第三个参数设置为HP_HASHVAL的调用前，这样才能分配正确数量的内存。
	if (CryptGetHashParam(hHash,HP_HASHSIZE,pbHashSize,&dwHashLen,0))      
	{
		free(pbHashSize);
	}
	else
	{
		cout<<"get size go wrong"<<GetLastError()<<endl;
	}*/
	if (CryptGetHashParam(hHash,HP_HASHVAL,NULL,&dwHashLen,0))            
	{
	}
	else
	{
		//cout<<"get length wrong"<<endl;
	}
	if(pbHash=(byte*)malloc(dwHashLen))
	{}
	else
	{
		//cout<<"allocation failed"<<endl;
	}
	if(CryptGetHashParam(hHash,HP_HASHVAL,pbHash,&dwHashLen,0))            
	{
		//输出md5值
		for(DWORD i=0;i<dwHashLen;i++)
		{
			CString str;
			str.Format(L"%02x",pbHash[i]);
			re += str;
			//printf("%02x",pbHash[i]);
		}
		//cout<<endl;
	}
	//善后工作
	if(CryptDestroyHash(hHash)==FALSE)          
	{
		//销毁hash对象
		//cout<<"CryptDestroyHash go wrong:"<<GetLastError()<<endl;
	}
	if(CryptReleaseContext(hProv,0)==FALSE)
	{
		//cout<<"CryptReleaseContext go wrong:"<<GetLastError()<<endl;
	}

	return re;
}
// CversionDlg 对话框

//获取文件号线程
UINT ThreadWebDesktop(LPVOID pParam)
{
	CversionDlg* pDlg = (CversionDlg* )pParam;

	CFileFind finder;
	if(!finder.FindFile(_T("Bin")))
	{
		pDlg->m_List.AddString(_T("no Bin!"));
		return 0;
	}

	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);//得到程序模块名称，全路径
	CString strdir,tmpdir = exeFullPath; 
    strdir = tmpdir.Left(tmpdir.ReverseFind('\\'));
	strdir += L"\\Bin\\";
	pDlg->m_CurFile = strdir.GetLength();

	pDlg->BrowseAllFile(_T("Bin"));

	string fileName = "version.xml";
    CreateXmlFile(fileName);


	return 0;
}



CversionDlg::CversionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CversionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CversionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_LIST1,m_List);
}

BEGIN_MESSAGE_MAP(CversionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CversionDlg 消息处理程序

BOOL CversionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

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
	AfxBeginThread(ThreadWebDesktop,this);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CversionDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CversionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CversionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
