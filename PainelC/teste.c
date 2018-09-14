#include <sys/sysinfo.h>
#include <stdio.h>
#include <string.h>
#include <sys/statvfs.h>
#include <time.h>

int main() {
  int days, hours, mins;
  struct sysinfo sys_info;
  float f_load;
  struct statvfs stat;
  struct timespec inicio, agora;
  int difSeg, difMseg;

  memset(&sys_info, 0, sizeof sys_info);
  if(sysinfo(&sys_info) != 0)
    perror("sysinfo");

  // Uptime
  days = sys_info.uptime / 86400;
  hours = (sys_info.uptime / 3600) - (days * 24);
  mins = (sys_info.uptime / 60) - (days * 1440) - (hours * 60);

  printf("Uptime: %ddays, %dhours, %dminutes, %ldseconds\n",
                      days, hours, mins, sys_info.uptime % 60);

  // Load Averages for 1,5 and 15 minutes
  printf("Load Avgs: 1min(%ld) 5min(%ld) 15min(%ld)\n",
          sys_info.loads[0], sys_info.loads[1], sys_info.loads[2]);
  f_load = 1.f / (1 << SI_LOAD_SHIFT);
  printf("load average (1 min): %.2f (%.0f%% CPU)\n",
        sys_info.loads[0] * f_load,
        sys_info.loads[0] * f_load * 100/get_nprocs());
  
  // Total and free ram.
  printf("Total Ram: %ldk\tFree: %ldk\n", sys_info.totalram / 1024,
                                        sys_info.freeram / 1024);
  printf("In use: %d%%\n", 100 - (100*(sys_info.freeram/1024))/(sys_info.totalram/1024));

  // Shared and buffered ram.
  printf("Shared Ram: %ldk\n", sys_info.sharedram / 1024);
  printf("Buffered Ram: %ldk\n", sys_info.bufferram / 1024);

  // Swap space
  printf("Total Swap: %ldk\tFree: %ldk\n", sys_info.totalswap / 1024,
                                           sys_info.freeswap / 1024);

  // Number of processes currently running.
  printf("Number of processes: %d\n", sys_info.procs);

  if (statvfs("/", &stat) == 0)
  {
      float total, avail, perc;
      printf ("f_bsize= %ld\n", stat.f_bsize);
      printf ("f_frsize= %ld\n", stat.f_frsize);
      printf ("f_blocks= %ld\n", stat.f_blocks);
      printf ("f_bfree= %d\n", stat.f_bfree);
      printf ("f_bavail= %ld\n", stat.f_bavail);
      total = (float) stat.f_frsize * (float) stat.f_blocks;
      avail = (float) stat.f_bavail * (float) stat.f_bsize;
      printf ("Em uso %2.0f%%\n", 100.0*(total-avail)/total);
  }

  clock_gettime(CLOCK_REALTIME, &inicio);
  sleep(3);
  clock_gettime(CLOCK_REALTIME, &agora);
  printf ("Inicio = %ld.%03ld\n", inicio.tv_sec, inicio.tv_nsec/1000000);
  printf ("Agora = %ld.%03ld\n", agora.tv_sec, agora.tv_nsec/1000000);
  difSeg = (int) (agora.tv_sec - inicio.tv_sec);
  difMseg = (int) (agora.tv_nsec/1000000 - inicio.tv_nsec/1000000);
  printf ("Dif: %d %d %d\n", difSeg, difMseg, (difSeg*1000 + difMseg)/1000);
  
  return 0;
}

