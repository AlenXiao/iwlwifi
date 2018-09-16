#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/pci.h>
#include <linux/pci-aspm.h>
#include <linux/acpi.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <net/mac80211.h>

#include "ieee80211_i.h"
#include "driver-ops.h"
#include "net/awss.h"
//#include "core.h"
//#include "rdev-ops.h"
//#include "nl80211.h"

#if defined(__cplusplus)
extern "C"
{
#endif

static struct net_device *g_awss_ndev = NULL;

int awss_kernel_start_scan(void);

int awss_kernel_set_ndev(void *ndev)
{
    g_awss_ndev = (struct net_device *)ndev;
    return 0;
}
EXPORT_SYMBOL(awss_kernel_set_ndev);

int awss_kernel_start_scan(void)
{
#if 1
//    struct cfg80211_registered_device *rdev = NULL;
	struct cfg80211_scan_request *request = NULL;
    //struct ieee80211_sub_if_data *sdata = NULL;
	struct wireless_dev *wdev = NULL;
	struct wiphy *wiphy;
    int n_channels;
    int err = 0;
    int i;

    pr_info("%s, line:%u\n", __func__, __LINE__);
    if (g_awss_ndev == NULL)
        return -ENODEV;

#if 0
    pr_info("%s, line:%u\n", __func__, __LINE__);
    sdata = netdev_priv(g_awss_ndev);
    if (sdata == NULL)
        return -ENODEV;
#endif

    wdev = g_awss_ndev->ieee80211_ptr;
    wiphy = wdev->wiphy;
    //rdev = wiphy_to_rdev(wiphy);

    if (wiphy == NULL)
        return -ENODEV;

    n_channels = ieee80211_get_num_supported_channels(wiphy);

	request = kzalloc(sizeof(*request)
			+ sizeof(*request->channels) * n_channels, GFP_KERNEL);
	if (!request) {
		err = -ENOMEM;
		goto start_scan_err;
	}

    do { // all channels
		enum nl80211_band freq_band;
        i = 0;

		for (freq_band = 0; freq_band < NUM_NL80211_BANDS; freq_band ++) {
			int j;

			if (!wiphy->bands[freq_band])
				continue;
			for (j = 0; j < wiphy->bands[freq_band]->n_channels; j ++) {
				struct ieee80211_channel *channel;

				channel = &wiphy->bands[freq_band]->channels[j];

				if (channel->flags & IEEE80211_CHAN_DISABLED)
					continue;

				request->channels[i ++] = channel;
			}
		}

        if (i == 0) {
		    err = -EINVAL;
            goto start_scan_err;
        }

	    request->n_channels = i;
    } while (0);

	for (i = 0; i < NUM_NL80211_BANDS; i ++) {
        if (wiphy->bands[i] == 0)
            continue;
		request->rates[i] = (1 << wiphy->bands[i]->n_bitrates) - 1;
    }

	request->wdev = wdev;
	request->wiphy = wiphy;
	request->scan_start = jiffies;
    err = mac80211_config_ops.scan(wiphy, request);
    pr_info("%s, line:%u, ret:%d\n", __func__, __LINE__, err);
    if (err == 0) request = NULL;
#if 0
    //rdev->scan_req = request;
    //err = rdev_scan(rdev, request);
    err = rdev->ops->scan(wiphy, request);
    if (err) {
        rdev->scan_req = NULL;
    } else {
        //nl80211_send_scan_start(rdev, wdev);
        dev_hold(g_awss_ndev);
        request = NULL;
    }
#endif

start_scan_err:
    //release buffer
    if (request) kfree(request);
    return err;
#else
    return 0;
#endif
}

#if 0
int awss_kernel_open(struct inode *inode, struct file *filp)
{
    return 0;
}

int awss_kernel_release(struct inode *inode, struct file *filp)
{
    return 0;
}
#endif
static long awss_kernel_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    //int ret = -1;

    pr_info("%s\n", __func__);

    if (_IOC_TYPE(cmd) != AWSS_IOC_MAGIC) {
        pr_err("input cmd type [%c!=%c(awss magic)] err!\n", _IOC_TYPE(cmd), AWSS_IOC_MAGIC);
        return -ENOTTY;
    }

    if (_IOC_NR(cmd) >= AWSS_IOC_CMD_MAX_IDX) {
        pr_err("input cmd idx [%d >= %d(awss cmd max idx)] err!\n", _IOC_NR(cmd), AWSS_IOC_CMD_MAX_IDX);
        return -ENOTTY;
    }
#if 0
    if (_IOC_DIR(cmd) & _IOC_READ)
        ret = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        ret= !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
    if (ret)
        return -EFAULT;
#endif

    switch (_IOC_NR(cmd)) {
        case AWSS_IOC_SCAN_IDX:
            pr_info("AWSS_IOC_SCAN\n");
            awss_kernel_start_scan();
            break;
        case AWSS_IOC_OPEN_MGMT_FILTER_IDX:
            pr_info("AWSS_IOC_OPEN_MGMT_FILTER\n");
            //awss_kernel_open_mgmt_filter();
            break;
        case AWSS_IOC_CLOSE_MGMT_FILTER_IDX:
            pr_info("AWSS_IOC_CLOSE_MGMT_FILTER\n");
            //awss_kernel_close_mgmt_filter();
            break;
        case AWSS_IOC_OPEN_SNIFFER_IDX:
            pr_info("AWSS_IOC_OPEN_SNIFFER\n");
            //awss_kernel_open_sniffer();
            break;
        case AWSS_IOC_CLOSE_SNIFFER_IDX:
            pr_info("AWSS_IOC_CLOSE_SNIFFER\n");
            //awss_kernel_close_sniffer();
            break;
        case AWSS_IOC_CONN_AP_IDX:
            pr_info("AWSS_IOC_CONN_AP\n");
            //awss_kernel_connect_ap();
            break;
        case AWSS_IOC_SWITCH_CHN_IDX:
            pr_info("AWSS_IOC_SWITCH_CHN\n");
            //awss_switch_channel();
            break;
        default:
            pr_info("===========================================%d", _IOC_NR(cmd));
            break;
    }
    return 0;
}

