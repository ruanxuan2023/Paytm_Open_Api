#ifndef PAYTM_TLS_VERIFY_H
#define PAYTM_TLS_VERIFY_H

int Paytm_Cal_Hash(void* buf, uint8* out_digest);
int Paytm_Cal_File_Hash(Paytm_location_t sl, char* filename, uint8* out_digest);

#endif