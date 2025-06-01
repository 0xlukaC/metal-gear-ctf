#include "webserver.h"

void all(Request *req, Response *res) {
  if (strcmp(req->urlRoute, "/") == 0) {
    res->content.filePath = "./index.html";
    return;
  }

  char buffer[1024];
  snprintf(buffer, sizeof(buffer), "/frontend/%s", req->urlRoute);
  printf("%s\n", buffer);
  res->content.filePath = buffer;
  /*
  /frontend//index.js
/frontend//assets/vol0.png*/
}

int main(int argc, char *argv[]) {
  celp(80);
  addRoute("/", "/frontend/index.html", NULL, GET);
  addRoute("/*", NULL, all, GET);
  // addRoute("/*", NULL, NULL, GET);
  // addStaticFiles("./frontend/");
  //  addRoute("/index.js", "./frontend/index.js", getJS, GET);
  inorder();
  keepAlive();
  return 0;
}
