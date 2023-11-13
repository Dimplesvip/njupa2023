#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

static int evtdev = 3;
static int fbctl = 4;
static int fbdev = 5;
static int screen_w = 0, screen_h = 0;
static int canvas_w = 0, canvas_h = 0;

uint32_t NDL_GetTicks() {
  struct timeval tv = {0};
  struct timezone tz = {0};
  gettimeofday(&tv, &tz);
  return tv.tv_usec;
}

int NDL_PollEvent(char *buf, int len) {
  int succ = 0;
  succ = read(evtdev, buf, len);
  close(evtdev);
  return (succ != 0);
}

void NDL_OpenCanvas(int *w, int *h) {
  char readbuf[32];
  read(fbctl, readbuf, strlen(readbuf));
  sscanf(readbuf, "WIDTH : %d\nHEIGHT : %d\n", &screen_w, &screen_h);
  if (*w>screen_w || *w==0) { *w = screen_w; }
  if (*h>screen_h || *h==0) { *h = screen_h; }
  printf("w=%d, h=%d\n", *w, *h);
  
  if (getenv("NWM_APP")) {
    canvas_w = *w; canvas_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", canvas_w, canvas_h);
    printf("cw=%d, ch=%d\n", canvas_w, canvas_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {


  for (uint32_t j=y; j<h; j++) {
    //for (int i=x; i<w; i++) {
    printf("j%d, w=%d, x=%d, off = %d\n", j, screen_w, x, j*screen_w+x);
    lseek(fbdev, j*screen_w + x, SEEK_SET);
    write(fbdev, pixels +j*w, w);
    //}
  }
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  return 0;
}

void NDL_Quit() {
}
