#ifndef UTILITY_H
#define UTILITY_H
#include <stdbool.h>
#include <QTime>
#include <QCoreApplication>

unsigned int CRC16(unsigned char *, unsigned char);

bool CRC16Verify(unsigned char *, unsigned char);

unsigned char Get_CheckXor(unsigned char* temp, int len);

unsigned char daiMaHe(unsigned char* temp, int len);

void qtSleep(unsigned int msec);

#endif // UTILITY_H
