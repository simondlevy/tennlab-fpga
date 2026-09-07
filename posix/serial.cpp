#include <iostream>

#include <fcntl.h>   
#include <string.h>
#include <termios.h> 
#include <unistd.h>  
#include <sys/ioctl.h>
#include <sys/select.h>

#include "serial.h"

static const size_t kMaxBuf = 256;
static const speed_t kFpgaBaudRate = B4000000;

static int fd_;
static fd_set read_fds_;
static uint8_t buf_[kMaxBuf];

void neuro::Serial::Begin(const std::string port)
{
    fd_ = open(port.c_str(), O_RDWR);

    if (fd_ < 0) {
        std::cerr << "Unable to open port " << port << std::endl;
        return;
    }

    struct termios tty = {};

    // Read existing settings from the port
    if(tcgetattr(fd_, &tty) != 0) {
        std::cerr << "Error from tcgetatt";
        close(fd_);
        return;
    }

    tty.c_cflag &= ~PARENB;        // Clear parity bit (No parity)
    tty.c_cflag &= ~CSTOPB;        // Clear stop field (1 stop bit)
    tty.c_cflag &= ~CSIZE;         // Clear bits-per-byte size
    tty.c_cflag |= CS8;            // 8 data bits
    tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines

    tty.c_lflag &= ~ICANON;        // Disable canonical mode (raw data mode)
    tty.c_lflag &= ~ECHO;          // Disable echo
    tty.c_lflag &= ~ISIG;          // Disable interpretation of INTR, QUIT, SUSP

    cfsetispeed(&tty, kFpgaBaudRate);
    cfsetospeed(&tty, kFpgaBaudRate);

    if (tcsetattr(fd_, TCSANOW, &tty) != 0) {
        std::cerr << "Error from tcsetatt";
        return;
    }

    FD_ZERO(&read_fds_);
    FD_SET(fd_, &read_fds_);
}

void neuro::Serial::Write(const uint8_t byte)
{
    auto n = write(fd_, &byte, 1);
    (void)n;
}

uint8_t neuro::Serial::Available()
{
    uint8_t available = 0;

    int select_res = select(fd_ + 1, &read_fds_, NULL, NULL, NULL);

    if (select_res > 0 && FD_ISSET(fd_, &read_fds_)) {

        available = read(fd_, buf_, sizeof(buf_) - 1);
    }

    return available;
}

uint8_t neuro::Serial::Read(uint8_t index)
{
    return buf_[index];
}

void neuro::Serial::Close()
{
    close(fd_);
}
