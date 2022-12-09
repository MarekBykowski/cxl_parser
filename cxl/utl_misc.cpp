// Description: misc utilities needed by the package


// INCLUDE ENVIRONMENT
#include "common.h"

using namespace std;

//-----------------------------------------------------------------------------------
// #include <time.h>
// int nanosleep(const struct timespec *rqtp, struct timespec *rmtp);
// The nanosleep() function causes the current thread to be suspended from execution 
// until either the time interval specified by the rqtp argument has elapsed or a 
// signal is delivered to the calling thread and its action is to invoke a 
// signal-catching function or to terminate the process. The suspension time may be 
// longer than requested because the argument value is rounded up to an integer 
// multiple of the sleep resolution or because of the scheduling of other activity 
// by the system. But, except for the case of being interrupted by a signal, 
// the suspension time will not be less than the time specified by rqtp, as 
// measured by the system clock, CLOCK_REALTIME. 
//-----------------------------------------------------------------------------------
int better_sleep (double sleep_time)
{
   struct timespec tv;
   /* Construct the timespec from the number of whole seconds... */
   tv.tv_sec  = (time_t) sleep_time;
   /* ... and the remainder in nanoseconds. */
   tv.tv_nsec = (long) ((sleep_time - tv.tv_sec) * 1e+9);

   while (1)
   {
      /* Sleep for the time specified in tv. If interrupted by a
         signal, place the remaining time left to sleep back into tv. */
      int rval = nanosleep (&tv, &tv);   // precision= 10 ms
      if (rval == 0)
         /* Completed the entire sleep time; all done. */
         return 0;
      else if (errno == EINTR)
         /* Interrupted by a signal. Try again. */
         continue;
      else if (errno == ENOSYS)
      {
         cout << "Error in " << __FILE__ << " at line " << __LINE__
              << " nanonsleep function is not supported by this implementation " << endl;
         /* Some other error; bail out. */
         exit(1);
      }
      else 
      {
         cout << "Error in " << __FILE__ << " at line " << __LINE__
              << " nanonsleep set unknown errno= " << errno << endl;
         exit(1);
      }
   }
   return 0;
}

double diff_time(struct timeval *start, struct timeval *end)
{

   return (
      (double) end->tv_sec + (double) end->tv_usec * 1e-6 - 
      (double) start->tv_sec - (double) start->tv_usec * 1e-6
   );
}
   

// Find the signal names using "kill -l" command
const char *signames[] = 
{
   NULL,                                 // signal numbers start at "1"

   "HUP",  "INT",  "QUIT",   "ILL",  "TRAP", "ABRT",  "BUS",    "FPE",     "KILL",    "USR1",    "SEGV",    "USR2",    "PIPE",
   "ALRM", "TERM", "STKFLT", "CHLD", "CONT", "STOP",  "TSTP",   "TTIN",    "TTOU",    "URG",     "XCPU",    "XFSZ",    "VTALRM",
   "PROF", "WINCH", "POLL",  "PWR",  "SYS" //  "RTMIN", "RTMIN+1","RTMIN+2", "RTMIN+3", "RTMAX-3", "RTMAX-2", "RTMAX-1", "RTMAX"
}; 

unsigned sigcount[sizeof(signames)/sizeof(char *)];

// signal catching routine
void sigcatch(int sig)
{
   signal(sig, SIG_IGN);

   sigcount[sig] += 1;

   cout << signames[sig] << "(sig# " << sig << ")" << " received " 
        << sigcount[sig] << " time(s)"
        << endl;

   switch (sig)
   {
      case 2:                            // INTerrupt
      case 3:                            // Quit
      case 6:                            // ABRT
      case 15:                           // TERMinate
            cout << "Exiting ... " << endl;
            exit(1);                     // initial setup
         break;

      case 11:                           // SIGSEGV
         cout << "Exiting Immediately .... " << endl;
         exit(1);              

      default:
         break;
   }

   signal(sig, sigcatch);
}

// setup singal handlers of interest
void setup_signal_handlers(char *pgmname)
{
   // first, find and print my own process ID
   cout << pgmname << ": my pid= " << getpid() << endl;

   // Handlers
   for (unsigned i=1; i < sizeof(signames)/sizeof(char *); i++)
   {
      if ( (i ==  9) ||                  // KILL can not be caught or ignored
           (i == 10) ||                  // USR1 is used by EVE
           (i == 12) ||                  // USR2 is used by EVE
           (i == 19)                     // STOP can not be caught or ignored
      )
         continue;

      if (signal(i, sigcatch) == SIG_ERR)
      {
         cout << "Signal " << i << " => " <<  signames[i] <<  endl;
         perror("setup_signal_handlers: ");
         exit(1);
      }
   }
}

void open_file(ifstream &file, const char *fname)
{
   file.open(fname);

   if (file.is_open())
      cout << "File "  << fname << " was successfully opened for read" << endl;
   else
   {
      cerr << "ERROR in " << __FILE__ << " at " << __LINE__ 
           <<": Couldn't open file:" << fname << endl;
      exit(1);
   }
}

void open_file(ofstream &file, const char *fname)
{
   file.open(fname);

   if (file.is_open())
      cout << "File "  << fname << " was successfully opened for write" << endl;
   else
   {
      cerr << "ERROR in " << __FILE__ << " at " << __LINE__ 
           <<": Couldn't open file:" << fname << endl;
      exit(1);
   }
}