static struct file_operations awss_fops = {
    .owner = THIS_MODULE,
#if 0
    .open = awss_kernel_open,
    .release = awss_kernel_release,
#endif
    .unlocked_ioctl = awss_kernel_ioctl,
};

static dev_t awss_dev;
static struct cdev *awss_cdev;
static int awss_major = 0;
static int awss_minor = 0;

int awss_kernel_create_device(void)
{
    int ret;

    pr_info("awss_create_deice\n");
    ret = alloc_chrdev_region(&awss_dev, 0, 1, AWSS_NAME);
    pr_info("alloc chrdev region ret:%d\n", ret);
    if (ret < 0)
        return ret;
    awss_major = MAJOR(awss_dev);
    awss_minor = MINOR(awss_dev);
    pr_info("awss_major:%d, awss_minor:%d\n", awss_major, awss_minor);

    awss_cdev = cdev_alloc();
    pr_info("awss_cdev:%p\n", awss_cdev);
    if (awss_cdev == NULL)
        return -1;
    cdev_init(awss_cdev, &awss_fops);
    awss_cdev->owner = THIS_MODULE;
    awss_cdev->ops = &awss_fops;
    ret = cdev_add(awss_cdev, awss_dev, 1);
    pr_info("cdev add %d\n", ret);
    return 0;
}
EXPORT_SYMBOL(awss_kernel_create_device);

int awss_kernel_destroy_device(void)
{
    awss_dev = MKDEV(awss_major, awss_minor);
    pr_info("destroy awss device\n");
    cdev_del(awss_cdev);
    unregister_chrdev_region(awss_dev, 1);
    awss_cdev = NULL;
    awss_major = 0;
    awss_minor = 0;
    return 0;
}
EXPORT_SYMBOL(awss_kernel_destroy_device);

MODULE_LICENSE("GPL");

#if defined(__cplusplus)
}
#endif
