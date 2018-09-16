#include <sys/ioctl.h>

#if defined(__cplusplus)
extern "C"
{
#endif

enum {
    AWSS_IOC_SCAN_IDX = 0,
    AWSS_IOC_OPEN_MGMT_FILTER_IDX,
    AWSS_IOC_CLOSE_MGMT_FILTER_IDX,
    AWSS_IOC_OPEN_SNIFFER_IDX,
    AWSS_IOC_CLOSE_SNIFFER_IDX,
    AWSS_IOC_CONN_AP_IDX,
    AWSS_IOC_SWITCH_CHN_IDX,
    AWSS_IOC_CMD_MAX_IDX,
};

#define AWSS_IOC_MAGIC              'c'
#define AWSS_IOC_CMD(idx)           _IO(AWSS_IOC_MAGIC, idx)

#define AWSS_NAME                   "awss"

extern int awss_kernel_create_device(void);
extern int awss_kernel_destroy_device(void);
extern int awss_kernel_set_ndev(void *);

#if defined(__cplusplus)
}
#endif
