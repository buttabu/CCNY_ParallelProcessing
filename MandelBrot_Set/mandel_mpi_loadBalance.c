#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <mpi.h>

  long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
    int cpu, int group_fd, unsigned long flags)
{
  int ret;

  ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
      group_fd, flags);
  return ret;
}

void mandel_mpi(int rank, int size) {
  // x is real component of c, y is imaginary component
  float creal, cimag;
  // lower inc -> higher resolution image at cost of more computation
  float inc = 0.0025f;
  // higher max_iterations -> more distinct possible colors, 
  // more detailed and precise image, more computation
  int max_iterations = 1000;
  // number of iterations done on current c being examined
  int iterations, loc=0;

  // divide the imaginary axis into 16 parts
  // this corresponds to the first segment that this rank is
  // responsible for
  float first = rank*2.1 / (size*2) - 1;
  float second = (rank+1)*2.1 / (size*2) - 1;

  for (cimag = first; cimag < second; cimag += inc) {

    // x goes from -2.5 to 1.0
    for (creal = -2.5; creal < 1.1; creal += inc) {
      float zreal = creal;
      float zimag = cimag;
      iterations = 0;
      while (iterations < max_iterations) {
        // R(z_n+1) = R(z_n)^2 - I(z_n)^2 + R(c)
        float zreal1 = zreal*zreal - zimag*zimag + creal;
        // I(z_n+1) = 2(R(z_n)*I(z_n)) + I(c)
        float zimag1 = 2*(zreal*zimag) + cimag;
        // R(z_n) = R(z_n+1)
        zreal = zreal1;
        zimag = zimag1;
        iterations++;
        // break if R(z_n)^2 + I(z_n)^2 >= 2^2
        // i.e. if abs(z_n) > 2, break since
        // we know sequence will diverge
        if (zreal*zreal + zimag*zimag >= 4.0f) {
          break;
        }
      }
    }
  }

  // perform computation on the other segment corresponding
  // to this rank
  first = (rank+size)*2.1 / (size*2) - 1;
  second = (rank+size+1)*2.1 / (size*2) - 1;

  for (cimag = first; cimag < second; cimag += inc) {
    // x goes from -2.5 to 1.0
    for (creal = -2.5; creal < 1.1; creal += inc) {
      float zreal = creal;
      float zimag = cimag;
      iterations = 0;
      while (iterations < max_iterations) {
        // R(z_n+1) = R(z_n)^2 - I(z_n)^2 + R(c)
        float zreal1 = zreal*zreal - zimag*zimag + creal;
        // I(z_n+1) = 2(R(z_n)*I(z_n)) + I(c)
        float zimag1 = 2*(zreal*zimag) + cimag;
        // R(z_n) = R(z_n+1)
        zreal = zreal1;
        zimag = zimag1;
        iterations++;
        // break if R(z_n)^2 + I(z_n)^2 >= 2^2
        // i.e. if abs(z_n) > 2, break since
        // we know sequence will diverge
        if (zreal*zreal + zimag*zimag >= 4.0f) {
          break;
        }
      }
    }
  }
}

int main(int argc, char **argv) {

  int rank = -1, size = -1;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  struct perf_event_attr pe;
  long long count;
  int fd;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CPU_CYCLES;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  fd = perf_event_open(&pe, 0, -1, -1, 0);
  if (fd == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
    exit(EXIT_FAILURE);
  }

  ioctl(fd, PERF_EVENT_IOC_RESET, 0);
  ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

  mandel_mpi(rank, size);

  ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
  read(fd, &count, sizeof(long long));

  printf("Rank %d Mandelbrot took %lld cycles\n", rank, count);    

  close(fd);

  MPI_Finalize();
  return 0;
}