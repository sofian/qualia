// gcc -o parity parity.c -I/usr/local/include/mapper-0 -lmapper-0 -llo
#include <stdio.h>
#include <mapper/mapper.h>

mapper_signal insig=0, outsig=0;

void insig_handler(mapper_signal sig, mapper_db_signal props,
    mapper_timetag_t *timetag, void *value)
{
  float v = *((float*)value);
  int r = ((int)v) & 1;
  printf("input: %f, output: %d\n", v, r);
  msig_update(outsig, &r);
}

int main(int argc, char** argv)
{
  mapper_admin adm = mapper_admin_new(argv[1], 0, 0);
  mapper_device dev = mdev_new("parity", 9000, adm);

  float fmn = 0, fmx = 256;
  insig = mdev_add_input(dev, "/insig", 1, 'f',
      0, &fmn, &fmx, insig_handler, 0);

  int imn = 0, imx = 1;
  outsig = mdev_add_output(dev, "/outsig", 1, 'i',
      0, &imn, &imx);

  while (1) {
    mdev_poll(dev, 1000);
  }

  mdev_free(dev);

  return 0;
}
