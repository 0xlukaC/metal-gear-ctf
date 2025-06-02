#include "webserver.h"
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>

char *lastoct;

void all(Request *req, Response *res) {
  if (strcmp(req->urlRoute, "/") == 0) {
    res->content.filePath = "./index.html";
    return;
  }
  if (strcmp(req->urlRoute, "/text") == 0) {
    return;
  }

  char buffer[1024];
  snprintf(buffer, sizeof(buffer), "/frontend/%s", req->urlRoute);
  printf("%s\n", buffer);
  res->content.filePath = buffer;
  // /frontend//index.js
  // /frontend//assets/vol0.png
}

const char *dialogue(char *str, int size, char buffer[size]) {
  snprintf(
      buffer, size,
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod\n"
      "tempor incididunt ut labore et dolore magna\n"
      "Another message here.\n"
      "%s",
      str);
  return buffer;
}

void text(Request *req, Response *res) {

  char buffer[2048];

  if (strcmp(lastoct, "11") == 0) {
    dialogue("secret 1", 2048, buffer);
  } else if (strcmp(lastoct, "12") == 0) {
    dialogue("secret 2", 2048, buffer);
  } else {
    dialogue("secret fail", 2048, buffer);
  }
  res->statusCode = 200;
  res->content.data = buffer;
  printf("buffer: %s\n", buffer);
  printf("ip: %s\n", lastoct);
}

int main(int argc, char *argv[]) {

  struct ifaddrs *ifaddr, *ifa;
  char ip[INET_ADDRSTRLEN];

  getifaddrs(&ifaddr);
  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET) continue;

    // Check if the interface name is "eth1"
    if (strcmp(ifa->ifa_name, "eth1") == 0) {
      void *addr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
      inet_ntop(AF_INET, addr, ip, INET_ADDRSTRLEN);

      // Extract the last octet of the IP address
      char *last = strrchr(ip, '.');
      if (last) lastoct = last + 1;
      break;
    }
  }
  freeifaddrs(ifaddr);
  /*printf("%s\n", lastoct);*/
  /*printf("%s", ip);*/

  celp(80);
  addRoute("/", "/frontend/index.html", NULL, GET);
  addRoute("/text", NULL, text, GET);
  addRoute("/*", NULL, all, GET);

  // addRoute("/*", NULL, NULL, GET);
  // addStaticFiles("./frontend/");
  //  addRoute("/index.js", "./frontend/index.js", getJS, GET);
  inorder();
  keepAlive();
  return 0;
}
