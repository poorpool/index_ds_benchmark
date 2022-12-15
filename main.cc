#include "adaptors.h"
#include <bits/types/struct_timeval.h>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <string>
#include <sys/time.h>
using namespace std;

long long getCurrentTimeInUs() {
  struct timeval currTime;
  gettimeofday(&currTime, nullptr);
  return currTime.tv_sec * 1000000 + currTime.tv_usec;
}

string int2key(int tmp) { return "prefix" + to_string(tmp); }

void test(IndexDS<string, int> *ds) {
  printf("---testing %s", ds->GetName().c_str());
  if (ds->IsSupportScan()) {
    printf(", it support scan---\n");
  } else {
    printf(", it does not support scan---\n");
  }

  // 顺序读写，0.4 write，0.05 delete，0.1 update，0.4 get，0.05 scan
  // 非有序结构，0.4 write，0.05 delete，0.1 update，0.45 get
  {
    long long timeStart = getCurrentTimeInUs();
    int lst = 0;
    int cntWrite = 0, cntDelete = 0, cntUpdate = 0, cntGet = 0, cntScan = 0;
    for (int i = 0; i < 4000000; i++) {
      int opt = rand() % 100;
      if (opt < 40) { // 0.4 write
        cntWrite++;
        ds->Put(int2key(lst), i);
        lst++;
      } else if (opt < 45) {
        cntDelete++;
        int tmp = (long long)rand() * rand() % (lst ? lst : 114514);
        ds->Erase(int2key(tmp));
      } else if (opt < 55) {
        cntUpdate++;
        int tmp = (long long)rand() * rand() % (lst ? lst : 114514);
        ds->Put(int2key(tmp), i);
      } else if (opt < 60 && ds->IsSupportScan()) {
        cntScan++;
        int startTmp = (long long)rand() * rand() % (lst ? lst : 114514);
        int endTmp = startTmp + rand() % 1037;
        ds->Scan(int2key(startTmp), int2key(endTmp));
      } else {
        cntGet++;
        int tmp = (long long)rand() * rand() % (lst ? lst : 114514);
        int ans;
        ds->Get(int2key(tmp), ans);
      }
    }
    printf("...write %d, get %d, scan %d, delete %d, update %d\n", cntWrite,
           cntGet, cntScan, cntDelete, cntUpdate);
    printf("...顺序增删改查 %.6f 秒\n",
           (getCurrentTimeInUs() - timeStart) / 1000000.0);
  }

  // for (int i = 0; i < 10000000; i++) {
  //   int tmp = (long long)rand() * rand() % 10000000;
  //   ds->Put("prefix" + to_string(tmp), i);
  // }
  // int ret;
  // for (int i = 0; i < 10000000; i++) {
  //   bool isGet = ds->Get("prefix" + to_string(i), ret);
  // }

  // gettimeofday(&timeEnd, nullptr);
  // double timeUse = (timeEnd.tv_sec - timeStart.tv_sec) +
  //                  (double)(timeEnd.tv_usec - timeStart.tv_usec) / 1000000.0;
  // printf("... it used %.6f seconds\n", timeUse);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  if (argc != 2) {
    printf("Usage: %s the_data_structure_you_want_to_test\n", argv[0]);
    return 0;
  }
  string testName = string(argv[1]);
  IndexDS<string, int> *ds;
  if (testName == "map" || testName == "all") {
    ds = new MapAdaptor<string, int>;
    test(ds);
    delete ds;
  }
  if (testName == "unordered_map" || testName == "all") {
    ds = new UnorderedMapAdaptor<string, int>;
    test(ds);
    delete ds;
  }

  return 0;
}