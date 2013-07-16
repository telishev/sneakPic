// Modified (for our needs) example from http://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>

#define BUFSIZE 4096

HANDLE child_stdout = NULL;
HANDLE child_stderr = NULL;
HANDLE child_stdin = NULL;
HANDLE mediator_stdout = NULL;
HANDLE mediator_stderr = NULL;
HANDLE mediator_stdin = NULL;
HANDLE parent_stdout = NULL;
HANDLE parent_stderr = NULL;
HANDLE parent_stdin = NULL;

HANDLE g_hInputFile = NULL;

void CreateChildProcess(void);
void WriteToPipe(void);
void ReadFromPipe(void);
void ErrorExit(PTSTR);

struct pipe_info
{
  PHANDLE in;
  PHANDLE out;
};

DWORD WINAPI process_pipe_thread (LPVOID lpParam)
{
  pipe_info *info = (pipe_info *) lpParam;
  DWORD dwRead, dwWritten;
  CHAR chBuf[BUFSIZE];
  BOOL bSuccess = FALSE;

   while (1)
   {
      bSuccess = ReadFile (*(info->in), chBuf, BUFSIZE, &dwRead, NULL);
      if (!bSuccess || dwRead == 0)
        break;

      bSuccess = WriteFile (*(info->out), chBuf, dwRead, &dwWritten, NULL);
      if (!bSuccess )
        break;
   }
   return 0;
}

int _tmain ()
{
   SECURITY_ATTRIBUTES saAttr;
   saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
   saAttr.bInheritHandle = TRUE;
   saAttr.lpSecurityDescriptor = NULL;

   parent_stdin  = GetStdHandle (STD_INPUT_HANDLE);
   parent_stderr = GetStdHandle (STD_ERROR_HANDLE);
   parent_stdout = GetStdHandle (STD_OUTPUT_HANDLE);

   pipe_info info[3];

   if (!CreatePipe (&child_stdin, &mediator_stdin, &saAttr, 0))
     exit (1);

   if (!CreatePipe (&mediator_stdout, &child_stdout, &saAttr, 0))
     exit (1);

   if (!CreatePipe (&mediator_stderr, &child_stderr, &saAttr, 0))
     exit (1);

   CreateChildProcess();

   HANDLE threads[3];
   info[0].in  = &parent_stdin;
   info[0].out = &mediator_stdin;
   info[1].in  = &mediator_stdout;
   info[1].out = &parent_stdout;
   info[2].in  = &mediator_stderr;
   info[2].out = &parent_stderr;
   for (int i = 0; i < 3; i++)
     threads[i] = CreateThread (NULL, 0, process_pipe_thread, info + i, 0, 0);
   WaitForMultipleObjects (2, threads + 1, TRUE, INFINITE); // Waiting for stdout and stderr threads
  return 0;
}

void CreateChildProcess()
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{
   wchar_t szFileName[MAX_PATH];
   GetModuleFileNameW ( NULL, szFileName, MAX_PATH );
   wchar_t *period_pos = wcsrchr (szFileName, L'.');
   if (!period_pos)
     exit (1);
   wcscpy_s (period_pos, 5, L".exe");

   wchar_t *command_line = GetCommandLineW ();
   PROCESS_INFORMATION piProcInfo;
   STARTUPINFOW siStartInfo;
   BOOL bSuccess = FALSE;

// Set up members of the PROCESS_INFORMATION structure.

   ZeroMemory (&piProcInfo, sizeof(PROCESS_INFORMATION));

// Set up members of the STARTUPINFO structure.
// This structure specifies the STDIN and STDOUT handles for redirection.

   ZeroMemory (&siStartInfo, sizeof(STARTUPINFOW));
   siStartInfo.cb = sizeof(STARTUPINFOW);
   siStartInfo.hStdError = child_stderr;
   siStartInfo.hStdOutput = child_stdout;
   siStartInfo.hStdInput = child_stdin;
   siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

// Create the child process.

   bSuccess = CreateProcessW (szFileName,
      command_line,     // command line
      NULL,          // process security attributes
      NULL,          // primary thread security attributes
      TRUE,          // handles are inherited
      0,             // creation flags
      NULL,          // use parent's environment
      NULL,          // use parent's current directory
      &siStartInfo,  // STARTUPINFO pointer
      &piProcInfo);  // receives PROCESS_INFORMATION

   if (! bSuccess)
      exit (1);
   else
     {
        CloseHandle (piProcInfo.hProcess);
        CloseHandle (piProcInfo.hThread);
        CloseHandle (child_stdin);
        CloseHandle (child_stdout);
        CloseHandle (child_stderr);
     }
}
