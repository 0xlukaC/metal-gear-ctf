#include "webserver.h"
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *lastoct;

struct entries {
  char *key, *file, *secret, *std;
};

char *ips(char *input, char *type) {
  if (input == NULL) fprintf(stdout, "nothing passed into mapip");

  // Map of IP addresses to file paths and secrets 
  static const struct entries ip_map[] = {
      {"11", "/frontend/assets/015.png",
       "22666C6167206973206174202F666C61672E747874h", "flag{L3g1t-fl_g}"},
      {"12", "/frontend/assets/096.png", "404 Not Found",
       "in this request, look above"},
      {"13", "/frontend/assets/112.png", "Nothing here?", "Nothing here?"}};

  for (int i = 0; i < sizeof(ip_map) / sizeof(ip_map[0]); i++) {
    if (strcmp(input, ip_map[i].key) == 0) {
      if (strcmp(type, "secret") == 0)
        return (char *)ip_map[i].secret;
      else if (strcmp(type, "filepath") == 0)
        return (char *)ip_map[i].file;
      else if (strcmp(type, "std") == 0)
        return (char *)ip_map[i].std;
    }
  }
  printf("error, returning NULL\n");
  return NULL;
}

// handles the /* path
void all(Request *req, Response *res) {
  if (strcmp(req->urlRoute, "/") == 0) {
    res->content.filePath = "./index.html";
    return;
  }
  if (strcmp(req->urlRoute, "/s3cr3t/pasleyword") == 0)
    return; // can probably remove

  if (req->path[0] >= 48 && req->path[0] <= 57) { // for the radio images path because they start with a number
    printf("path %s\n", req->path);
    char *filepath = ips(lastoct, "filepath");
    printf("new filepath: %s\n", filepath);
    res->content.filePath = filepath;
    return;
  }

  char buffer[1024];
  snprintf(buffer, sizeof(buffer), "/frontend/%s", req->urlRoute);
  /*printf("%s\n", buffer);*/
  res->content.filePath = buffer;
  // returns: 
  // /frontend//index.js
  // /frontend//assets/vol0.png
}

// function to format dialogue
const char *dialogue(char *str, int size, char buffer[size]) {
  snprintf(buffer, size,
           "Snake, intel identified attackers on this very network."
           "\n"
           "Its believed they've already comprimised most of it already.\n"
           "Try using Accept: application/secret on some requests\n"
           "One other thin- %s",
           str);
  return buffer;
}

void text(Request *req, Response *res) {

  char buffer[2048];
  dialogue(ips(lastoct, "secret"), sizeof(buffer), buffer);

  res->statusCode = 200;
  res->content.data = buffer;
  printf("buffer: %s\n", buffer);
  printf("ip: %s\n", lastoct);
}

void standrd(Request *req, Response *res) {
  char output[1024];
  req->body("Accept", output, req);
  if (strcmp(output, "application/secret") == 1) return;
  res->content.data = ips(lastoct, "std");
  if (!strcmp(lastoct, "12")) {
    char *output = malloc(1024);
    char *message =
        "msg: Snake, some of the comms have been hijacked, set the query "
        "= condition."
        "the parameter is 'trust' \r\n";
    headerBuilder("text/plain", 200, output, 1024, message, strlen(message));
    res->body = output;
    // free(output);
    printf("header: %s\n", res->body);
    // printf("%s %s\n", message, res->body);
  }
  if (!strcmp(lastoct, "13")) {
    char output[1024];
    char *message = "msg: check access.log for 'Secret'\r\n";
    res->body = headerBuilder("text/plain", 200, output, 1024, message,
                              strlen(message));
  }
}

void top_secret(Request *req, Response *res) {
  printf("top secret, %s, %s", req->query, req->param);
  char output[1024];
  req->body("Accept", output, req);
  // if (strcmp(lastoct, "12") != 0) return;
  if (strcmp(output, "application/secret") != 0) return;
  if (strcmp(req->param, "condition") != 0) return;
  if (strcmp(req->query, "trust") != 0) return;
  res->content.data =
      "Snake, back on web3, the ssh on there is testuser:testpass. A "
      "protected_pc is broken and fetching a json file every so often from "
      "there";
}

int main(int argc, char *argv[]) {

  struct ifaddrs *ifaddr, *ifa;
  char ip[INET_ADDRSTRLEN];

  getifaddrs(&ifaddr);
  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET) continue;

    // check if the interface name is "eth1"
    if (strcmp(ifa->ifa_name, "eth1") == 0) {
      void *addr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
      inet_ntop(AF_INET, addr, ip, INET_ADDRSTRLEN);

      // extract the last octet of the IP address
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
  addRoute("/s3cr3t/pasleyword", NULL, text, GET);
  addRoute("/secret/password", NULL, standrd, GET);
  addRoute("/top/secret/password", NULL, top_secret, GET);
  addRoute("/access.log", "/access.log", NULL, GET);
  addRoute("/status.json", "/status.json", NULL, GET);
  addRoute("/*", NULL, all, GET);

  // addRoute("/*", NULL, NULL, GET);
  // addStaticFiles("./frontend/");
  //  addRoute("/index.js", "./frontend/index.js", getJS, GET);
  inorder();
  keepAlive();
  return 0;
}
