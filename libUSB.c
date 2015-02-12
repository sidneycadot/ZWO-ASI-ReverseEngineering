
// libUSB: instrumented bridge to libusb.

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

int libUSB_init(libusb_context **context)
{
    assert(context == NULL); // only used with the default context.

    printf("[libUSB/%lu] libusb_init(context = %p)\n", pthread_self(), context);
    int retval = libusb_init(context);
    printf("[libUSB/%lu] libusb_init --> retval = %d\n", pthread_self(), retval);
    return retval;
}

void libUSB_exit(struct libusb_context *ctx)
{
    assert(ctx == NULL); // only used with the default context.

    printf("[libUSB/%lu] libusb_exit(context = %p)\n", pthread_self(), ctx);
    return libusb_exit(ctx);
}

// These five functions are only called from ASIGetNumOfConnectedCameras().

ssize_t libUSB_get_device_list(libusb_context * ctx, libusb_device *** list)
{
    assert(ctx == NULL); // only used with the default context.

    printf("[libUSB/%lu] libusb_get_device_list(ctx = %p, list = %p)\n", pthread_self(), ctx, list);
    ssize_t retval = libusb_get_device_list(ctx, list);
    printf("[libUSB/%lu] libusb_get_device_list --> retval = %ld ; *list = %p\n", pthread_self(), retval, *list);
    return retval;
}

int libUSB_get_device_descriptor(libusb_device *dev, struct libusb_device_descriptor *desc)
{
    printf("[libUSB/%lu] libusb_get_device_descriptor(dev = %p, desc = %p)\n", pthread_self(), dev, desc);
    int retval = libusb_get_device_descriptor(dev, desc);
    printf("[libUSB/%lu] libusb_get_device_descriptor --> retval = %u\n", pthread_self(), retval);
    return retval;
}

uint8_t libUSB_get_device_address(libusb_device *dev)
{
    printf("[libUSB/%lu] libusb_get_device_address(dev = %p)\n", pthread_self(), dev);
    uint8_t retval = libusb_get_device_address(dev);
    printf("[libUSB/%lu] libusb_get_device_address --> retval = %u\n", pthread_self(), retval);
    return retval;
}

uint8_t libUSB_get_bus_number(libusb_device *dev)
{
    printf("[libUSB/%lu] libusb_get_bus_number(dev = %p)\n", pthread_self(), dev);
    uint8_t retval = libusb_get_bus_number(dev);
    printf("[libUSB/%lu] libusb_get_bus_number --> retval = %u\n", pthread_self(), retval);
    return retval;
}

void libUSB_free_device_list(libusb_device **list, int unref_devices)
{
    printf("[libUSB/%lu] libusb_free_device_list(list = %p, unref_devices = %d)\n", pthread_self(), list, unref_devices);
    return libusb_free_device_list (list, unref_devices);
}

// These three functions are only called from ASIOpenCamera().

libusb_device_handle * libUSB_open_device_with_vid_pid (libusb_context *ctx, uint16_t vendor_id, uint16_t product_id)
{
    assert(ctx == NULL); // only used with the default context.

    printf("[libUSB/%lu] libusb_open_device_with_vid_pid(ctx = %p, vendor_id = %u, product_id = %u)\n", pthread_self(), ctx, vendor_id, product_id);
    libusb_device_handle * retval = libusb_open_device_with_vid_pid(ctx, vendor_id, product_id);
    printf("[libUSB/%lu] libusb_open_device_with_vid_pid --> retval = %p\n", pthread_self(), retval);
    return retval;
}

int libUSB_set_configuration(libusb_device_handle *dev, int configuration)
{
    printf("[libUSB/%lu] libusb_set_configuration(dev = %p, configuration = %d)\n", pthread_self(), dev, configuration);
    int retval = libusb_set_configuration(dev, configuration);
    printf("[libUSB/%lu] libusb_set_configuration --> retval = %d\n", pthread_self(), retval);
    return retval;
}

