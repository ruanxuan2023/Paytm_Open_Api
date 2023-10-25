#ifndef PAYTM_TLS_VERIFY_H
#define PAYTM_TLS_VERIFY_H

int Paytm_Cal_Hash(void* buf, int buf_len, char* out_digest);
int Paytm_Cal_File_Hash(Paytm_location_t sl, char* filename, char* out_digest);

#endif