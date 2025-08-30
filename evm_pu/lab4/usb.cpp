#include <iostream>
#include <libusb-1.0/libusb.h>
#include <stdio.h>

//g++ usb.cpp -lusb-1.0

using namespace std;
void printdev(libusb_device *dev);
int main()
{
    libusb_device **devs; // указатель на указатель на устройство // используется для получения списка устройств
    libusb_context *ctx = NULL; // контекст сессии libusb
    int r;                      // для возвращаемых значений
    ssize_t cnt;                // число найденных USB-устройств
    ssize_t i;                  // индексная переменная цикла перебора всех устройств
    
    // инициализировать библиотеку libusb, открыть сессию работы с libusb
    r = libusb_init(&ctx);
    if (r < 0)
    {
        fprintf(stderr,
                "Ошибка: инициализация не выполнена, код: %d.\n", r);
        return 1;
    }
    // задать уровень подробности отладочных сообщений
    libusb_set_debug(ctx, 3);
    // получить список всех найденных USB- устройств
    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0)
    {
        fprintf(stderr,
                "Ошибка: список USB устройств не получен, код: %d.\n", r);
        return 1;
    }
    printf("найдено устройств: %ld\n", cnt);
    printf("=============================="
           "=============================\n");

    for (i = 0; i < cnt; i++)
    { // цикл перебора всех устройств
        printf("==========  Устройство № %ld  ===========\n", i + 1);
        printdev(devs[i]); // печать параметров устройства
    }
    printf("=============================="
           "=============================\n");
    // освободить память, выделенную функцией получения списка устройств
    libusb_free_device_list(devs, 1);
    libusb_exit(ctx); // завершить работу с библиотекой libusb,
    // закрыть сессию работы с libusb
    return 0;
}
void printdev(libusb_device *dev)
{

    libusb_device_descriptor desc; // дескриптор устройства
    libusb_device_handle *dev_handle;
    unsigned char str[256];
    int len;

    int r = libusb_get_device_descriptor(dev, &desc);
    if (r < 0)
    {
        fprintf(stderr,
                "Ошибка: дескриптор устройства не получен, код: %d.\n", r);
        return;
    }

    // Открытие устройства
    r = libusb_open(dev, &dev_handle);
    if (r < 0)
    {
        fprintf(stderr, "Ошибка открытия устройства: %s\n", libusb_error_name(r));
        return;
    }

    printf("Код класса устройства : %.2d \n", (int)desc.bDeviceClass);

    if (desc.iProduct)
    {
        len = libusb_get_string_descriptor_ascii(dev_handle, desc.iProduct, str, sizeof(str));
        if (len)
        {
            printf("Класс устройства: %s\n", str);
        }
        else
        {
            fprintf(stderr, "Ошибка получения класса устройства: %s\n", libusb_error_name(len));
        }
    }
    else
    {
        printf("Класс устройства не определен\n");
    }

    printf("Идентификатор производителя : %.4x\n", desc.idVendor);

    printf("Идентификатор устройства : %.4x\n", desc.idProduct);

    if (desc.iSerialNumber)
    {
        len = libusb_get_string_descriptor_ascii(dev_handle, desc.iSerialNumber, str, sizeof(str));
        if (len)
        {
            printf("Серийный номер устройства: %s\n", str);
        }
        else
        {
            fprintf(stderr, "Ошибка получения серийного номера: %s\n", libusb_error_name(len));
        }
    }
    else
    {
        printf("У устройства нет серийного номера\n");
    }

    // Закрытие устройства
    libusb_close(dev_handle);
}