int libUSB_claim_interface(libusb_device_handle *dev, int interface_number)
{
    printf("[libUSB/%lu] libusb_claim_interface(dev = %p, interface_number = %d)\n", pthread_self(), dev, interface_number);
    int retval = libusb_claim_interface(dev, interface_number);
    printf("[libUSB/%lu] libusb_claim_interface --> retval = %d\n", pthread_self(), retval);
    return retval;
}

// This function is only called from ASICloseCamera().

void libUSB_close(libusb_device_handle *dev_handle)
{
    printf("[libUSB/%lu] libusb_close(dev = %p)\n", pthread_self(), dev_handle);
    return libusb_close(dev_handle);
}

// Miscellaneous functions.

int libUSB_reset_device(libusb_device_handle *dev)
{
    printf("[libUSB/%lu] libusb_reset_device(dev = %p)\n", pthread_self(), dev);
    return libusb_reset_device(dev);
}

int libUSB_clear_halt(libusb_device_handle *dev, unsigned char endpoint)
{
    printf("[libUSB/%lu] libusb_clear_halt()\n", pthread_self());
    int retval = libusb_clear_halt(dev, endpoint);
    printf("[libUSB/%lu] libusb_clear_halt --> retval = %u\n", pthread_self(), retval);
    return retval;
}

// "control" and "bulk" transfers.

static void dump_hex_data(FILE * f, unsigned char * data, unsigned size)
{
    const unsigned MAX_BYTES_PER_LINE = 64;
    unsigned count = 0;

    for (unsigned i = 0; i < size; ++i)
    {
        if (count != 0)
        {
            fprintf(f, " ");
        }
        fprintf(f, "%02x", data[i]);
        ++count;
        if (count == MAX_BYTES_PER_LINE)
        {
            fprintf(f, "\n");
            count = 0;
        }
    }
    if (count != 0)
    {
        fprintf(f, "\n");
        count = 0;
    }
}

int libUSB_control_transfer(libusb_device_handle *dev_handle, uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout)
{
    static FILE * f = NULL;

    if (f == NULL)
    {
        f = fopen("usb_control_transfer.log", "w");
    }

    printf("[libUSB/%lu] libusb_control_transfer(dev = %p, bmRequestType = 0x%02x, bRequest = 0x%02x, wValue = 0x%04x, wIndex = %5u, data = %14p, wLength = %4u, timeout = %3u)\n", pthread_self(), dev_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);

    if (0) // log data to be sent to file (TODO: understand when this happens)
    {
        fprintf(f, "[libUSB/%lu] libusb_control_transfer(dev = %p, bmRequestType = 0x%02x, bRequest = 0x%02x, wValue = 0x%04x, wIndex = %5u, data = %14p, wLength = %4u, timeout = %3u)\n", pthread_self(), dev_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);

        if (data != NULL)
        {
            fprintf(f, "DATA BEFORE CALL:\n");
            dump_hex_data(f, data, wLength);
        }
    }

    int retval = libusb_control_transfer(dev_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);

    if (1) // log data received to file
    {
        fprintf(f, "[libUSB/%lu] libusb_control_transfer(dev = %p, bmRequestType = 0x%02x, bRequest = 0x%02x, wValue = 0x%04x, wIndex = %5u, data = %14p, wLength = %4u, timeout = %3u)\n", pthread_self(), dev_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);

        if (data != NULL)
        {
            fprintf(f, "DATA AFTER CALL:\n");
            dump_hex_data(f, data, wLength);
        }
    }

    printf("[libUSB/%lu] libusb_control_transfer --> retval = %u\n", pthread_self(), retval);
    return retval;
}

int libUSB_bulk_transfer(struct libusb_device_handle *dev_handle, unsigned char endpoint, unsigned char *data, int length, int *transferred, unsigned int timeout)
{
    printf("[libUSB/%lu] libusb_bulk_transfer(dev = %p, endpoint = %u, data = %p, length = %d, transferred = %p, timeout = %u)\n", pthread_self(), dev_handle, endpoint, data, length, transferred, timeout);
    int retval = libusb_bulk_transfer(dev_handle, endpoint, data, length, transferred, timeout);
    printf("[libUSB/%lu] libusb_bulk_transfer --> retval = %d\n", pthread_self(), retval);
    return retval;
}
