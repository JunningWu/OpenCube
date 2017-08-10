/**
 * \file huffman.h
 */
#ifndef HUFFMAN_H
#define HUFFMAN_H

int vlc_init_start();
int vlc_stop_done();
void ConvertDCMagnitudeC(unsigned char magnitude, unsigned short int *out, unsigned short int *lenght);
void ConvertACMagnitudeC(unsigned char magnitude, unsigned short int *out, unsigned short int *lenght);
void ConvertDCMagnitudeY(unsigned char magnitude, unsigned short int *out, unsigned short int *lenght);
void ConvertACMagnitudeY(unsigned char magnitude, unsigned short int *out, unsigned short int *lenght);
char Extend(char additional, unsigned char magnitude);
void ReverseExtend(char value, unsigned char *magnitude, unsigned char *bits);
void WriteRawBits16(unsigned char amount_bits, unsigned int bits);
void HuffmanEncodeFinishSend();
void HuffmanEncodeUsingDCTable(unsigned char magnitude);
void HuffmanEncodeUsingACTable(unsigned char mag);
char EncodeDataUnit(char dataunit[64], unsigned int color);

#endif /* HUFFMAN_H */
