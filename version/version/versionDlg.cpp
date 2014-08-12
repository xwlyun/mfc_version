// versionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "version.h"
#include "versionDlg.h"
#include <Wincrypt.h>//��ȡmd5
#include "tinyxml.h"

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

//�����ļ���
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
			BrowseAllFile(strPath); //�ݹ����
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
	//����xml�ļ�,szFilePathΪ�ļ������·��,�������ɹ�����true,����false
    try
    {
        //����һ��XML���ĵ�����
        TiXmlDocument *myDocument = new TiXmlDocument();
        ////����һ����Ԫ�ز����ӡ�
        //TiXmlElement *RootElement = new TiXmlElement("Persons");
        //myDocument->LinkEndChild(RootElement);
        ////����һ��PersonԪ�ز����ӡ�
        //TiXmlElement *PersonElement = new TiXmlElement("Person");
        //RootElement->LinkEndChild(PersonElement);
        ////����PersonԪ�ص����ԡ�
        //PersonElement->SetAttribute("ID", "1");
        ////����nameԪ�ء�ageԪ�ز����ӡ�
        //TiXmlElement *NameElement = new TiXmlElement("name");
        //TiXmlElement *AgeElement = new TiXmlElement("age");
        //PersonElement->LinkEndChild(NameElement);
        //PersonElement->LinkEndChild(AgeElement);
        ////����nameԪ�غ�ageԪ�ص����ݲ����ӡ�
        //TiXmlText *NameContent = new TiXmlText("������");
        //TiXmlText *AgeContent = new TiXmlText("22");
        //NameElement->LinkEndChild(NameContent);
        //AgeElement->LinkEndChild(AgeContent);
        //CString appPath = GetAppPath();
        //string seperator = "\\";
        //string fullPath = appPath.GetBuffer(0) +seperator+szFileName;
        //myDocument->SaveFile(fullPath.c_str());//���浽�ļ�
    }
    catch (string& e)
    {
        return false;
    }
    return true;
}

CString CversionDlg::GetAppPath()
{
	//��ȡӦ�ó����Ŀ¼
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
		//���CreateFile����ʧ��
		//��ʾCreateFile����ʧ�ܣ����������š�visual studio�п��ڡ����ߡ�>��������ҡ������ô���ŵõ�������Ϣ��
		//cout<<"CreateFile go wrong :"<<GetLastError()<<endl;                
		CloseHandle(hFile);
	}
	HCRYPTPROV hProv=NULL;
	if(CryptAcquireContext(&hProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT)==FALSE)       
	{
		//���CSP��һ����Կ�����ľ��
		//cout<<"CryptAcquireContext go wrong:"<<GetLastError()<<endl;
	}
	HCRYPTPROV hHash=NULL;
	if(CryptCreateHash(hProv,CALG_MD5,0,0,&hHash)==FALSE)     
	{
		//��ʼ������������hash������������һ����CSP��hash������صľ��������������������CryptHashData���á�
		//cout<<"CryptCreateHash go wrong:"<<GetLastError()<<endl;
	}
	DWORD dwFileSize=GetFileSize(hFile,0);    //��ȡ�ļ��Ĵ�С
	if (dwFileSize==0xFFFFFFFF)               //�����ȡ�ļ���Сʧ��
	{
		//cout<<"GetFileSize go wrong:"<<GetLastError()<<endl;
	}
	byte* lpReadFileBuffer=new byte[dwFileSize];
	DWORD lpReadNumberOfBytes;
	if (ReadFile(hFile,lpReadFileBuffer,dwFileSize,&lpReadNumberOfBytes,NULL)==0)        
	{
		//��ȡ�ļ�
		//cout<<"ReadFile go wrong:"<<GetLastError()<<endl;
	}
    if(CryptHashData(hHash,lpReadFileBuffer,lpReadNumberOfBytes,0)==FALSE)     
	{
		 //hash�ļ�
		//cout<<"CryptHashData go wrong:"<<GetLastError()<<endl;
	}

	delete[] lpReadFileBuffer;
	CloseHandle(hFile);          //�ر��ļ����
    BYTE *pbHash;
	DWORD dwHashLen=sizeof(DWORD);
	//����ע�͵��Ĵ��벻��ʹ�ã���Ϊ�Ѿ�֪��md5ֵ��ռ32���ֽڣ�û�б�Ҫͨ��CryptGetHashParam�������õ��ֽ�����
	/*
	BYTE *pbHashSize;
	if (!(pbHashSize=(byte*)malloc(dwHashLen)))      //ΪpbHashSize�����ڴ�
	{
		cout<<"memory allocation failed:"<<GetLastError()<<endl;
	}
	//���ڶ���������ֵ��ΪHP_HASHSIZE��dwHashLen�д����hashֵ���ֽ�����������ñ����ڽ���������������ΪHP_HASHVAL�ĵ���ǰ���������ܷ�����ȷ�������ڴ档
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
		//���md5ֵ
		for(DWORD i=0;i<dwHashLen;i++)
		{
			CString str;
			str.Format(L"%02x",pbHash[i]);
			re += str;
			//printf("%02x",pbHash[i]);
		}
		//cout<<endl;
	}
	//�ƺ���
	if(CryptDestroyHash(hHash)==FALSE)          
	{
		//����hash����
		//cout<<"CryptDestroyHash go wrong:"<<GetLastError()<<endl;
	}
	if(CryptReleaseContext(hProv,0)==FALSE)
	{
		//cout<<"CryptReleaseContext go wrong:"<<GetLastError()<<endl;
	}

	return re;
}
// CversionDlg �Ի���

//��ȡ�ļ����߳�
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
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);//�õ�����ģ�����ƣ�ȫ·��
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


// CversionDlg ��Ϣ�������

BOOL CversionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

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
	AfxBeginThread(ThreadWebDesktop,this);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CversionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CversionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
