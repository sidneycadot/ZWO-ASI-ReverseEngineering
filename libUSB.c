
// libUSB: instrumented bridge to libusb.

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

int libUSB_init(libusb_context **context)
{
    assert(context == NULL); // only used with the default context.

    printf("@@@ [%lu] libUSB_init(context = %p)\n", pthread_self(), context);
    int retval = libusb_init(context);
    printf("@@@ [%lu] --> retval = %d\n", pthread_self(), retval);
    return retval;
}

void libUSB_exit(struct libusb_context *ctx)
{
    assert(ctx == NULL); // only used with the default context.

    printf("@@@ [%lu] libUSB_exit(context = %p)\n", pthread_self(), ctx);
    return libusb_exit(ctx);
}

// These four are only used during the "enumerate cameras" call.

ssize_t libUSB_get_device_list(libusb_context * ctx, libusb_device *** list)
{
    assert(ctx == NULL); // only used with the default context.

    printf("@@@ [%lu] libUSB_get_device_list(ctx = %p, list = %p)\n", pthread_self(), ctx, list);
    ssize_t retval = libusb_get_device_list(ctx, list);
    printf("@@@ [%lu] --> retval = %ld ; *list = %p\n", pthread_self(), retval, *list);
    return retval;
}

int libUSB_get_device_descriptor(libusb_device *dev, struct libusb_device_descriptor *desc)
{
    printf("@@@ [%lu] libUSB_get_device_descriptor(dev = %p, desc = %p)\n", pthread_self(), dev, desc);
    int retval = libusb_get_device_descriptor(dev, desc);
    printf("@@@ [%lu] --> retval = %u\n", pthread_self(), retval);
    return retval;
}

uint8_t libUSB_get_device_address(libusb_device *dev)
{
    printf("@@@ [%lu] libUSB_get_device_address(dev = %p)\n", pthread_self(), dev);
    uint8_t retval = libusb_get_device_address(dev);
    printf("@@@ [%lu] --> retval = %u\n", pthread_self(), retval);
    return retval;
}

uint8_t libUSB_get_bus_number(libusb_device *dev)
{
    printf("@@@ [%lu] libUSB_get_bus_number(dev = %p)\n", pthread_self(), dev);
    uint8_t retval = libusb_get_bus_number(dev);
    printf("@@@ [%lu] --> retval = %u\n", pthread_self(), retval);
    return retval;
}

void libUSB_free_device_list(libusb_device **list, int unref_devices)
{
    printf("@@@ [%lu] libUSB_free_device_list(list = %p, unref_devices = %d)\n", pthread_self(), list, unref_devices);
    return libusb_free_device_list (list, unref_devices);
}

//////////////////////////////////////////////////////////////// below are used exclusively during open

libusb_device_handle * libUSB_open_device_with_vid_pid (libusb_context *ctx, uint16_t vendor_id, uint16_t product_id)
{
    assert(ctx == NULL); // only used with the default context.

    printf("@@@ [%lu] libUSB_open_device_with_vid_pid(ctx = %p, vendor_id = %u, product_id = %u)\n", pthread_self(), ctx, vendor_id, product_id);
    libusb_device_handle * retval = libusb_open_device_with_vid_pid(ctx, vendor_id, product_id);
    printf("@@@ [%lu] --> retval = %p\n", pthread_self(), retval);
    return retval;
}

int libUSB_set_configuration(libusb_device_handle *dev, int configuration)
{
    printf("@@@ [%lu] libUSB_set_configuration(dev = %p, configuration = %d)\n", pthread_self(), dev, configuration);
    int retval = libusb_set_configuration(dev, configuration);
    printf("@@@ [%lu] --> retval = %d\n", pthread_self(), retval);
    return retval;
}

int libUSB_claim_interface(libusb_device_handle *dev, int interface_number)
{
    printf("@@@ [%lu] libUSB_claim_interface(dev = %p, interface_number = %d)\n", pthread_self(), dev, interface_number);
    int retval = libusb_claim_interface(dev, interface_number);
    printf("@@@ [%lu] --> retval = %d\n", pthread_self(), retval);
    return retval;
}

/////////////////////////////////////////////////////////////////

int libUSB_reset_device(libusb_device_handle *dev)
{
    printf("@@@ [%lu] libUSB_reset_device(dev = %p)\n", pthread_self(), dev);
    return libusb_reset_device(dev);
}

int libUSB_bulk_transfer(struct libusb_device_handle *dev_handle, unsigned char endpoint, unsigned char *data, int length, int *transferred, unsigned int timeout)
{
    printf("@@@ [%lu] libUSB_bulk_transfer(dev = %p, endpoint = %u, data = %p, length = %d, transferred = %p, timeout = %u)\n", pthread_self(), dev_handle, endpoint, data, length, transferred, timeout);
    return libusb_bulk_transfer(dev_handle, endpoint, data, length, transferred, timeout);
}

int libUSB_control_transfer(libusb_device_handle *dev_handle, uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout)
{
    printf("@@@ [%lu] libUSB_control_transfer(dev = %p, bmRequestType = %3u, bRequest = 0x%02x, wValue = 0x%04x, wIndex = %5u, data = %14p, wLength = %4u, timeout = %3u)\n", pthread_self(), dev_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
    return libusb_control_transfer(dev_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
}

int libUSB_clear_halt(libusb_device_handle *dev, unsigned char endpoint)
{
    printf("@@@ [%lu] libUSB_clear_halt()\n", pthread_self());
    return libusb_clear_halt(dev, endpoint);
}

void libUSB_close(libusb_device_handle *dev_handle)
{
    printf("@@@ [%lu] libUSB_close(dev = %p)\n", pthread_self(), dev_handle);
    return libusb_close(dev_handle);
}
