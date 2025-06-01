#include "webserver.h"

void getTest(Request *req, Response *res) {
  res->content.data = "hello world from web1";
}

int main(int argc, char *argv[]) {
  celp(80);
  addRoute("/", NULL, getTest, GET);
  keepAlive();

  return 0;
}
