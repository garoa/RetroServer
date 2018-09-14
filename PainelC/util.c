/*
    util.c   Rotinas auxiliares    
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/sysinfo.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>

#include <sys/statvfs.h>

#include <time.h>

// Obtem o IP associado a eth0
void getIP(char *sIP)
{
    struct ifaddrs *ifaddr, *ifa;
    int s;
    char host[NI_MAXHOST];
    
    // Obtem lista de todos os IPs associados a todas as interfaces
    strcpy (sIP, "(indefinido)");
    if (getifaddrs(&ifaddr) == -1) 
    {
        return;
    }

    // Procura o IP que nos interessa
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr == NULL)
            continue;  

        if ((strcmp(ifa->ifa_name,"eth0")==0) && 
            (ifa->ifa_addr->sa_family == AF_INET))
        {
            // Converte o endereco para o formato xxx.xxx.xxx.xxx
            s = getnameinfo (ifa->ifa_addr, sizeof(struct sockaddr_in), host, 
                    NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s == 0)
            {
                // deu tudo certo
                strcpy (sIP, host);
            }
            freeifaddrs (ifaddr);
            return;
        }
    }

    freeifaddrs(ifaddr);
}

// Obtem estatisticas de uso da memoria e processador
void getProcMem(int *percProc, int *percMem)
{
    struct sysinfo sys_info;
    float f_load;

    *percProc = *percMem = 0;
    memset (&sys_info, 0, sizeof sys_info);
    if (sysinfo(&sys_info) == 0)
    {
        f_load = 1.f / (1 << SI_LOAD_SHIFT);
        *percProc = (int) (sys_info.loads[0] * f_load * 100/get_nprocs());
        *percMem = 100 - (100*(sys_info.freeram/1024))/(sys_info.totalram/1024);
    }
}

// Determina a porcentagem de disco em uso
int getDisk()
{
    struct statvfs stat;
    float total, avail, perc;
    
    if (statvfs("/", &stat) != 0)
        return 0;
    total = (float) stat.f_frsize * (float) stat.f_blocks;
    avail = (float) stat.f_bavail * (float) stat.f_bsize;
    perc = 100.0*(total-avail)/total;
    return (int) perc;
}

// Calcula a diferenca em segundos entre dois timespec
int clockDif(struct timespec *inicio, struct timespec *agora)
{
    int difSeg = (int) (agora->tv_sec - inicio->tv_sec);
    int difMseg = (int) (agora->tv_nsec/1000000 - inicio->tv_nsec/1000000);
    
    return (difSeg*1000 + difMseg)/1000;
}

